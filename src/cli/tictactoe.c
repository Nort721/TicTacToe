#include "../cli/tictactoe.h"

/*
 * ToDo:
 * - separate this cli version of the project to a separate project file
 */

const int _ROWS = 3;
const int _COLS = 3;
char _board[3][3];

const char _PLAYER = 'X';
const char _COMPUTER = 'O';

char get_winner(char _board[3][3]);
void reset_board();
void print_board();
void player_move();
void computer_move();
int get_empty_spaces_amount();


int current_turn;

int main(void) {
	printf("\n=+= TicTacToe =+=\n\n");

	srand(time(NULL));

	char winner = ' ';

	reset_board();
	print_board();

	current_turn = rand() % 10 > 5 ? -1 : 1;

	while (get_empty_spaces_amount() != 0 && winner == ' ')
	{
		if (current_turn == 1)
		{
			player_move();
		}
		else
		{
			computer_move();
		}

		print_board();

		winner = get_winner(_board);

		current_turn *= -1;
	}

	if (winner == ' ')
	{
		printf("\nno winner.\n");
	}
	else
	{
		printf("\n%c has won!", winner);
	}

	printf("\nDone!\n");
	return EXIT_SUCCESS;
}

char get_winner(char _board_arg[3][3])
{
	for (int i = 0; i < 3; i++)
	{
		// check __ROWS
		if (_board_arg[i][0] == _board_arg[i][1] && _board_arg[i][1] == _board_arg[i][2] && _board_arg[i][2] != ' ')
		{
			return _board_arg[i][0];
		}

		// check colms
		if (_board_arg[0][i] == _board_arg[1][i] && _board_arg[1][i] == _board_arg[2][i] && _board_arg[2][i] != ' ')
		{
			return _board_arg[0][i];
		}
	}

	// right to left diagonal
	if (_board_arg[0][0] == _board_arg[1][1] && _board_arg[1][1] == _board_arg[2][2])
	{
		return _board_arg[1][1];
	}
	else
	{
		if (_board_arg[0][2] == _board_arg[1][1] && _board_arg[1][1] == _board_arg[2][0])
		{
			return _board_arg[0][2];
		}
	}

	return ' ';
}

void reset_board()
{
	for (int i = 0; i < _ROWS; i++)
	{
		for (int j = 0; j < _COLS; j++)
		{
			_board[i][j] = ' ';
		}
	}
}

void print_board()
{
	//system("clear");
	printf("\n %c | %c | %c \n", _board[0][0], _board[0][1], _board[0][2]);
	printf("---|---|--- \n");
	printf(" %c | %c | %c \n", _board[1][0], _board[1][1], _board[1][2]);
	printf("---|---|--- \n");
	printf(" %c | %c | %c \n", _board[2][0], _board[2][1], _board[2][2]);
}

bool is_pos_not_legal(int row, int col)
{
	return row < 0 || col < 0 || row > 3 || col > 3 || _board[row][col] != ' ';
}

void player_move()
{
	int row = -1;
	int col = -1;
	int round = 0;

	while (is_pos_not_legal(row, col))
	{
		if (round > 0)
		{
			printf("Bad position.\n");
		}

		printf("row: ");
		scanf("%d", &row);
		row--;

		printf("column: ");
		scanf("%d", &col);
		col--;

		round++;
	}

	_board[row][col] = _PLAYER;
	printf("player marked [%d, %d]\n", row+1, col+1);
}

// bruteforce searches for winning moves
move bf_winning_move()
{
	// generate all possible moves
	int index = 0;
	move mv;

	// allocate enough memory for maximum amount of moves
	move * possible_moves_ptr = malloc(_ROWS * _COLS * sizeof(mv));

	if (possible_moves_ptr == NULL)
	{
		fprintf(stderr, "Error: failed to allocate memory for possible moves\n");
		exit(1);
	}

	for (int i = 0; i < _ROWS; i++)
	{
		for (int j = 0; j < _COLS; j++)
		{
			if (_board[i][j] == ' ')
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
	move * winning_moves = malloc(_ROWS * _COLS * sizeof(mv));
	int winning_moves_index = 0;
	move * player_blocking_moves = malloc(_ROWS * _COLS * sizeof(mv));
	int blocking_moves_index = 0;
	for (int i = 0; i < index; i++)
	{
		char board_copy[_ROWS][_COLS];

		// copies board data to boardCopy
		for (int idx = 0; idx < _ROWS; idx++)
		{
			for (int j = 0; j < _COLS; j++)
			{
				board_copy[idx][j] = _board[idx][j];
			}
		}

		move next_move = possible_moves_ptr[i];

		// check if that move can make computer or player win
		board_copy[next_move.row][next_move.col] = _COMPUTER;
		char winner_comp = get_winner(board_copy);
		board_copy[next_move.row][next_move.col] = _PLAYER;
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

		move * legal_second_best_moves = malloc(_ROWS * _COLS * sizeof(mv));
		int sindex = 0;

		for (int j = 0; j < 8; j++)
		{
			move index_move = surroundings[j];

			// if pos is legal, check whats in it on the board
			if (!is_pos_not_legal(index_move.row, index_move.col))
			{
				// if one of its surroundings is ours, its a second best move
				if (_board[index_move.row][index_move.col] == _COMPUTER)
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

void computer_move()
{
	printf("\ncalculating move . . .\n");
	int move_origin = 1;

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
		row = rand() % _ROWS;
		//sleep(1);
		col = rand() % _COLS;
		move_origin = 2;
	}

	_board[row][col] = _COMPUTER;

	char *org = (move_origin == 2 ? "RND" : "AI");

	printf("computer marked [%d, %d][%s]\n", row+1, col+1, org);
}

int get_empty_spaces_amount()
{
	int empty_spaces = 9;

	for (int i = 0; i < _ROWS; i++)
	{
		for (int j = 0; j < _COLS; j++)
		{
			if (_board[i][j] != ' ')
			{
				empty_spaces--;
			}
		}
	}

	return empty_spaces;
}

