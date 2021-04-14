#include "Entity.h"

Entity::Entity() {};

void Entity::setTexture(sf::Texture texture) {
	mTexture = texture;
	mSprite.setTexture(mTexture);
}

void Entity::sendToGraveyard(int* blackOffset, int* whiteOffset) {
	getSprite()->setScale(0.5, 0.5);
	boardPos = sf::Vector2i(-1, -1);
	if (rank == 'b') {
		getSprite()->setPosition(*blackOffset * 90, 7 * 60);
		*blackOffset += 1;
	}
	else {
		getSprite()->setPosition(*whiteOffset * 90, 8 * 60);
		*whiteOffset += 1;
	}
}

/*
void Entity::eat(Entity* target) {
	this->boardPos = target->boardPos;
	this->getSprite()->setPosition(target->boardPos.x * 90, target->boardPos.y * 60);
}
*/

sf::Sprite* Entity::getSprite(){
	return &mSprite;
}