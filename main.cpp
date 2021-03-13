#include <iostream>
#include <cstdlib>
#include <vector>
#include <SFML/Graphics.hpp>
#include "piece.h"

int main() {

	unsigned int row = 8;
	unsigned int column = 9;

	std::vector<std::vector<piece>> boardActual(row, std::vector<piece>(column));		//8 rows(y), 9 columns(x) of objects with class piece

	std::vector<char> eaten;															//player's pieces that have been eaten

	sf::RenderWindow window(sf::VideoMode(1600, 900), "GoG");
	window.setFramerateLimit(60);

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}


	}

	return 0;
}