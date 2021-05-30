#include "Piece.h"
#include "game.h"

Piece::Piece(int newRank, bool isWhite, std::string key) {
	sprite = new sf::Sprite();
	rank = newRank;
	team = isWhite;
	
	sf::Texture* texture;
	if (!isWhite) texture = TextureManager::getInstance()->getTexture("hidden");
	else texture = TextureManager::getInstance()->getTexture(key);
	sprite->setTexture(*texture);
}

Piece::~Piece() {

}

void Piece::place(int row, int col) {
	currentRow = row;
	currentCol = col;
	sprite->setPosition(Game::TILE_SIZE * (col + 1), Game::TILE_SIZE * row);
}

void Piece::select() {
	sprite->setColor(sf::Color::Green);
}

void Piece::deselect() {
	sprite->setColor(sf::Color::Transparent);
}