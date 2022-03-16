#include "chess_board.h"
#include <ncurses.h>

#ifndef NDEBUG
#include "log.h"
extern Log gLog;
#endif

ChessBoard::ChessBoard() : last_turn()
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

    for (int x = 0; x < kBoardSize; x++) {
        board[1][x] = new PawnPiece(TeamID::Black);
        board[6][x] = new PawnPiece(TeamID::White);

        for (int y = 2; y < kBoardSize - 2; y++)
            board[y][x] = nullptr;
    }

#ifndef NDEBUG
    fprintf(gLog, "%s: Board -\n", __func__);
    for (int y = 0; y < kBoardSize; y++) {
        fprintf(gLog, "\t\t");
        for (int x = 0; x < kBoardSize; x++) {
            fprintf(gLog, "%2d",
                    board[y][x] ? board[y][x]->GetPieceID() + 1 : 0);
        }
        fprintf(gLog, "\n");
    }
#endif
}

ChessBoard::~ChessBoard()
{
    for (int y = 0; y < kBoardSize; y++)
        for (int x = 0; x < kBoardSize; x++)
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
    for (int y = 0; y < kBoardSize; y++) {
        fprintf(gLog, "\t\t");
        for (int x = 0; x < kBoardSize; x++) {
            fprintf(gLog, "%2d",
                    board[y][x] ? board[y][x]->GetPieceID() + 1 : 0);
        }
        fprintf(gLog, "\n");
    }
#endif
    for (int y = 0; y < kBoardSize; y++)
        for (int x = 0; x < kBoardSize; x++)
            DrawBoardCell(x, y);
}

void ChessBoard::DrawBoardBorder() const
{
    for (int y = 0; y < kBoardSize + 2; y++) {
        for (int x = 0; x < (kBoardSize + 2) * 2; x++) {
            attrset(COLOR_PAIR(5) | A_BOLD);
            mvaddch(y, x, ' ');
            attroff(COLOR_PAIR(5) | A_BOLD);
        }
    }

    for (int y = 0; y < kBoardSize; y++) {
        attrset(COLOR_PAIR(5) | A_BOLD);
        mvaddch(y + 1, 1, '8' - y);
        attroff(COLOR_PAIR(5) | A_BOLD);
        attrset(COLOR_PAIR(6) | A_BOLD);
        mvaddch(y + 1, 19, '8' - y);
        attroff(COLOR_PAIR(6) | A_BOLD);
    }

    for (int x = 0; x < kBoardSize; x++) {
        attrset(COLOR_PAIR(5) | A_BOLD);
        mvaddch(0, (x * 2) + 3, x + 'a');
        attroff(COLOR_PAIR(5) | A_BOLD);
        attrset(COLOR_PAIR(6) | A_BOLD);
        mvaddch(9, (x * 2) + 3, x + 'a');
        attroff(COLOR_PAIR(6) | A_BOLD);
    }
}

void ChessBoard::MovePiece(int piece_x, int piece_y, int dest_x, int dest_y)
{
    if (board[piece_y][piece_x] &&
        board[piece_y][piece_x]->CanMovePiece(piece_x, piece_y, dest_x, dest_y,
                                              board, last_turn)) {
        last_turn.ChangeLastTurn(piece_x, piece_y, dest_x, dest_y,
                                 board[piece_y][piece_x]);
        board[dest_y][dest_x]   = board[piece_y][piece_x];
        board[piece_y][piece_x] = nullptr;
    }
}

