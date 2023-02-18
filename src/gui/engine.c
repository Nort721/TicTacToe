#include "../gui/headers/engine.h"

/*
 * compiling command
 * gcc -shared -fPIC engine.c uiapi.c -I./headers -o engine.so
 */

const char PLAYER = 'P';
const char COMPUTER = 'C';

const int ROWS = 3;
const int COLS = 3;
char board[ROWS][COLS];

void init_engine(void) {
	srand(time(NULL));

	// init the board
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			board[i][j] = ' ';
		}
	}
}

char decide_first_turn()
{
	return (rand() % 10 > 5 ? -1 : 1) == -1 ? COMPUTER : PLAYER;
}

char get_winner(char board_arg[ROWS][COLS])
{
	for (int i = 0; i < 3; i++)
	{
		// check rows
		if (board_arg[i][0] == board_arg[i][1] && board_arg[i][1] == board_arg[i][2] && board_arg[i][2] != ' ')
		{
			return board_arg[i][0];
		}

		// check colms
		if (board_arg[0][i] == board_arg[1][i] && board_arg[1][i] == board_arg[2][i] && board_arg[2][i] != ' ')
		{
			return board_arg[0][i];
		}
	}

	// right to left diagonal
	if (board_arg[0][0] == board_arg[1][1] && board_arg[1][1] == board_arg[2][2])
	{
		return board_arg[1][1];
	}
	else
	{
		if (board_arg[0][2] == board_arg[1][1] && board_arg[1][1] == board_arg[2][0])
		{
			return board_arg[0][2];
		}
	}

	return ' ';
}

char get_winner_wrapped()
{
	return get_winner(board);
}

bool is_pos_not_legal(int row, int col)
{
	return row < 0 || col < 0 || row > ROWS || col > COLS || board[row][col] != ' ';
}

void player_move(int row, int col)
{
	board[row][col] = PLAYER;
}

// bruteforce searches for winning moves
move bf_winning_move()
{
	// generate all possible moves
	//struct move possibleMoves[9];
	int index = 0;
	move mv;

	move * possible_moves_ptr = malloc(sizeof(mv));

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (board[i][j] == ' ')
			{
				move * new_possible_moves_ptr = realloc(possible_moves_ptr, index * sizeof(mv));
				new_possible_moves_ptr[index].row = i;
				new_possible_moves_ptr[index].col = j;
				index++;

				possible_moves_ptr = new_possible_moves_ptr;
			}
		}
	}

	int possible_moves_ptr_size = index * sizeof(mv);

	/*
	 * for each possible move copy the board in
	 * its current state and apply the move, than
	 * check for winner
	 */
	for (int i = 0; i < possible_moves_ptr_size; i++)
	{
		char board_copy[ROWS][COLS];

		// copies board data to boardCopy
		for (int idx = 0; idx < ROWS; idx++)
		{
			for (int j = 0; j < COLS; j++)
			{
				board_copy[idx][j] = board[idx][j];
			}
		}

		move next_move = possible_moves_ptr[i];

		// check if that move can make computer or player win
		board_copy[next_move.row][next_move.col] = COMPUTER;
		char winner_comp = get_winner(board_copy);
		board_copy[next_move.row][next_move.col] = PLAYER;
		char winner_plyr = get_winner(board_copy);

		if (winner_comp != ' ' || winner_plyr != ' ')
		{
			return next_move;
		}
	}

	//free(possible_moves_ptr);

	// no winning move, return an illegal move as a flag
	move no_win_move;
	no_win_move.row = -1;
	no_win_move.col = -1;

	return no_win_move;
}

move computer_move()
{
	int row = -1;
	int col = -1;

	/*
	 * Check to see if there is a winning move for the
	 * computer or the opponent, if there is a winning
	 * move for computer than take it, otherwise block
	 * the opponent
	 */
	move winning_move = bf_winning_move();
	row = winning_move.row;
	col = winning_move.col;

	/*
	 * illegal position means bot logic
	 * wasn't able to generate a move in
	 * which case will generate a random
	 * move
	 */
	while (is_pos_not_legal(row, col))
	{
		row = rand() % ROWS;
		col = rand() % COLS;
	}

	board[row][col] = COMPUTER;

	move final_move;
	final_move.row = row;
	final_move.col = col;

	return final_move;
}

int get_empty_spaces_amount()
{
	int empty_spaces = 9;

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (board[i][j] != ' ')
			{
				empty_spaces--;
			}
		}
	}

	return empty_spaces;
}
