#include <stdio.h>
#include <stdlib.h>

// --- Game Config ---
#define BOARD_SIZE 15
#define WIN_COUNT 5
#define EMPTY '*'
#define BLACK 'X'
#define WHITE 'O'

// --- Function Prototypes ---
void initialize_board(char board[BOARD_SIZE][BOARD_SIZE]);
void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
int on_board(char board[BOARD_SIZE][BOARD_SIZE], int row, int col);
int is_valid_move(char board[BOARD_SIZE][BOARD_SIZE], int row, int col);
int check_win(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char curr_player_char);

int main() {
    char board[BOARD_SIZE][BOARD_SIZE];
    int curr_player = 1; // black: 1, white: 2
    char curr_player_char;
    int row, col;
    int moves = 0;

    initialize_board(board);

    printf("--- Hello Gomoku! ---\n");
    printf("Player 1 plays black, Player 2 plays white. First to consecutive 5 wins!\n");

    while (1) {
        print_board(board);

        curr_player_char = curr_player == 1 ? BLACK : WHITE;

        // Get player input
        printf("Turn to Player %d (%c), please input the position (format: row col): ", curr_player, curr_player_char);
        if (scanf("%d %d", &row, &col) != 2) {
            printf("Malformed input.\n");
            printf("Usage: row col\n");
            while (getchar() != '\n');
            continue;
        }
        if (!is_valid_move(board, row, col)) {
            printf("Invalid move. Please try again.\n");
            continue;
        }

        // Execute move
        board[row][col] = curr_player_char;

        // Check gameover
        if (check_win(board, row, col, curr_player_char)) {
            print_board(board);
            printf("\nCONGRATULATIONS!!!\n");
            printf("Player %d (%c) wins the game!\n", curr_player, curr_player_char);
            break;
        }
        if (moves == BOARD_SIZE * BOARD_SIZE) {
            print_board(board);
            printf("\nGame is a draw! No more moves possible.\n");
            break;
        }

        curr_player = 3 - curr_player;
    }
    return EXIT_SUCCESS;
}

void initialize_board(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }
}

void print_board(char board[BOARD_SIZE][BOARD_SIZE]) {
    printf("\n");
    for (int j = 0; j <= BOARD_SIZE; j++) {
        printf("%2d", j);
    }
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%2d", i + 1);
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf(" %c", board[i][j]);
        }
        printf("\n");
    }
}

int on_board(char board[BOARD_SIZE][BOARD_SIZE], int row, int col) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return 0;
    }
    return 1;
}

int is_valid_move(char board[BOARD_SIZE][BOARD_SIZE], int row, int col) {
    if (!on_board(board, row, col)) {
        return 0;
    }
    if (board[row][col] != EMPTY) {
        return 0;
    }
    return 1;
}

int check_win(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char curr_player_char) {
    int directions[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    for (int i = 0; i < 4; i++) {
        int dx = directions[i][0];
        int dy = directions[i][1];
        int count = 1;
        for (int j = 1; j < WIN_COUNT; j++) {
            int x = row + dx * j;
            int y = col + dy * j;
            if (on_board(board, x, y) && board[x][y] == curr_player_char) {
                count++;
            } else {
                break;
            }
        }
        for (int j = 1; j < WIN_COUNT; j++) {
            int x = row - dx * j;
            int y = col - dy * j;
            if (on_board(board, x, y) && board[x][y] == curr_player_char) {
                count++;
            } else {
                break;
            }
        }
        if (count >= WIN_COUNT) {
            return 1;
        }
    }
    return 0;
}
