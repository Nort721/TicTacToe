#include "tictactoe.h"

/*
 * ToDo:
 * - make GUI
 */

const int ROWS = 3;
const int COLS = 3;
char board[3][3];

const char PLAYER = 'X';
const char COMPUTER = 'O';

char get_winner(char board[3][3]);
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

		winner = get_winner(board);

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

char get_winner(char board_arg[3][3])
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

void reset_board()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			board[i][j] = ' ';
		}
	}
}

void print_board()
{
	//system("clear");
	printf("\n %c | %c | %c \n", board[0][0], board[0][1], board[0][2]);
	printf("---|---|--- \n");
	printf(" %c | %c | %c \n", board[1][0], board[1][1], board[1][2]);
	printf("---|---|--- \n");
	printf(" %c | %c | %c \n", board[2][0], board[2][1], board[2][2]);
}

bool is_pos_not_legal(int row, int col)
{
	return row < 0 || col < 0 || row > 3 || col > 3 || board[row][col] != ' ';
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

	board[row][col] = PLAYER;
	printf("player marked [%d, %d]\n", row+1, col+1);
}

struct move {
	int row;
	int col;
};
typedef struct move move;

// bruteforce searches for winning moves
move bf_winning_move()
{
	// generate all possible moves
	//struct move possibleMoves[9];
	int index = 0;
	move mv;

	move * possible_moves_ptr = malloc(sizeof(mv));

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
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
		char board_copy[3][3];

		// copies board data to boardCopy
		for (int idx = 0; idx < 3; idx++)
		{
			for (int j = 0; j < 3; j++)
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

	free(possible_moves_ptr);

	// no winning move, return an illegal move as a flag
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
		row = rand() % ROWS;
		//sleep(1);
		col = rand() % COLS;
		move_origin = 2;
	}

	board[row][col] = COMPUTER;

	char *org = (move_origin == 2 ? "RND" : "AI");

	printf("computer marked [%d, %d][%s]\n", row+1, col+1, org);
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


