#include <stdio.h>
#include <math.h>
#include "calc.h"

void c2_r_p (short int flag, double r_param, double p_param, double c2_param, double *r, double *p, double *c2);

int main (int argc, char *argv[])
{
	char filename[24][100] = {"Fig3A_simulation_lambda=0.010_r=0.1_p=1.000_c2=001.dat", "Fig3A_simulation_lambda=0.010_r=0.1_p=0.100_c2=001.dat", "Fig3A_simulation_lambda=0.010_r=0.1_p=0.010_c2=001.dat", "Fig3A_simulation_lambda=0.010_r=0.1_p=0.001_c2=001.dat",
	                          "Fig3B_simulation_lambda=0.010_r=0.1_p=1.000_c2=100.dat", "Fig3B_simulation_lambda=0.010_r=0.1_p=0.100_c2=100.dat", "Fig3B_simulation_lambda=0.010_r=0.1_p=0.010_c2=100.dat", "Fig3B_simulation_lambda=0.010_r=0.1_p=0.001_c2=100.dat",
	                          "Fig3C_simulation_lambda=0.001_r=0.1_p=1.000_c2=001.dat", "Fig3C_simulation_lambda=0.001_r=0.1_p=0.100_c2=001.dat", "Fig3C_simulation_lambda=0.001_r=0.1_p=0.010_c2=001.dat", "Fig3C_simulation_lambda=0.001_r=0.1_p=0.001_c2=001.dat",
	                          "Fig3D_simulation_lambda=0.001_r=0.1_p=1.000_c2=100.dat", "Fig3D_simulation_lambda=0.001_r=0.1_p=0.100_c2=100.dat", "Fig3D_simulation_lambda=0.001_r=0.1_p=0.010_c2=100.dat", "Fig3D_simulation_lambda=0.001_r=0.1_p=0.001_c2=100.dat",
	                          "Fig3E_simulation_lambda=0.001_r=1.0_p=1.000_c2=001.dat", "Fig3E_simulation_lambda=0.001_r=1.0_p=0.100_c2=001.dat", "Fig3E_simulation_lambda=0.001_r=1.0_p=0.010_c2=001.dat", "Fig3E_simulation_lambda=0.001_r=1.0_p=0.001_c2=001.dat",
	                          "Fig3F_simulation_lambda=0.001_r=1.0_p=1.000_c2=100.dat", "Fig3F_simulation_lambda=0.001_r=1.0_p=0.100_c2=100.dat", "Fig3F_simulation_lambda=0.001_r=1.0_p=0.010_c2=100.dat", "Fig3F_simulation_lambda=0.001_r=1.0_p=0.001_c2=100.dat"};

	double lambda[24] = {0.01 , 0.01 , 0.01 , 0.01 ,
	                     0.01 , 0.01 , 0.01 , 0.01 ,
	                     0.001, 0.001, 0.001, 0.001,
	                     0.001, 0.001, 0.001, 0.001,
	                     0.001, 0.001, 0.001, 0.001,
	                     0.001, 0.001, 0.001, 0.001};

	double c2[24] = {  1,   1,   1,   1,
	                 100, 100, 100, 100,
	                   1,   1,   1,   1,
	                 100, 100, 100, 100,
	                   1,   1,   1,   1,
	                 100, 100, 100, 100};

	double r[24] = {0.1, 0.1, 0.1, 0.1,
	                0.1, 0.1, 0.1, 0.1,
	                0.1, 0.1, 0.1, 0.1,
	                0.1, 0.1, 0.1, 0.1,
	                1  , 1  , 1  , 1  ,
	                1  , 1  , 1  , 1  };

	double p[24] = {1, 0.1, 0.01, 0.001,
	                1, 0.1, 0.01, 0.001,
	                1, 0.1, 0.01, 0.001,
	                1, 0.1, 0.01, 0.001,
	                1, 0.1, 0.01, 0.001,
	                1, 0.1, 0.01, 0.001};

	int min_exponent[24] = {1,  1,  1,  1,
	                        1,  1,  1,  1,
	                        1,  1,  1,  1,
	                        1,  1,  1,  1,
	                        9, 10, 11, 12,
	                        9, 10, 11, 12};

	double c1=1;

	int n = 1000;
	int N = 16;

	int i, m;

	double f, avg, stderror;

	FILE *datafile[24];

	void (*c2_r_p_set)(short int, double, double, double, double*, double*, double*)=c2_r_p;

	for (m=0; m<24; m++)
	{
		datafile[m] = fopen (filename[m], "w");

		for (i=min_exponent[m]; i<40; i++)
		{
			f = 0.01*pow(10,i/(double)10);
			printf("file number = %d, f = %lf\n", m, f);
			c2_r_p_set (1, r[m], p[m], c2[m], 0, 0, 0);
			calculate (f, c2_r_p_set, lambda[m], c1, n, N, 0, 0, &avg, &stderror);
			fprintf (datafile[m], "%lf %lf %lf\n", f, avg, stderror);
		}

		fclose (datafile[m]);
	}

	return 0;
}

void c2_r_p (short int flag, double r_param, double p_param, double c2_param, double *r, double *p, double *c2)
{
	static double r_set, p_set, c2_set;

	if (flag==1) {r_set=r_param; p_set=p_param; c2_set=c2_param;}
	else if (flag==2) {*r=r_set; *p=p_set; *c2=c2_set;}

	return;
}
