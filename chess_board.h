#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "chess_pieces.h"
#include <ncurses.h>

class ChessPiece;
class LastTurn;

const int kBoardSize = 8;

class ChessBoard {
    ChessPiece *board[8][8];
    LastTurn last_turn;

public:
    ChessBoard();
    ~ChessBoard();

    void MovePiece(int piece_x, int piece_y, int dest_x, int dest_y);

    void DrawBoardCell(int x, int y) const;
    void DrawBoard() const;
    void DrawBoardBorder() const;
    bool IsCellEmpty(int x, int y) const { return board[y][x] == nullptr; }

private:
    void InitColors();
};

#endif
