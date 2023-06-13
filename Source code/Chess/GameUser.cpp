#include "GameUser.h"

GameUser::GameUser(PieceColor color) {
    this->_score = 0;

    this->_userColor = color;
}

//destructor, no need to do anything
GameUser::~GameUser() {

}

int GameUser::getScore() {
    return this->_score;
}

PieceColor GameUser::getColor() {
    return this->_userColor;
}

void GameUser::setScore(int scr) {
    this->_score = scr;
}

shared_ptr<Piece> GameUser::getRandomPiece()
{
    bool anyPiecesExist = false;
    for (const auto& pair : _pieces) {
        if (!pair.second.empty()) {
            anyPiecesExist = true;
            break;
        }
    }
    if (!anyPiecesExist) {
        return nullptr;
    }

    int randomIndex = rand() % _pieces.size();
    auto it = _pieces.begin();
    advance(it, randomIndex);
    PieceType randomPieceType = it->first;
    vector<weak_ptr<Piece>>& pieces = _pieces[randomPieceType];
    if (it->second.size() == 0)
        return getRandomPiece();
    int randomPieceIndex = rand() % pieces.size();

    weak_ptr<Piece>& randomWeakPiece = pieces[randomPieceIndex];

    if (randomWeakPiece.expired()) {
        return getRandomPiece();
    }

    shared_ptr<Piece> randomPiece = randomWeakPiece.lock();
    return randomPiece;

}

void GameUser::addPiece(PieceType type, const shared_ptr<Piece>& piece) {
    //check if the color of the piece added is the same
    //with the color of player
    if (piece->getPieceColor() != this->_userColor) {
        throw std::runtime_error("Colors of the piece added and the user don't match");
    }

    //we can manage if this piece has been added yet, so we do not need to check
    weak_ptr<Piece> ptr = piece;
    this->_pieces[type].push_back(ptr);
}

void GameUser::removePiece(PieceType type, const shared_ptr<Piece>& piece)
{
    if (piece->getPieceColor() != this->_userColor) {
        throw std::runtime_error("Colors of the piece added and the user don't match");
    }
    auto it = this->_pieces.find(type);
    if (it != this->_pieces.end()) {
        std::vector<std::weak_ptr<Piece>>& pieceVec = it->second;
        for (auto vecIt = pieceVec.begin(); vecIt != pieceVec.end(); ++vecIt) {
            std::shared_ptr<Piece> sharedPiece = vecIt->lock();
            if (sharedPiece && sharedPiece == piece) {
                pieceVec.erase(vecIt);
                //std::cout << "Piece removed successfully." << std::endl;
                return;
            }
        }
    }
}

void GameUser::clearPieces() {
    this->_pieces.clear();
}

bool GameUser::isChecked(const vector<vector<Cell>>& cells) {
    //find the position of the King in cells
    Vector2i pos(-1, -1);
    for (int i = 0; i < cells.size(); i++) {
        for (int j = 0; j < cells[i].size(); j++) {
            if (cells[i][j]._status == CellStatus::OCCUPIED && cells[i][j]._piece == nullptr);
            if (cells[i][j]._status == CellStatus::EMPTY || cells[i][j]._piece == nullptr)
            {// || cells[i][j]._piece == nullptr) {
                continue;
            }
            else if (cells[i][j]._piece->getPieceType() == PieceType::KING && cells[i][j]._piece->getPieceColor() == this->_userColor) {
                pos = Vector2i(i, j);

                break;
            }
        }
    }

    //check if the King is found
    if (pos.x == -1 || pos.y == -1) {
        throw std::runtime_error("No King found for this user");
    }

    //check if the King is checked by the Bishops, Rooks or Queen
    //there is 8 directions need checking
    vector<vector<Vector2i>> offsets = {
        {Vector2i(1, 1), Vector2i(1, -1), Vector2i(-1, 1), Vector2i(-1, -1),},
        {Vector2i(1, 0), Vector2i(0, 1), Vector2i(0, -1), Vector2i(-1, 0)}
    };

    vector<vector<PieceType>> types = {
        {PieceType::BISHOP, PieceType::QUEEN},
        {PieceType::ROOK, PieceType::QUEEN}
    };

    for (int i = 0; i < offsets.size(); i++) {
        for (int j = 0; j < offsets[i].size(); j++) {
            Vector2i destination = pos + offsets[i][j];

            //check as far as possible to each direction of the eight ones
            while (true) {
                if (utilities::assisstants::isOnBoardPosition(destination) == false) {
                    break;
                }

                Cell cell = cells[destination.x][destination.y];
                if (cell._status == CellStatus::OCCUPIED) {
                    if (cell._piece == nullptr)
                    {
                        cout << destination.x << " " << destination.y << " BUG\n";
                    }
                    else
                        if (cell._piece->getPieceColor() != this->_userColor && (cell._piece->getPieceType() == types[i][0] || cell._piece->getPieceType() == types[i][1])) {
                            return true;
                        }
                        else {
                            break;
                        }
                }
                else {
                    destination += offsets[i][j];
                }
            }
        }
    }

    //check if the King is checked by Knights
    //there is 8 directions need checking
    vector<Vector2i> knightOffsets = {
        Vector2i(2, 1), Vector2i(2, -1),
        Vector2i(1, 2), Vector2i(1, -2),
        Vector2i(-1, 2), Vector2i(-1, -2),
        Vector2i(-2, 1), Vector2i(-2, -1)
    };

    for (int i = 0; i < knightOffsets.size(); i++) {
        //do not need to check as far as possible,
        //because the Knight can only move one L-shape at a time
        Vector2i destination = pos + knightOffsets[i];

        if (utilities::assisstants::isOnBoardPosition(destination) == false) {
            continue;
        }

        Cell cell = cells[destination.x][destination.y];

        if (cell._status == CellStatus::OCCUPIED) {
            if (cell._piece->getPieceColor() != this->_userColor && cell._piece->getPieceType() == PieceType::KNIGHT) {
                return true;
            }
        }
    }

    //check if the King is checked by Pawns
    //there is 2 directions need checking
    float xoffset;
    if (this->_userColor == PieceColor::WHITE) {
        xoffset = -1;
    }
    else {
        xoffset = 1;
    }

    vector<Vector2i> pawnOffsets = {
        Vector2i(xoffset, 1), Vector2i(xoffset, -1)
    };

    for (int i = 0; i < pawnOffsets.size(); i++) {
        Vector2i destination = pos + pawnOffsets[i];

        if (utilities::assisstants::isOnBoardPosition(destination) == false) {
            continue;
        }

        Cell cell = cells[destination.x][destination.y];

        if (cell._status == CellStatus::OCCUPIED) {
            if (cell._piece->getPieceColor() != this->_userColor && cell._piece->getPieceType() == PieceType::PAWN) {
                return true;
            }
        }
    }

    //check if the King is checked by the King
    //there is 8 directions need checking
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            Vector2i destination = pos + Vector2i(i, j);

            if ((i == 0 && j == 0) || utilities::assisstants::isOnBoardPosition(destination) == false) {
                continue;
            }

            Cell cell = cells[destination.x][destination.y];

            if (cell._status == CellStatus::OCCUPIED) {
                if (cell._piece->getPieceColor() != this->_userColor && cell._piece->getPieceType() == PieceType::KING) {
                    return true;
                }
            }
        }
    }

    return false;
}

//check if this user has any legal moves left
//only be used when this user is checked
bool GameUser::hasLegalMoves(const vector<vector<Cell>>& cells) {
    vector<ChessMove> possibleMoves;

    Vector2i pos;

    vector<vector<Cell>> copyCells;

    shared_ptr<Piece> king = nullptr;
    for (const auto& element : this->_pieces) {

        for (const auto& piecePtr : element.second) {

            auto piece = piecePtr.lock();

            //we don't consider King's moves in this loop
            if (piece->getPieceType() == PieceType::KING) {
                king = piece;
                continue;
            }

            //find the position and possible moves of this piece
            pos = piece->getPosition();
            possibleMoves = piece->computePossbibleMoves(cells);
            if (!cells[pos.x][pos.y]._piece) continue;
            if (cells[pos.x][pos.y]._piece->getPieceColor() != this->getColor()) continue;
            if (cells[pos.x][pos.y]._piece != piece) continue;
            //check if there is any move so that the King will not be checked
            for (const auto& move : possibleMoves) {
                copyCells = cells;

                copyCells[move._position.x][move._position.y]._piece = std::move(copyCells[pos.x][pos.y]._piece);
                copyCells[pos.x][pos.y]._status = CellStatus::EMPTY;
                copyCells[move._position.x][move._position.y]._status = CellStatus::OCCUPIED;


                for (int q = 0; q < 8; q++)
                {
                    if (copyCells[0][q]._piece == nullptr && copyCells[0][q]._status == CellStatus::OCCUPIED)
                    {
                        //cout << "0 " << q << " BUGGG\n";
                    }
                    if (cells[0][q]._piece == nullptr && cells[0][q]._status == CellStatus::OCCUPIED)
                    {
                        //cout << "0 " << q << " BUGGG AT SOURCE\n";
                    }
                }
                if (this->isChecked(copyCells) == false) {
                    return true;
                }
                //if (saved) {
                //    nextUser->addPiece(saved->getPieceType(), saved);
                //}
            }
        }
    }

    //there is no King (!)
    if (king == nullptr) {
        throw std::runtime_error("No King found for this user");
    }

    //if there is no possible move of the other pieces so that the King will not be checked,
    //check if there is any possible moves of the King
    pos = king->getPosition();
    possibleMoves = king->computePossbibleMoves(cells);

    for (const auto& move : possibleMoves) {
        copyCells = cells;

        copyCells[move._position.x][move._position.y]._piece = std::move(copyCells[pos.x][pos.y]._piece);

        copyCells[pos.x][pos.y]._status = CellStatus::EMPTY;
        copyCells[move._position.x][move._position.y]._status = CellStatus::OCCUPIED;
        if (this->isChecked(copyCells) == false) {
            return true;
        }
    }

    return false;
}

void GameUser::checkKingCell(bool check, vector<vector<Cell>>& cells) {
    if (auto king = this->_pieces[PieceType::KING].at(0).lock()) {
        auto pos = king->getPosition();
        cells[pos.x][pos.y].setIsChecked(check);
    }
    else {
        throw std::runtime_error("No King found for this user");
    }
}