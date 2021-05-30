#include <SFML/Graphics.hpp>
#include<string>
#include"TextureManager.h"
#pragma once

class Piece {
public:
	enum MoveType {
		Up = 0,
		Right = 1,
		Down = 2,
		Left = 3
	};

	Piece(int newRank, bool isWhite);
	~Piece();
	void place(int row, int col);
	void select();
	void deselect();

	sf::Sprite* sprite;
	int currentRow;
	int currentCol;
	int rank;
	bool team;

	bool isAlive = false;
};