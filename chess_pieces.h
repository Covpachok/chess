#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H

#include <ncurses.h>

enum class TeamID { White, Black };
enum PieceID { Pawn, Knight, Bishop, Rook, Queen, King };

const char kPieceChars[] = {'p', 'N', 'B', 'R', 'Q', 'K'};

class ChessPiece;

class LastTurn {
public:
    int         from_x, from_y;
    int         to_x, to_y;
    ChessPiece *piece;

    LastTurn();
    ~LastTurn() {}
    void ChangeLastTurn(int old_x, int old_y, int new_x, int new_y,
                        ChessPiece *new_piece);
};

class ChessPiece {
protected:
    PieceID piece_id;
    TeamID  team_id;
    int     color_pair_id;

    bool has_moved_before = false;

public:
    ChessPiece(PieceID pid, TeamID tid);
    virtual ~ChessPiece(){};

    virtual bool CanMovePiece(int curr_x, int curr_y, int dest_x, int dest_y,
                              ChessPiece *board[8][8], LastTurn &last_turn) = 0;

    PieceID GetPieceID() const { return piece_id; }
    TeamID  GetTeamID() const { return team_id; }
    char    GetColorPairID() const { return color_pair_id; }
    bool    HasMovedBefore() const { return has_moved_before; }

protected:
    bool IsTargetCapturable(ChessPiece *target)
    {
        return target ? target->GetTeamID() != team_id : false;
    }
};

class PawnPiece : public ChessPiece {
public:
    PawnPiece(TeamID tid);
    virtual ~PawnPiece() {}

    virtual bool CanMovePiece(int curr_x, int curr_y, int dest_x, int dest_y,
                              ChessPiece *board[8][8], LastTurn &last_turn);
};

class KnightPiece : public ChessPiece {
public:
    KnightPiece(TeamID tid);
    virtual ~KnightPiece() {}

    virtual bool CanMovePiece(int curr_x, int curr_y, int dest_x, int dest_y,
                              ChessPiece *board[8][8], LastTurn &last_turn);
};

class BishopPiece : public ChessPiece {
public:
    BishopPiece(TeamID tid);
    virtual ~BishopPiece() {}

    virtual bool CanMovePiece(int curr_x, int curr_y, int dest_x, int dest_y,
                              ChessPiece *board[8][8], LastTurn &last_turn);
};

class RookPiece : public ChessPiece {
public:
    RookPiece(TeamID tid);
    virtual ~RookPiece() {}

    virtual bool CanMovePiece(int curr_x, int curr_y, int dest_x, int dest_y,
                              ChessPiece *board[8][8], LastTurn &last_turn);
};

class QueenPiece : public ChessPiece {
public:
    QueenPiece(TeamID tid);
    virtual ~QueenPiece() {}

    virtual bool CanMovePiece(int curr_x, int curr_y, int dest_x, int dest_y,
                              ChessPiece *board[8][8], LastTurn &last_turn);
};

class KingPiece : public ChessPiece {
public:
    KingPiece(TeamID tid);
    virtual ~KingPiece() {}

    virtual bool CanMovePiece(int curr_x, int curr_y, int dest_x, int dest_y,
                              ChessPiece *board[8][8], LastTurn &last_turn);
};

#endif
