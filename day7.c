#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"

#define MAXBUF 2048

void init_ranks(char*);
int get_hand_type(char*);
int get_hand_score(char*, char*);
int check_five_kind(char*);
int check_four_kind(char*);
int check_full_house(char*);
int check_three_kind(char*);
int check_two_pair(char*);
int check_one_pair(char*);
int is_first_hand_stronger(char*, char*, char*);

int main(int argc, char** argv) {
	if (argc < 2) {
		puts("provide filename");
		return -1;
	}

	char* ranks = calloc(256, 1);
	init_ranks(ranks);

	FILE* f = fopen(argv[1], "r");
	char line[MAXBUF];

	int num_sets = 0;
	int num_types[7];
	memset(num_types, 0, 7 * sizeof(int));
	int types[7][MAXBUF];
	char hands[MAXBUF][6];
	memset(hands, 0, MAXBUF * 6);
	int bids[MAXBUF];
	int scores[MAXBUF];
	while (fgets(line, MAXBUF, f)) {
		sscanf(line, "%s %i", hands[num_sets], &bids[num_sets]);
		scores[num_sets] = get_hand_score(hands[num_sets], ranks);
		int type = get_hand_type(hands[num_sets]);
		if (type == 6) printf("%s %i\n", hands[num_sets], num_sets);
		types[type][num_types[type]] = num_sets;
		num_types[type]++;
		num_sets++;
	}

	// insertion sort by type
	int overall_rank = 1;
	unsigned long sum = 0;
	for (int t = 0; t < 7; t++) {
		for (int h = 1; h < num_types[t]; h++) {
			for (int s = h; s > 0 && scores[types[t][s]] < scores[types[t][s-1]]; s--) {
				int temp = types[t][s-1];
				types[t][s-1] = types[t][s];
				types[t][s] = temp;
			}
		}
		printf("type: %i\n", t);
		for (int h = 0; h < num_types[t]; h++) {
			printf(" %s %i\n", hands[types[t][h]], overall_rank);
			printf("  %i %i\n", bids[types[t][h]], bids[types[t][h]] * overall_rank);
			sum += bids[types[t][h]] * overall_rank;
			overall_rank++;
		}
		puts("");
	}

	// now do the thing
	printf("sum: %lu\n", sum);

	printf("%i\n", get_hand_type(argv[2]));
	return 0;
}
void init_ranks(char* ranks) {
	ranks['J'] = 0;
	ranks['2'] = 1;
	ranks['3'] = 2;
	ranks['4'] = 3;
	ranks['5'] = 4;
	ranks['6'] = 5;
	ranks['7'] = 6;
	ranks['8'] = 7;
	ranks['9'] = 8;
	ranks['T'] = 9;
	ranks['Q'] = 10;
	ranks['K'] = 11;
	ranks['A'] = 12;
}
int get_hand_type(char* hand) {
	int res = 0;
	(res = check_five_kind(hand))
	|| (res = check_four_kind(hand))
	|| (res = check_full_house(hand))
	|| (res = check_three_kind(hand))
	|| (res = check_two_pair(hand))
	|| (res = check_one_pair(hand));
	return res;
}
int get_hand_score(char* hand, char* ranks) {
	int score = 0;
	for (int i = 0; i < 5; i++) {
		score += ranks[hand[i]] * (int)pow(13, 4-i);
	}
	return score;
}
int check_five_kind(char* hand) {
	//if (hand[0] == hand[1] && hand[1] == hand[2] && hand[2] == hand[3] && hand[3] == hand[4])
	//	return 6;
	char unique_card = 0;
	for (int i = 0; i < 5; i++) {
		if (hand[i] == 'J') continue;
		if (hand[i] == unique_card) continue;
		if (!unique_card) unique_card = hand[i];
		else return 0;
	}
	return 6;
}
int check_four_kind(char* hand) {
	char totals[256];
	memset(totals, 0, 256);
	for (int i = 0; i < 5; i++) {
		if (hand[i] == 'J') totals[hand[i]]++;
	}
	for (int i = 0; i < 5; i++) {
		if (hand[i] == 'J') continue;
		totals[hand[i]]++;
		if (totals[hand[i]] + totals['J'] == 4) return 5;
	}
	return 0;
}
// only configuration where J could help here is any order of 1212J
int check_full_house(char* hand) {
	char totals[256];
	memset(totals, 0, 256);
	char three_card = 0;
	for (int i = 0; i < 5; i++) {
		if (hand[i] == 'J') totals[hand[i]]++;
	}
	for (int i = 0; i < 5; i++) {
		if (hand[i] == 'J') continue;
		totals[hand[i]]++;
		if (totals[hand[i]] == 3 || (totals[hand[i]] == 2 && totals['J'] > 0)) {
			three_card = hand[i];
		}
	}
	if (!three_card) return 0;
	memset(totals, 0, 256);
	for (int i = 0; i < 5; i++) {
		if (hand[i] == 'J') continue;
		totals[hand[i]]++;
		if (totals[hand[i]] == 2 && hand[i] != three_card) {
			return 4;
		}
	}
	return 0;
}
int check_three_kind(char* hand) {
	char totals[256];
	memset(totals, 0, 256);
	for (int i = 0; i < 5; i++) {
		if (hand[i] == 'J') totals[hand[i]]++;
	}
	for (int i = 0; i < 5; i++) {
		if (hand[i] == 'J') continue;
		totals[hand[i]]++;
		if (totals[hand[i]] + totals['J'] == 3) return 3;
	}
	return 0;
}
// there is never a configuration where a J could make this and not 3 of a kind
int check_two_pair(char* hand) {
	char totals[256];
	memset(totals, 0, 256);
	int pairs = 0;
	for (int i = 0; i < 5; i++) {
		if (totals[hand[i]] == -1) continue;
		totals[hand[i]]++;
		if (totals[hand[i]] == 2) {
			totals[hand[i]] = -1;
			pairs++;
			if (pairs == 2) return 2;
		}
	}
	return 0;
}
int check_one_pair(char* hand) {
	char totals[256];
	memset(totals, 0, 256);
	for (int i = 0; i < 5; i++) {
		if (hand[i] == 'J') return 1;
		totals[hand[i]]++;
		if (totals[hand[i]] == 2) return 1;
	}
	return 0;
}
