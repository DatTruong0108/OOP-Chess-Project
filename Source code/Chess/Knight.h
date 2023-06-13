#pragma once

#include "Piece.h"

class Knight : public Piece {
private:
    static const int _pieceValue = 3;
protected:

public:

    explicit Knight(PieceColor pieceColor = PieceColor::WHITE); //fully-parameterized constructor

    ~Knight();

    //re-define pure virtual methods in class Piece

    void draw(RenderWindow& window);

    vector<ChessMove> computePossbibleMoves(const vector<vector<Cell>>& cells);
};