#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define MAXBUF 2048
#define SMALLBUF 256

int extrap(int** arr, int num, int lvl, int accumulator) {
	for (int i = 0; i < num; i++) {
		printf("%i ", arr[lvl][i]);
	}
	//accumulator += arr[lvl-1][num];
	accumulator = arr[lvl-1][0] - accumulator;
	printf("lvl: %i num: %i accum: %i\n", lvl, num, accumulator);
	if (lvl > 1) return extrap(arr, num + 1, lvl - 1, accumulator);
	else return accumulator;
}

int check(int** arr, int num, int lvl) {
	int all_zeros = 1;
	for (int i = 1; i < num; i++) {
		int diff = (int)(arr[lvl-1][i]) - (int)(arr[lvl-1][i-1]);
		arr[lvl][i - 1] = diff;
		if (diff) all_zeros = 0;
	}
	if (!all_zeros) return check(arr, num-1, lvl + 1);
	return extrap(arr, num - 1, lvl, 0);
}

int main(int argc, char** argv) {
	if (argc < 2) {
		puts("provide filename");
		return -1;
	}

	FILE* f = fopen(argv[1], "r");
	char line[MAXBUF];
	int lines = 0;
	unsigned long sum = 0;
	while (fgets(line, MAXBUF, f)) {
		int nums[MAXBUF];
		int c = 0;
		char* tok = strtok(line, " ");
		nums[0] = (int)strtol(tok, NULL, 10);
		c++;
		while ((tok = strtok(NULL, " "))) {
			nums[c] = (int)strtol(tok, NULL, 10);
			c++;
		}

		// init tree for this line
		int** tree = calloc(c, sizeof(int*));
		for (int i = 0; i < c; i++) {
			tree[i] = calloc(c, sizeof(int));
		}
		memcpy(tree[0], nums, c * sizeof(int));

		// do the stuff for this tree
		sum += check((int**)tree, c, 1);
		for (int i = 0; i < c; i++) {
			for (int j = 0; j < c-i; j++) {
				//printf("%i ", tree[i][j]);
			}
			//puts("");
		}

		// free that shiiii
		for (int i = 0; i < c; i++) {
			//free(tree[i]);
		}
		lines++;
		free(tree);
	}
	printf("sum: %lu\n", sum);

	return 0;
}
