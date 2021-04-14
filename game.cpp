#include "game.h"
#include "TextureManager.h""
#include <SFML/Graphics.hpp>

Game::Game() :
    mWindow(sf::VideoMode(810, 600), "Salpakan") {
    TextureManager::getInstance()->loadAll();
    for (int i = 0; i <= 14; i++) {
        std::string str_i = std::to_string(i);
        createEntity(str_i, 0, 9, i, 'w');
        createEntity("hidden", 0, 0, i, 'b');
    }


    TextureManager::getInstance()->testFunction();
};

void Game::createEntity(std::string key, float x, float y, int rank, char team) {
    Entity* entity = new Entity();
    sf::Texture* mTexture;
    mTexture = TextureManager::getInstance()->getTexture(key);
    entity->setTexture(*mTexture);
    entity->getSprite()->setPosition(x * 90, y * 60);
    entity->boardPos = sf::Vector2i(x, y);
    entity->rank = rank;
    entity->team = team;
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
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition();
        if(mousePos.y < 480)
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