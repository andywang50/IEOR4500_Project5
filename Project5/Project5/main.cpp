#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string>

//#include <gurobi_c++.h>	
int arbitrage(double *prices, int n, double K, double r, int N, double *x);
/*
	read data from datafile.
 */
int readit(const char *filename, int *address_of_n, int *address_of_K, double *address_of_r, int *address_of_N, double **pprices)
{
	int readcode = 0, fscancode;
	FILE *datafile = NULL;
	char buffer[100];
	int n, K, i, j, N;
	double r;
	double *prices = NULL;

	datafile = fopen(filename, "r");
	if (!datafile) {
		printf("cannot open file %s\n", filename);
		readcode = 2;  return readcode;
	}

	printf("reading data file %s\n", filename);

	fscanf(datafile, "%s", buffer);
	fscancode = fscanf(datafile, "%s", buffer);
	if (fscancode == EOF) {
		printf("problem: premature file end at ...\n");
		readcode = 4; return readcode;
	}

	n = *address_of_n = atoi(buffer);

	fscanf(datafile, "%s", buffer);
	fscanf(datafile, "%s", buffer);
	K = *address_of_K = atoi(buffer);




	fscanf(datafile, "%s", buffer);
	fscanf(datafile, "%s", buffer);
	r = *address_of_r = atof(buffer);

	fscanf(datafile, "%s", buffer);
	fscanf(datafile, "%s", buffer);
	N = *address_of_N = atoi(buffer);

	printf("n = %d\n", n);
	printf("K = %d\n", K);
	printf("r = %f\n", r);
	printf("N = %d\n", N);


	prices = (double *)calloc(n*(K + 1), sizeof(double));
	if (prices == NULL) {
		printf("not enough memory for lb\n"); readcode = 3; goto BACK;
	}

	for (i = 0; i <= K; i++) {
		fscanf(datafile, "%s", buffer);
		for (j = 0; j < n; j++) {
			fscanf(datafile, "%s", buffer);
			prices[i*n + j] = atof(buffer);
		}
	}

	*pprices = prices;

	fclose(datafile);

BACK:
	return readcode;
}

int main(void)
{
	int retcode = 0;

	int n, K, N;
	double r;

	double *prices = NULL;
	double *x = NULL;

	retcode = readit("arb.dat", &n, &K, &r, &N, &prices);

	for (int i = 0; i <= K; ++i) {
		printf("Scenario_%d: ", i);
		for (int j = 0; j < n; ++j) {
			printf("%f ", prices[i*n + j]);
		}
		printf("\n");
	}

	retcode = arbitrage(prices, n, K, r, N, x);
	if (retcode == 1) {
		printf("Arb found.\n");
	}
	else {
		printf("No Arb found.\n");
	}

BACK:
	printf("\nexiting with retcode %d\n", retcode);
	return retcode;
}
