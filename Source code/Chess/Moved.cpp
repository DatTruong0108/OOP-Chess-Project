#include"Moved.h"

Moved::Moved()
{
}

Moved::~Moved()
{
}

bool Moved::getHasMoved()
{
	return this->_hasMoved;
}

Vector2i Moved::getOri()
{
	return this->_ori;
}

Vector2i Moved::getDes()
{
	return this->_des;
}

PieceColor Moved::getColor()
{
	return this->_color;
}

bool Moved::getIsCapture()
{
	return this->_isCapture;
}

PieceType Moved::getCapturedPiece()
{
	return this->_capturedPiece;
}

MoveType Moved::getMoveType()
{
	return this->_moveType;
}

PieceType Moved::getSpecialPiece()
{
	return this->_specialPiece;
}

void Moved::setHasMoved(bool check)
{
	this->_hasMoved = check;
}

void Moved::setOri(Vector2i src)
{
	this->_ori = src;
}

void Moved::setDes(Vector2i src)
{
	this->_des = src;
}

void Moved::setPieceColor(PieceColor src)
{
	this->_color = src;
}

void Moved::setIsCapture(bool check)
{
	this->_isCapture = check;
}

void Moved::setCapturedPiece(PieceType src)
{
	this->_capturedPiece = src;
}

void Moved::setMoveType(MoveType src)
{
	this->_moveType = src;
}

void Moved::setSpecialPiece(PieceType src)
{
	this->_specialPiece = src;
}

void Moved::output()
{
	cout << "MOVED:\n";
	cout << "ORI: " << _ori.x << " " << _ori.y << endl;
	cout << "DES: " << _des.x << " " << _des.y << endl;
	cout << "color: ";
	if (this->_color == PieceColor::WHITE)
	{
		cout << "White\n";
	}
	else cout << "Black\n";
	cout << "Cap: " << _isCapture << endl;
    if (_isCapture == true)
    {
        if (_capturedPiece == PieceType::BISHOP)
        {
            cout << "Bishop\n";
        }
        if (_capturedPiece == PieceType::QUEEN)
        {
            cout << "QUEEN\n";
        }
        if (_capturedPiece == PieceType::ROOK)
        {
            cout << "ROOK\n";
        }
        if (_capturedPiece == PieceType::KNIGHT)
        {
            cout << "KNIGHT\n";
        }
        if (_capturedPiece == PieceType::PAWN)
        {
            cout << "PAWN\n";
        }
    }
    
    if (_moveType == MoveType::PROMOTION)
    {
        cout << "Pro\n";
    }
    if (_moveType == MoveType::EN_PASSANT)
    {
        cout << "EN_PASSANT\n";
    }
    if (_moveType == MoveType::LONG_CASTLING)
    {
        cout << "LONG_CASTLING\n";
    }
    if (_moveType == MoveType::SHORT_CASTLING)
    {
        cout << "SHORT_CASTLING\n";
    }
}

void Moved::setPiece(shared_ptr<Piece> src) {
    this->_piece = src;
}

shared_ptr<Piece> Moved::getPiece()
{
    return this->_piece;
}

void Moved::setPawn(shared_ptr<Piece> src)
{
    this->_pawn = src;
}

shared_ptr<Piece> Moved::getPawn()
{
    return this->_pawn;
}