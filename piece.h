#pragma once

class piece {
private:
	sf::Sprite sprite;
	sf::Vector2f size;
	sf::Vector2f position;
	bool seen;
	char rank;
	char team;
	void setSprite(char rank, char team);

public:
	piece(sf::Vector2f newposition, char newrank, char newteam);
	~piece();
	void move();
	void eat();
	bool isSeen();
};