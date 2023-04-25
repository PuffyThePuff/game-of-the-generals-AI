#include <SFML/Graphics.hpp>
#pragma once

class Entity
{
	public:
		Entity();
		void setTexture(sf::Texture texture);
		void sendToGraveyard(int*, int*);
		//void eat(Entity*);
		sf::Sprite* getSprite();
		void handleInput();

		sf::Vector2i boardPos;
		int rank;
		int team;

	private:
		sf::Texture mTexture;
		sf::Sprite mSprite;
};
