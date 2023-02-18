/*
 * uimanager.h
 *
 *  Created on: 18 Feb 2023
 *      Author: nort
 */

#ifndef GUI_HEADERS_UIAPI_H_
#define GUI_HEADERS_UIAPI_H_

#include <stdio.h>
#include <stdlib.h>
#include "engine.h"

void init();
char get_first_turn();
char on_player_turn(int row, int col);
ai_turn play_computer_turn();

#endif /* GUI_HEADERS_UIAPI_H_ */
