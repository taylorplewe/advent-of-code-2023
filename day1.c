#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#define MAXBUF 999
#define WORDBUF 16

const char* numwords[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

void clear_buf(void*, int, int*);
int main(int argc, char** argv) {
	if (argc < 2) return -1;

	FILE* f = fopen(argv[1], "r");
	char line[MAXBUF];
	unsigned long sum = 0;
	while (fgets(line, MAXBUF, f)) {
		char num1filled = 0;
		char num[3];
		unsigned long len = strlen(line);
		for (unsigned long i = 0; i < len; i++) {
			// is num digit
			if (line[i] >= '0' && line[i] <= '9') {
				if (!num1filled) {
					num[0] = line[i];
					num[1] = line[i];
					num1filled = 1;
				}
				else num[1] = line[i];
			} else {
				for (int w = 0; w < 10; w++) {
					if (line[i] == numwords[w][0]) {
						int match = 1;
						for (int c = 0; c < strlen(numwords[w]); c++) {
							if (line[i + c] == numwords[w][c]) continue;
							else {
								match = 0;
								break;
							}
						}
						// use match
						if (match) {
							if (!num1filled) {
								num[0] = w + 0x30;
								num[1] = w + 0x30;
								num1filled = 1;
							}
							else num[1] = w + 0x30;
							//i += strlen(numwords[w]) - 1;
						}
					}
				}
			}
		}
		num[2] = 0;
		sum += atoi(num);
	}
	puts("sum:");
	printf("%lu\n", sum);

	puts("end");
	return 0;
}
void clear_buf(void* buf, int buflen, int* ind) {
	memset(buf, 0, buflen);
	*ind = 0;
}
