#include <SFML/Graphics.hpp>

class Entity
{
	public:
		Entity();
		void setTexture(sf::Texture texture);
		sf::Sprite* getSprite();
		sf::Vector2i boardPos;
		int rank;
		int team;

	private:
		sf::Texture mTexture;
		sf::Sprite mSprite;
};
