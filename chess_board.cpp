#include "chess_board.h"
#include <ncurses.h>

#ifndef NDEBUG
#include "log.h"
extern Log gLog;
#endif

ChessBoard::ChessBoard()
{
    board[0][0] = new RookPiece(TeamID::Black);
    board[0][1] = new KnightPiece(TeamID::Black);
    board[0][2] = new BishopPiece(TeamID::Black);
    board[0][3] = new QueenPiece(TeamID::Black);
    board[0][4] = new KingPiece(TeamID::Black);
    board[0][5] = new BishopPiece(TeamID::Black);
    board[0][6] = new KnightPiece(TeamID::Black);
    board[0][7] = new RookPiece(TeamID::Black);

    board[7][0] = new RookPiece(TeamID::White);
    board[7][1] = new KnightPiece(TeamID::White);
    board[7][2] = new BishopPiece(TeamID::White);
    board[7][3] = new QueenPiece(TeamID::White);
    board[7][4] = new KingPiece(TeamID::White);
    board[7][5] = new BishopPiece(TeamID::White);
    board[7][6] = new KnightPiece(TeamID::White);
    board[7][7] = new RookPiece(TeamID::White);

    for (int x = 0; x < kBoardSize; ++x) {
        board[1][x] = new PawnPiece(TeamID::Black);
        board[6][x] = new PawnPiece(TeamID::White);

        for (int y = 2; y < kBoardSize - 2; ++y)
            board[y][x] = nullptr;
    }

#ifndef NDEBUG
    fprintf(gLog, "%s: Board -\n", __func__);
    for (int y = 0; y < kBoardSize; ++y) {
        fprintf(gLog, "\t\t");
        for (int x = 0; x < kBoardSize; ++x) {
            fprintf(gLog, "%2d",
                    board[y][x] ? board[y][x]->GetPieceID() + 1 : 0);
        }
        fprintf(gLog, "\n");
    }
#endif
}

ChessBoard::~ChessBoard()
{
    for (int y = 0; y < kBoardSize; ++y)
        for (int x = 0; x < kBoardSize; ++x)
            if (board[y][x])
                delete board[y][x];
}

void ChessBoard::DrawBoardCell(int x, int y) const
{
    char ch;
    int  color_pair;
    if (board[y][x]) {
        ch         = kPieceChars[board[y][x]->GetPieceID()];
        color_pair = board[y][x]->GetColorPairID() + ((x + y) % 2 == 0);
    } else {
        ch         = ' ';
        color_pair = ((x + y) % 2 == 0) + 1;
    }

    attrset(COLOR_PAIR(color_pair) | A_BOLD);
    mvprintw(1 + y, 2 + x * 2, " %c", ch);
    attroff(COLOR_PAIR(color_pair) | A_BOLD);
}

void ChessBoard::DrawBoard() const
{
#ifndef NDEBUG
    fprintf(gLog, "%s: Board -\n", __func__);
    for (int y = 0; y < kBoardSize; ++y) {
        fprintf(gLog, "\t\t");
        for (int x = 0; x < kBoardSize; ++x) {
            fprintf(gLog, "%2d",
                    board[y][x] ? board[y][x]->GetPieceID() + 1 : 0);
        }
        fprintf(gLog, "\n");
    }
#endif
    for (int y = 0; y < kBoardSize; ++y)
        for (int x = 0; x < kBoardSize; ++x)
            DrawBoardCell(x, y);
}

void ChessBoard::HighlightBoardCell(int x, int y) const
{
    char ch;
    int  color_pair;
    if (board[y][x]) {
        ch         = kPieceChars[board[y][x]->GetPieceID()];
        color_pair = static_cast<int>(board[y][x]->GetTeamID()) + 7;
    } else {
        ch         = ' ';
        color_pair = 7;
    }

    attrset(COLOR_PAIR(color_pair) | A_BOLD);
    mvprintw(1 + y, 2 + x * 2, " %c", ch);
    attroff(COLOR_PAIR(color_pair) | A_BOLD);
}

void ChessBoard::DrawBoardBorder() const
{
    for (int y = 0; y < kBoardSize + 2; ++y) {
        for (int x = 0; x < (kBoardSize + 2) * 2; ++x) {
            attrset(COLOR_PAIR(5) | A_BOLD);
            mvaddch(y, x, ' ');
            attroff(COLOR_PAIR(5) | A_BOLD);
        }
    }

    for (int i = 0; i < kBoardSize; ++i) {
        attrset(COLOR_PAIR(5) | A_BOLD);
        mvaddch(i + 1, 1, '8' - i);
        mvaddch(0, (i * 2) + 3, i + 'a');
        attroff(COLOR_PAIR(5) | A_BOLD);
        attrset(COLOR_PAIR(6) | A_BOLD);
        mvaddch(i + 1, 19, '8' - i);
        mvaddch(9, (i * 2) + 3, i + 'a');
        attroff(COLOR_PAIR(6) | A_BOLD);
    }
}

bool ChessBoard::MovePiece(TeamID team_id, int piece_x, int piece_y, int dest_x,
                           int dest_y, TurnInfo &last_turn)
{
    bool success = false;

    if (AreCoordsCorrect(piece_x, piece_y) &&
        AreCoordsCorrect(dest_x, dest_y) && board[piece_y][piece_x] &&
        board[piece_y][piece_x]->GetTeamID() == team_id) {

        // Castling
        if (board[dest_y][dest_x] &&
            CheckForCastling(piece_x, piece_y, dest_x, dest_y) &&
            CanDoCastling(piece_x, piece_y, dest_x, dest_y)) {
            int         team_y, rook_x, king_x;
            ChessPiece *king, *rook;
            if (board[piece_y][piece_x]->GetPieceID() == PieceID::King) {
                king   = board[piece_y][piece_x];
                rook   = board[dest_y][dest_x];
                rook_x = dest_x;
                king_x = piece_x;
            } else {
                rook   = board[piece_y][piece_x];
                king   = board[dest_y][dest_x];
                rook_x = piece_x;
                king_x = dest_x;
            }

            if (team_id == TeamID::White)
                team_y = 7;
            else
                team_y = 0;

            if (rook_x > king_x) {
                board[team_y][6] = king;
                board[team_y][5] = rook;
            } else {
                board[team_y][1] = king;
                board[team_y][2] = rook;
            }
            board[piece_y][piece_x] = nullptr;
            board[dest_y][dest_x]   = nullptr;

            success = true;
            // Simple movement
        } else if (board[piece_y][piece_x]->CanMovePiece(
                       piece_x, piece_y, dest_x, dest_y, board, last_turn)) {
            last_turn.ChangeTurnInfo(piece_x, piece_y, dest_x, dest_y,
                                     board[piece_y][piece_x]);

            board[dest_y][dest_x]   = board[piece_y][piece_x];
            board[piece_y][piece_x] = nullptr;

            success = true;
        }
    }

    return success;
}

bool ChessBoard::CheckForCastling(int first_x, int first_y, int secnd_x,
                                  int secnd_y)
{
    bool have_not_moved = !board[secnd_y][secnd_x]->HasMovedBefore() &&
                          !board[secnd_y][secnd_x]->HasMovedBefore();

    bool king_and_rook =
        (board[first_y][first_x]->GetPieceID() == PieceID::King &&
         board[secnd_y][secnd_x]->GetPieceID() == PieceID::Rook) ||
        (board[first_y][first_x]->GetPieceID() == PieceID::Rook &&
         board[secnd_y][secnd_x]->GetPieceID() == PieceID::King);

    bool have_same_team = board[secnd_y][secnd_x]->GetTeamID() ==
                          board[first_y][first_x]->GetTeamID();

    return have_not_moved && king_and_rook && have_same_team;
}

bool ChessBoard::CanDoCastling(int first_x, int first_y, int secnd_x,
                               int secnd_y)
{
    bool success = true;
    int  from    = first_x > secnd_x ? secnd_x : secnd_x;
    int  to      = from == first_x ? secnd_x : first_x;

    for (int x = from + 1; x < to; ++x) {
        if (board[first_y][x]) {
            success = false;
            break;
        }
    }

    return success;
}

// Under construction :^) 
bool ChessBoard::CheckForCheckMate(TeamID team_id, int king_x, int king_y)
{
    int      checkmate = false;
    TurnInfo temp;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (board[y][x] && board[y][x]->GetTeamID() != team_id &&
                board[y][x]->CanMovePiece(x, y, king_x, king_y, board, temp)) {
                checkmate = true;
                break;
            }
        }
        if (checkmate)
            break;
    }
    return checkmate;
}

