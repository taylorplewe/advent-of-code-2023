#include "stdio.h"
#include "stdlib.h"
#include "string.h"

long unsigned get_file_size(FILE* f);
long unsigned get_line_len(FILE* f);
int main(int argc, char** argv) {
	if (argc < 2) {
		puts("provide filename");
		return -1;
	}
	FILE* f = fopen(argv[1], "r");
    char line[4096];

    long unsigned fsize = get_file_size(f);
    long unsigned line_len = get_line_len(f);
    long unsigned num_lines = fsize / line_len;

    int cols_with_no_stars[line_len];
    int rows_with_no_stars[num_lines];
    memset(cols_with_no_stars, 0, line_len * sizeof(int));
    memset(rows_with_no_stars, 0, num_lines * sizeof(int));

    // look for empty cols
    for (int c = 0; c < line_len; c++) {
        int col_has_star = 0;
        while (fgets(line, 4096, f)) {
            if (line[c] == '#') {
                col_has_star = 1;
                break;
            }
        }
        if (!col_has_star) {
            cols_with_no_stars[c] = 1;
        }
        fseek(f, 0, SEEK_SET);
    }

    // look for galaxies & empty rows
    int** galaxies = calloc(fsize, sizeof(int*));
    unsigned int galaxies_len = 0;
    unsigned int r = 0;
    while (fgets(line, 4096, f)) {
        int row_has_star = 0;
        for (unsigned int c = 0; c < line_len; c++) {
            if (line[c] == '#') {
                row_has_star = 1;
                galaxies[galaxies_len] = calloc(2, sizeof(int));
                galaxies[galaxies_len][0] = r;
                galaxies[galaxies_len++][1] = c;
            }
        }
        if (!row_has_star) {
            rows_with_no_stars[r] = 1;
        }
        r++;
    }

    // get distances
    unsigned long long NUM_EXTRA_EMPTY_SPACE_LINES = 999999;
    unsigned long long distance_sum = 0;
    for (int i = 0; i < galaxies_len; i++) {
        for (int j = i + 1; j < galaxies_len; j++) {
            unsigned long long dist = abs(galaxies[i][0] - galaxies[j][0]) + abs(galaxies[i][1] - galaxies[j][1]);
            unsigned int upper_galaxy = galaxies[i][0] < galaxies[j][0] ? i : j;
            unsigned int lower_galaxy = upper_galaxy == i ? j : i;
            unsigned int left_galaxy = galaxies[i][1] < galaxies[j][1] ? i : j;
            unsigned int right_galaxy = left_galaxy == i ? j : i;
            // empty space?
            for (int r = galaxies[upper_galaxy][0] + 1; r < galaxies[lower_galaxy][0]; r++) {
                if (rows_with_no_stars[r]) {
                    dist += NUM_EXTRA_EMPTY_SPACE_LINES;
                }
            }
            for (int c = galaxies[left_galaxy][1] + 1; c < galaxies[right_galaxy][1]; c++) {
                if (cols_with_no_stars[c]) {
                    dist += NUM_EXTRA_EMPTY_SPACE_LINES;
                }
            }
            distance_sum += dist;
        }
    }

    printf("distance sum: %llu\n", distance_sum);

    // wrap up
    for (int i = 0; i < galaxies_len; i++) {
        free(galaxies[i]);
    }
    free(galaxies);
    return 0;
}
long unsigned get_file_size(FILE* f) {
    fseek(f, 0, SEEK_END);
    long unsigned fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    return fsize;
}
long unsigned get_line_len(FILE* f) {
    char line[4096];
    fgets(line, 4096, f);
    return strlen(strtok(line, "\n"));
}