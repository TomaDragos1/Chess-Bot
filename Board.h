#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include "Move.h"
#include "PlaySide.h"
#include "BoardPiece.h"

class Board {
public:
    static const int SIZE = 8;

    Board();

    // Piece getPiece(int row, int col) const;
    // void setPiece(int row, int col, const Piece& piece);

    BoardPiece** getPiece() const;

    Board (const Board & other);

    BoardPiece getPiece(int i, int j) {
        return matrix[i][j];
    }


private:
    BoardPiece matrix[SIZE][SIZE];
};

#endif
