#include "../lib/headers/engine.h"

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

static char get_winner(char board_arg[ROWS][COLS])
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
static move bf_winning_move()
{
	// generate all possible moves
	int index = 0;
	move mv;

	// allocate enough memory for maximum amount of moves
	move * possible_moves_ptr = malloc(ROWS * COLS * sizeof(mv));

	if (possible_moves_ptr == NULL)
	{
		fprintf(stderr, "Error: failed to allocate memory for possible moves\n");
		exit(1);
	}

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (board[i][j] == ' ')
			{
				possible_moves_ptr[index].row = i;
				possible_moves_ptr[index].col = j;
				index++;
			}
		}
	}

	/*
	 * for each possible move copy the board in
	 * its current state and apply the move, than
	 * check for winner
	 */
	move * winning_moves = malloc(ROWS * COLS * sizeof(mv));
	int winning_moves_index = 0;
	move * player_blocking_moves = malloc(ROWS * COLS * sizeof(mv));
	int blocking_moves_index = 0;
	for (int i = 0; i < index; i++)
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

		if (winner_comp != ' ')
		{
			winning_moves[winning_moves_index] = next_move;
			winning_moves_index++;
		}

		if (winner_plyr != ' ')
		{
			player_blocking_moves[blocking_moves_index] = next_move;
			blocking_moves_index++;
		}
	}

	// first priority - winning moves
	if (winning_moves_index > 0)
	{
		move the_winning_move = winning_moves[rand() % winning_moves_index];
		free(winning_moves);
		return the_winning_move;
	}

	// second priority - block the player from winning
	if (blocking_moves_index > 0)
	{
		move cock_block_move = player_blocking_moves[rand() % blocking_moves_index];
		free(player_blocking_moves);
		return cock_block_move;
	}

	// no winning or blocking move found, check for second best move
	for (int i = 0; i < index; i++)
	{
		move next_move = possible_moves_ptr[i];

		move surroundings[8];

		surroundings[0].row = next_move.row + 1; // row down
		surroundings[1].row = next_move.row - 1; // row up
		surroundings[2].col = next_move.col + 1; // col right
		surroundings[3].col = next_move.col - 1; // col left

		/*
		X 0 0
		0 1 0
		0 0 0
		*/
		surroundings[4].row = next_move.row - 1;
		surroundings[4].row = next_move.col - 1;

		/*
		0 0 0
		0 1 0
		0 0 X
		*/
		surroundings[5].row = next_move.row + 1;
		surroundings[5].row = next_move.col + 1;

		/*
		0 0 X
		0 1 0
		0 0 0
		*/
		surroundings[6].row = next_move.row - 1;
		surroundings[6].row = next_move.col + 1;

		/*
		0 0 0
		0 1 0
		X 0 0
		*/
		surroundings[7].row = next_move.row + 1;
		surroundings[7].row = next_move.col - 1;

		move * legal_second_best_moves = malloc(ROWS * COLS * sizeof(mv));
		int sindex = 0;

		for (int j = 0; j < 8; j++)
		{
			move index_move = surroundings[j];

			// if pos is legal, check whats in it on the board
			if (!is_pos_not_legal(index_move.row, index_move.col))
			{
				// if one of its surroundings is ours, its a second best move
				if (board[index_move.row][index_move.col] == COMPUTER)
				{
					legal_second_best_moves[sindex] = index_move;
					sindex++;
				}
			}
		}

		// if there are any legal second best moves, return a random one
		if (sindex > 0)
		{
			move second_best_move = legal_second_best_moves[rand() % sindex];
			free(possible_moves_ptr);
			free(legal_second_best_moves);
			return second_best_move;
		}

		free(legal_second_best_moves);
	}

	free(possible_moves_ptr);

	// no winning or blocking or second-best move, return an illegal move as a flag
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
