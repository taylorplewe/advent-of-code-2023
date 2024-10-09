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

    long unsigned fsize = get_file_size(f);
    long unsigned line_len = get_line_len(f);

    int cols_with_no_stars[line_len*2];
    memset(cols_with_no_stars, 0, line_len*2 * sizeof(int));
    int num_cols_with_no_stars = 0;

    // check for cols with no stars
    for (int c = 0; c < line_len; c++) {
        int col_has_star = 0;

        char line[4096];
        while (fgets(line, 4096, f)) {
            if (line[c] == '#') {
                col_has_star = 1;
                break;
            }
        }
        fseek(f, 0, SEEK_SET);
        if (!col_has_star) {
            cols_with_no_stars[c + num_cols_with_no_stars] = 1;
            num_cols_with_no_stars++;
        }
    }

    // now set the star field with appropriate spaces
    int r = 0;
    int c = 0;
    int ch;
    char* starfield = calloc(fsize * 4, sizeof(char));
    while ((ch = fgetc(f)) && ch != EOF) {
        starfield[r * line_len + c] = ch;
        if (cols_with_no_stars[c]) {
            c++;
            starfield[r * line_len + c] = ch;
        }
        if (c == '\n') {
            c = 0;
            r++;
        } else {
            c++;
        }
    }

    puts(starfield);

    fclose(f);
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