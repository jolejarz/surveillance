#include <stdio.h>
#include <math.h>
#include "gsl/gsl_rng.h"
#include "omp.h"

void calculate (double f, void (*c2_r_p_set)(short int, double, double, double, double*, double*, double*), double lambda, double c1, int n, int N, short int n_sq, double phi, double *avg, double *stderror)
{
	double time_sample_max;

	double r, p, c2;

	double cost_sum, cost_sum_squared;

	double T, time_division, time_division_previous, time_sample, cost, random_number;
	unsigned int low, high;
	unsigned long int seed;
	int i, j, k, num_cells;
	short int detected;

	asm ("rdtsc" : "=a" (low), "=d" (high));
	seed=(unsigned long int)high;
	seed*=(unsigned long int)pow(2,16);
	seed*=(unsigned long int)pow(2,16);
	seed+=(unsigned long int)low;

	gsl_rng *rng_mt = gsl_rng_alloc (gsl_rng_mt19937);

	double largest_random_number_plus_1=(double)(gsl_rng_max(rng_mt))+1;

	gsl_rng_set (rng_mt, seed);

	T = 1/f;

	cost_sum=0; cost_sum_squared=0;
	#pragma omp parallel for private(cost,time_division_previous,time_sample_max,j,r,p,c2,random_number,time_division,time_sample,num_cells,detected) shared(cost_sum,cost_sum_squared)
	for (k=0; k<N; k++)
	{
		printf("  thread = %d, loop index = %02d\n", omp_get_thread_num(), k);

		cost=0;
		time_division_previous=0;
		time_sample_max=0;
		for (j=0; j<n; j++)
		{
			c2_r_p_set (2, 0, 0, 0, &r, &p, &c2);

			random_number = gsl_rng_get(rng_mt)/largest_random_number_plus_1;

			time_division=time_division_previous-log(1-random_number)/lambda;
			time_division_previous=time_division;

			time_sample=((int)(time_division/T)+1)*T;

			do
			{
				num_cells=0;
				detected=0;

				do
				{
					do
					{
						num_cells++;

						random_number = gsl_rng_get(rng_mt)/largest_random_number_plus_1;
						time_division-=log(1-random_number)/(num_cells*r);
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
				while (detected==0);

				if (n_sq==0) cost+=c2*num_cells; else cost+=c2*num_cells*num_cells;
				if (time_sample>time_sample_max) time_sample_max=time_sample;

				time_division=time_sample;
				time_sample+=T;

				random_number = gsl_rng_get(rng_mt)/largest_random_number_plus_1;
			}
			while (random_number<phi);
		}
		cost/=time_sample_max;
		cost+=c1/(double)T;

		cost_sum+=cost;
		cost_sum_squared+=cost*cost;
	}
	*avg = cost_sum/(double)N;
	*stderror = sqrt((cost_sum_squared/(double)N-(cost_sum/(double)N)*(cost_sum/(double)N))/(double)(N-1));

	return;
}
