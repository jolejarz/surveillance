#include <stdio.h>
#include <math.h>
#include "gsl/gsl_rng.h"

int main (int argc, char *argv[])
{
	double lambda = strtod (argv[1], 0);
	double k = strtod (argv[2], 0);
	double c = strtod (argv[3], 0);
	int N = strtod (argv[4], 0);
	int min_i = strtod (argv[5], 0);

	double f, T, time_division, time_division_previous, time_sample, cost, random_number;
	unsigned int low, high;
	unsigned long int seed;
	int i, j, num_cells;
	short int detected;

	double p;

	int n;

	asm ("rdtsc" : "=a" (low), "=d" (high));
	seed=(unsigned long int)high;
	seed*=(unsigned long int)pow(2,16);
	seed*=(unsigned long int)pow(2,16);
	seed+=(unsigned long int)low;

	gsl_rng *rng_mt = gsl_rng_alloc (gsl_rng_mt19937);

	double largest_random_number_plus_1=(double)(gsl_rng_max(rng_mt))+1;

	gsl_rng_set (rng_mt, seed);

	FILE *datafile[4] = {fopen ("p=1.dat", "w"), fopen ("p=0.1.dat", "w"), fopen ("p=0.01.dat", "w"), fopen ("p=0.001.dat", "w")};

	for (n=0; n<=3; n++)
	{
		if (n==0) p=1; else if (n==1) p=0.1; else if (n==2) p=0.01; else p=0.001;

		for (i=min_i; i<=39; i++)
		{
			f = 0.01*pow(10,i/(double)10);
			T = 1/f;

			printf("p = %.4lf, f = %.4lf\n", p, f);

			cost=0;
			time_division_previous=0;
			for (j=0; j<N; j++)
			{
				random_number = gsl_rng_get(rng_mt)/largest_random_number_plus_1;

				time_division=time_division_previous-log(1-random_number)/lambda;
				time_division_previous=time_division;

				time_sample=((int)(time_division/T)+1)*T;

				num_cells=0;
				detected=0;

				while (detected==0)
				{
					do
					{
						num_cells++;

						random_number = gsl_rng_get(rng_mt)/largest_random_number_plus_1;
						time_division-=log(1-random_number)/(num_cells*k);
					}
					while (time_division<=time_sample);

					do
					{
						random_number = gsl_rng_get(rng_mt)/largest_random_number_plus_1;

						if (random_number>pow((1-p),num_cells)) detected=1;
						else time_sample+=T;
					}
					while (time_sample<time_division && detected==0);
				}

				cost+=c*num_cells;
			}
			cost/=time_sample;
			cost+=1/(double)T;

			fprintf (datafile[n], "%lf %lf\n", f, cost);
		}
	}

	fclose (datafile[3]);
	fclose (datafile[2]);
	fclose (datafile[1]);
	fclose (datafile[0]);

	return 0;
}
