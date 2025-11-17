#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define BIRD '0'
#define BUFFER '*'
#define GRAVITY -1
#define FLAP_VELOCITY 3
#define BIRD_START_X 10
#define BUFFER_NUM 5
#define BUFFER_WIDTH 6
#define BUFFER_GAP_HEIGHT 6
#define BUFFER_SPACING 20
#define GAME_SPEED_MS 100 // game loop delay in milliseconds (100ms = 10FPS)

typedef struct {
    int center_x;
    int center_y;
    bool passed;
} Buffer;

int width, height;

void init_ncurses();
void reset_buffer(Buffer* buffer, int center_x);
void draw_bird(int y, int x);
void draw_buffers(Buffer buffers[]);
void draw_score(int score);
void show_game_over(int score);

int main() {
    printf("Hello Flappy Bird!\n");

    srand(time(NULL));

    while (1) {
        init_ncurses();
        getmaxyx(stdscr, height, width);

        int bird_y = height / 2;
        int bird_v = 0;
        int score = 0;
        bool game_over = FALSE;

        Buffer buffers[BUFFER_NUM];
        for (int i = 0; i < BUFFER_NUM; i++) {
            // Start buffers off-screen to the right
            reset_buffer(&buffers[i], width + i * BUFFER_SPACING + BUFFER_WIDTH / 2);
        }

        while (!game_over) {
            // 1. --- Handle Input ---
            int ch = getch();
            // pressed 'd' or right arrow
            if (ch == 100 || ch == 261) {
                bird_v = FLAP_VELOCITY;
            } else if (ch == 113) { // pressed 'q'
                game_over = TRUE;
            }

            // 2. --- Update Game State ---
            bird_v += GRAVITY;
            bird_y += bird_v;

            for (int i = 0; i < BUFFER_NUM; i++) {
                buffers[i].center_x--;
                if (buffers[i].center_x < - BUFFER_WIDTH / 2) {
                    int last_buffer_index = (i + BUFFER_NUM - 1) % BUFFER_NUM;
                    int new_x = buffers[last_buffer_index].center_x + BUFFER_SPACING;
                    reset_buffer(&buffers[i], new_x);
                }
            }

            // 3. --- Check for Collisions ---
            // ground and sky collision
            if (bird_y <= 0 || bird_y >= height - 1) {
                game_over = TRUE;
            }

            // buffer collision
            for (int i = 0; i < BUFFER_NUM; i++) {
                Buffer b = buffers[i];
                if (BIRD_START_X >= b.center_x - BUFFER_WIDTH / 2 && BIRD_START_X <= b.center_x + BUFFER_WIDTH / 2) {
                    int gap_bottom = b.center_y - BUFFER_GAP_HEIGHT / 2;
                    int gap_top = b.center_y + BUFFER_GAP_HEIGHT / 2;
                    if (bird_y <= gap_bottom || bird_y >= gap_top) {
                        game_over = TRUE;
                        break;
                    }
                }
                if (!b.passed && BIRD_START_X > b.center_x + BUFFER_WIDTH / 2) {
                    buffers[i].passed = TRUE;
                    score++;
                }
            }

            clear();

            // 4. --- Render the Scene ---
            draw_buffers(buffers);
            draw_bird(bird_y, BIRD_START_X);
            draw_score(score);

            refresh();

            // 5. --- Delay ---
            usleep(GAME_SPEED_MS * 1000);
        }
        show_game_over(score);

        int ch_end;
        while (1) {
            ch_end = getch();
            if (ch_end == 113) { // quit the game
                endwin();
                return 0;
            }
            if (ch_end == 114) { // restart
                break;
            }
        }
    }

    endwin();

    return 0;
}

void init_ncurses() {
    initscr(); // start ncurses mode
    cbreak(); // disable line buffering
    noecho(); // don't print input chars to the screen
    keypad(stdscr, TRUE); // enable special keys like F1 and right arrow
    nodelay(stdscr, TRUE); // make getch() non-blocking
    curs_set(0); // hide the terminal cursor
    clear(); // clear the screen
}

void reset_buffer(Buffer* buffer, int center_x) {
    int min_gap_y = height / 4;
    int max_gap_y = height * 3 / 4;
    buffer->center_y = min_gap_y + (rand() % (max_gap_y - min_gap_y + 1));
    buffer->passed = FALSE;
}

void draw_bird(int y, int x) {
    mvprintw(y, x, "%c", BIRD);
}

void draw_buffers(Buffer buffers[]) {
    for (int i = 0; i < BUFFER_NUM; i++) {
        Buffer b = buffers[i];
        int gap_bottom = b.center_y - BUFFER_GAP_HEIGHT / 2;
        int gap_top = b.center_y + BUFFER_GAP_HEIGHT / 2;

        for (int y = 0; y < height; y++) {
            if (y < gap_bottom || y > gap_top) {
                for (int x = 0; x < BUFFER_WIDTH; x++) {
                    int screen_x = b.center_x + x;
                    if (screen_x >= 0 && screen_x < width) {
                        mvprintw(y, screen_x, "%c", BUFFER);
                    }
                }
            }
        }
    }
}

void draw_score(int score) {
    mvprintw(1, 1, "Score: %d", score);
}

void show_game_over(int score) {
    nodelay(stdscr, FALSE);
    clear();

    char* msg1 = "GAME OVER!";
    char* msg2 = "Your Score: %d";
    char* msg3 = "Press 'r' to restart, 'q' to quit.";
    mvprintw(height / 2 - 1, (width - strlen(msg1)) / 2, msg1);
    mvprintw(height / 2, (width - strlen(msg2)) / 2, msg2, score);
    mvprintw(height / 2 + 1, (width - strlen(msg3)) / 2, msg3);

    refresh();
}
