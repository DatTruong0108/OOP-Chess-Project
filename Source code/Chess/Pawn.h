#pragma once

#include "Piece.h"

class Pawn : public Piece {
private:
    static const int _pieceValue = 1;
protected:

public:
    explicit Pawn(PieceColor pieceColor = PieceColor::WHITE); //fully-parameterized constructor

    ~Pawn();

    //re-define pure virtual methods in class Piece

    void draw(RenderWindow& window);

    vector<ChessMove> computePossbibleMoves(const vector<vector<Cell>>& cells);    
};