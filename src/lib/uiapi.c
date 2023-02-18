#include "../lib/headers/uiapi.h"

const char NO_WINNER_CONTINUE_GAME = 'N';
const char NO_WINNER_END_GAME = 'X';
const char ILLEGAL_POSITION = 'I';

void init()
{
	init_engine();
}

char get_first_turn()
{
	return decide_first_turn();
}

char on_player_turn(int row, int col)
{
	if (is_pos_not_legal(row, col))
	{
		return ILLEGAL_POSITION;
	}

	player_move(row, col);

	char winner = get_winner_wrapped();

	if (winner == ' ')
	{
		if (get_empty_spaces_amount() == 0)
		{
			return NO_WINNER_END_GAME;
		}

		return NO_WINNER_CONTINUE_GAME;
	}

	return winner;
}

ai_turn play_computer_turn()
{
	move ai_move = computer_move();

	ai_turn turn;
	turn.row = ai_move.row;
	turn.col = ai_move.col;

	char winner = get_winner_wrapped();

	if (winner == ' ')
	{
		if (get_empty_spaces_amount() == 0)
		{
			turn.flag = NO_WINNER_END_GAME;
		}
		else
		{
			turn.flag = NO_WINNER_CONTINUE_GAME;
		}
	}
	else
	{
		turn.flag = winner;
	}

	return turn;
}
