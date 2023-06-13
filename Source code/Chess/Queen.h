#pragma once
#include "Piece.h"

class Queen : public Piece {
private:
    static const int _pieceValue = 9; //the Quen is the strongest piece, so its value is 9
protected:

public:
    explicit Queen(PieceColor pieceColor = PieceColor::WHITE); //fully-parameterized constructor

    ~Queen();

    //re-define pure virtual methods in class Piece

    void draw(RenderWindow& window);

    vector<ChessMove> computePossbibleMoves(const vector<vector<Cell>>& cells);
};