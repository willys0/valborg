#include "../parallel.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void concat_thrd(distribution dist, int id, par_array* out, void* f, void* p, void* args) {
	int size = dist.b_size[id];

	const par_array* A = dist.arrs + 0;
	const par_array* B = dist.arrs + 1;
	maybe* A_values = A->a + dist.blocks[id];
	maybe* B_values = B->a + dist.blocks[id];
	int base = dist.blocks[id];

	int A_len = length(*A);

	for(int i = base; i < base + size; i++) {
		if(i < A_len) {
			out->a[i] = A_values[i - base];	
		} else {
			out->a[i] = B_values[i - base - A_len];	
		}
	}
}

par_array par_concat(const par_array a, const par_array b) {
	distribution dist;
	par_array res_array;

	int res_size = length(a) + length(b);

	par_array arrs[] = {a, b};
	dist = distribute(arrs, 2, DISTRIBUTION_SUM);

	res_array = execute_in_parallel(concat_thrd, dist, a.m, a.m + res_size - 1, NULL, NULL, NULL);
	free_distribution(dist);

	return res_array;
}
