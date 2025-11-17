#include <ncurses.h>
#include <stdio.h>

int main() {
    // initialize ncurses
    initscr();

    // disable line buffering
    cbreak();
    // don't show the key I pressed
    noecho();

    // enable special keys like F1, arrow keys, and KEY_RESIZE
    // where KEY_RESIZE is "the window has changed"
    keypad(stdscr, TRUE);

    printw("Press any key to test getch()...\n");
    refresh();

    int input = 0;
    // loop until getch() gets a key that isn't a resize event
    while (input == 0 || input == KEY_RESIZE) {
        input = getch();
    }

    // clean up ncurses
    endwin();

    printf("Key pressed! Value: %d ('%c')\n", input, input);

    return 0;
}
