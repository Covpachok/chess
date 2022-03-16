#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "chess_pieces.h"
#include <ncurses.h>

class ChessPiece;
class LastTurn;

const int kBoardSize = 8;

class ChessBoard {
public:
    ChessPiece *board[8][8];

    ChessBoard();
    ~ChessBoard();

    bool MovePiece(TeamID team_id, int piece_x, int piece_y, int dest_x,
                   int dest_y, TurnInfo &last_turn);

    void DrawBoardCell(int x, int y) const;
    void DrawBoard() const;
    void DrawBoardBorder() const;
    bool IsCellEmpty(int x, int y) const { return board[y][x] == nullptr; }
};

#endif
