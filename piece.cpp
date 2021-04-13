#include <SFML/Graphics.hpp>
#include "piece.h"

void piece::setSprite(char rank, char team) {
	sf::Texture texture;
	
	sprite.setTexture(texture);
}

piece::piece(sf::Vector2f newposition, char newrank, char newteam) {
	piece::setSprite(newrank, newteam);
	position = newposition;
	rank = newrank;
	team = newteam;
	seen = false;
}

piece::~piece() {

}

void piece::move() {

}

void piece::eat() {

}

bool piece::isSeen() {
	return seen;
}