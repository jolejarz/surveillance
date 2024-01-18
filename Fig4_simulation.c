#include <stdio.h>
#include <math.h>
#include "gsl/gsl_rng.h"
#include "calc.h"

void a_r_p (short int flag, double r_param, double p_param, double c2_param, double *r, double *p, double *c2);
void c2_a_p (short int flag, double r_param, double p_param, double c2_param, double *r, double *p, double *c2);
void c2_r_a (short int flag, double r_param, double p_param, double c2_param, double *r, double *p, double *c2);

int main (int argc, char *argv[])
{
	char filename[12][100] = {"Fig4B_simulation_lambda=0.001_a=0.001000_r=0.1_p=0.01.dat", "Fig4B_simulation_lambda=0.001_a=0.003162_r=0.1_p=0.01.dat", "Fig4B_simulation_lambda=0.001_a=0.01000_r=0.1_p=0.01.dat", "Fig4B_simulation_lambda=0.001_a=0.03162_r=0.1_p=0.01.dat",
	                          "Fig4D_simulation_lambda=0.001_c2=1_a=08_p=0.01.dat"       , "Fig4D_simulation_lambda=0.001_c2=1_a=16_p=0.01.dat"       , "Fig4D_simulation_lambda=0.001_c2=1_a=32_p=0.01.dat"      , "Fig4D_simulation_lambda=0.001_c2=1_a=64_p=0.01.dat"      ,
	                          "Fig4F_simulation_lambda=0.001_c2=1_r=0.1_a=0.0001.dat"    , "Fig4F_simulation_lambda=0.001_c2=1_r=0.1_a=0.001.dat"     , "Fig4F_simulation_lambda=0.001_c2=1_r=0.1_a=0.01.dat"     , "Fig4F_simulation_lambda=0.001_c2=1_r=0.1_a=0.1.dat"      };

	double lambda[12] = {0.001, 0.001, 0.001, 0.001,
	                     0.001, 0.001, 0.001, 0.001,
	                     0.001, 0.001, 0.001, 0.001};

	double c2[12] = {0.001, 0.003162, 0.01, 0.03162,
	                 1    , 1       , 1   , 1      ,
	                 1    , 1       , 1   , 1      };

	double r[12] = {0.1,  0.1,  0.1,  0.1,
	                8  , 16  , 32  , 64  ,
	                0.1,  0.1,  0.1,  0.1};

	double p[12] = {0.01  , 0.01 , 0.01, 0.01,
	                0.01  , 0.01 , 0.01, 0.01,
	                0.0001, 0.001, 0.01, 0.1 };

	int min_exponent[12] = { 1, 1, 1, 1,
	                        10, 9, 8, 7,
	                         4, 3, 2, 1};

	int n[12] = {  1000,   1000,   1000,   1000,
	             100000, 100000, 100000, 100000,
	               1000,   1000,   1000,   1000};

	double c1=1;

	int N = 16;

	int i, m;

	double f, avg, stderror;

	FILE *datafile[12];

	void (*c2_r_p_set)(short int, double, double, double, double*, double*, double*);

	a_r_p (0, 0, 0, 0, 0, 0, 0);
	c2_a_p (0, 0, 0, 0, 0, 0, 0);
	c2_r_a (0, 0, 0, 0, 0, 0, 0);

	for (m=0; m<12; m++)
	{
		if (m<4) c2_r_p_set=a_r_p; else if (m<8) c2_r_p_set=c2_a_p; else c2_r_p_set=c2_r_a;

		datafile[m] = fopen (filename[m], "w");

		for (i=min_exponent[m]; i<40; i++)
		{
			f = 0.01*pow(10,i/(double)10);
			printf("file number = %d, f = %lf\n", m, f);
			c2_r_p_set (1, r[m], p[m], c2[m], 0, 0, 0);
			calculate (f, c2_r_p_set, lambda[m], c1, n[m], N, 0, 0, &avg, &stderror);
			fprintf (datafile[m], "%lf %lf %lf\n", f, avg, stderror);
		}

		fclose (datafile[m]);
	}

	return 0;
}

void a_r_p (short int flag, double r_param, double p_param, double c2_param, double *r, double *p, double *c2)
{
	static double random_number;
	static unsigned int low, high;
	static unsigned long int seed;

	static double largest_random_number_plus_1;

	static gsl_rng *rng_mt;

	static double c2_set, r_set, p_set;

	static double x;

	static int i;

	if (flag==0)
	{
		asm ("rdtsc" : "=a" (low), "=d" (high));
		seed=(unsigned long int)high;
		seed*=(unsigned long int)pow(2,16);
		seed*=(unsigned long int)pow(2,16);
		seed+=(unsigned long int)low;

		rng_mt = gsl_rng_alloc (gsl_rng_mt19937);

		largest_random_number_plus_1=(double)(gsl_rng_max(rng_mt))+1;

		gsl_rng_set (rng_mt, seed);
	}
	else if (flag==1) {c2_set=c2_param; r_set=r_param; p_set=p_param;}
	else
	{
		*r=r_set; *p=p_set;

		random_number = gsl_rng_get(rng_mt)/largest_random_number_plus_1;

		i=0;
		do
		{
			*c2=(i++)/(double)100;
		}
		while (erf((*c2)*sqrt(c2_set))<random_number && i<10000);
	}

	return;
}

void c2_a_p (short int flag, double r_param, double p_param, double c2_param, double *r, double *p, double *c2)
{
	static double random_number;
	static unsigned int low, high;
	static unsigned long int seed;

	static double largest_random_number_plus_1;

	static gsl_rng *rng_mt;

	static double c2_set, r_set, p_set;

	static double x;

	static double r_previous;

	static int i;

	if (flag==0)
	{
		asm ("rdtsc" : "=a" (low), "=d" (high));
		seed=(unsigned long int)high;
		seed*=(unsigned long int)pow(2,16);
		seed*=(unsigned long int)pow(2,16);
		seed+=(unsigned long int)low;

		rng_mt = gsl_rng_alloc (gsl_rng_mt19937);

		largest_random_number_plus_1=(double)(gsl_rng_max(rng_mt))+1;

		gsl_rng_set (rng_mt, seed);
	}
	else if (flag==1) {c2_set=c2_param; r_set=r_param; p_set=p_param;}
	else
	{
		*c2=c2_set; *p=p_set;

		random_number = gsl_rng_get(rng_mt)/largest_random_number_plus_1;

		i=0;
		do
		{
			r_previous=i/(double)1000;
			*r=(++i)/(double)1000;
		}
		while (1-exp(-r_set*(*r)*(*r))<random_number && i<1000);
		*r=((*r)+r_previous)/(double)2;
	}

	return;
}

void c2_r_a (short int flag, double r_param, double p_param, double c2_param, double *r, double *p, double *c2)
{
	static double random_number;
	static unsigned int low, high;
	static unsigned long int seed;

	static double largest_random_number_plus_1;

	static gsl_rng *rng_mt;

	static double c2_set, r_set, p_set;

	static double x;

	static int i;

	if (flag==0)
	{
		asm ("rdtsc" : "=a" (low), "=d" (high));
		seed=(unsigned long int)high;
		seed*=(unsigned long int)pow(2,16);
		seed*=(unsigned long int)pow(2,16);
		seed+=(unsigned long int)low;

		rng_mt = gsl_rng_alloc (gsl_rng_mt19937);

		largest_random_number_plus_1=(double)(gsl_rng_max(rng_mt))+1;

		gsl_rng_set (rng_mt, seed);
	}
	else if (flag==1) {c2_set=c2_param; r_set=r_param; p_set=p_param;}
	else
	{
		*c2=c2_set; *r=r_set;

		random_number = gsl_rng_get(rng_mt)/largest_random_number_plus_1;

		i=0;
		do
		{
			*p=p_set*pow(0.1,(i++)/(double)100);
		}
		while (((*p)+(1-(*p))*log(1-(*p)))/(p_set+(1-p_set)*log(1-p_set))>random_number && *p>0.000001 && i<500);
	}

	return;
}
