#include <stdio.h>
#include <math.h>

void c2_r_p (short int flag, double r_param, double p_param, double c2_param, double *r, double *p, double *c2);

int main (int argc, char *argv[])
{
	char filename[4][100] = {"Fig5_formula_lambda=0.001_r=0.1_p=1.000_c2=001.dat", "Fig5_formula_lambda=0.001_r=0.1_p=0.100_c2=001.dat", "Fig5_formula_lambda=0.001_r=0.1_p=0.010_c2=001.dat", "Fig5_formula_lambda=0.001_r=0.1_p=0.001_c2=001.dat"};

	double lambda[4] = {0.001, 0.001, 0.001, 0.001};

	double c2[4] = {1, 1, 1, 1};

	double r[4] = {0.1, 0.1, 0.1, 0.1};

	double p[4] = {1, 0.1, 0.01, 0.001};

	double c1[4] = {1, 0.1, 0.01, 0.001};

	int min_exponent[4] = {0, 0, 0, 0};

	int N = 1000;

	int i, m;

	double f, T;

	FILE *datafile[4];

	for (m=0; m<4; m++)
	{
		datafile[m] = fopen (filename[m], "w");

		for (i=0; i<=N; i++)
		{
			f = 0.01*pow(10000,i/(double)N);
			T = 1/f;

			fprintf (datafile[m], "%lf %lf\n", f, c1[m]/T+c2[m]*lambda[m]*(exp(r[m]*T)-1)/r[m]/T*(1-(1-exp(-r[m]*T))/log(1-p[m])));
		}

		fclose (datafile[m]);
	}

	return 0;
}
