#include"Piece.h"
#pragma once

class Move
{
public:
	Move() {}
	unsigned int pieceIndex;
	Piece::MoveType moveType;
};