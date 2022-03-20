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

    void HighlightBoardCell(int x, int y) const;
    void DrawBoardCell(int x, int y) const;
    void DrawBoard() const;
    void DrawBoardBorder() const;

    bool CheckForCheckMate(TeamID team_id);

    bool IsCellEmpty(int x, int y) const { return board[y][x] == nullptr; }
    bool AreCoordsCorrect(int x, int y) const
    {
        return x >= 0 && x <= 7 && y >= 0 && y <= 7;
    }
private:
    bool CheckForCastling(int first_x, int first_y, int secnd_x, int secnd_y);
    bool CanDoCastling(int first_x, int first_y, int secnd_x, int secnd_y);
};

#endif
