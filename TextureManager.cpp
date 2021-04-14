#include <stddef.h>
#include <iostream>
#include "TextureManager.h"

TextureManager* TextureManager::sharedInstance = NULL;

TextureManager* TextureManager::getInstance() {
		//initialize only when we need it
		if (sharedInstance == NULL) {
			sharedInstance = new TextureManager();
		}
		return sharedInstance;
}

void TextureManager::loadAll() {
	for (int i = 0; i <= 14; i++) {
		std::string str_i = std::to_string(i);
		loadTexture(str_i, "assets/pieces/white/" + str_i + ".png");
	}
	loadTexture("hidden", "assets/pieces/hidden.png");
	loadTexture("quit", "assets/quitmenu.png");
	loadTexture("win", "assets/winScreen.png");
	loadTexture("loss", "assets/loseScreen.png");
}

void TextureManager::loadTexture(std::string key, std::string path) {
	sf::Texture* texture = new sf::Texture();
	texture->loadFromFile(path);
	textureMap[key] = texture;
}

sf::Texture* TextureManager::getTexture(std::string key) {
	if (textureMap[key] != NULL) {
		return textureMap[key];
	}
	else {
		std::cout << "No texture found for " << key;
		return NULL;
	}
}