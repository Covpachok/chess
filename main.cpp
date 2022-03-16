#include "chess_board.h"
#include "chess_pieces.h"

#include "log.h"
#include <iostream>
#include <ncurses.h>

void init_colors()
{
    init_pair(1, COLOR_WHITE, COLOR_CYAN);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_BLACK, COLOR_CYAN);
    init_pair(4, COLOR_BLACK, COLOR_BLUE);
    init_pair(5, COLOR_WHITE, COLOR_RED);
    init_pair(6, COLOR_BLACK, COLOR_RED);
}

void ncinit()
{
    initscr();
    noecho();
    cbreak();
    curs_set(false);
    keypad(stdscr, true);
    mouseinterval(0);
    mousemask(ALL_MOUSE_EVENTS, nullptr);
    start_color();
    refresh();
}

int main()
{
    ncinit();
    init_colors();

    MEVENT     mevent;
    ChessBoard game;

    bool src_selected = false;
    int  key, conv_x, conv_y, src_x, src_y;

    game.DrawBoardBorder();
    game.DrawBoard();

    while ((key = getch()) != 'q') {
        switch (key) {
        case KEY_MOUSE:
            if (getmouse(&mevent) == OK) {
                if (mevent.bstate & BUTTON1_RELEASED) {
                    conv_x = (mevent.x - 2) / 2;
                    conv_y = mevent.y - 1;
                    mvprintw(14, 0, "x:%d y:%d\t", conv_x, conv_y);
                    if (conv_x >= 0 && conv_x < 8 && conv_y >= 0 &&
                        conv_y < 8) {
                        if (!src_selected) {
                            src_x        = conv_x;
                            src_y        = conv_y;
                            src_selected = true;
                            mvprintw(15, 0, "SELECTED x:%d y:%d\t", conv_x,
                                     conv_y);
                            mvprintw(16, 0, "\t\t\t\t\t\t");
                        } else {
                            mvprintw(16, 0, "MOVED TO x:%d y:%d\t", conv_x,
                                     conv_y);
                            game.MovePiece(src_x, src_y, conv_x, conv_y);
                            game.DrawBoard();
                            refresh();
                            src_selected = false;
                        }
                    }
                }
            }
            break;
        default:
            break;
        }
    }

    endwin();
    return 0;
}
