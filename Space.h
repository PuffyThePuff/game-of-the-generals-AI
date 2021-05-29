#include"Piece.h"
#pragma once

class Space
{
public:
	Space() {}
	bool isOccupied = false;
	Piece* piece = NULL;
};