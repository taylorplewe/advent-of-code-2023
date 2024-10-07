#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define MAXBUF 2048
#define MAXRED 12
#define MAXGREEN 13
#define MAXBLUE 14
#define RED "red"
#define GREEN "green"
#define BLUE "blue"

struct game {
	int highest_red;
	int highest_green;
	int highest_blue;
	int id;
};

int main(int argc, char** argv) {
	if (argc < 2) {
		puts("please provide filename");
		return -1;
	}

	struct game games[200];
	FILE* f = fopen(argv[1], "r");
	char line[MAXBUF];
	int l = 0;
	unsigned long sum = 0;
	unsigned long power_sum = 0;
	while (fgets(line, MAXBUF, f)) {
		int game_id;
		sscanf(line, "Game %i", &game_id);
		printf("%i\n", game_id);
		games[l].id = game_id;
		games[l].highest_red = 0;
		games[l].highest_green = 0;
		games[l].highest_blue = 0;

		// get draw config
		char* tok = strtok(line, ":;");
		while ((tok = strtok(NULL, ":;"))) {
			//puts(tok);
			int nums[3];
			char colors[3][9];
			memset(colors, 0, 3 * 9);
    			sscanf(tok, "%i %[a-z], %i %[a-z], %i %[a-z]", &nums[0], colors[0], &nums[1], colors[1], &nums[2], colors[2]);
			for (int i = 0; i < 3; i++) {
				if (strcmp(colors[i], RED) == 0) {
					if (nums[i] > games[l].highest_red) games[l].highest_red = nums[i];
				} else if (strcmp(colors[i], GREEN) == 0) {
					if (nums[i] > games[l].highest_green) games[l].highest_green = nums[i];
				} else if (strcmp(colors[i], BLUE) == 0) {
					if (nums[i] > games[l].highest_blue) games[l].highest_blue = nums[i];
				}
			}
		}

		printf("highest red: %i\nhighest green: %i\nhighest blue: %i\n", games[l].highest_red, games[l].highest_green, games[l].highest_blue);

		// possibly add this game's id to sum if it's possible
		if (
			games[l].highest_red <= MAXRED
			&& games[l].highest_green <= MAXGREEN
			&& games[l].highest_blue <= MAXBLUE
		) {
			printf(" I'M POSSIBLE: %i\n", games[l].id);
			sum += games[l].id;
		}

		power_sum += games[l].highest_red * games[l].highest_green * games[l].highest_blue;
		l++;
	}
	printf("sum: %lu\n", sum);
	printf("power sum: %lu\n", power_sum);

	return 0;
}
