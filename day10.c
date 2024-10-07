#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define MAXBUF 2048
#define t(_x,_y,_d,_c) (steps[_y][_x] > step && check(map,_x,_y,_c) && (_d = traverse(map, steps, _x, _y, step+1)))
#define trav_l (x && t(x-1,y,l,'l'));
#define trav_u (y && t(x,y-1,u,'u'));
#define trav_r (!is_right && t(x+1,y,r,'r'));
#define trav_d (!is_bottom && t(x,y+1,d,'d'));

int check(char** map, unsigned char x, unsigned char y, char d) {
	char c = map[y][x];
	switch (d) {
		case 'u':
			if (c == 'F' || c == '|' || c == '7') return 1;
			return 0;
		case 'd':
			if (c == 'L' || c == '|' || c == 'J') return 1;
			return 0;
		case 'l':
			if (c == 'L' || c == '-' || c == 'F') return 1;
			return 0;
		case 'r':
		default:
			if (c == 'J' || c == '-' || c == '7') return 1;
			return 0;
	}
}
unsigned short traverse(char** map, unsigned short** steps, unsigned char x, unsigned char y, unsigned short step) {
	//printf("%c step: %hu location: %i, %i\n", map[y][x], step, x, y);
	steps[y][x] = step;
	char c = map[y][x];
	int is_right = (int)x == 139;
	int is_bottom = (int)y == 139;
	unsigned short u = 0;
	unsigned short d = 0;
	unsigned short l = 0;
	unsigned short r = 0;
	switch (c) {
		case 'S':
			trav_l
			trav_u
			trav_r
			trav_d
			break;
		case '|':
			trav_u
			trav_d
			break;
		case '-':
			trav_l
			trav_r
			break;
		case 'J':
			trav_u
			trav_l
			break;
		case 'L':
			trav_u
			trav_r
			break;
		case 'F':
			trav_r
			trav_d
			break;
		case '7':
		default:
			trav_l
			trav_d
			break;
	}

	if (u || d || l || r) {
		unsigned short h = u;
		h = d > h ? d : h;
		h = l > h ? l : h;
		h = r > h ? r : h;
		return h;
	}
	return step;
}

int main(int argc, char** argv) {
	if (argc < 2) {
		puts("provide filename");
		return -1;
	}

	FILE* f = fopen(argv[1], "r");
	char line[MAXBUF];

	// allocate memory for map and step map
	char** map = calloc(140, sizeof(char*));
	char** ins = calloc(140, sizeof(char*));
	unsigned short** steps = calloc(140, sizeof(short*));
	for (int i = 0; i < 140; i++) {
		map[i] = calloc(140, 1);
		ins[i] = calloc(140, 1);
		memset(ins[i], '.', 140);
		steps[i] = calloc(140, sizeof(unsigned short));
		memset(steps[i], -1, 140 * sizeof(unsigned short));
	}

	int num_line = 0;
	short start = 0;
	while (fgets(line, MAXBUF, f)) {
		for (int i = 0; i < strlen(line); i++) {
			map[num_line][i] = line[i];
			if (line[i] == 'S') {
				steps[num_line][i] = 0;
				start = (short)num_line<<8 | (short)i;
			}
		}
		num_line++;
	}

	// now we have the whole map in memory
	unsigned short farthest = traverse(map, steps, start & 0xff, start >> 8, 0);

	// print out step map
	for (int r = 0; r < 140; r++) {
		for (int c = 0; c < 140; c++) {
			//printf("%6hu", steps[r][c] == 65535 ? 0 : steps[r][c]);
		}
		//printf("\n");
	}

	// find out how many tiles are encased inside
	int num_inside = 0;
	for (int r = 0; r < 140; r++) {
		char inside = 0;
		for (int c = 0; c < 140; c++) {
			if (steps[r][c] == (unsigned short)-1) {
				if (inside) {
					num_inside++;
					ins[r][c] = 'I';
				}
			} else {
				switch (map[r][c]) {
					case 'L':
						if (!inside) inside = 'L';
						else if (inside == '|') inside = 'F';
						break;
					case 'F':
						if (!inside) inside = 'F';
						else if (inside == '|') inside = 'L';
						break;
					case 'J':
						if (inside == 'L') inside = 0;
						else if (inside == 'F') inside = '|';
						break;
					case '7':
						if (inside == 'F') inside = 0;
						else if (inside == 'L') inside = '|';
						break;
					case '|':
						if (!inside) inside = '|';
						else inside = 0;
					default:
						break;
				}
			}
		}
	}
	printf("num inside: %i\n", num_inside);

	// print inside map
	for (int r = 0; r < 140; r++) {
		for (int c = 0; c < 140; c++) {
			putchar(ins[r][c]);
		}
		printf("\n");
	}

	// free map and return
	for (int i = 0; i < 140; i++) {
		free(map[i]);
		free(steps[i]);
		free(ins[i]);
	}
	free(map);
	free(steps);
	free(ins);
	return 0;
}
