
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <limits.h>
#include <stdlib.h>

#define BOARD_WIDTH 20
#define BOARD_HEIGHT 10
char board[BOARD_HEIGHT][BOARD_WIDTH];
typedef struct {
    int x;
    int y;
} player;

typedef struct {
    int x;
    int y;
} point;

static player p1 = {
.x = 1,
.y = 1
};

void character_print(){
    printf("Point: %d, %d", p1.x, p1.y);
}

void board_reset(){
    int i, j;
    for (i = 0; i < BOARD_HEIGHT; i++){
        for (j = 0; j < BOARD_WIDTH; j++){
            if ((i == 0) || (j == 0) || (j == BOARD_WIDTH - 1) || (i == BOARD_HEIGHT - 1)){
                board[i][j] = 'x';
            } else {
                board[i][j] = '-';
            }
        }
    }
}

void board_print(){
    int i, j;
    for (i = 0; i < BOARD_HEIGHT; i++){
        for (j = 0; j < BOARD_WIDTH; j++){
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}

void player_move_right(){
    if ((p1.x + 1) > BOARD_WIDTH - 2 ){return;}
    else {
        p1.x += 1;
    }
}
void player_move_left(){
    if ((p1.x - 1) < 1){return;}
    else {
        p1.x -= 1;
    }
}
void player_move_down(){
    if ((p1.y + 1) > BOARD_HEIGHT - 2){return;}
    else {
        p1.y += 1;
    }
}
void player_move_up(){
    if ((p1.y - 1) < 1){return;}
    else {
        p1.y -= 1;
    }
}

void character_input(){
    int ch;
    ch = getch();
    switch(ch){
        case KEY_UP:
            player_move_up();
            break;
        case KEY_DOWN:
            player_move_down();
            break;
        case KEY_LEFT:
            player_move_left();
            break;
        case KEY_RIGHT:
            player_move_right();
            break;
        default:
            break;
    }
}

void board_update(WINDOW *win){
    board_reset();
    board[p1.y][p1.x] = 'o';
    int i, j;
    for (i = 0; i < BOARD_HEIGHT; i++){
        for (j = 0; j < BOARD_WIDTH; j++){
            if ((i == p1.y) && (j == p1.x)){
                wattron(win, COLOR_PAIR(1));
                wprintw(win, "%c", board[i][j]);
                wattroff(win, COLOR_PAIR(1));
                wrefresh(win);
            } else {
                wprintw(win, "%c", board[i][j]);
            }
        }
        wprintw(win, "\n");
    }
}

void t_color_check(){
    if (has_colors() == FALSE){
        endwin();
        fprintf(stderr, "Error: colors not available in terminal\n");
        exit(1);
    } else {
        start_color();
    }
}

int main() {
    board_reset();

    // ncurses initializations
    initscr();
    noecho();
    cbreak();
    refresh();
    keypad(stdscr, TRUE);
    t_color_check();
    init_pair(1, COLOR_RED, COLOR_BLACK);

    /* int i, j; */
    /* for (i = 0; i < BOARD_HEIGHT; i++){ */
    /*     for (j = 0; j < BOARD_WIDTH; j++){ */
    /*         printw("%c", board[i][j]); */
    /*     } */
    /*     printw("\n"); */
    /* } */

    WINDOW *game_win;
    game_win = newwin(BOARD_HEIGHT+1, BOARD_WIDTH+1, 0, 0);
    wrefresh(game_win);
    board_update(game_win);
    wrefresh(game_win);

    int ch;
    while((ch = getch()) != KEY_F(1)){
        wclear(game_win);
        switch(ch){
            case KEY_LEFT:
                player_move_left();
                board_update(game_win);
                wrefresh(game_win);
                break;
            case KEY_RIGHT:
                player_move_right();
                board_update(game_win);
                wrefresh(game_win);
                break;
            case KEY_DOWN:
                player_move_down();
                board_update(game_win);
                wrefresh(game_win);
                break;
            case KEY_UP:
                player_move_up();
                board_update(game_win);
                wrefresh(game_win);
                break;
        }
        usleep(100);
    }
    getch();
}
