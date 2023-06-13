#pragma once

#include "Piece.h"

class King : public Piece {
private:
    static const int _pieceValue = 10; //value of the King is a special number, if we lose the King, we lose the game
protected:

public:
    explicit King(PieceColor pieceColor = PieceColor::WHITE); //fully-parameterized constructor

    ~King();

    //re-define pure virtual methods in class Piece

    void draw(RenderWindow& window);

    vector<ChessMove> computePossbibleMoves(const vector<vector<Cell>>& cells);

};