#include <stdio.h>
#include <math.h>

void a_r_p (short int flag, double r_param, double p_param, double c2_param, double *r, double *p, double *c2);
void c2_a_p (short int flag, double r_param, double p_param, double c2_param, double *r, double *p, double *c2);
void c2_r_a (short int flag, double r_param, double p_param, double c2_param, double *r, double *p, double *c2);

int main (int argc, char *argv[])
{
	char filename[12][100] = {"Fig4B_formula_lambda=0.001_a=0.001000_r=0.1_p=0.01.dat", "Fig4B_formula_lambda=0.001_a=0.003162_r=0.1_p=0.01.dat", "Fig4B_formula_lambda=0.001_a=0.01000_r=0.1_p=0.01.dat", "Fig4B_formula_lambda=0.001_a=0.03162_r=0.1_p=0.01.dat",
	                          "Fig4D_formula_lambda=0.001_c2=1_a=08_p=0.01.dat"       , "Fig4D_formula_lambda=0.001_c2=1_a=16_p=0.01.dat"       , "Fig4D_formula_lambda=0.001_c2=1_a=32_p=0.01.dat"      , "Fig4D_formula_lambda=0.001_c2=1_a=64_p=0.01.dat"      ,
	                          "Fig4F_formula_lambda=0.001_c2=1_r=0.1_a=0.0001.dat"    , "Fig4F_formula_lambda=0.001_c2=1_r=0.1_a=0.001.dat"     , "Fig4F_formula_lambda=0.001_c2=1_r=0.1_a=0.01.dat"     , "Fig4F_formula_lambda=0.001_c2=1_r=0.1_a=0.1.dat"      };

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

	int min_exponent[12] = {0, 0, 0, 0,
	                        3, 2, 1, 0,
	                        0, 0, 0, 0};

	int max_exponent[12] = {30, 30, 30, 30,
	                        30, 30, 30, 30,
	                        40, 40, 40, 40};

	double c1=1;

	int N = 1000;

	int i, m;

	double f, T, C;

	FILE *datafile[12];

	for (m=0; m<12; m++)
	{
		datafile[m] = fopen (filename[m], "w");

		for (i=0; i<=N; i++)
		{
			f = 0.01*pow(10,min_exponent[m]/(double)10)*pow(pow(10,max_exponent[m]/(double)10)/pow(10,min_exponent[m]/(double)10),i/(double)N);
			T = 1/f;

			if (m<4) C = 1/T+lambda[m]/sqrt(acos(-1)*c2[m])*(exp(r[m]*T)-1)/r[m]/T*(1-(1-exp(-r[m]*T))/log(1-p[m]));
			else if (m<8) C = 1/T+lambda[m]*c2[m]*sqrt(acos(-1)*r[m])/T*exp(T*T/4/r[m])*(erf(T/2/sqrt(r[m]))+(1-2/log(1-p[m]))*(1-exp(-T*T/4/r[m])));
			else C = 1/T+lambda[m]*c2[m]*(exp(r[m]*T)-1)/r[m]/T*(1+p[m]*(1-exp(-r[m]*T))/(p[m]+(1-p[m])*log(1-p[m])));

			fprintf (datafile[m], "%lf %lf\n", f, C);
		}

		fclose (datafile[m]);
	}

	char filename2[12][100] = {"Fig4A_a=0.001000.dat", "Fig4A_a=0.003162.dat", "Fig4A_a=0.01000.dat", "Fig4A_a=0.03162.dat",
	                           "Fig4C_a=08.dat"      , "Fig4C_a=16.dat"      , "Fig4C_a=32.dat"     , "Fig4C_a=64.dat"     ,
	                           "Fig4E_a=0.0001.dat"  , "Fig4E_a=0.001.dat"   , "Fig4E_a=0.01.dat"   , "Fig4E_a=0.1.dat"    };

	for (m=0; m<4; m++)
	{
		datafile[m] = fopen (filename2[m], "w");

		for (i=0; i<=N; i++)
		{
			f = 50*i/(double)N;
			C = 2*sqrt(c2[m]/acos(-1))*exp(-c2[m]*f*f);
			fprintf (datafile[m], "%lf %lf\n", f, C);
		}

		fclose (datafile[m]);
	}

	for (m=4; m<8; m++)
	{
		datafile[m] = fopen (filename2[m], "w");

		for (i=0; i<=N; i++)
		{
			f = 0.8*i/(double)N;
			C = 2*r[m]*f*exp(-r[m]*f*f);
			fprintf (datafile[m], "%lf %lf\n", f, C);
		}

		fclose (datafile[m]);
	}

	for (m=8; m<12; m++)
	{
		datafile[m] = fopen (filename2[m], "w");

		for (i=0; i<=N; i++)
		{
			f = 0.000001*pow(p[m]/0.000001,i/(double)N);
			C = -log(1-f)/(p[m]+(1-p[m])*log(1-p[m]));
			fprintf (datafile[m], "%.8lf %lf\n", f, C);
		}
		fprintf (datafile[m], "%lf %lf\n", f, 0.001);

		fclose (datafile[m]);
	}

	return 0;
}
