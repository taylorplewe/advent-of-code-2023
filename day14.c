#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAXBUF 2048

// call function on line ABOVE the first empty spot and ending with the index of the pound symbol
void roll(char** lines, int rs, int re, int col) {
	for (int r = rs; r < re; r++) {
		lines[r-1][col] = lines[r][col];
	}
	lines[re-1][col] = '.';
}

int find_len_and_roll(char** lines, int r, int col, int nr) {
	char c;
	int i = r;
	while ((c = lines[i][col]) && c != '#' && i < nr)
		i++;
	if (i == r + 1) return 0;
	roll(lines, r, i, col);
	return 1;
}
#define rollemup(r,c) (find_len_and_roll(lines, r+1, c, num_lines))

void print_board(char** lines, int nr, int num_cols) {
	for(int r = 0; r < 6; r++) {
		for (int c = 0; c < num_cols; c++) {
			putchar(lines[r][c]);
		}
		printf("\n");
	}
}
#define printboard print_board(lines, num_lines, num_columns)

int main(int argc, char** argv) {
	if (argc < 2) {
		puts("provide filename");
		return -1;
	}

	FILE* f = fopen(argv[1], "r");
	char** lines = calloc(MAXBUF, sizeof(char*));
	for (int i = 0; i < MAXBUF; i++) {
		lines[i] = calloc(MAXBUF, 1);
	}

	int num_lines = 0;
	int num_columns = 0;
	// first get number of lines
	while (fgets(lines[num_lines], MAXBUF, f)) {
		num_lines++;
	}
	rewind(f);
	num_columns = strlen(lines[0]) - 1;

	printboard;
	// roll rocks upwards
	for (int c = 0; c < num_columns; c++) {
		for (int r = 0; r < num_lines; r++) {
			if (lines[r][c] == '.') {
				if (rollemup(r,c)) r--;
			}
		}
	}
	printf("\n\n\nAFTER:\n");
	printboard;

	return 0;
}
