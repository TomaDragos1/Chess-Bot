#ifndef BOT_H
#define BOT_H
#include <bits/stdc++.h>

#include <vector>

#include "Board.h"
#include "BoardPiece.h"
#include "Move.h"
#include "PlaySide.h"

class Bot {
 private:
  static const std::string BOT_NAME;
  BoardPiece board[8][8];
  std::array<int, 7>
      whiteCapturedPieces;  // array to store count of captured pieces for white
  std::array<int, 7>
      blackCapturedPieces;  // array to store count of captured pieces for black

 public:
  /* Declare custom fields below */

  /* Declare custom fields above */
  Bot();

  std::vector<std::pair<int, int>> nextPawnPositions(
      const std::pair<int, int> &position, PlaySide color,
      BoardPiece boardCopy[8][8]);
  std::vector<std::pair<int, int>> nextKnightPositions(
      const std::pair<int, int> &position, PlaySide color,
      BoardPiece currentBoard[8][8]);
  std::vector<std::pair<int, int>> nextBishopPositions(
      const std::pair<int, int> &position, PlaySide color,
      BoardPiece currentBoard[8][8]);
  std::vector<std::pair<int, int>> nextRookPositions(
      const std::pair<int, int> &position, PlaySide color,
      BoardPiece currentBoard[8][8]);
  std::vector<std::pair<int, int>> nextQueenPositions(
      const std::pair<int, int> &position, PlaySide color,
      BoardPiece currentBoard[8][8]);
  std::vector<std::pair<int, int>> nextKingPositions(
      const std::pair<int, int> &position, PlaySide color, int &ifCastle);
  std::vector<std::pair<int, int>> findAttackers(std::pair<int, int> position,
                                                 PlaySide side,
                                                 BoardPiece board[8][8]);
  std::vector<Move *> returnSafeMoves(PlaySide side);
  BoardPiece generateOneMove(Move *currentMove);
  void removeOneMove(Move *currentMove, BoardPiece capturedPiece);
  int minimax(int depth, int alpha, int beta, bool maximizingPlayer);
  std::vector<Move *> returnCaptures(PlaySide side);
  int AllCaputres(int alpha, int beta, bool maximizingPlayer);

  int pawnStructure(PlaySide side);
  int kingSafety(PlaySide side);
  int pieceDevelopment(PlaySide side);

  bool CompareMoves(Move *move1, Move *move2);
  int evaluateMove(Move *move);
  int getScore(Piece currentPiece);
  int evaluateBoard(bool side);

  bool isCheck(bool side);
  bool isCheckmate(PlaySide side);
  bool isCheckAfterMove(int srcRow, int srcCol, int destRow, int destCol,
                        Piece piece);

  int pieceIsAttacked(int row, int col);

  std::vector<Move *> generateMovesForPiece(PlaySide side, int row, int col);
  std::vector<Move *> generateLegalMoves(PlaySide side);

  /**
   * Record move (either by enemy in normal mode, or by either side
   * in force mode) in custom structures
   * @param move received move
   * @param sideToMode side to move
   */
  void recordMove(Move *move, PlaySide sideToMove);

  /**
   * Calculates and return the bot's next move
   * @return your move
   */
  Move *calculateNextMove();

  static std::string getBotName();
};
#endif