#include "stdio.h"
#include "stdlib.h"
#include "string.h"

long unsigned get_num_possible_sequences(char*, int*, int);
int main(int argc, char** argv) {
	if (argc < 2) {
		puts("provide filename");
		return -1;
	}
	FILE* f = fopen(argv[1], "r");

    unsigned long sum = 0;
    char line[4096];
    while (fgets(line, 4096, f)) {
        // get spring field and broken spring sequence
        char* field = strtok(line, " ");
        char* seq_text = strtok(NULL, " ");
        int seq[32];
        int seq_len = 0;
        char* seq_num = strtok(seq_text, ",");
        while (seq_num != NULL) {
            seq[seq_len++] = atoi(seq_num);
            seq_num = strtok(NULL, ",");
        }
        
        // calculate how many possible arrangements of broken/fixed spring sequences there are
        sum += get_num_possible_sequences(field, seq, seq_len);
    }

    printf("sum: %lu\n", sum);

    // wrap up
    fclose(f);
    return 0;
}

// .?#
long unsigned get_num_possible_sequences(char* field, int* seq, int seq_len) {
	long unsigned sum = 0;
	int field_len = strlen(field);
    for (int i = 0; i < field_len; i++) {
		printf("seq[0]: %d field: %s\n", seq[0], field);
        int lookahead_ind = 1;
        while (lookahead_ind <= seq[0]) {
			if (i + lookahead_ind >= field_len) return (seq_len == 1) ? sum : 0;
			char c = field[i + lookahead_ind];
			if (lookahead_ind == seq[0]) {
				if (c == '.' || c == '?') {
					if (i + lookahead_ind + 1 >= field_len)
						return (seq_len == 1) ? 1 : 0;
					sum += get_num_possible_sequences(field + i + lookahead_ind + 1, seq + 1, seq_len - 1);
				} else break; // #
			} else {
				if (c == '.') break;
			}
			lookahead_ind++;
		}
    }

    return sum;
}