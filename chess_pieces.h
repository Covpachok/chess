#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H

#include <ncurses.h>

enum class TeamID { White, Black };

const char kPieceChars[] = {'p', 'N', 'B', 'R', 'Q', 'K'};

class ChessPiece;

class TurnInfo {
    int         src_x, src_y;
    int         dest_x, dest_y;
    ChessPiece *piece;

public:
    TurnInfo();
    ~TurnInfo() {}
    void ChangeTurnInfo(int src_x, int src_y, int dest_x, int dest_y,
                        ChessPiece *piece);

    int GetXDistance() const { return src_x - dest_x; }
    int GetYDistance() const { return src_y - dest_y; }
    int GetSourceX() const { return src_x; }
    int GetSourceY() const { return src_y; }
    int GetDestinationX() const { return dest_x; }
    int GetDestinationY() const { return dest_y; }

    ChessPiece *GetPiece() const { return piece; }
};

class ChessPiece {
public:
    enum PieceID { Pawn, Knight, Bishop, Rook, Queen, King };

protected:
    PieceID piece_id;
    TeamID  team_id;
    int     color_pair_id;

    bool has_moved_before = false;

public:
    ChessPiece(PieceID pid, TeamID tid);
    virtual ~ChessPiece(){};

    virtual bool CanMovePiece(int curr_x, int curr_y, int dest_x, int dest_y,
                              ChessPiece *board[8][8], TurnInfo &prev_turn) = 0;

    PieceID GetPieceID() const { return piece_id; }
    TeamID  GetTeamID() const { return team_id; }
    char    GetColorPairID() const { return color_pair_id; }
    bool    HasMovedBefore() const { return has_moved_before; }

protected:
    bool CanMoveTo(ChessPiece *dest) const
    {
        return !dest || dest->GetTeamID() != team_id;
    }
    bool IsTargetCapturable(ChessPiece *target) const
    {
        return target && target->GetTeamID() != team_id;
    }
};

class PawnPiece : public ChessPiece {
public:
    PawnPiece(TeamID tid);
    virtual ~PawnPiece() {}

    virtual bool CanMovePiece(int curr_x, int curr_y, int dest_x, int dest_y,
                              ChessPiece *board[8][8], TurnInfo &prev_turn);

private:
    bool CheckForEnPassant(TurnInfo prev_turn, int dest_x);
};

class KnightPiece : public ChessPiece {
public:
    KnightPiece(TeamID tid);
    virtual ~KnightPiece() {}

    virtual bool CanMovePiece(int curr_x, int curr_y, int dest_x, int dest_y,
                              ChessPiece *board[8][8], TurnInfo &prev_turn);
};

class BishopPiece : public ChessPiece {
public:
    BishopPiece(TeamID tid);
    virtual ~BishopPiece() {}

    virtual bool CanMovePiece(int curr_x, int curr_y, int dest_x, int dest_y,
                              ChessPiece *board[8][8], TurnInfo &prev_turn);
};

class RookPiece : public ChessPiece {
public:
    RookPiece(TeamID tid);
    virtual ~RookPiece() {}

    virtual bool CanMovePiece(int curr_x, int curr_y, int dest_x, int dest_y,
                              ChessPiece *board[8][8], TurnInfo &prev_turn);
};

class QueenPiece : public ChessPiece {
public:
    QueenPiece(TeamID tid);
    virtual ~QueenPiece() {}

    virtual bool CanMovePiece(int curr_x, int curr_y, int dest_x, int dest_y,
                              ChessPiece *board[8][8], TurnInfo &prev_turn);
};

class KingPiece : public ChessPiece {
public:
    KingPiece(TeamID tid);
    virtual ~KingPiece() {}

    virtual bool CanMovePiece(int curr_x, int curr_y, int dest_x, int dest_y,
                              ChessPiece *board[8][8], TurnInfo &prev_turn);
};

#endif
