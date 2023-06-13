#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <math.h>
#include "Cell.h"

class Moved {
	bool _hasMoved;

	Vector2i _ori;

	Vector2i _des;

	PieceColor _color;

	bool _isCapture;

	PieceType _capturedPiece;

	shared_ptr<Piece> _piece; //captured piece

	MoveType _moveType;

	PieceType _specialPiece; //use for promotion and en_passant

	shared_ptr<Piece> _pawn;
public:
	Moved();

	~Moved();

	bool getHasMoved();

	Vector2i getOri();

	Vector2i getDes();

	PieceColor getColor();

	bool getIsCapture();

	PieceType getCapturedPiece();

	MoveType getMoveType();

	PieceType getSpecialPiece();

	void setHasMoved(bool check);

	void setOri(Vector2i src);

	void setDes(Vector2i src);

	void setPieceColor(PieceColor src);

	void setIsCapture(bool check);

	void setCapturedPiece(PieceType src);

	void setMoveType(MoveType src);

	void setSpecialPiece(PieceType src);

	void output();

	void setPiece(shared_ptr<Piece> src);

	shared_ptr<Piece> getPiece();

	void setPawn(shared_ptr<Piece> src);

	shared_ptr<Piece> getPawn();
};