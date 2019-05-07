#include "../parallel.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void get_thrd(distribution dist, int id, par_array* out, void* f, void* p, void* args) {
	int size = dist.b_size[id];

	int (*func)(int i) =
		(int (*)(int i)) f;
	int (*pred)(int i) =
		(int (*)(int i)) p;

	const par_array* A = dist.arrs;
	maybe* A_values = A->a;
	int base = dist.blocks[id];

	int src_i = 0;

	for(int i = base; i < base + size; i++) {
		src_i = func( L2G(*A, i) );

		// Check predicate here
		out->a[i] = A_values[ G2L(*A, src_i) ];	
	}
}

par_array par_get(const par_array a, int (*f)(int i), int (*p)(int i)) {
	distribution dist;
	par_array res_array;

	dist = distribute(&a, 1, DISTRIBUTION_STRICT);

	res_array = execute_in_parallel(get_thrd, dist, a.m, a.n, f, p, NULL);
	free_distribution(dist);

	return res_array;
}
