#include "game.h"
#include "TextureManager.h""
#include <SFML/Graphics.hpp>

Game::Game() :
    mWindow(sf::VideoMode(640, 480), "SFML Application") {
    TextureManager::getInstance()->loadAll();
    for (int i = 0; i <= 14; i++) {
        std::string str_i = std::to_string(i);
        createEntity(str_i, 0, 0);
    }

    createEntity("hidden", 100, 100);

    TextureManager::getInstance()->testFunction();
};

void Game::createEntity(std::string key, float x, float y) {
    Entity* entity = new Entity();
    sf::Texture* mTexture;
    mTexture = TextureManager::getInstance()->getTexture(key);
    entity->setTexture(*mTexture);
    entity->getSprite()->setPosition(x, y);
    entityList.push_back(entity);
}

void Game::run() {
    while (mWindow.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) mWindow.close();
    }
}

void Game::update() {

}

void Game::render() {
    mWindow.clear();
    for (int i = 0; i < entityList.size(); i++) {
        mWindow.draw(*entityList[i]->getSprite());
    }
    mWindow.display();
}