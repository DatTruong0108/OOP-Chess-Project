#include "King.h"
#include <iostream>
using namespace std;

const int King::_pieceValue;

King::King(PieceColor pieceColor) {
    this->_pieceColor = pieceColor;

    this->_pieceType = PieceType::KING;

    this->_hasMoved = false;

    this->loadTexture();
}

King::~King() { }

void King::draw(RenderWindow& window) {
    window.draw(this->_pieceSprite);
}

vector<ChessMove> King::computePossbibleMoves(const vector<vector<Cell>>& cells) {
    Vector2i pos = this->getPosition();

    vector<ChessMove> possibleMoves;

    for (int i = pos.x - 1; i <= pos.x + 1; i++) {
        for (int j = pos.y - 1; j <= pos.y + 1; j++) {
            //filter the same position as before
            //or the position is out of board
            if (i == pos.x && j == pos.y || utilities::assisstants::isOnBoardPosition(i, j) == false) {
                continue;
            }

            Cell cell = cells[i][j];

            //check if the destination is empty or occupied
            //if that cell is occupied, check if that is ally or enemy
            if (cell._status == CellStatus::OCCUPIED && cell._piece->getPieceColor() == this->_pieceColor) {
                continue; //ally piece (same color), can not move to or capture
            }
            else {
                possibleMoves.push_back(ChessMove(Vector2i(i, j)));
            }
        }
    }

    //check for castles (both short one and long one)
    //both the King and the Rook have not moved yet
    //cells between them are all empty
    if (this->_hasMoved == false) {
        //if the condition is true, aka the King has not moved yet
        //then we do not need to check if pos.y + 3 or pos.y - 4 is out of board

        //check for short castle
        //with the Rook on the right side of players' looking direction
        if (cells[pos.x][pos.y + 3]._piece != nullptr) {
            if (cells[pos.x][pos.y + 1]._status == CellStatus::EMPTY
                && cells[pos.x][pos.y + 2]._status == CellStatus::EMPTY
                && cells[pos.x][pos.y + 3]._status == CellStatus::OCCUPIED
                && cells[pos.x][pos.y + 3]._piece->getPieceType() == PieceType::ROOK
                && cells[pos.x][pos.y + 3]._piece->getHasMoved() == false
                ) possibleMoves.push_back(ChessMove(Vector2i(pos.x, pos.y + 2), MoveType::SHORT_CASTLING));;
        }
        //check for long castle
        //with the Rook on the left side of players' looking direction
        if (cells[pos.x][pos.y - 4]._piece != nullptr) {
            if (cells[pos.x][pos.y - 1]._status == CellStatus::EMPTY
                && cells[pos.x][pos.y - 2]._status == CellStatus::EMPTY
                && cells[pos.x][pos.y - 3]._status == CellStatus::EMPTY
                && cells[pos.x][pos.y - 4]._status == CellStatus::OCCUPIED
                && cells[pos.x][pos.y - 4]._piece->getPieceType() == PieceType::ROOK
                && cells[pos.x][pos.y - 4]._piece->getHasMoved() == false
                ) possibleMoves.push_back(ChessMove(Vector2i(pos.x, pos.y - 2), MoveType::LONG_CASTLING));
        }
    }

    return possibleMoves;
}