
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>

#define BOARD_WIDTH 20
#define BOARD_HEIGHT 10

char board[BOARD_HEIGHT][BOARD_WIDTH];

typedef struct {
    int x;
    int y;
} player;

// struct point definition
typedef struct {
    int x;
    int y;
} point;

bool b_point_equal(point a, point b){
    return ((a.x == b.x) && (a.y == b.y));
}
// struct point end

static player p1 = {
.x = 1,
.y = 1
};

void character_print(){
    printf("Point: %d, %d", p1.x, p1.y);
}

void board_reset(void){
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

void board_print(void){
    int i, j;
    for (i = 0; i < BOARD_HEIGHT; i++){
        for (j = 0; j < BOARD_WIDTH; j++){
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}

// int x, int y -> bool
// true if game border ('x')
bool b_game_border(point p){
    if ((p.x == 0) || (p.x == 19)){
        return true;
    }
    if ((p.y == 0) || (p.y == 9)){
        return true;
    }
    return false;
}

void player_skill_q(void){
    point up_left = {.x = p1.x-1, .y = p1.y-1};
    point up = {.x = p1.x, .y = p1.y-1};
    point up_right = {.x = p1.x+1, .y = p1.y-1};
    //
    point left = {.x = p1.x-1, .y = p1.y};
    point right = {.x = p1.x+1, .y = p1.y};
    //
    point down_left = {.x = p1.x-1, .y = p1.y+1};
    point down = {.x = p1.x, .y = p1.y+1};
    point down_right = {.x = p1.x+1, .y = p1.y+1};

    if (!b_game_border(up_left)){
        board[up_left.y][up_left.x] = '*';
    }
    if (!b_game_border(up)){
        board[up.y][up.x] = '*';
    }
    if (!b_game_border(up_right)){
        board[up_right.y][up_right.x] = '*';
    }
    if (!b_game_border(left)){
        board[left.y][left.x] = '*';
    }
    if (!b_game_border(right)){
        board[right.y][right.x] = '*';
    }
    if (!b_game_border(down_left)){
        board[down_left.y][down_left.x] = '*';
    }
    if (!b_game_border(down)){
        board[down.y][down.x] = '*';
    }
    if (!b_game_border(down_right)){
        board[down_right.y][down_right.x] = '*';
    }
}

void player_move_right(void){
    board_reset();
    if ((p1.x + 1) > BOARD_WIDTH - 2 ){
        board[p1.y][p1.x] = 'o';
        return;
    } else {
        p1.x += 1;
        board[p1.y][p1.x] = 'o';
    }
}
void player_move_left(void){
    board_reset();
    if ((p1.x - 1) < 1){
        board[p1.y][p1.x] = 'o';
        return;
    } else {
        p1.x -= 1;
        board[p1.y][p1.x] = 'o';
    }
}
void player_move_down(void){
    board_reset();
    if ((p1.y + 1) > BOARD_HEIGHT - 2){
        board[p1.y][p1.x] = 'o';
        return;
    } else {
        p1.y += 1;
        board[p1.y][p1.x] = 'o';
    }
}
void player_move_up(void){
    board_reset();
    if ((p1.y - 1) < 1){
        board[p1.y][p1.x] = 'o';
        return;
    } else {
        p1.y -= 1;
        board[p1.y][p1.x] = 'o';
    }
}


void board_print_to_window(WINDOW *win){
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

void t_color_check(void){
    if (has_colors() == FALSE){
        endwin();
        fprintf(stderr, "Error: colors not available in terminal\n");
        exit(1);
    } else {
        start_color();
    }
}


//     board[p1.y][p1.x] = 'o';
void parse_input(WINDOW *win, int ch){
    switch(ch){
        case KEY_LEFT:
            player_move_left();
            board_print_to_window(win);
            wrefresh(win);
            break;
        case KEY_RIGHT:
            player_move_right();
            board_print_to_window(win);
            wrefresh(win);
            break;
        case KEY_DOWN:
            player_move_down();
            board_print_to_window(win);
            wrefresh(win);
            break;
        case KEY_UP:
            player_move_up();
            board_print_to_window(win);
            wrefresh(win);
            break;
        case 'q':
            player_skill_q();
            board_print_to_window(win);
            wrefresh(win);
            break;
    }
}

void ncurses_init(void){
    // ncurses initializations
    initscr();
    noecho();
    cbreak();
    refresh();
    keypad(stdscr, TRUE);
    t_color_check();
    init_pair(1, COLOR_RED, COLOR_BLACK);
}

int main() {
    board_reset();

    ncurses_init();

    WINDOW *game_win;
    game_win = newwin(BOARD_HEIGHT+1, BOARD_WIDTH+1, 0, 0);
    wrefresh(game_win);
    board_print_to_window(game_win);
    wrefresh(game_win);

    int ch;
    while((ch = getch()) != KEY_F(1)){
        wclear(game_win);
        parse_input(game_win, ch);
        usleep(2500);
    }
}
