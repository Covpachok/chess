#include "chess_pieces.h"
#include "chess_board.h"

#include <cstdlib>
#include <ncurses.h>

#ifndef NDEBUG
#include "log.h"
extern Log gLog;
#endif

TurnInfo::TurnInfo()
{
    src_x  = -1;
    src_y  = -1;
    dest_x = -1;
    dest_y = -1;
    piece  = nullptr;
}

void TurnInfo::ChangeTurnInfo(int src_x, int src_y, int dest_x, int dest_y,
                              ChessPiece *piece)
{
    this->src_x  = src_x;
    this->src_y  = src_y;
    this->dest_x = dest_x;
    this->dest_y = dest_y;
    this->piece  = piece;
}

ChessPiece::ChessPiece(PieceID pid, TeamID tid) : piece_id(pid), team_id(tid)
{
    color_pair_id = team_id == TeamID::White ? 1 : 3;
}

PawnPiece::PawnPiece(TeamID tid) : ChessPiece(PieceID::Pawn, tid) {}

bool PawnPiece::CanMovePiece(int curr_x, int curr_y, int dest_x, int dest_y,
                             ChessPiece *board[8][8], TurnInfo &prev_turn)
{
    int success = false;
#ifndef NDEBUG
    fprintf(gLog,
            "%s: Type Pawn (curr_x)[%d] (curr_y)[%d] "
            "(dest_x)[%d] (dest_y)[%d] (team_id)[%d]\n",
            __func__, curr_x, curr_y, dest_x, dest_y,
            static_cast<int>(team_id));
    fflush(gLog);
#endif
    int distance_x = curr_x - dest_x;
    int distance_y = curr_y - dest_y;
    if ((distance_x == -1 || distance_x == 1)) {
        if (IsTargetCapturable(board[dest_y][dest_x]) &&
            ((team_id == TeamID::White && distance_y == 1) ||
             (team_id == TeamID::Black && distance_y == -1))) {

            success = true;

        } else if (CheckForEnPassant(prev_turn, dest_x)) {
            if (distance_x == -1) {

                delete board[curr_y][curr_x + 1];
                board[curr_y][curr_x + 1] = nullptr;

                success = true;
            } else if (distance_x == 1) {

                delete board[curr_y][curr_x - 1];
                board[curr_y][curr_x - 1] = nullptr;

                success = true;
            }
        }
    } else if (distance_x == 0) {
        if (team_id == TeamID::White && distance_y > 0 &&
            distance_y <= 1 + !has_moved_before) {

            if (curr_y - 1 >= 0 && !board[curr_y - 1][curr_x])
                success = true;

        } else if (team_id == TeamID::Black && -distance_y > 0 &&
                   -distance_y <= 1 + !has_moved_before) {

            if (curr_y + 1 <= 7 && !board[curr_y + 1][curr_x])
                success = true;
        }
    }

    if (success)
        has_moved_before = true;
#ifndef NDEBUG
    else
        fprintf(gLog,
                "%s: Pawn not moved from position (curr_x)[%d] (curr_y)[%d] to "
                "(dest_x)[%d] (dest_y)[%d] (board[dest_y][dest_x])[%d]\n",
                __func__, curr_x, curr_y, dest_x, dest_y,
                board[dest_y][dest_x] != nullptr);
    fflush(gLog);
#endif

    return success;
}

bool PawnPiece::CheckForEnPassant(TurnInfo prev_turn, int dest_x)
{
    ChessPiece *piece = prev_turn.GetPiece();
    return piece && piece->GetPieceID() == PieceID::Pawn &&
           (prev_turn.GetXDistance() == 2 || prev_turn.GetYDistance() == -2) &&
           (prev_turn.GetDestinationX() == dest_x);
}

KnightPiece::KnightPiece(TeamID tid) : ChessPiece(PieceID::Knight, tid) {}

bool KnightPiece::CanMovePiece(int curr_x, int curr_y, int dest_x, int dest_y,
                               ChessPiece *board[8][8], TurnInfo &prev_turn)
{
    bool success = false;

#ifndef NDEBUG
    fprintf(gLog,
            "%s: Type Knight (curr_x)[%d] (curr_y)[%d] "
            "(dest_x)[%d] (dest_y)[%d] (team_id)[%d]\n",
            __func__, curr_x, curr_y, dest_x, dest_y,
            static_cast<int>(team_id));
    fflush(gLog);
#endif
    int distance_x = curr_x - dest_x;
    int distance_y = curr_y - dest_y;
    if (CanMoveTo(board[dest_y][dest_x])) {
        if (((distance_x == 1 || distance_x == -1) &&
             (distance_y == 2 || distance_y == -2)) ||
            ((distance_x == 2 || distance_x == -2) &&
             (distance_y == 1 || distance_y == -1)))
            success = true;
    }

    if (success)
        has_moved_before = true;
#ifndef NDEBUG
    else
        fprintf(
            gLog,
            "%s: Knight not moved from position (curr_x)[%d] (curr_y)[%d] to "
            "(dest_x)[%d] (dest_y)[%d] (board[dest_y][dest_x])[%d]\n",
            __func__, curr_x, curr_y, dest_x, dest_y,
            board[dest_y][dest_x] != nullptr);
    fflush(gLog);
#endif

    return success;
}

BishopPiece::BishopPiece(TeamID tid) : ChessPiece(PieceID::Bishop, tid) {}

bool BishopPiece::CanMovePiece(int curr_x, int curr_y, int dest_x, int dest_y,
                               ChessPiece *board[8][8], TurnInfo &prev_turn)
{
    bool success = false;

#ifndef NDEBUG
    fprintf(gLog,
            "%s: Type Bishop (curr_x)[%d] (curr_y)[%d] "
            "(dest_x)[%d] (dest_y)[%d] (team_id)[%d]\n",
            __func__, curr_x, curr_y, dest_x, dest_y,
            static_cast<int>(team_id));
    fflush(gLog);
#endif

    int distance_x = curr_x - dest_x;
    int distance_y = curr_y - dest_y;
    if (CanMoveTo(board[dest_y][dest_x])) {
        if (distance_x == distance_y || -distance_x == distance_y) {
            int distance    = std::abs(distance_x);
            int x_direction = distance_x > 0 ? -1 : 1;
            int y_direction = distance_y > 0 ? -1 : 1;

            success = true;
            for (int i = 1; i < distance; ++i) {
#ifndef NDEBUG
                fprintf(
                    gLog,
                    "%s: Bishop (x_direction)[%d] (y_direction)[%d] "
                    "(board[%d][%d])[%d]\n",
                    __func__, x_direction, y_direction,
                    curr_y + y_direction * i, curr_x + x_direction * i,
                    board[curr_y + y_direction * i][curr_x + x_direction * i] !=
                        nullptr);
                fflush(gLog);
#endif
                if (board[curr_y + y_direction * i][curr_x + x_direction * i]) {
                    success = false;
                    break;
                }
            }
        }
    }

    if (success)
        has_moved_before = true;
#ifndef NDEBUG
    else
        fprintf(
            gLog,
            "%s: Bishop not moved from position (curr_x)[%d] (curr_y)[%d] to "
            "(dest_x)[%d] (dest_y)[%d] (board[dest_y][dest_x])[%d]\n",
            __func__, curr_x, curr_y, dest_x, dest_y,
            board[dest_y][dest_x] != nullptr);
    fflush(gLog);
#endif

    return success;
}

RookPiece::RookPiece(TeamID tid) : ChessPiece(PieceID::Rook, tid) {}

bool RookPiece::CanMovePiece(int curr_x, int curr_y, int dest_x, int dest_y,
                             ChessPiece *board[8][8], TurnInfo &prev_turn)
{
    bool success = false;

#ifndef NDEBUG
    fprintf(gLog,
            "%s: Type Rook (curr_x)[%d] (curr_y)[%d] "
            "(dest_x)[%d] (dest_y)[%d] (team_id)[%d]\n",
            __func__, curr_x, curr_y, dest_x, dest_y,
            static_cast<int>(team_id));
    fflush(gLog);
#endif

    int distance_x = curr_x - dest_x;
    int distance_y = curr_y - dest_y;
    if (CanMoveTo(board[dest_y][dest_x])) {
        if ((distance_x != 0 && distance_y == 0) ||
            (distance_y != 0 && distance_x == 0)) {
            int distance    = std::abs(distance_x) + std::abs(distance_y);
            int x_direction = distance_x == 0 ? 0 : (distance_x > 0 ? -1 : 1);
            int y_direction = distance_y == 0 ? 0 : (distance_y > 0 ? -1 : 1);

            success = true;
            for (int i = 1; i < distance; ++i) {
#ifndef NDEBUG
                fprintf(
                    gLog,
                    "%s: Rook (x_direction)[%d] (y_direction)[%d] "
                    "(board[%d][%d])[%d]\n",
                    __func__, x_direction, y_direction,
                    curr_y + y_direction * i, curr_x + x_direction * i,
                    board[curr_y + y_direction * i][curr_x + x_direction * i] !=
                        nullptr);
                fflush(gLog);
#endif
                if (board[curr_y + y_direction * i][curr_x + x_direction * i]) {
                    success = false;
                    break;
                }
            }
        }
    }

    if (success)
        has_moved_before = true;
#ifndef NDEBUG
    else
        fprintf(gLog,
                "%s: Rook not moved from position (curr_x)[%d] (curr_y)[%d] to "
                "(dest_x)[%d] (dest_y)[%d] (board[dest_y][dest_x])[%d]\n",
                __func__, curr_x, curr_y, dest_x, dest_y,
                board[dest_y][dest_x] != nullptr);
    fflush(gLog);
#endif

    return success;
}

QueenPiece::QueenPiece(TeamID tid) : ChessPiece(PieceID::Queen, tid) {}

bool QueenPiece::CanMovePiece(int curr_x, int curr_y, int dest_x, int dest_y,
                              ChessPiece *board[8][8], TurnInfo &prev_turn)
{
    bool success = false;

#ifndef NDEBUG
    fprintf(gLog,
            "%s: Type Queen (curr_x)[%d] (curr_y)[%d] "
            "(dest_x)[%d] (dest_y)[%d] (team_id)[%d]\n",
            __func__, curr_x, curr_y, dest_x, dest_y,
            static_cast<int>(team_id));
    fflush(gLog);
#endif

    int distance_x = curr_x - dest_x;
    int distance_y = curr_y - dest_y;
    if (CanMoveTo(board[dest_y][dest_x])) {
        if (distance_x == distance_y || -distance_x == distance_y ||
            (distance_x != 0 && distance_y == 0) ||
            (distance_y != 0 && distance_x == 0)) {
            int x_direction = distance_x == 0 ? 0 : (distance_x > 0 ? -1 : 1);
            int y_direction = distance_y == 0 ? 0 : (distance_y > 0 ? -1 : 1);
            int distance =
                x_direction ? std::abs(distance_x) : std::abs(distance_y);

            success = true;
            for (int i = 1; i < distance; ++i) {
#ifndef NDEBUG
                fprintf(
                    gLog,
                    "%s: Queen (x_direction)[%d] (y_direction)[%d] "
                    "(board[%d][%d])[%d]\n",
                    __func__, x_direction, y_direction,
                    curr_y + y_direction * i, curr_x + x_direction * i,
                    board[curr_y + y_direction * i][curr_x + x_direction * i] !=
                        nullptr);
                fflush(gLog);
#endif
                if (board[curr_y + y_direction * i][curr_x + x_direction * i]) {
                    success = false;
                    break;
                }
            }
        }
    }

    if (success)
        has_moved_before = true;
#ifndef NDEBUG
    else
        fprintf(
            gLog,
            "%s: Queen not moved from position (curr_x)[%d] (curr_y)[%d] to "
            "(dest_x)[%d] (dest_y)[%d] (board[dest_y][dest_x])[%d]\n",
            __func__, curr_x, curr_y, dest_x, dest_y,
            board[dest_y][dest_x] != nullptr);
    fflush(gLog);
#endif

    return success;
}

KingPiece::KingPiece(TeamID tid) : ChessPiece(PieceID::King, tid) {}

bool KingPiece::CanMovePiece(int curr_x, int curr_y, int dest_x, int dest_y,
                             ChessPiece *board[8][8], TurnInfo &prev_turn)
{
    bool success = false;

#ifndef NDEBUG
    fprintf(gLog,
            "%s: Type King (curr_x)[%d] (curr_y)[%d] "
            "(dest_x)[%d] (dest_y)[%d] (team_id)[%d]\n",
            __func__, curr_x, curr_y, dest_x, dest_y,
            static_cast<int>(team_id));
    fflush(gLog);
#endif

    int distance_x = curr_x - dest_x;
    int distance_y = curr_y - dest_y;
    if (CanMoveTo(board[dest_y][dest_x])) {
        if ((distance_x <= 1 && distance_x >= -1) &&
            (distance_y <= 1 && distance_y >= -1)) {
            success = true;
        }
    }

    if (success)
        has_moved_before = true;
#ifndef NDEBUG
    else
        fprintf(gLog,
                "%s: King not moved from position (curr_x)[%d] (curr_y)[%d] to "
                "(dest_x)[%d] (dest_y)[%d] (board[dest_y][dest_x])[%d]\n",
                __func__, curr_x, curr_y, dest_x, dest_y,
                board[dest_y][dest_x] != nullptr);
    fflush(gLog);
#endif

    return success;
}

