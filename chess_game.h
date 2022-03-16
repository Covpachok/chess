#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <ncurses.h>

#include "chess_board.h"
#include "chess_pieces.h"

class ChessBoard;
class ChessPiece;
class TurnInfo;

class ChessGame {
    TeamID   team_current_turn = TeamID::White;
    TurnInfo last_turn;

    ChessBoard game_board;

    bool   exit = false;
    int    from_x, from_y, to_x, to_y;
    MEVENT mouse_event;

public:
    ChessGame();
    ~ChessGame();

    void Chess();

private:
    void InitScreen();
    void InitColors();
    void HandleInput();
};

#endif
