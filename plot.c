#include <stdio.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_sf_lambert.h>
#include <gsl/gsl_sf_gamma.h>

int main()
{
	double lambda, k, c, p;
	double T, T_min, T_max;

	int i, N;

	FILE *datafile0 = fopen ("avgC_vs_T_p=1.dat", "w");
	FILE *datafile1 = fopen ("avgC_vs_T_p=0.1.dat", "w");
	FILE *datafile2 = fopen ("avgC_vs_T_p=0.01.dat", "w");
	FILE *datafile3 = fopen ("avgC_vs_T_p=0.001.dat", "w");

	T_min=0.01;
	T_max=100;
	N=1000;

	lambda=0.01; k=0.1; c=1;

	double avgC;
	
	p=1;
	for (i=0; i<=N; i++)
	{
		T = T_min*pow(T_max/T_min,i/(double)N);

		avgC=1/T+c*lambda*(exp(k*T)-1)/k/T*(1-(1-exp(-k*T))/log(1-p));

		fprintf (datafile0, "%lf %lf\n", T, avgC);
	}

	p=0.1;
	for (i=0; i<=N; i++)
	{
		T = T_min*pow(T_max/T_min,i/(double)N);

		avgC=1/T+c*lambda*(exp(k*T)-1)/k/T*(1-(1-exp(-k*T))/log(1-p));

		fprintf (datafile1, "%lf %lf\n", T, avgC);
	}

	p=0.01;
	for (i=0; i<=N; i++)
	{
		T = T_min*pow(T_max/T_min,i/(double)N);

		avgC=1/T+c*lambda*(exp(k*T)-1)/k/T*(1-(1-exp(-k*T))/log(1-p));

		fprintf (datafile2, "%lf %lf\n", T, avgC);
	}

	p=0.001;
	for (i=0; i<=N; i++)
	{
		T = T_min*pow(T_max/T_min,i/(double)N);

		avgC=1/T+c*lambda*(exp(k*T)-1)/k/T*(1-(1-exp(-k*T))/log(1-p));

		fprintf (datafile3, "%lf %lf\n", T, avgC);
	}

	fclose (datafile3);
	fclose (datafile2);
	fclose (datafile1);
	fclose (datafile0);

	return 0;
}
