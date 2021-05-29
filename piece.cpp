#include "Piece.h"

Piece::Piece(int newRank, bool isWhite) {
	sprite = new sf::Sprite();
	rank = newRank;
	team = isWhite;

	sf::Texture* texture;
	if (!isWhite) texture = TextureManager::getInstance()->getTexture("hidden");
	else texture = TextureManager::getInstance()->getTexture(to_string(rank));
}

Piece::~Piece() {

}

void Piece::place(int row, int col) {
	boardPosition = sf::Vector2i(row, col);
	sprite->setPosition(game::TILE_SIZE * (col + 1), game::TILE_SIZE * row);
}

void Piece::move() {

}

void Piece::select() {
	isSelected = true;
	sprite->setColor(sf::Color::Red);
}

void Piece::deselect() {
	isSelected = false;
	sprite->setColor(sf::Color::Transparent);
}