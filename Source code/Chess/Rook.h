#pragma once
#include "Piece.h"

class Rook : public Piece {
private:
    static const int _pieceValue = 5;
protected:

public:
    explicit Rook(PieceColor pieceColor = PieceColor::WHITE); //fully-parameterized constructor

    ~Rook();

    //re-define pure virtual methods in class Piece

    void draw(RenderWindow& window);

    vector<ChessMove> computePossbibleMoves(const vector<vector<Cell>>& cells);
};