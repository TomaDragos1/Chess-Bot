#ifndef BOARDPIECE_H
#define BOARDPIECE_H

#include "Piece.h"
// #include "Color.h"
#include "PlaySide.h"

class BoardPiece
{
public:
    BoardPiece();
    BoardPiece(PlaySide c, Piece t);

    PlaySide getColor() const;
    Piece getType() const;
    bool isEmpty() const;
    bool getHasMoved() const;
    void setHasMoved(bool value);
    void setColor(PlaySide c);
    void setType(Piece t);
    void setDoubleMove(bool value);
    bool getDoubleMove() const;
    void setPromotion(bool value);
    bool getPromotion() const;

private:
    PlaySide color;
    Piece type;
    bool hasMoved;
    bool doubleMove;
    bool isPromoted;
};

#endif /* BOARDPIECE_H */
