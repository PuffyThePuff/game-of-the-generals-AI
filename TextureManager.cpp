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
	
	//white pieces
	loadTexture("1star_white", "assets/pieces/white/1star.png");
	loadTexture("2star_white", "assets/pieces/white/2star.png");
	loadTexture("3star_white", "assets/pieces/white/3star.png");
	loadTexture("4star_white", "assets/pieces/white/4star.png");
	loadTexture("5star_white", "assets/pieces/white/5star.png");
	loadTexture("flag_white", "assets/pieces/white/flag.png");
	loadTexture("private_white", "assets/pieces/white/private.png");
	loadTexture("spy_white", "assets/pieces/white/spy.png");
	loadTexture("sergeant_white", "assets/pieces/white/sergeant.png");
	loadTexture("captain_white", "assets/pieces/white/captain.png");
	loadTexture("1stlt_white", "assets/pieces/white/1stlt.png");
	loadTexture("2ndlt_white", "assets/pieces/white/2ndlt.png");
	loadTexture("colonel_white", "assets/pieces/white/colonel.png");
	loadTexture("ltcolonel_white", "assets/pieces/white/ltcolonel.png");
	loadTexture("major_white", "assets/pieces/white/major.png");

	//hidden piece
	loadTexture("hidden", "assets/pieces/hidden.png");

	//screens
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