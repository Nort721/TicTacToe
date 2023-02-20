/*
 * uiapi.h
 *
 *  Created on: 18 Feb 2023
 *      Author: nort
 */

#ifndef LIB_HEADERS_UIAPI_H_
#define LIB_HEADERS_UIAPI_H_

#include <stdio.h>
#include <stdlib.h>

#include "../../lib/headers/engine.h"

void init();
char get_first_turn();
char on_player_turn(int row, int col);
ai_turn play_computer_turn();

#endif /* LIB_HEADERS_UIAPI_H_ */
