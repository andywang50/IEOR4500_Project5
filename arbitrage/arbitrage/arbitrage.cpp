#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <gurobi_c++.h>	

int arbitrage(double *prices, int n, double K, double r, int N, double *x) {
	int retcode = 0;
	int num_var = 2 * n + 1;
	double *obj = NULL;
	double *lb = NULL;
	double *ub = NULL;
	char *vtypes = NULL;
	GRBenv   *env = NULL;
	GRBmodel *model = NULL;
	int *cind;
	double rhs;
	char sense;
	double *cval;
	int numnonz;
	double optimal = 0.0;

	//char *mynames[] = { "x0", "x1", "x2", "x3", "x4", "x5", "x6", "y1", "y2", "y3", "y4", "y5", "y6"};



	retcode = GRBloadenv(&env, "arb.log");
	if (retcode) goto BACK;

	/* Create initial model */
	retcode = GRBnewmodel(env, &model, "arb", num_var,
		NULL, NULL, NULL, NULL, NULL);
	if (retcode) goto BACK;

	/** next we create the 4 columns **/
	obj = (double *)calloc(num_var, sizeof(double));
	ub = (double *)calloc(num_var, sizeof(double));
	lb = (double *)calloc(num_var, sizeof(double));
	x = (double *)calloc(num_var, sizeof(double));
	vtypes = (char *)calloc(num_var, sizeof(char));

	for (int i = 0; i < num_var; ++i) {
		if (i == 0) {
			obj[i] = 1.0;
			vtypes[i] = 'C';
		}
		else if (i <= n) {
			obj[i] = prices[i - 1];
			vtypes[i] = 'C';
		}
		else {
			obj[i] = 0.0;
			vtypes[i] = 'B';

		}
		ub[i] = 1.0;
		lb[i] = -1.0;
	}


	/* initialize variables */
	for (int j = 0; j < num_var; j++) {
		std::string var_name;
		if (j <= n) {
			var_name = "x" + std::to_string(j);
		}
		else {
			var_name = "y" + std::to_string(j - n);
		}
		char var_name_arr[1024];
		strcpy(var_name_arr, var_name.c_str());

		retcode = GRBsetstrattrelement(model, "VarName", j, var_name_arr);
		if (retcode) goto BACK;

		retcode = GRBsetdblattrelement(model, "Obj", j, obj[j]);
		if (retcode) goto BACK;

		retcode = GRBsetdblattrelement(model, "LB", j, lb[j]);
		if (retcode) goto BACK;

		retcode = GRBsetdblattrelement(model, "UB", j, ub[j]);
		if (retcode) goto BACK;

		retcode = GRBsetcharattrelement(model, "VType", j, vtypes[j]);
		if (retcode) goto BACK;
	}

	/** now we will add one constraint at a time **/
	/** we need to have a couple of auxiliary arrays **/

	cind = (int *)calloc(num_var, sizeof(int));
	cval = (double *)calloc(num_var, sizeof(double));

	for (int i = 0; i < num_var; ++i) {
		cind[i] = 0;
		cval[i] = 0;
	}

	for (int i = 1; i <= K; ++i) {
		for (int j = 0; j <= n; ++j) {
			cind[j] = j;
			if (j == 0) {
				cval[j] = 1.0 + r;
			}
			else {
				cval[j] = prices[i*n + j - 1];
			}
		}
		numnonz = n + 1;
		rhs = 0.0;
		sense = GRB_GREATER_EQUAL;
		retcode = GRBaddconstr(model, numnonz, cind, cval, sense, rhs, "scenario_constr");
		if (retcode) goto BACK;
	}

	for (int i = 1; i <= n; ++i) {
		for (int j = 0; j < num_var; ++j) {
			cind[j] = 0;
			cval[j] = 0;
		}
		numnonz = 2;
		rhs = 0.0;
		sense = GRB_GREATER_EQUAL;
		cind[0] = i;
		cval[0] = 1;
		cind[1] = i + n;
		cval[1] = 1;
		retcode = GRBaddconstr(model, numnonz, cind, cval, sense, rhs, "binary_constr");
		if (retcode) goto BACK;

		sense = GRB_LESS_EQUAL;
		cval[1] = -1;
		retcode = GRBaddconstr(model, numnonz, cind, cval, sense, rhs, "binary_constr");
		if (retcode) goto BACK;

	}


	for (int j = 0; j < num_var; ++j) {
		cind[j] = 0;
		cval[j] = 0;
	}
	numnonz = n;
	rhs = N;
	sense = GRB_LESS_EQUAL;
	for (int j = n + 1; j < num_var; ++j) {
		cind[j - n - 1] = j;
		cval[j - n - 1] = 1;
	}
	retcode = GRBaddconstr(model, numnonz, cind, cval, sense, rhs, "max_limit");
	if (retcode) goto BACK;

	retcode = GRBupdatemodel(model);
	if (retcode) goto BACK;


	/** optional: write the problem **/

	//retcode = GRBwrite(model, "C:\\Users\\AndyWang\\Desktop\\4500\\Project5\\Project5\\mysecond.lp");
	retcode = GRBwrite(model, "mysecond.lp");

	if (retcode) goto BACK;


	retcode = GRBoptimize(model);
	if (retcode) goto BACK;


	/** get solution **/


	retcode = GRBgetdblattrarray(model,
		GRB_DBL_ATTR_X, 0, num_var,
		x);
	if (retcode) goto BACK;

	/** now let's see the values **/

	for (int j = 0; j < num_var; j++) {
		std::string var_name;
		if (j <= n) {
			var_name = "x" + std::to_string(j);
		}
		else {
			var_name = "y" + std::to_string(j - n);
		}
		char var_name_arr[1024];
		strcpy(var_name_arr, var_name.c_str());
		printf("%s = %g\n", var_name_arr, x[j]);
	}
	for (int i = 0; i <= n; ++i) {
		optimal += x[i] * obj[i];
	}
	printf("Optimal Obj: %g\n", optimal);
	if (optimal < -1.0 * 1e-6) {
		retcode = 1;
	}

	GRBfreeenv(env);


BACK:
	//The function returns 0 if there is no arbitrage, and 1 if there is
	return retcode;

}
