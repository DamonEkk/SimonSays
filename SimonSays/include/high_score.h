#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define SCORE_ROWS 5

extern volatile char *high_scores[SCORE_ROWS][2];
extern volatile char *name;
extern volatile char *score;


void Place_score(int row);

void Override_score(char *new_name, char *new_score, uint8_t position);

void Check_score(void);

void High_score_init(void);