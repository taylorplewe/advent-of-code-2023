#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"

#define MAXBUF 2048

void clearline(char*);
int issymbol(char);
int getnum(char*, int*);
int main(int argc, char** argv) {
	if (argc < 2) {
		puts("please provide filename");
		return -1;
	}

	FILE* f = fopen(argv[1], "r");

	// init line strings	
	char prevline[MAXBUF], nextline[MAXBUF], line[MAXBUF];
	clearline(prevline);
	clearline(nextline);
	clearline(line);

	// first 2 lines go into line and nextline
	fgets(line, MAXBUF, f);
	fgets(nextline, MAXBUF, f);
	unsigned long len = strlen(line);

	// start loop
	unsigned long sum = 0;
	unsigned long gearsum = 0;
	int iter = 1;
	while (1) {
		// debug
		printf("%i\n", iter);
		//printf(" prevline: %s", prevline+100);
		//printf(" line: %s", line+100);
		//printf(" nextline: %s", nextline+100);
		//puts("");
		
		// first check on current line vals
		for (int i = 0; i < len; i++) {
			if (isdigit(line[i])) {
				// get length of number & number
				int numi = i + 1;
				while (numi < len && isdigit(line[numi])) {
					numi++;
				}
				numi -= i;
				//printf("%s\n", num);
				
				// check columns
				for (int j = i - 1 == -1 ? 0 : i - 1; j <= i + numi && j < len; j++) {
					if (issymbol(prevline[j]) || issymbol(line[j]) || issymbol(nextline[j])) {
						// part number is valid; add to sum
						char numstr[len];
						memset(numstr, 0, len);
						strncpy(numstr, line+i, numi);
						int num = atoi(numstr);
						//printf(" %i\n", num);
						sum += num;
						//printf(" sum: %lu\n", sum);
						break;
					}
				}

				// skip to end of number
				i += numi;
			}
			if (line[i] == '*') {
				int num1, num2;
				int num1filled = 0;
				int num2filled = 0;
				int morethan2 = 0;
				for (int j = i - 1 == -1 ? 0 : i - 1; j <= i + 1 && j < len; j++) {
					if (isdigit(prevline[j])) {
						if (!num1filled) {
							num1 = getnum(prevline, &j);
							num1filled++;
						}
						else if (!num2filled) {
							num2 = getnum(prevline, &j);
							num2filled++;
						} else {
							morethan2++;
							break;
						}
					}
				}
				for (int j = i - 1 == -1 ? 0 : i - 1; j <= i + 1 && j < len; j++) {
					if (morethan2) break;
					if (isdigit(line[j])) {
						if (!num1filled) {
							num1 = getnum(line, &j);
							num1filled++;
						}
						else if (!num2filled) {
							num2 = getnum(line, &j);
							num2filled++;
						} else {
							morethan2++;
							break;
						}
					}
				}
				for (int j = i - 1 == -1 ? 0 : i - 1; j <= i + 1 && j < len; j++) {
					if (morethan2) break;
					if (isdigit(nextline[j])) {
						if (!num1filled) {
							num1 = getnum(nextline, &j);
							num1filled++;
						}
						else if (!num2filled) {
							num2 = getnum(nextline, &j);
							num2filled++;
						} else {
							morethan2++;
							break;
						}
					}
				}
				if (morethan2) continue;
				if (num1filled && num2filled) {
					printf("  gear: %i %i\n", num1, num2);
					gearsum += num1 * num2;
				}
			}
		}
		
		// then increment lines
		if (!strlen(nextline)) break;
		strcpy(prevline, line);
		strcpy(line, nextline);
		char* res = fgets(nextline, MAXBUF, f);
		if (!res) clearline(nextline);
		iter++;
	}

	printf("sum: %lu\n", sum);
	printf("gear sum: %lu\n", gearsum);

	// end
	fclose(f);
	return 0;
}
void clearline(char* line) {
	memset(line, 0, MAXBUF);
}
int issymbol(char c) {
	if (c == '.') return 0;
	if (ispunct(c)) return 1;
	return 0;
}
int getnum(char* line, int* i) {
	// find start of number
	while (*i >= 1 && isdigit(line[(*i)-1])) {
		(*i)--;
	}

	int len = 0;
	int e = *i;
	// find length of number
	while (e < strlen(line) && isdigit(line[e])) {
		e++;
	}
	e -= *i;
	char num_str[MAXBUF];
	memset(num_str, 0, MAXBUF);
	strncpy(num_str, line+(*i), e);
	int num = atoi(num_str);
	*i += e;
	return num;
}
