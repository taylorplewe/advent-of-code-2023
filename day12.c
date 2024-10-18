#include "stdio.h"
#include "stdlib.h"
#include "string.h"

long unsigned get_num_possible_sequences(char*, int, int*, int, int);
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
        sum += get_num_possible_sequences(field, 0, seq, seq_len, 0);
    }

    printf("sum: %lu\n", sum);

    // wrap up
    fclose(f);
    return 0;
}

// .?#
long unsigned get_num_possible_sequences(char* field, int field_ind, int* seq, int seq_len, int seq_ind) {
    for (int i = field_ind; i < strlen(field); i++) {
        int seq_val = seq[seq_ind];
        int lookahead_ind = 1;
        while (lookahead_ind <= seq_val)
    }

    puts("");

    return 1;
}