#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define MAXBUF 2056

int main(int argc, char** argv) {
	if (argc < 2) {
		puts("provide filename");
		return -1;
	}

	FILE* f = fopen(argv[1], "r");
	char line[MAXBUF];

	int times[MAXBUF];
	int numraces = 0;

	// get times
	fgets(line, MAXBUF, f);
	char* tok = strtok(line, ":");
	int race = 0;
	while ((tok = strtok(NULL, " "))) {
		times[race] = strtol(tok, NULL, 10);
		race++;
	}
	numraces = race;

	// get records
	int records[numraces];
	race = 0;
	fgets(line, MAXBUF, f);
	tok = strtok(line, ":");
	while ((tok = strtok(NULL, " "))) {
		records[race] = strtol(tok, NULL, 10);
		race++;
	}

	// okay do math
	int numbeats[numraces];
	memset(numbeats, 0, numraces * sizeof(int));
	for (int r = 0; r < numraces; r++) {
		for (int t = 0; t < times[r]; t++) {
			if ((times[r] - t) * t > records[r]) numbeats[r]++;
		}
		printf(" ways to beat for race#%i: %i\n", r, numbeats[r]);
	}

	// total them up
	unsigned long sum = numbeats[0];
	for (int r = 1; r < numraces; r++) {
		sum *= numbeats[r];
	}

	printf("sum: %lu\n", sum);

	// part 2
	unsigned long time = 45988373;
	unsigned long record = 295173412781210;
	unsigned long numbeat = 0;
	for (int t = 0; t < time; t++) {
		if ((time - t) * t > record) numbeat++;
	}
	printf("# beat part2: %lu\n", numbeat);

	return 0;
}
