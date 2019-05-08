#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "../src/runtime/sequential.h"
#include "../src/runtime/parallel.h"
#include "../src/runtime/runtime.h"

double square(double x) {
	return x*x;
}
double neg(double x) {
	return -x;
}
double sum2(double x, double y) {
	return x+y;
}
double mul3(double x, double y, double z) {
	return x*y*z;
}

int x_lt2(int i, double x, double y) {
	return x < 2.0;
}

int is_neg(int i, double x) {
	return x < 0.0;
}

int main(int argc, char** argv) {
	par_array R, R2; 	// Resulting array
	
	double arr1[] = { 	1.0, 	2.0, 	3.0, 	4.0,	5.0,	6.0 };
	double arr2[] = { 	3.0, 	4.0, 	5.0, 	6.0, 	7.0, 	8.0 };
	double arr3[] = { 	-1.0,	-2.0, 	-3.0, 	-4.0, 	-5.0, 	-6.0 };
	double arr4[] = { 	-1.0,	2.0, 	-3.0, 	-4.0, 	5.0, 	-6.0 };

	const par_array A = mk_array(arr1, 3, 8);
	const par_array B = mk_array(arr2, 0, 5);
	const par_array C = mk_array(arr3, 0, 5);
	const par_array D = mk_array(arr4, 0, 5);

	init_par_env();

	R = par_map1(square, A, NULL);
	printf("Square A:\n");
	for(int i = 0; i < length(R); i++) {
		printf("%f ", VAL(R.a[i]));
	}
	free(R.a);

	printf("\nSum A B:\n");
	R = par_map2(sum2, A, B, x_lt2);
	for(int i = 0; i < length(R); i++) {
		printf("%f ", VAL(R.a[i]));
	}
	free(R.a);

	printf("\nMul A B C:\n");
	R = par_map3(mul3, A, B, C, NULL);
	for(int i = 0; i < length(R); i++) {
		printf("%f ", VAL(R.a[i]));
	}
	free(R.a);

	R = par_map1(neg, D, is_neg);
	printf("Abs D:\n");
	for(int i = 0; i < length(R); i++) {
		printf("%f ", VAL(R.a[i]));
	}
	free(R.a);

	printf("\n");
	destroy_par_env();
	return 0;
}
