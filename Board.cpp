#include "Board.h"

Board ::Board()
{
  // Initialize the matrix with pieces
  // White pieces
  matrix[0][0] = BoardPiece(PlaySide::WHITE, Piece::ROOK);
  matrix[0][1] = BoardPiece(PlaySide::WHITE, Piece::KNIGHT);
  matrix[0][2] = BoardPiece(PlaySide::WHITE, Piece::BISHOP);
  matrix[0][3] = BoardPiece(PlaySide::WHITE, Piece::QUEEN);
  matrix[0][4] = BoardPiece(PlaySide::WHITE, Piece::KING);
  matrix[0][5] = BoardPiece(PlaySide::WHITE, Piece::BISHOP);
  matrix[0][6] = BoardPiece(PlaySide::WHITE, Piece::KNIGHT);
  matrix[0][7] = BoardPiece(PlaySide::WHITE, Piece::ROOK);
  for (int i = 0; i < 8; i++)
  {
    matrix[1][i] = BoardPiece(PlaySide::WHITE, Piece::PAWN);
  }

  // Black pieces
  matrix[7][0] = BoardPiece(PlaySide::BLACK, Piece::ROOK);
  matrix[7][1] = BoardPiece(PlaySide::BLACK, Piece::KNIGHT);
  matrix[7][2] = BoardPiece(PlaySide::BLACK, Piece::BISHOP);
  matrix[7][3] = BoardPiece(PlaySide::BLACK, Piece::QUEEN);
  matrix[7][4] = BoardPiece(PlaySide::BLACK, Piece::KING);
  matrix[7][5] = BoardPiece(PlaySide::BLACK, Piece::BISHOP);
  matrix[7][6] = BoardPiece(PlaySide::BLACK, Piece::KNIGHT);
  matrix[7][7] = BoardPiece(PlaySide::BLACK, Piece::ROOK);
  for (int i = 0; i < 8; i++)
  {
    matrix[6][i] = BoardPiece(PlaySide::BLACK, Piece::PAWN);
  }

  for (int i = 2; i < 6; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      matrix[i][j] = BoardPiece(PlaySide::NONE, Piece::EMPTY);
    }
  }
}

BoardPiece **Board::getPiece() const { return (BoardPiece **)matrix; }

bool isEmpty(BoardPiece board[][8], int row, int col)
{
  return board[row][col].getType() == EMPTY;
}

Board ::Board(const Board &other)
{
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      matrix[i][j] = other.matrix[i][j];
    }
  }
}

