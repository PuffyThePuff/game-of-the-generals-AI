#include "Piece.h"
#include "game.h"

Piece::Piece(int newRank, bool isWhite) {
	sprite = new sf::Sprite();
	rank = newRank;
	team = isWhite;
	
	sf::Texture* texture;
	if (!isWhite) texture = TextureManager::getInstance()->getTexture("hidden");
	else texture = TextureManager::getInstance()->getTexture(std::to_string(rank));
}

Piece::~Piece() {

}

void Piece::place(int row, int col) {
	boardPosition = sf::Vector2i(row, col);
	sprite->setPosition(Game::TILE_SIZE * (col + 1), Game::TILE_SIZE * row);
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