#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define SCORE_ROWS 5

volatile char *high_scores[SCORE_ROWS][2];
volatile char *name = "john";
volatile char *score = "10";

/*
Setter function that sets the specific row's name and score

@Param int row used to find the row needing to be altered
*/
void Place_score(int row){
    high_scores[row][0] = name;
    high_scores[row][1] = score;
}

/*
This function is called if there is a lower score than the current being saved score.
It recursivly calls itself passing in the old values till it reaches the bottom.
This function essentially makes the leaderboard slotable.

@Param String new_name the name we want to override the old name with.
@Param String new_score the score we want to override the old score with.
@Param uint8_t position the position in the 2D array we want to alter.
*/
void Override_score(char *new_name, char *new_score, uint8_t position){
    // Checks if the recursion needs to continue. 
    if ((position < SCORE_ROWS) && (!(high_scores[position][0] == NULL))){
        // Creates temp values of the old values so they arent lost and can be passed to the lower position.
        char *temp_name = high_scores[position][0];
        char *temp_score = high_scores[position][1];
        
        name = new_name;
        score = new_score;
        Place_score(position);
        
        position++;
        Override_score(temp_name, temp_score, position);
    }
    // final loop of recursion. Sets values but doesnt continue the cycle.
     else{
        name = new_name;
        score = new_score;
        Place_score(position);
        
     }
}

/*
 Checks if the current score beats any / null values scores.
*/
void Check_score(void){
    for (uint8_t i = 0; i < SCORE_ROWS; i++){
        // if highscore row is null
        if (high_scores[i][0] == NULL){
            Place_score(i);
            printf("%s %s\n", high_scores[i][0], high_scores[i][1]);
            break;
        }
            
        // if highscore row is lower than current score
        else if (atoi(score) > atoi(high_scores[i][1])){
            Override_score(name, score, i);
            break;
        } 
    }
}

/*
 Fills the 2d array with null values.
*/
void High_score_init(void){
    for (uint8_t i = 0; i < SCORE_ROWS; i++){
        for (uint8_t k = 0; k < 2; k++){
            high_scores[i][k] = NULL;
        }
    }
}