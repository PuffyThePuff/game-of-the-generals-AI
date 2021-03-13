#include <SFML/Graphics.hpp>
#include "piece.h"

void piece::setSprite(char rank, char team) {
	sf::Texture texture;

	//don't remembet how to load dynamic file names but below should look like assets/<team>/<rank>.jpg
	//texture.loadFromFile(test.jpg);
	
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