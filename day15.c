#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"

#define SMALLBUF 16

struct lens {
	char label[SMALLBUF];
	char focal_length;
	void* next;
};

struct lens* get_last_in_box(struct lens** boxes, unsigned int box_id, char* label) {
	if (!boxes[box_id]) {
		boxes[box_id] = calloc(1, sizeof(struct lens));
		return boxes[box_id];
	} else {
		struct lens* c = boxes[box_id];
		while (1) {
			if (strcmp(c->label, label) == 0) {
				puts("ayyyy lmao");
				return c;
			}
			if (c->next) c = c->next;
			else break;
		}
		struct lens* new_lens = calloc(1, sizeof(struct lens));
		c->next = new_lens;
		return new_lens;
	}
}
void remove_lens(struct lens** boxes, unsigned int box_id, char* label) {
	if (!boxes[box_id]) return;
	struct lens* l = boxes[box_id];
	struct lens* prev = 0;
	while (l) {
		if (strcmp(l->label, label) == 0) {
			if (prev) prev->next = l->next;
			else boxes[box_id] = 0;
			free(l);
			return;
		}
		prev = l;
		l = l->next;
	}
}
unsigned long print_boxes(struct lens** boxes) {
	unsigned long sum = 0;
	for (int i = 0; i < 256; i++) {
		printf("\n");
		int slot = 0;
		printf("box %i: ", i);
		struct lens* l = boxes[i];
		if (!l) continue;
		while (1) {
			slot++;
			sum += (i+1) * slot * (l->focal_length);
			printf("%s.%i ", l->label, l->focal_length);
			if (l->next) l = l->next;
			else break;
		}
	}
	printf("\n");
	return sum;
}
void free_me(struct lens* me) {
	if (!me) return;
	if (me->next) free_me(me->next);
	free(me);
}

int main(int argc, char** argv) {
	if (argc < 2) {
		puts("provide filename;");
		return -1;
	}

	FILE* f = fopen(argv[1], "r");
	char c;
	int i = 0;
	char* term = calloc(SMALLBUF, 1);
	unsigned int curr = 0;
	unsigned long sum = 0;

	// part 2
	struct lens** boxes = calloc(256, sizeof(struct lens*));
	struct lens c_lens;
	char label[SMALLBUF];
	char func;

	while ((c = fgetc(f)) && c != EOF) {
		if (c == '=') {
			strcpy(label, term);
		} else if (c == '-') {
			remove_lens(boxes, curr, label);
		} else if (isdigit(c)) {
			struct lens* c_lens = get_last_in_box(boxes, curr, label);
			strcpy(c_lens->label, label);
			c_lens->focal_length = c - 0x30;
		}
		if (c != ',') {
			if (isspace(c)) continue;
			curr = ((curr + c) * 17) % 256;
			term[i] = c;
			i++;
		} else {
			printf("%8s %4i %13lu\n", term, curr, sum);
			sum += curr;
			curr = 0;
			i = 0;
			int len = strlen(term);
			memset(term, 0, len);
		}
	}


	sum += curr;
	printf("\"%s\" %i\n", term, curr);
	printf("current value: %i\n", curr);
	printf("sum: %lu\n", sum);

	unsigned long pt2sum = print_boxes(boxes);
	printf("pt2 sum: %lu\n", pt2sum);

	// free
	free(term);
	for (int i = 0; i < 256; i++) {
		free_me(boxes[i]);
	}
	free(boxes);

	return 0;
}
