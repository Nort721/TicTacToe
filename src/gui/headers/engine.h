/*
 * engine.h
 *
 *  Created on: 18 Feb 2023
 *      Author: nort
 */

#ifndef GUI_ENGINE_H_
#define GUI_ENGINE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

struct move {
	int row;
	int col;
};
typedef struct move move;

struct ai_turn {
	int row;
	int col;
	char flag;
};
typedef struct ai_turn ai_turn;

void init_engine();
bool is_pos_not_legal(int row, int col);
void player_move(int row, int col);
move computer_move();
char get_winner_wrapped();
int get_empty_spaces_amount();
char decide_first_turn();

#endif /* GUI_ENGINE_H_ */
