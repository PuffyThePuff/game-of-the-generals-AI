#include <iostream>
#include <cstdlib>
#include <vector>
#include <SFML/Graphics.hpp>
#include "game.h"

int main() {
	srand(time(0));
	Game game;
	game.run();
}