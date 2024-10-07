#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define MAXBUF 2048
#define NUM_WINNING_NUMS 10
#define NUM_REGULAR_NUMS 25

int main(int argc, char** argv) {
	if (argc < 2) {
		puts("provide input filename");
		return -1;
	}

	FILE* f = fopen(argv[1], "r");
	char line[MAXBUF];
	unsigned long sum = 0;

	// get number of cards
	int num_cards = 0;
	while (fgets(line, MAXBUF, f)) {
		num_cards++;
	}
	rewind(f);

	unsigned int cards[num_cards];
	for (int i = 0; i < num_cards; i++) {
		cards[i] = 1;	
	}
	int line_num = 0;
	while (fgets(line, MAXBUF, f)) {
		printf("card %i: %i\n", line_num+1, cards[line_num]);
		// get winning nums for this card
		char* tok = strtok(line, ":");
		int winning_nums[NUM_WINNING_NUMS];
		for (int i = 0; i < NUM_WINNING_NUMS; i++) {
			tok = strtok(NULL, " ");
			winning_nums[i] = atoi(tok);
		}

		// get regular nums for this card
		int bonus = 0;
		tok = strtok(NULL, " ");
		for (int i = 0; i < NUM_REGULAR_NUMS; i++) {
			tok = strtok(NULL, " ");
			int num = atoi(tok);
			for (int j = 0; j < NUM_WINNING_NUMS; j++) {
				if (num == winning_nums[j]) {
					bonus++;
					break;
				}
			}
		}

		// get new cards based on bonus
		if (bonus) {
			for (int i = line_num + 1; i <= line_num + bonus; i++) {
				cards[i] += cards[line_num];
			}
		}

		line_num++;
	}
	for (int i = 0; i < num_cards; i++) {
		sum += cards[i];
	}

	printf("sum: %lu\n", sum);
	return 0;
}
