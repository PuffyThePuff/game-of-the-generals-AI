#include <SFML/Graphics.hpp>
#include"TextureManager.h"
#pragma once

class Piece {
public:
	Piece(int newRank, bool isWhite);
	~Piece();
	void move();
	void select();
	void deselect();

	sf::Sprite* sprite;
	sf::Vector2i boardPosition;
	int rank;
	bool team;

	bool isAlive = true;
	bool isSelected = false;
};