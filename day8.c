#include "stdio.h"
#include "math.h"
#include "string.h"
#include "ctype.h"

#define MAXBUF 2048

struct point {
	char left[4];
	char right[4];
};

int getkey(char*);
int main(int argc, char** argv) {
	if (argc < 2) {
		puts("provide filename");
		return -1;
	}

	FILE* f = fopen(argv[1], "r");
	char line[MAXBUF];

	// get instructions
	char instructions[MAXBUF];
	fgets(line, MAXBUF, f);	
	sscanf(line, "%s", instructions);
	
	struct point map[(int)pow(26, 3)];
	char a_endings[MAXBUF][4];
	int num_a_endings = 0;
	while (fgets(line, MAXBUF, f)) {
		if (!isalpha(line[0])) continue;
		char key[4], left[4], right[4];
		if (key[2] == 'A') {
			strcpy(a_endings[num_a_endings], key);
			num_a_endings++;
		}
		memset(key, 0, 4);
		memset(left, 0, 4);
		memset(right, 0, 4);
		strncpy(key, line, 3);
		strncpy(left, line+7, 3);
		strncpy(right, line+12, 3);
		strcpy(map[getkey(key)].left, left);
		strcpy(map[getkey(key)].right, right);
	}

	for (int i = 0; i < num_a_endings; i++) {
		puts(a_endings[i]);
	}

	int step = 0;
	int n = strlen(instructions);
	char loc[4];
	strcpy(loc, "AAA");
	while (strcmp(loc, "ZZZ") != 0) {
		//printf("%s -> %c\n", loc, instructions[step % n]);
		if (instructions[step % n] == 'L') strcpy(loc, map[getkey(loc)].left);
		else strcpy(loc, map[getkey(loc)].right);
		step++;
	}
	printf("steps: %i\n", step);

	unsigned long multi_steps = 0;
	int num_to_z[num_a_endings];
	memset(num_to_z, 0, num_a_endings * sizeof(int));
	for (int i = 0; i < num_a_endings; i++) {
		printf("%s\n", a_endings[i]);
		while (a_endings[i][2] != 'Z') {
			//printf("%s -> %c\n", loc, instructions[step % n]);
			if (instructions[num_to_z[i] % n] == 'L') strcpy(a_endings[i], map[getkey(a_endings[i])].left);
			else strcpy(a_endings[i], map[getkey(a_endings[i])].right);
			num_to_z[i]++;
		}
		printf(" stopped at %s in %i steps\n", a_endings[i], num_to_z[i]);
	}
	printf("part 2 steps: %lu\n", multi_steps);

	fclose(f);
	return 0;
}
int getkey(char* in) {
    int key = 0;
    for (int i = 0; i < 3; i++) {
        key += (in[i] - 0x41) * pow(26, 2-i);
    }
    return key;
}
