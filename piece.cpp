#include "Piece.h"

Piece::Piece(int newRank, bool isWhite) {
	sprite = new sf::Sprite();
	rank = newRank;
	team = isWhite;


}

Piece::~Piece() {

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