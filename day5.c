#include "stdio.h"
#include "ctype.h"
#include "string.h"
#include "stdlib.h"

#define MAXBUF 2048
#define MAP_BUF_SIZE 32
#define NUM_MAPS 7

struct range {
	unsigned long dest;
	unsigned long source;
	unsigned long length;
};
int main(int argc, char** argv) {
	if (argc < 2) {
		puts("provide filename");
		return -1;
	}

	FILE* f = fopen(argv[1], "r");
	char line[MAXBUF];

	struct range** ranges = calloc(NUM_MAPS, sizeof(void*));
	for (int m = 0; m < NUM_MAPS; m++) {
		ranges[m] = calloc(MAP_BUF_SIZE, sizeof(struct range));
	}
	int map_ind = -1;
	int range_ind = 0;
	int num_ranges[NUM_MAPS];
	memset(num_ranges, 0, NUM_MAPS * sizeof(int));
	unsigned long seeds[56];
	int num_seeds = 0;

	int line_num = 0;
	while (fgets(line, MAXBUF, f)) {
		if (line_num == 0) {
			// get seeds
			char* tok = strtok(line, ":"); // go over "seeds:"
			while ((tok = strtok(NULL, " "))) {
				unsigned long n = strtol(tok, NULL, 10);
				seeds[num_seeds] = n;
				num_seeds++;
			}
		}
		else if (isdigit(line[0])) {
			char* tok = strtok(line, " ");
			ranges[map_ind][range_ind].dest = strtol(tok, NULL, 10);
			tok = strtok(NULL, " ");
			ranges[map_ind][range_ind].source = strtol(tok, NULL, 10);
			tok = strtok(NULL, " ");
			ranges[map_ind][range_ind].length = strtol(tok, NULL, 10);

			printf("%lu %lu %lu\n", ranges[map_ind][range_ind].dest, ranges[map_ind][range_ind].source, ranges[map_ind][range_ind].length);  
			range_ind++;
			num_ranges[map_ind]++;
		}
		else if (isalpha(line[0])) {
			// new map
			map_ind++;
			range_ind = 0;
			puts("");
		}
		line_num++;
	}

	// now we have everything in memory; the seeds and the ranges
	unsigned long lowest_location = -1; // maximum number an unsigned long can represent
	for (int s = 0; s < num_seeds; s++) {
		unsigned long c = seeds[s];
		printf("seed %i: %lu\n", s, c);
		for (int m = 0; m < NUM_MAPS; m++) {
			printf(" map number %i\n", m);
			for (int r = 0; r < num_ranges[m]; r++) {
				if (c < ranges[m][r].source) continue;
				if (c >= ranges[m][r].source + ranges[m][r].length) continue;
				unsigned long offs = c - ranges[m][r].source;
				c = ranges[m][r].dest + offs;
				printf("  fits range %i\n  outcome: %lu\n", r, c);
				break;
			}
		}
		lowest_location = c < lowest_location ? c : lowest_location;
		puts("");
	}
	puts("");

	puts("RANGED--------------");
	unsigned long lowest_range_location;
	//for (int s = 0; s < num_seeds; s += 2) {
	//	printf("seed #%i\n", s);
	//	for (unsigned long sc = seeds[s]; sc < seeds[s] + seeds[s+1]; sc += seeds[s+1]/4) {
		for (unsigned long sc = 4018529087; sc < 4135337336; sc++) {
			unsigned long c = sc;
			//printf(" seed: %lu\n", c);
			for (int m = 0; m < NUM_MAPS; m++) {
				//printf("  map number %i\n", m);
				for (int r = 0; r < num_ranges[m]; r++) {
					if (c < ranges[m][r].source) continue;
					if (c >= ranges[m][r].source + ranges[m][r].length) continue;
					unsigned long offs = c - ranges[m][r].source;
					c = ranges[m][r].dest + offs;
					//printf("   fits range %i\n   outcome: %lu\n", r, c);
					break;
				}
			}
			if (c < lowest_range_location) {
				lowest_range_location = c;
				printf("      LOWEST FOUND: %lu\n", lowest_range_location);
			}
		}
	//	puts("");
	//}

	printf("lowest location: %lu\n", lowest_location);
	printf("lowest location with ranges: %lu\n", lowest_range_location);

	// free memory
	for (int m = 0; m < NUM_MAPS; m++) {
		free(ranges[m]);
	}
	free(ranges);
	return 0;
}
