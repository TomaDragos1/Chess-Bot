#include "Bot.h"

#include <bits/stdc++.h>

#include <iostream>

using namespace std;

const std::string Bot::BOT_NAME =
    "MyBot"; /* Edit this, escaped characters are forbidden */
extern PlaySide getEngineSide();

Bot::Bot() { /* Initialize custom fields here */
  Board new_board = Board();
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      board[i][j] = new_board.getPiece(i, j);
    }
  }

  for (int i = 0; i < 7; i++) {
    whiteCapturedPieces[i] = 0;
    blackCapturedPieces[i] = 0;
  }
}

// Move for a pawn
std::vector<std::pair<int, int>> Bot::nextPawnPositions(
    const std::pair<int, int> &position, PlaySide color,
    BoardPiece currentBoard[8][8])  //, Move *last_move)
{
  std::vector<std::pair<int, int>> nextPositions;
  int row = position.first;
  int col = position.second;
  int direction = (color == WHITE) ? 1 : -1;

  // Check one step forward
  int nextRow = row + direction;
  if (nextRow >= 0 && nextRow < Board::SIZE &&
      currentBoard[nextRow][col].isEmpty()) {
    nextPositions.emplace_back(nextRow, col);
  }

  // Check two steps forward (only on the first move)
  if ((color == WHITE && row == 1) || (color == BLACK && row == 6)) {
    int doubleStepRow = row + 2 * direction;
    if (currentBoard[nextRow][col].isEmpty() &&
        currentBoard[doubleStepRow][col].isEmpty()) {
      board[nextRow][col].setDoubleMove(true);
      nextPositions.emplace_back(doubleStepRow, col);
    }
  }

  // Check capture moves
  auto addIfValid = [&](int r, int c) {
    if (r >= 0 && r < Board::SIZE && c >= 0 && c < Board::SIZE) {
      const auto &square = currentBoard[r][c];
      if (!square.isEmpty() && square.getColor() != color) {
        nextPositions.emplace_back(r, c);
      }
    }
  };

  //  En passant capture
  // for (int i = -1; i >= 1; i += 2) {

  // }

  addIfValid(nextRow, col - 1);
  addIfValid(nextRow, col + 1);

  //   for (int i = -1; i <= 1; i += 2)
  // {
  //   if (col + i >= 0 && col + i < Board::SIZE)
  //   {
  //     const auto &square = currentBoard[row][col + i];
  //     if (!square.isEmpty() && square.getColor() != color && square.getType()
  //     == PAWN && square.getDoubleMove() && )
  //     {
  //       nextPositions.emplace_back(row + direction, col + i);
  //       board[row][col + i].setType(EMPTY);
  //       board[row][col + i].setDoubleMove(false);
  //       // add to whiteCaptured or blackcaptured
  //       if (color == WHITE)
  //       {
  //         whiteCapturedPieces[PAWN]++;
  //       }
  //       else
  //       {
  //         blackCapturedPieces[PAWN]++;
  //       }
  //     }
  //   }
  // }

  // Check en passant captures
  // if (lastMove != nullptr && lastMove->isDoublePawnPush())
  // {
  //   int lastRow = lastMove->destination.value()[1] - '1';
  //   int lastCol = lastMove->destination.value()[0] - 'a';
  //   if (lastRow == row && std::abs(lastCol - col) == 1)
  //   {
  //     // Add en passant capture move
  //     nextPositions.emplace_back(row + direction, lastCol);
  //   }
  // }

  return nextPositions;
}

// Move for a knight
std::vector<std::pair<int, int>> Bot::nextKnightPositions(
    const std::pair<int, int> &position, PlaySide color,
    BoardPiece currentBoard[8][8]) {
  std::vector<std::pair<int, int>> nextPositions;
  int row = position.first;
  int col = position.second;

  // Calculate possible moves for a knight
  int dr[] = {-2, -2, -1, -1, 1, 1, 2, 2};
  int dc[] = {-1, 1, -2, 2, -2, 2, -1, 1};

  for (int i = 0; i < 8; i++) {
    int nextRow = row + dr[i];
    int nextCol = col + dc[i];

    // Check if the next position is valid and empty or has an opponent piece
    if (nextRow >= 0 && nextRow < Board::SIZE && nextCol >= 0 &&
        nextCol < Board::SIZE &&
        (currentBoard[nextRow][nextCol].getType() == EMPTY ||
         currentBoard[nextRow][nextCol].getColor() != color)) {
      nextPositions.push_back(std::make_pair(nextRow, nextCol));
    }
  }

  return nextPositions;
}

// Move for a bishop
std::vector<std::pair<int, int>> Bot::nextBishopPositions(
    const std::pair<int, int> &position, PlaySide color,
    BoardPiece currentBoard[8][8]) {
  std::vector<std::pair<int, int>> nextPositions;

  int row = position.first;
  int col = position.second;

  // Check diagonal moves in all four directions
  int dirs[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
  for (int d = 0; d < 4; ++d) {
    int r = row + dirs[d][0];
    int c = col + dirs[d][1];

    while (r >= 0 && r < Board::SIZE && c >= 0 && c < Board::SIZE) {
      BoardPiece piece = currentBoard[r][c];
      if (piece.getType() == EMPTY) {
        nextPositions.push_back(std::make_pair(r, c));
      } else if (piece.getColor() != color) {
        nextPositions.push_back(std::make_pair(r, c));
        break;
      } else {
        break;
      }
      r += dirs[d][0];
      c += dirs[d][1];
    }
  }

  return nextPositions;
}

// Move fo a rook
std::vector<std::pair<int, int>> Bot::nextRookPositions(
    const std::pair<int, int> &position, PlaySide color,
    BoardPiece currentBoard[8][8]) {
  std::vector<std::pair<int, int>> nextPositions;
  int row = position.first;
  int col = position.second;

  // Check possible moves along the row to the right
  for (int i = col + 1; i < Board::SIZE; i++) {
    if (currentBoard[row][i].getColor() != color) {
      nextPositions.push_back(std::make_pair(row, i));
      if (currentBoard[row][i].getType() != EMPTY) break;
    } else {
      break;
    }
  }

  // Check possible moves along the row to the left
  for (int i = col - 1; i >= 0; i--) {
    if (currentBoard[row][i].getColor() != color) {
      nextPositions.push_back(std::make_pair(row, i));
      if (currentBoard[row][i].getType() != EMPTY) break;
    } else {
      break;
    }
  }

  // Check possible moves along the column upwards
  for (int i = row - 1; i >= 0; i--) {
    if (currentBoard[i][col].getColor() != color) {
      nextPositions.push_back(std::make_pair(i, col));
      if (currentBoard[i][col].getType() != EMPTY) break;
    } else {
      break;
    }
  }

  // Check possible moves along the column downwards
  for (int i = row + 1; i < Board::SIZE; i++) {
    if (currentBoard[i][col].getColor() != color) {
      nextPositions.push_back(std::make_pair(i, col));
      if (currentBoard[i][col].getType() != EMPTY) break;
    } else {
      break;
    }
  }

  return nextPositions;
}

// Move for a queen
std::vector<std::pair<int, int>> Bot::nextQueenPositions(
    const std::pair<int, int> &position, PlaySide color,
    BoardPiece currentBoard[8][8]) {
  std::vector<std::pair<int, int>> nextPositions;

  // Get possible moves for a bishop and rook
  std::vector<std::pair<int, int>> bishopMoves =
      nextBishopPositions(position, color, currentBoard);
  std::vector<std::pair<int, int>> rookMoves =
      nextRookPositions(position, color, currentBoard);

  // Combine the moves into a single vector
  nextPositions.insert(nextPositions.end(), bishopMoves.begin(),
                       bishopMoves.end());
  nextPositions.insert(nextPositions.end(), rookMoves.begin(), rookMoves.end());

  return nextPositions;
}

std::vector<std::pair<int, int>> Bot::findAttackers(
    std::pair<int, int> position, PlaySide attackingSide,
    BoardPiece boardCopy[8][8]) {
  std::vector<std::pair<int, int>> attackers;
  PlaySide enemySide = getEngineSide() == WHITE ? BLACK : WHITE;
  // Iterate over all pieces on the board
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      std::vector<std::pair<int, int>> nextPositions;
      if (boardCopy[i][j].getColor() == enemySide) {
        std::pair<int, int> currentPos = std::make_pair(i, j);

        switch (boardCopy[i][j].getType()) {
          case PAWN:
            nextPositions = nextPawnPositions(currentPos, enemySide, boardCopy);
            break;
          case KNIGHT:
            nextPositions =
                nextKnightPositions(currentPos, enemySide, boardCopy);
            break;
          case BISHOP:
            nextPositions =
                nextBishopPositions(currentPos, enemySide, boardCopy);
            break;
          case ROOK:
            nextPositions = nextRookPositions(currentPos, enemySide, boardCopy);
            break;
          case QUEEN:
            nextPositions =
                nextQueenPositions(currentPos, enemySide, boardCopy);
            break;
          case KING:
            for (int k = -1; k <= 1; k++) {
              for (int l = -1; l <= 1; l++) {
                if (k == 0 && l == 0) {
                  continue;
                }
                int x = currentPos.first + k;
                int y = currentPos.second + l;
                if (x >= 0 && x < 8 && y >= 0 && y < 8 &&
                    boardCopy[x][y].getType() == KING &&
                    boardCopy[x][y].getColor() == getEngineSide()) {
                  attackers.push_back(currentPos);
                  break;
                }
              }
            }
            break;
          default:
            break;
        }
        for (auto pos : nextPositions) {
          if (boardCopy[pos.first][pos.second].getType() == KING &&
              boardCopy[pos.first][pos.second].getColor() == getEngineSide()) {
            attackers.push_back(currentPos);
            break;
          }
        }
      }
    }
  }
  return attackers;
}

bool Bot::isCheckmate(PlaySide side) {
  // Check if the player's king is in check
  if (!isCheck(side)) {
    return false;  // King is not in check, not a checkmate
  }

  std::vector<Move *> safeMoves = returnSafeMoves(side);

  return safeMoves.empty();  // No valid move can prevent checkmate
}

bool Bot::isCheck(bool side) {
  // Find the player's king
  PlaySide playerSide;
  int kingRow, kingCol;
  PlaySide enemySide;

  if (side == true) {
    playerSide = getEngineSide();
    enemySide = getEngineSide() == WHITE ? BLACK : WHITE;
  } else {
    enemySide = getEngineSide();
    playerSide = getEngineSide() == WHITE ? BLACK : WHITE;
  }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (board[i][j].getColor() == playerSide &&
          board[i][j].getType() == KING) {
        kingRow = i;
        kingCol = j;
        break;
      }
    }
  }

  // Check if the player's king is under attack
  std::pair<int, int> kingPosition = std::make_pair(kingRow, kingCol);
  std::vector<std::pair<int, int>> attackers =
      findAttackers(kingPosition, enemySide, board);
  return !attackers.empty();
}

/**
 * Check if the move puts the enemy king in check.
 * @param move move to be checked
 * @return true if the move puts the enemy king in check, false otherwise
 */
bool Bot::isCheckAfterMove(int srcRow, int srcCol, int destRow, int destCol,
                           Piece piece) {
  // Make a copy of the board and apply the move to it
  BoardPiece boardCopy[8][8];
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      boardCopy[i][j] = board[i][j];
    }
  }

  if (piece != Piece::EMPTY && srcRow == -1 && srcCol == -1) {
    // Drop-in move
    boardCopy[destRow][destCol] = BoardPiece(getEngineSide(), piece);
  } else if (boardCopy[srcRow][srcCol].getType() == PAWN &&
             destRow == srcRow + (getEngineSide() == WHITE ? -2 : 2)) {
    // En passant move
    boardCopy[srcRow][destCol] = BoardPiece(PlaySide::NONE, Piece::EMPTY);
    boardCopy[destRow][destCol] = boardCopy[srcRow][srcCol];
    boardCopy[srcRow][srcCol] = BoardPiece(PlaySide::NONE, Piece::EMPTY);
  } else {
    // Normal move
    boardCopy[destRow][destCol] = boardCopy[srcRow][srcCol];
    boardCopy[srcRow][srcCol] = BoardPiece(PlaySide::NONE, Piece::EMPTY);

    // Promotion move
    if (boardCopy[destRow][destCol].getType() == PAWN &&
        (destRow == 0 || destRow == 7)) {
      boardCopy[destRow][destCol] = BoardPiece(getEngineSide(), QUEEN);
    }
  }
  // int kingc, kingl;
  // for (int i = 0; i < 8; i++)
  // {
  //   for (int j = 0; j < 8; j++)
  //   {
  //     if (boardCopy[i][j].getType() == KING)
  //     {
  //       kingl = i;
  //       kingc = j;
  //     }
  //   }
  // }

  // Find the enemy king
  PlaySide enemySide = getEngineSide() == WHITE ? BLACK : WHITE;
  int kingRow, kingCol;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (boardCopy[i][j].getColor() == getEngineSide() &&
          boardCopy[i][j].getType() == KING) {
        kingRow = i;
        kingCol = j;
        break;
      }
    }
  }
  // Check if the enemy king is under attack
  std::pair<int, int> kingPosition = std::make_pair(kingRow, kingCol);
  std::vector<std::pair<int, int>> attackers =
      findAttackers(kingPosition, enemySide, boardCopy);

  return !attackers.empty();
}

// Move for king
std::vector<std::pair<int, int>> Bot::nextKingPositions(
    const std::pair<int, int> &position, PlaySide color, int &ifCastle) {
  std::vector<std::pair<int, int>> nextPositions;
  int row = position.first;
  int col = position.second;
  if (board[row][col].getHasMoved() == false) {
    // Check if king and rook are in their starting positions
    int kingRow = color == WHITE ? 0 : 7;
    if (col == 4 && row == kingRow) {
      BoardPiece kingSideRook = board[kingRow][7];
      BoardPiece queenSideRook = board[kingRow][0];
      if (kingSideRook.getType() == ROOK && !board[kingRow][7].getHasMoved()) {
        // Check if the squares between king and king-side rook are empty
        if (board[kingRow][5].getType() == EMPTY &&
            board[kingRow][6].getType() == EMPTY) {
          // Check if moving to the squares between king and king-side rook
          // would not result in the king being in check
          if (!isCheckAfterMove(row, col, kingRow, 5, EMPTY) &&
              !isCheckAfterMove(row, col, kingRow, 6, EMPTY)) {
            // write the code for change rook
            ifCastle = 1;
            nextPositions.push_back(std::make_pair(kingRow, 6));
          }
        }
      }
      if (queenSideRook.getType() == ROOK && !board[kingRow][0].getHasMoved()) {
        // Check if the squares between king and queen-side rook are empty
        if (board[kingRow][1].getType() == EMPTY &&
            board[kingRow][2].getType() == EMPTY &&
            board[kingRow][3].getType() == EMPTY) {
          // Check if moving to the squares between king and queen-side rook
          // would not result in the king being in check
          if (!isCheckAfterMove(row, col, kingRow, 2, EMPTY) &&
              !isCheckAfterMove(row, col, kingRow, 3, EMPTY)) {
            ifCastle = 1;
            nextPositions.push_back(std::make_pair(kingRow, 2));
          }
        }
      }
    }
  }
  // Check moves in all eight directions
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      // Ignore current position
      if (i == 0 && j == 0) {
        continue;
      }

      int nextRow = row + i;
      int nextCol = col + j;

      // Check if next position is within board bounds
      if (nextRow >= 0 && nextRow < Board::SIZE && nextCol >= 0 &&
          nextCol < Board::SIZE) {
        BoardPiece piece = board[nextRow][nextCol];
        // Check if next position is empty or contains an enemy piece
        if (piece.getType() == EMPTY || piece.getColor() != color) {
          // Check if moving to the next position would not result in the king
          // being in check
          if (!isCheckAfterMove(row, col, nextRow, nextCol, EMPTY)) {
            nextPositions.push_back(std::make_pair(nextRow, nextCol));
          }
        }
      }
    }
  }
  return nextPositions;
}

void Bot::recordMove(Move *move, PlaySide sideToMove) {
  /* You might find it useful to also separately
   * record last move in another custom field */

  if (move->isNormal()) {
    int sourceRow = move->source.value()[1] - '1';
    int sourceCol = move->source.value()[0] - 'a';
    int destRow = move->destination.value()[1] - '1';
    int destCol = move->destination.value()[0] - 'a';

    // PlaySide side = getEngineSide();
    if (board[sourceRow][sourceCol].getType() == KING &&
        abs(sourceCol - destCol) == 2) {
      if (destCol == 6) {
        // Kingside castling
        board[destRow][5] =
            board[destRow][7];  // Move the rook to its new position
        board[destRow][7] = BoardPiece(
            PlaySide::NONE,
            Piece::EMPTY);  // Set the original rook position to an empty space
        board[destRow][5].setHasMoved(true);

        // Move the king to its new position
        board[destRow][6] = board[sourceRow][sourceCol];
        board[sourceRow][sourceCol] = BoardPiece(
            PlaySide::NONE,
            Piece::EMPTY);  // Set the original king position to an empty space
        board[destRow][6].setHasMoved(true);
      } else {
        // Queenside castling
        board[destRow][3] =
            board[destRow][0];  // Move the rook to its new position
        board[destRow][0] = BoardPiece(
            PlaySide::NONE,
            Piece::EMPTY);  // Set the original rook position to an empty space
        board[destRow][3].setHasMoved(true);

        // Move the king to its new position
        board[destRow][2] = board[sourceRow][sourceCol];
        board[sourceRow][sourceCol] = BoardPiece(
            PlaySide::NONE,
            Piece::EMPTY);  // Set the original king position to an empty space
        board[destRow][2].setHasMoved(true);
      }
    }

    int index = board[destRow][destCol].getPromotion() == true
                    ? PAWN
                    : board[destRow][destCol].getType();
    if (board[destRow][destCol].getType() != EMPTY) {
      if (sideToMove == WHITE) {
        whiteCapturedPieces[index]++;
      } else {
        blackCapturedPieces[index]++;
      }
    }

    // cout << "MUIE MASA LUI YANNICK " << sideToMove << " " << destRow << " "
    //      << board[sourceRow][sourceCol].getType() << "\n";

    board[destRow][destCol] = board[sourceRow][sourceCol];
    board[sourceRow][sourceCol] = BoardPiece(PlaySide::NONE, Piece::EMPTY);
    board[destRow][destCol].setHasMoved(true);
  }

  if (move->isDropIn()) {
    // cout << "aici trebuie intrat baaa\n";
    int destRow = move->destination.value()[1] - '1';
    int destCol = move->destination.value()[0] - 'a';
    Piece newPiece = move->getReplacement().value();
    board[destRow][destCol] = BoardPiece(sideToMove, newPiece);
    board[destRow][destCol].setHasMoved(false);
    if (sideToMove == WHITE) {
      whiteCapturedPieces[newPiece]--;
    } else {
      blackCapturedPieces[newPiece]--;
    }
  }

  if (move->isPromotion()) {
    int sourceRow = move->source.value()[1] - '1';
    int sourceCol = move->source.value()[0] - 'a';
    int destRow = move->destination.value()[1] - '1';
    int destCol = move->destination.value()[0] - 'a';
    board[sourceRow][sourceCol].setType(QUEEN);
    board[sourceRow][sourceCol].setPromotion(true);
    board[destRow][destCol] = board[sourceRow][sourceCol];
    board[sourceRow][sourceCol] = BoardPiece(PlaySide::NONE, Piece::EMPTY);
    board[destRow][destCol].setHasMoved(true);
  }
}

std::vector<Move *> Bot::generateMovesForPiece(PlaySide side, int row,
                                               int col) {
  std::vector<Move *> moves;
  std::vector<std::pair<int, int>> nextPositions;

  std::pair<int, int> piece = std::make_pair(row, col);

  // cout << "asta e tipu fraiere"
  //      << " " << board[row][col].getType() << "\n";

  switch (board[row][col].getType()) {
    case PAWN:
      nextPositions = nextPawnPositions(piece, side, board);
      break;
    case KNIGHT:
      nextPositions = nextKnightPositions(piece, side, board);
      break;
    case BISHOP:
      nextPositions = nextBishopPositions(piece, side, board);
      break;
    case ROOK:
      nextPositions = nextRookPositions(piece, side, board);
      break;
    case QUEEN:
      nextPositions = nextQueenPositions(piece, side, board);
      break;
    case KING: {
      int ifCastle = 0;
      nextPositions = nextKingPositions(piece, side, ifCastle);
      break;
    }
    case EMPTY: {
      // cout << "am intrat aici in plm\n";
      std::string destStr = std::string(1, 'a' + col) + std::to_string(row + 1);
      for (int i = 0; i < 7; i++) {
        Piece piece;
        if (side == WHITE) {
          if (whiteCapturedPieces[i] != 0) {
            if (i != 0 || (i == 0 && (row != 0 && row != 7))) {
              piece = Piece(i);
              Move *move = Move::dropIn(destStr, piece);
              moves.push_back(move);
            }
          }
        } else {
          if (blackCapturedPieces[i] != 0) {
            if (i != 0 || (i == 0 && (row != 0 && row != 7))) {
              piece = Piece(i);
              Move *move = Move::dropIn(destStr, piece);
              moves.push_back(move);
            }
          }
        }
      }
      // cout << "a intrat in mortii lui\n";
      // for (Move* move : moves){
      //   cout << move->getDestination().value() << " " <<
      //   move->getReplacement().value() << "\n";
      // }
      return moves;
    }
    default:
      break;
  }

  for (const auto &nextPos : nextPositions) {
    std::string sourceStr = std::string(1, 'a' + col) + std::to_string(row + 1);
    std::string destStr = std::string(1, 'a' + nextPos.second) +
                          std::to_string(nextPos.first + 1);

    Move *move = Move::moveTo(sourceStr, destStr);
    moves.push_back(move);
  }

  return moves;
}

std::vector<Move *> Bot::generateLegalMoves(PlaySide side) {
  std::vector<Move *> legalMoves;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (board[i][j].getColor() == side || board[i][j].getType() == EMPTY) {
        std::vector<Move *> nextMoves = generateMovesForPiece(side, i, j);
        legalMoves.insert(legalMoves.end(), nextMoves.begin(), nextMoves.end());
      }
    }
  }
  return legalMoves;
}

std::vector<Move *> Bot::returnSafeMoves(PlaySide side) {
  std::vector<Move *> legalMoves = generateLegalMoves(side);
  std::vector<Move *> safeMoves;
  for (Move *move : legalMoves) {
    // Apply the move to a copy of the board
    // BoardPiece boardCopy[8][8];
    // copyBoard(board, boardCopy);
    // applyMove(move, boardCopy);
    int sourceRow;
    int sourceCol;
    if (move->getSource().has_value()) {
      sourceRow = move->source.value()[1] - '1';
      sourceCol = move->source.value()[0] - 'a';
    } else  // cout << "a intrat in mortii lui\n";
    // for (Move* move : moves){
    //   cout << move->getDestination().value() << " " <<
    //   move->getReplacement().value() << "\n";
    // }
    {
      sourceRow = -1;
      sourceCol = -1;
    }
    int destRow = move->destination.value()[1] - '1';
    int destCol = move->destination.value()[0] - 'a';
    // Check if the king is still in check after the move
    bool isInCheck;
    if (sourceRow == -1 && sourceCol == -1) {
      isInCheck = isCheckAfterMove(sourceRow, sourceCol, destRow, destCol,
                                   move->getReplacement().value());
    } else {
      isInCheck =
          isCheckAfterMove(sourceRow, sourceCol, destRow, destCol, EMPTY);
    }
    if (!isInCheck) {
      // The move is safe
      safeMoves.push_back(move);
    } else {
      // The move puts the king in check, so delete it
      delete move;
    }
  }
  return safeMoves;
}

BoardPiece Bot::generateOneMove(Move *move) {
  PlaySide sideToMove = getEngineSide();
  // Store the captured piece (if any)
  BoardPiece capturedPiece = BoardPiece();

  if (move->isNormal()) {
    int sourceRow = move->source.value()[1] - '1';
    int sourceCol = move->source.value()[0] - 'a';
    int destRow = move->destination.value()[1] - '1';
    int destCol = move->destination.value()[0] - 'a';

    if (board[destRow][destCol].getType() != EMPTY) {
      capturedPiece = board[destRow][destCol];
    }

    // PlaySide side = getEngineSide();
    if (board[sourceRow][sourceCol].getType() == KING &&
        abs(sourceCol - destCol) == 2) {
      if (destCol == 6) {
        // Kingside castling
        board[destRow][5] =
            board[destRow][7];  // Move the rook to its new position
        board[destRow][7] = BoardPiece(
            PlaySide::NONE,
            Piece::EMPTY);  // Set the original rook position to an empty space
        board[destRow][5].setHasMoved(true);

        // Move the king to its new position
        board[destRow][6] = board[sourceRow][sourceCol];
        board[sourceRow][sourceCol] = BoardPiece(
            PlaySide::NONE,
            Piece::EMPTY);  // Set the original king position to an empty space
        board[destRow][6].setHasMoved(true);
      } else {
        // Queenside castling
        board[destRow][3] =
            board[destRow][0];  // Move the rook to its new position
        board[destRow][0] = BoardPiece(
            PlaySide::NONE,
            Piece::EMPTY);  // Set the original rook position to an empty space
        board[destRow][3].setHasMoved(true);

        // Move the king to its new position
        board[destRow][2] = board[sourceRow][sourceCol];
        board[sourceRow][sourceCol] = BoardPiece(
            PlaySide::NONE,
            Piece::EMPTY);  // Set the original king position to an empty space
        board[destRow][2].setHasMoved(true);
      }
      return capturedPiece;
    }

    int index = board[destRow][destCol].getPromotion() == true
                    ? PAWN
                    : board[destRow][destCol].getType();
    if (board[destRow][destCol].getType() != EMPTY) {
      if (sideToMove == WHITE) {
        whiteCapturedPieces[index]++;
      } else {
        blackCapturedPieces[index]++;
      }
    }

    board[destRow][destCol] = board[sourceRow][sourceCol];
    board[sourceRow][sourceCol] = BoardPiece(PlaySide::NONE, Piece::EMPTY);
    board[destRow][destCol].setHasMoved(true);
  }

  if (move->isDropIn()) {
    int destRow = move->destination.value()[1] - '1';
    int destCol = move->destination.value()[0] - 'a';
    Piece newPiece = move->getReplacement().value();
    board[destRow][destCol] = BoardPiece(sideToMove, newPiece);
    board[destRow][destCol].setHasMoved(false);
    if (sideToMove == WHITE) {
      whiteCapturedPieces[newPiece]--;
    } else {
      blackCapturedPieces[newPiece]--;
    }
  }

  if (move->isPromotion()) {
    int sourceRow = move->source.value()[1] - '1';
    int sourceCol = move->source.value()[0] - 'a';
    int destRow = move->destination.value()[1] - '1';
    int destCol = move->destination.value()[0] - 'a';
    if (board[destRow][destCol].getType() != EMPTY) {
      capturedPiece = board[destRow][destCol];
    }

    board[sourceRow][sourceCol].setType(QUEEN);
    board[sourceRow][sourceCol].setPromotion(true);
    board[destRow][destCol] = board[sourceRow][sourceCol];
    board[sourceRow][sourceCol] = BoardPiece(PlaySide::NONE, Piece::EMPTY);
    board[destRow][destCol].setHasMoved(true);
  }

  return capturedPiece;
}

void Bot::removeOneMove(Move *move, BoardPiece capturedPiece) {
  PlaySide sideToMove = getEngineSide();

  if (move->isNormal()) {
    int sourceRow = move->source.value()[1] - '1';
    int sourceCol = move->source.value()[0] - 'a';
    int destRow = move->destination.value()[1] - '1';
    int destCol = move->destination.value()[0] - 'a';
    // Reverse the move logic
    if (board[destRow][destCol].getType() == KING &&
        abs(sourceCol - destCol) == 2) {
      if (destCol == 6) {
        // Reverse kingside castling
        board[destRow][7] =
            board[destRow][5];  // Move the rook back to its original position
        board[destRow][7].setHasMoved(false);
        board[destRow][5] = BoardPiece(PlaySide::NONE, Piece::EMPTY);

        board[sourceRow][sourceCol] =
            board[destRow][6];  // Move the king back to its original position
        board[sourceRow][sourceCol].setHasMoved(false);
        board[destRow][6] = BoardPiece(PlaySide::NONE, Piece::EMPTY);
      } else {
        // Reverse queenside castling
        board[destRow][0] =
            board[destRow][3];  // Move the rook back to its original position
        board[destRow][0].setHasMoved(false);
        board[destRow][3] = BoardPiece(PlaySide::NONE, Piece::EMPTY);

        board[sourceRow][sourceCol] =
            board[destRow][2];  // Move the king back to its original position
        board[sourceRow][sourceCol].setHasMoved(false);
        board[destRow][2] = BoardPiece(PlaySide::NONE, Piece::EMPTY);
      }

      return;
    }

    // int index = board[sourceRow][sourceCol].getPromotion() == true
    //                 ? PAWN
    //                 : board[sourceRow][sourceCol].getType();

    // // Restore the captured piece
    // if (board[sourceRow][sourceCol].getType() != EMPTY) {
    //   if (sideToMove == WHITE) {
    //     whiteCapturedPieces[index]--;
    //     if (board[destRow][destCol].getType() != EMPTY) {
    //       Piece capturedPiece = board[destRow][destCol].getType();
    //       whiteCapturedPieces[capturedPiece]++;
    //     }
    //   } else {
    //     blackCapturedPieces[index]--;
    //     if (board[destRow][destCol].getType() != EMPTY) {
    //       Piece capturedPiece = board[destRow][destCol].getType();
    //       blackCapturedPieces[capturedPiece]++;
    //     }
    //   }
    // }

    board[sourceRow][sourceCol] = board[destRow][destCol];
    // board[sourceRow][sourceCol].setHasMoved(false);
    board[destRow][destCol] = BoardPiece(PlaySide::NONE, Piece::EMPTY);

    int index =
        capturedPiece.getPromotion() == true ? PAWN : capturedPiece.getType();

    // cout << "Type: " << board[destRow][destCol].getType()
    //      << " capture: " << capturedPiece.getType() << endl;

    if (capturedPiece.getType() != EMPTY) {
      if (sideToMove == WHITE) {
        whiteCapturedPieces[index]--;
      } else {
        blackCapturedPieces[index]--;
      }
    }

    // Restore the captured piece (if any)
    if (capturedPiece.getType() != Piece::EMPTY) {
      // Restore the captured piece on the destination square
      board[destRow][destCol] = capturedPiece;
    }
  }

  if (move->isDropIn()) {
    int destRow = move->destination.value()[1] - '1';
    int destCol = move->destination.value()[0] - 'a';
    // Piece newPiece = move->getReplacement().value();

    // Restore the captured piece
    if (sideToMove == WHITE) {
      // if (board[destRow][destCol].getType() != EMPTY) {
      //   Piece capturedPiece = board[destRow][destCol].getType();
      //   whiteCapturedPieces[capturedPiece]++;
      whiteCapturedPieces[board[destRow][destCol].getType()]++;
      // board[destRow][destCol] = BoardPiece();
    } else {
      // if (board[destRow][destCol].getType() != EMPTY) {
      //   Piece capturedPiece = board[destRow][destCol].getType();
      //   blackCapturedPieces[capturedPiece]++;
      blackCapturedPieces[board[destRow][destCol].getType()]++;
      // board[destRow][destCol] = BoardPiece();
    }

    board[destRow][destCol] = BoardPiece(PlaySide::NONE, Piece::EMPTY);
  }

  if (move->isPromotion()) {
    int sourceRow = move->source.value()[1] - '1';
    int sourceCol = move->source.value()[0] - 'a';
    int destRow = move->destination.value()[1] - '1';
    int destCol = move->destination.value()[0] - 'a';
    board[sourceRow][sourceCol].setType(PAWN);
    board[sourceRow][sourceCol].setPromotion(false);
    board[sourceRow][sourceCol].setHasMoved(false);

    board[destRow][destCol] = BoardPiece(PlaySide::NONE, Piece::EMPTY);

    // Restore the captured piece (if any)
    if (capturedPiece.getType() != Piece::EMPTY) {
      // Restore the captured piece on the destination square
      board[destRow][destCol] = capturedPiece;
      if (sideToMove == WHITE) {
        whiteCapturedPieces[capturedPiece.getType()]--;
      } else {
        blackCapturedPieces[capturedPiece.getType()]--;
      }
    }
  }
}

int pawnPositionScores[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},         {50, 50, 50, 50, 50, 50, 50, 50},
    {10, 10, 20, 30, 30, 20, 10, 10}, {5, 5, 10, 25, 25, 10, 5, 5},
    {0, 0, 0, 20, 20, 0, 0, 0},       {5, -5, -10, 0, 0, -10, -5, 5},
    {5, 10, 10, -20, -20, 10, 10, 5}, {0, 0, 0, 0, 0, 0, 0, 0}};

int knightPositionScores[8][8] = {{-50, -40, -30, -30, -30, -30, -40, -50},
                                  {-40, -20, 0, 0, 0, 0, -20, -40},
                                  {-30, 0, 10, 15, 15, 10, 0, -30},
                                  {-30, 5, 15, 20, 20, 15, 5, -30},
                                  {-30, 0, 15, 20, 20, 15, 0, -30},
                                  {-30, 5, 10, 15, 15, 10, 5, -30},
                                  {-40, -20, 0, 5, 5, 0, -20, -40},
                                  {-50, -40, -30, -30, -30, -30, -40, -50}};

int bishopPositionScores[8][8] = {{-20, -10, -10, -10, -10, -10, -10, -20},
                                  {-10, 0, 0, 0, 0, 0, 0, -10},
                                  {-10, 0, 5, 10, 10, 5, 0, -10},
                                  {-10, 5, 5, 10, 10, 5, 5, -10},
                                  {-10, 0, 10, 10, 10, 10, 0, -10},
                                  {-10, 10, 10, 10, 10, 10, 10, -10},
                                  {-10, 5, 0, 0, 0, 0, 5, -10},
                                  {-20, -10, -10, -10, -10, -10, -10, -20}};

int rookPositionScores[8][8] = {
    {0, 0, 0, 5, 5, 0, 0, 0},       {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5},     {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5},     {-5, 0, 0, 0, 0, 0, 0, -5},
    {5, 10, 10, 10, 10, 10, 10, 5}, {0, 0, 0, 0, 0, 0, 0, 0}};

int queenPositionScores[8][8] = {{-20, -10, -10, -5, -5, -10, -10, -20},
                                 {-10, 0, 0, 0, 0, 0, 0, -10},
                                 {-10, 0, 5, 5, 5, 5, 0, -10},
                                 {-5, 0, 5, 5, 5, 5, 0, -5},
                                 {0, 0, 5, 5, 5, 5, 0, -5},
                                 {-10, 5, 5, 5, 5, 5, 0, -10},
                                 {-10, 0, 5, 0, 0, 0, 0, -10},
                                 {-20, -10, -10, -5, -5, -10, -10, -20}};

int kingPositionScores[8][8] = {{-30, -40, -40, -50, -50, -40, -40, -30},
                                {-30, -40, -40, -50, -50, -40, -40, -30},
                                {-30, -40, -40, -50, -50, -40, -40, -30},
                                {-30, -40, -40, -50, -50, -40, -40, -30},
                                {-20, -30, -30, -40, -40, -30, -30, -20},
                                {-10, -20, -20, -20, -20, -20, -20, -10},
                                {20, 20, 0, 0, 0, 0, 20, 20},
                                {20, 30, 10, 0, 0, 10, 30, 20}};
int pawnScore = 1000;
int knightScore = 3200;
int bishopScore = 3300;
int rookScore = 5000;
int queenScore = 9000;
int checkPenalty = 500;

int Bot::pawnStructure(PlaySide side) {
  int score = 0;

  // Define the pawn structure evaluation scores
  const int isolatedPawnPenalty = -20;  // Penalty for isolated pawns
  const int doubledPawnPenalty = -10;   // Penalty for doubled pawns
  const int backwardPawnPenalty = -15;  // Penalty for backward pawns
  const int passedPawnBonus = 20;       // Bonus for passed pawns

  PlaySide enemySide = (side == WHITE) ? BLACK : WHITE;

  // Check pawn structure for each column
  for (int col = 0; col < 8; col++) {
    bool isolatedPawn = true;
    bool doubledPawn = false;
    bool backwardPawn = true;

    for (int row = 0; row < 8; row++) {
      BoardPiece piece = board[row][col];

      // Check if the piece is a pawn of the given side
      if (piece.getType() == PAWN && piece.getColor() == side) {
        // Check if the pawn is isolated
        if ((col > 0 && board[row][col - 1].getType() == PAWN &&
             board[row][col - 1].getColor() == side) ||
            (col < 7 && board[row][col + 1].getType() == PAWN &&
             board[row][col + 1].getColor() == side)) {
          isolatedPawn = false;
        }

        // Check if the pawn is doubled
        if (row > 0 && board[row - 1][col].getType() == PAWN &&
            board[row - 1][col].getColor() == side) {
          doubledPawn = true;
        }

        // Check if the pawn is backward
        if (row < 7 && board[row + 1][col].getType() == PAWN &&
            board[row + 1][col].getColor() == side) {
          backwardPawn = false;
        }

        // Check if the pawn is passed
        bool passedPawn = true;
        for (int enemyCol = 0; enemyCol < 8; enemyCol++) {
          for (int enemyRow = 0; enemyRow < 8; enemyRow++) {
            BoardPiece enemyPiece = board[enemyRow][enemyCol];
            if (enemyPiece.getType() == PAWN &&
                enemyPiece.getColor() == enemySide) {
              // Check if there is an enemy pawn that can block the path of the
              // pawn
              if ((col == enemyCol || col == enemyCol - 1 ||
                   col == enemyCol + 1) &&
                  enemyRow >= row && enemyRow <= row + 2) {
                passedPawn = false;
                break;
              }
            }
          }
          if (!passedPawn) {
            break;
          }
        }

        // Update the score based on the pawn structure
        if (isolatedPawn) {
          score += isolatedPawnPenalty;
        }
        if (doubledPawn) {
          score += doubledPawnPenalty;
        }
        if (backwardPawn) {
          score += backwardPawnPenalty;
        }
        if (passedPawn) {
          score += passedPawnBonus;
        }
      }
    }
  }

  return score;
}

int Bot::kingSafety(PlaySide side) {
  int score = 0;

  // Define the king safety evaluation scores
  const int kingPawnShieldBonus = 10;  // Bonus for pawns in front of the king
  const int kingExposedPenalty = -30;  // Penalty for an exposed king
  const int kingCastledBonus = 20;     // Bonus for castled king

  int kingRow, kingCol;

  // Find the player's king
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      BoardPiece piece = board[row][col];
      if (piece.getType() == KING && piece.getColor() == side) {
        kingRow = row;
        kingCol = col;
        break;
      }
    }
  }

  // Check pawn shield in front of the king
  int pawnShieldCount = 0;
  if (side == WHITE) {
    if (kingRow > 0 && kingCol > 0 &&
        board[kingRow - 1][kingCol - 1].getType() == PAWN &&
        board[kingRow - 1][kingCol - 1].getColor() == side) {
      pawnShieldCount++;
    }
    if (kingRow > 0 && board[kingRow - 1][kingCol].getType() == PAWN &&
        board[kingRow - 1][kingCol].getColor() == side) {
      pawnShieldCount++;
    }
    if (kingRow > 0 && kingCol < 7 &&
        board[kingRow - 1][kingCol + 1].getType() == PAWN &&
        board[kingRow - 1][kingCol + 1].getColor() == side) {
      pawnShieldCount++;
    }
  } else {
    if (kingRow < 7 && kingCol > 0 &&
        board[kingRow + 1][kingCol - 1].getType() == PAWN &&
        board[kingRow + 1][kingCol - 1].getColor() == side) {
      pawnShieldCount++;
    }
    if (kingRow < 7 && board[kingRow + 1][kingCol].getType() == PAWN &&
        board[kingRow + 1][kingCol].getColor() == side) {
      pawnShieldCount++;
    }
    if (kingRow < 7 && kingCol < 7 &&
        board[kingRow + 1][kingCol + 1].getType() == PAWN &&
        board[kingRow + 1][kingCol + 1].getColor() == side) {
      pawnShieldCount++;
    }
  }
  score += pawnShieldCount * kingPawnShieldBonus;

  // Check if the king is exposed
  if (kingRow > 0 && kingCol > 0 && kingRow < 7 && kingCol < 7) {
    if (board[kingRow - 1][kingCol - 1].getColor() != side &&
        board[kingRow - 1][kingCol].getColor() != side &&
        board[kingRow - 1][kingCol + 1].getColor() != side &&
        board[kingRow][kingCol - 1].getColor() != side &&
        board[kingRow][kingCol + 1].getColor() != side &&
        board[kingRow + 1][kingCol - 1].getColor() != side &&
        board[kingRow + 1][kingCol].getColor() != side &&
        board[kingRow + 1][kingCol + 1].getColor() != side) {
      score += kingExposedPenalty;
    }
  }

  // Check if the king is castled
  if (side == WHITE && kingRow == 0 && kingCol == 4) {
    if (board[0][0].getType() == ROOK && board[0][0].getColor() == side &&
        board[0][1].getType() == EMPTY && board[0][2].getType() == EMPTY &&
        board[0][3].getType() == EMPTY && board[0][4].getType() == KING &&
        board[0][4].getColor() == side && board[0][5].getType() == EMPTY &&
        board[0][6].getType() == EMPTY && board[0][7].getType() == ROOK &&
        board[0][7].getColor() == side) {
      score += kingCastledBonus;
    }
  } else if (side == BLACK && kingRow == 7 && kingCol == 4) {
    if (board[7][0].getType() == ROOK && board[7][0].getColor() == side &&
        board[7][1].getType() == EMPTY && board[7][2].getType() == EMPTY &&
        board[7][3].getType() == EMPTY && board[7][4].getType() == KING &&
        board[7][4].getColor() == side && board[7][5].getType() == EMPTY &&
        board[7][6].getType() == EMPTY && board[7][7].getType() == ROOK &&
        board[7][7].getColor() == side) {
      score += kingCastledBonus;
    }
  }

  return score;
}

int Bot::pieceDevelopment(PlaySide side) {
  int score = 0;

  // Define the piece development weights
  const int pieceDevelopmentBonus = 10;  // Bonus for developed pieces
  const int centralizationBonus = 5;     // Bonus for centralizing pieces

  // Evaluate the development of each side's pieces
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      BoardPiece piece = board[row][col];
      if (piece.getColor() == side && piece.getType() != EMPTY) {
        // Check if the piece is developed
        bool isDeveloped = false;

        if (side == WHITE && row <= 1) {
          isDeveloped = true;
        } else if (side == BLACK && row >= 6) {
          isDeveloped = true;
        }

        // Check if the piece is centralized
        bool isCentralized = false;

        if (col >= 3 && col <= 4) {
          isCentralized = true;
        }

        // Apply bonuses based on piece development and centralization
        if (isDeveloped) {
          score += pieceDevelopmentBonus;
        }
        if (isCentralized) {
          score += centralizationBonus;
        }
      }
    }
  }

  return score;
}
int Bot::evaluateBoard(bool side) {
  // Define the evaluation scores for each piece type
  PlaySide mySide;

  // if (side == true)
  // {
  //   mySide = getEngineSide();
  //   enemySide = (PlaySide)(1 - getEngineSide());
  // }
  // else
  // {
  //   mySide = (PlaySide)(1 - getEngineSide());
  //   enemySide = getEngineSide();
  // }
  int whiteScore = 0;
  int blackScore = 0;

  // Calculate the total score based on piece values and positions
  int score = 0;

  if (side == true) {
    mySide = getEngineSide();
    // enemySide = (PlaySide)(1 - getEngineSide());
  } else {
    mySide = (PlaySide)(1 - getEngineSide());
    // enemySide = getEngineSide();
  }
  // Check if the current side is in check or checkmate
  if (isCheck(side)) {
    score -= checkPenalty;
    if (isCheckmate(getEngineSide())) {
      if (side) {
        return -100000;  // Return a large negative value for checkmate
      } else {
        return 100000;  // Return a large positive value for checkmate
      }
    }
  }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      BoardPiece piece = board[i][j];

      // Check the piece type and add the corresponding value
      switch (piece.getType()) {
        case PAWN: {
          if (piece.getColor() == BLACK)
            blackScore += pawnScore + pawnPositionScores[i][j];
          else if (piece.getColor() == WHITE) {
            whiteScore += pawnScore + pawnPositionScores[i][j];
          }
          break;
        }
        case KNIGHT: {
          if (piece.getColor() == BLACK)
            blackScore += knightScore + knightPositionScores[i][j];
          else if (piece.getColor() == WHITE) {
            whiteScore += knightScore + knightPositionScores[i][j];
          }
          break;
        }
        case BISHOP: {
          if (piece.getColor() == BLACK)
            blackScore += bishopScore + bishopPositionScores[i][j];
          else if (piece.getColor() == WHITE) {
            whiteScore += bishopScore + bishopPositionScores[i][j];
          }
          break;
        }
        case ROOK: {
          if (piece.getColor() == BLACK)
            blackScore += rookScore + rookPositionScores[i][j];
          else if (piece.getColor() == WHITE) {
            whiteScore += rookScore + rookPositionScores[i][j];
          }
          break;
        }
        case QUEEN: {
          if (piece.getColor() == BLACK)
            blackScore += queenScore + queenPositionScores[i][j];
          else if (piece.getColor() == WHITE) {
            whiteScore += queenScore + queenPositionScores[i][j];
          }
          break;
        }
        case KING: {
          if (piece.getColor() == BLACK)
            blackScore += kingPositionScores[i][j];
          else if (piece.getColor() == WHITE) {
            whiteScore += kingPositionScores[i][j];
          }
          break;
        }
        default:
          break;
      }
    }
  }

  if (mySide == WHITE) {
    score += whiteScore - blackScore;
    score -= pawnStructure(WHITE);
    score -= kingSafety(WHITE);
    score -= pieceDevelopment(WHITE);
  } else {
    score += blackScore - whiteScore;
    score -= pawnStructure(BLACK);
    score -= kingSafety(BLACK);
    score -= pieceDevelopment(BLACK);
  }

  return score;
}

//  void Bot::applyMove(int srcRow, int srcCol, int destRow, int destCol,
//  BoardPiece board[8][8]) {
std::vector<Move *> Bot::returnCaptures(PlaySide side) {
  std::vector<Move *> legalMoves = generateLegalMoves(side);
  std::vector<Move *> safeMoves;
  for (Move *move : legalMoves) {
    // Apply the move to a copy of the board
    // BoardPiece boardCopy[8][8];
    // copyBoard(board, boardCopy);
    // applyMove(move, boardCopy);
    int sourceRow;
    int sourceCol;
    if (move->getSource().has_value()) {
      sourceRow = move->source.value()[1] - '1';
      sourceCol = move->source.value()[0] - 'a';
    } else  // cout << "a intrat in mortii lui\n";
    // for (Move* move : moves){
    //   cout << move->getDestination().value() << " " <<
    //   move->getReplacement().value() << "\n";
    // }
    {
      sourceRow = -1;
      sourceCol = -1;
    }
    int destRow = move->destination.value()[1] - '1';
    int destCol = move->destination.value()[0] - 'a';
    // Check if the king is still in check after the move
    bool isInCheck;
    if (sourceRow == -1 && sourceCol == -1) {
      isInCheck = isCheckAfterMove(sourceRow, sourceCol, destRow, destCol,
                                   move->getReplacement().value());
    } else {
      isInCheck =
          isCheckAfterMove(sourceRow, sourceCol, destRow, destCol, EMPTY);
    }
    if (board[destRow][destCol].getType() == EMPTY) {
      isInCheck = true;
    }
    if (!isInCheck) {
      // The move is safe
      safeMoves.push_back(move);
    } else {
      // The move puts the king in check, so delete it
      delete move;
    }
  }
  return safeMoves;
}

// int Bot::AllCaputres(int alpha, int beta, bool maximizingPlayer) {
//   int evaluation = evaluateBoard(maximizingPlayer);
//   if (evaluation >= beta) return beta;
//   alpha = max(alpha, evaluation);
//   PlaySide currentPlayerSide =
//       maximizingPlayer ? getEngineSide() : (PlaySide)(1 - getEngineSide());
//   std::vector<Move *> possibleMoves = returnCaptures(currentPlayerSide);
//   std::sort(
//       possibleMoves.begin(), possibleMoves.end(),
//       [this](Move *move1, Move *move2) { return CompareMoves(move1, move2);
//       });
//   for (auto move : possibleMoves) {
//     BoardPiece capturedPiece = generateOneMove(move);

//     // Recursively call minimax with alpha-beta pruning
//     if (maximizingPlayer == true) {
//       evaluation = (-1) * AllCaputres((-1) * alpha, (-1) * beta, false);
//     } else {
//       evaluation = (-1) * AllCaputres((-1) * alpha, (-1) * beta, true);
//     }
//     // Update the maximum evaluation and alpha value

//     // Degenerate the move
//     removeOneMove(move, capturedPiece);
//     if (evaluation >= beta) return beta;
//     alpha = max(alpha, evaluation);
//   }
//   return alpha;
// }

int Bot::minimax(int depth, int alpha, int beta, bool maximizingPlayer) {
  // Check if the game has ended or the maximum depth has been reached
  if (depth == 0) {
    // Evaluate the board position
    return evaluateBoard(maximizingPlayer);
  }

  // Find all possible moves for the current player
  PlaySide currentPlayerSide =
      maximizingPlayer ? getEngineSide() : (PlaySide)(1 - getEngineSide());
  std::vector<Move *> possibleMoves = returnSafeMoves(currentPlayerSide);
  std::sort(
      possibleMoves.begin(), possibleMoves.end(),
      [this](Move *move1, Move *move2) { return CompareMoves(move1, move2); });

  // Apply minimax with alpha-beta pruning
  if (maximizingPlayer) {
    // Maximizing player's turn
    int maxEval = std::numeric_limits<int>::min();
    for (auto move : possibleMoves) {
      // Apply the move
      BoardPiece capturedPiece = generateOneMove(move);

      // Recursively call minimax with alpha-beta pruning
      int eval = minimax(depth - 1, alpha, beta, false);

      // Update the maximum evaluation and alpha value
      maxEval = std::max(maxEval, eval);
      alpha = std::max(alpha, eval);

      // Degenerate the move
      removeOneMove(move, capturedPiece);

      // Perform alpha-beta pruning
      if (beta <= alpha) {
        break;
      }
    }
    return maxEval;
  } else {
    // Minimizing player's turn
    int minEval = std::numeric_limits<int>::max();
    for (auto move : possibleMoves) {
      // Apply the move
      BoardPiece capturedPiece = generateOneMove(move);

      // Recursively call minimax with alpha-beta pruning
      int eval = minimax(depth - 1, alpha, beta, true);

      // Update the minimum evaluation and beta value
      minEval = std::min(minEval, eval);
      beta = std::min(beta, eval);

      // Degenerate the move
      removeOneMove(move, capturedPiece);

      // Perform alpha-beta pruning
      if (beta <= alpha) {
        break;
      }
    }
    return minEval;
  }
}

int Bot::getScore(Piece currentPiece) {
  switch (currentPiece) {
    {
      case PAWN:
        return 100;
        break;
      case KNIGHT:
        return 320;
        break;
      case BISHOP:
        return 330;
        break;
      case ROOK:
        return 500;
        break;
      case QUEEN:
        return 900;
        break;
      case KING: {
        return 0;
        break;
      }
      case EMPTY: {
        return 0;
        break;
      }
    }
  }
  return 0;
}

int Bot::evaluateMove(Move *move) {
  int moveScore = 0;
  BoardPiece capturedPiece = generateOneMove(move);
  moveScore += getScore(capturedPiece.getType());
  removeOneMove(move, capturedPiece);

  return moveScore;
}

bool Bot::CompareMoves(Move *move1, Move *move2) {
  int score1 = evaluateMove(move1);
  int score2 = evaluateMove(move2);

  return score1 > score2;
}

Move *Bot::calculateNextMove() {
  /* Play move for the side the engine is playing (Hint: getEngineSide())
   * Make sure to record your move in custom structures before returning.
   *
   * Return move that you are willing to submit
   * Move is to be constructed via one of the factory methods declared in Move.
   */

  // Initialize the best move with invalid values
  PlaySide side = getEngineSide();

  // Set the initial alpha and beta values
  int alpha = std::numeric_limits<int>::min();
  int beta = std::numeric_limits<int>::max();
  // int maxEval = std::numeric_limits<int>::min();
  Move *bestMove;

  // Find all possible moves for the engine side
  std::vector<Move *> possibleMoves = returnSafeMoves(side);
  std::sort(
      possibleMoves.begin(), possibleMoves.end(),
      [this](Move *move1, Move *move2) { return CompareMoves(move1, move2); });

  if (possibleMoves.size() == 0) {
    return Move::resign();
  }

  //=============
  //   TODO
  int depth = 2;
  //=================
  // Iterate over each possible move and evaluate them using minimax with
  // alpha-beta pruning

  // Debug
  // Print the current state of the board
  // for (int row = 0; row < 8; row++)
  // {
  //   for (int col = 0; col < 8; col++)
  //   {
  //     Piece piece = board[row][col].getType();
  //     char symbol = '-'; // Default symbol for an empty square

  //     // Assign symbols for different pieces
  //     switch (piece)
  //     {
  //     case PAWN:
  //       symbol = 'P';
  //       break;
  //     case KNIGHT:
  //       symbol = 'N';
  //       break;
  //     case BISHOP:
  //       symbol = 'B';
  //       break;
  //     case ROOK:
  //       symbol = 'R';
  //       break;
  //     case QUEEN:
  //       symbol = 'Q';
  //       break;
  //     case KING:
  //       symbol = 'K';
  //       break;
  //     default:
  //       break;
  //     }

  //     // Print the symbol
  //     std::cout << symbol << ' ';
  //   }
  //   std::cout << std::endl;
  // }

  // Add a separator line
  // std::cout << "------------" << std::endl;
  // Debug

  for (auto move : possibleMoves) {
    // Make a copy of the board and apply the move to it

    // Apply the move
    BoardPiece capturedPiece = generateOneMove(move);

    // Evaluate the move using minimax with alpha-beta pruning
    // cout << "dau acici" << endl;
    int moveValue = minimax(depth - 1, alpha, beta, false);
    // Update the best move if the current move is better
    // cout << "asta e val " << moveValue << " " << alpha << endl;
    if (moveValue > alpha) {
      alpha = moveValue;
      bestMove = move;
    }
    removeOneMove(move, capturedPiece);
  }
  // cout << "muieeeee";
  //  Debug
  //  Print the current state of the board
  //  for (int row = 0; row < 8; row++)
  //  {
  //    for (int col = 0; col < 8; col++)
  //    {
  //      Piece piece = board[row][col].getType();
  //      char symbol = '-'; // Default symbol for an empty square

  //     // Assign symbols for different pieces
  //     switch (piece)
  //     {
  //     case PAWN:
  //       symbol = 'P';
  //       break;
  //     case KNIGHT:
  //       symbol = 'N';
  //       break;
  //     case BISHOP:
  //       symbol = 'B';
  //       break;
  //     case ROOK:
  //       symbol = 'R';
  //       break;
  //     case QUEEN:
  //       symbol = 'Q';
  //       break;
  //     case KING:
  //       symbol = 'K';
  //       break;
  //     default:
  //       break;
  //     }

  //     // Print the symbol
  //     std::cout << symbol << ' ';
  //   }
  //   std::cout << std::endl;
  // }

  // Add a separator line
  // std::cout << "------------" << std::endl;
  // Debug

  Move *nextMove = bestMove;
  // cout << "aici creade" << endl;
  cout << bestMove->destination.value()[0] << " "
       << bestMove->destination.value()[1] << endl;
  // cout << "paote apdsada\n";
  //  int randomIndex = rand() % possibleMoves.size();
  //  Move *nextMove = possibleMoves[randomIndex];

  // BoardPiece debug = generateOneMove(nextMove);
  // for (int i = 0; i < 6; i++)
  // {
  //   std::cout << "Bite captured "
  //             << ": " << blackCapturedPieces[i] << std::endl;
  // }

  // removeOneMove(nextMove, debug);
  // for (int i = 0; i < 6; i++)
  // {
  //   std::cout << "Bite captured "
  //             << ": " << blackCapturedPieces[i] << std::endl;
  // }

  if (!nextMove->isDropIn()) {
    int destRow = nextMove->destination.value()[1] - '1';
    // int destCol = nextMove->destination.value()[0] - 'a';
    int sourceRow = nextMove->source.value()[1] - '1';
    int sourceCol = nextMove->source.value()[0] - 'a';
    Piece currentPiece = board[sourceRow][sourceCol].getType();
    if (currentPiece == PAWN && (destRow == 7 || destRow == 0)) {
      Move *newMove =
          Move::promote(nextMove->source, nextMove->destination, QUEEN);
      recordMove(newMove, side);
      return newMove;
    } else {
      recordMove(nextMove, side);
      return nextMove;
    }
  } else {
    recordMove(nextMove, side);
    return nextMove;
  }

  return Move::resign();
}

std::string Bot::getBotName() { return Bot::BOT_NAME; }