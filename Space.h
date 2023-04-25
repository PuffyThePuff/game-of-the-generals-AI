#include"Piece.h"
#pragma once

class Space
{
public:
	Space() {}
	bool isOccupied = false;
	Piece* piece = NULL;
	Piece* challengers[2] = { NULL, NULL };
};