#include <SFML/Graphics.hpp>
#include<string>
#include"TextureManager.h"
#include"game.h"
#pragma once
using namespace std;

class Piece {
public:
	Piece(int newRank, bool isWhite);
	~Piece();
	void place(int row, int col);
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