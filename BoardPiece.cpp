
#include "BoardPiece.h"

BoardPiece::BoardPiece() : color(NONE), type(Piece::EMPTY) {}

BoardPiece::BoardPiece(PlaySide c, Piece t) : color(c), type(t), hasMoved(false), isPromoted(false) {}

PlaySide BoardPiece::getColor() const
{
    return color;
}

Piece BoardPiece::getType() const
{
    return type;
}

bool BoardPiece::isEmpty() const
{
    return type == EMPTY;
}

bool BoardPiece::getHasMoved() const
{
    return hasMoved;
}

void BoardPiece::setHasMoved(bool value)
{
    hasMoved = value;
}

void BoardPiece::setType(Piece t)
{
    type = t;
}

void BoardPiece::setColor(PlaySide c)
{
    color = c;
}

void BoardPiece::setDoubleMove(bool value)
{
    doubleMove = value;
}
bool BoardPiece::getDoubleMove() const
{
    return doubleMove;
}

void BoardPiece::setPromotion(bool value)
{
    isPromoted = value;
}

bool BoardPiece::getPromotion() const
{
    return isPromoted;
}
