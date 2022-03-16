#include "chess_game.h"

ChessGame::ChessGame() : last_turn(), game_board()
{
    InitScreen();
    InitColors();

    game_board.DrawBoardBorder();
    game_board.DrawBoard();
}

ChessGame::~ChessGame()
{
    curs_set(true);
    endwin();
}

void ChessGame::Chess()
{
    while (!exit) {
        HandleInput();
        if (game_board.MovePiece(team_current_turn, from_x, from_y, to_x, to_y,
                                 last_turn)) {
            team_current_turn = team_current_turn == TeamID::White
                                    ? TeamID::Black
                                    : TeamID::White;
        }
        game_board.DrawBoard();
        refresh();
    }
}

void ChessGame::InitScreen()
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

void ChessGame::InitColors()
{
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_CYAN);
    init_pair(3, COLOR_BLACK, COLOR_BLUE);
    init_pair(4, COLOR_BLACK, COLOR_CYAN);
    init_pair(5, COLOR_WHITE, COLOR_RED);
    init_pair(6, COLOR_BLACK, COLOR_RED);
    init_pair(7, COLOR_WHITE, COLOR_YELLOW);
    init_pair(8, COLOR_BLACK, COLOR_YELLOW);
}

void ChessGame::HandleInput()
{
    bool selected = false, success = false;
    int  key, converted_x, converted_y;
    while (!success) {
        key = getch();
        switch (key) {
        case KEY_MOUSE:
            if (getmouse(&mouse_event) == OK) {
                if (mouse_event.bstate & BUTTON1_PRESSED) {
                    converted_x = (mouse_event.x - 2) / 2;
                    converted_y = mouse_event.y - 1;
                    if (converted_x >= 0 && converted_x < 8 &&
                        converted_y >= 0 && converted_y < 8) {
                        if (!selected &&
                            !game_board.IsCellEmpty(converted_x, converted_y)) {
                            from_x = converted_x;
                            from_y = converted_y;
                            game_board.HighlightBoardCell(from_x, from_y);
                            selected = true;
                        } else {
                            to_x    = converted_x;
                            to_y    = converted_y;
                            success = true;
                        }
                    }
                } else if (mouse_event.bstate & BUTTON3_PRESSED) {
                    if (selected)
                        game_board.DrawBoardCell(from_x, from_y);
                    selected = false;
                }
            }
            break;
        case 'q':
            exit    = true;
            success = true;
            break;
        default:
            break;
        }
    }
}

