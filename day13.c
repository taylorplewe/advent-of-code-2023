#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
#include "string.h"

#define MAXBUF 2048
#define SMALLBUF 256

char** get_grid() {
	char** grid = calloc(SMALLBUF, sizeof(char*));
	for (int i = 0; i < SMALLBUF; i++) {
		grid[i] = calloc(SMALLBUF, 1);
	}
	return grid;
}
void free_grid(char** grid) {
	for (int i = 0; i < SMALLBUF; i++) {
		free(grid[i]);
	}
	free(grid);
}

int comp_2_cols(char** grid, int h, int c1, int c2) {
	char line1[MAXBUF];
	char line2[MAXBUF];
	memset(line1, 0, MAXBUF);
	memset(line2, 0, MAXBUF);
	for (int r = 0; r < h; r++) {
		line1[r] = grid[r][c1];
		line2[r] = grid[r][c2];
	}
	if (strcmp(line1, line2) != 0) return 0;
	
	return 1;
}
#define check_hor_top ((r*2)-(i+1))
int check_hor(char** grid, int h, int r) {
	for (int i = r-1; i >= 0 && check_hor_top < h; i--) {
		if (strcmp(grid[i], grid[check_hor_top]) != 0) return 0;
	}
	return 1;
}
#define check_ver_right ((c*2)-(i+1))
int check_ver(char** grid, int w, int h, int c) {
	for (int i = c-1; i >= 0 && check_ver_right < w; i--) {
		//printf("comparing columns %i and %i\n", i, check_ver_right);
		if (!comp_2_cols(grid, h, i, check_ver_right)) return 0;
	}
	return 1;
}
int get_res(char** grid, int w, int h) {
	// check for a horizontal mirror
	for (int r = 1; r < h; r++) {
		if (strcmp(grid[r-1], grid[r]) == 0) {
			if (check_hor(grid, h, r)) {
				printf("hor at %i\n", r);
				return r * 100;
			}
		}
	}
	
	// check for a vertical mirror
	for (int c = 1; c < w; c++) {
		if (comp_2_cols(grid, h, c, c-1)) {
			if (check_ver(grid, w, h, c)) {
				printf("ver at %i\n", c);
				return c;
			}
		}
	}
	return 0;
}

int check_smudge(char** grid, int w, int h) {
	// check for horizontal mirror
	int smudges = 0;
	for (int r = 1; r < h; r++) {
		smudges = 0;
		for (int i = r-1; i >= 0 && check_hor_top < h; i--) {
			for (int c = 0; c < w; c++) {
				if (grid[i][c] != grid[check_hor_top][c]) {
					smudges++;
				}
			}
			if (smudges > 1) break;
		}
		if (smudges == 1) {
			printf("horizontal smudge mirror at %i\n", r);
			return r * 100;
		}
	}

	// check for vertical mirror
	for (int c = 1; c < w; c++) {
		smudges = 0;
		for (int i = c-1; i >= 0 && check_ver_right < w; i--) {
			for (int r = 0; r < h; r++) {
				if (grid[r][i] != grid[r][check_ver_right]) {
					smudges++;
				}
			}
			if (smudges > 1) break;
		}
		if (smudges == 1) {
			printf("vertical smudge mirror at %i\n", c);
			return c;
		}
	}
	return 0;
}

int main(int argc, char** argv) {
	if (argc < 2) {
		puts("provide filename");
		return -1;
	}

	FILE* f = fopen(argv[1], "r");
	char line[MAXBUF];

	char** grid = 0;
	int r = 0;
	int len = 0;
	int grid_num = 0;
	unsigned long sum = 0;
	unsigned long smudge_sum = 0;
	while (fgets(line, MAXBUF, f)) {
		if (!grid) grid = get_grid();
		if (isspace(line[0])) {
			printf("grid %i\n", grid_num);
			int res = get_res(grid, len, r);
			sum += res;
			printf("%i\n", res);
			int smudge_mirror = check_smudge(grid, len, r);
			smudge_sum += smudge_mirror;
			puts("");
			free_grid(grid);
			grid = 0;
			r = 0;
			grid_num++;
		} else {
			len = strlen(line) - 1;
			strcpy(grid[r], line);
			r++;
		}
	}
	// last one
	int res = get_res(grid, len, r);
	printf("%i\n", res);
	sum += res;
	smudge_sum += check_smudge(grid, len, r);
	printf("sum: %lu\n", sum);
	printf("smudge sum: %lu\n", smudge_sum);

	return 0;
}
