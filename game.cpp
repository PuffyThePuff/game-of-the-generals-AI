#include "game.h"
#include "TextureManager.h""
#include <SFML/Graphics.hpp>

Game::Game() :
    mWindow(sf::VideoMode(900, 600), "Salpakan") {
    TextureManager::getInstance()->loadAll();
    for (int i = 0; i <= 14; i++) {
        createEntity("hidden", 0, 0, i, 'b');
    }

    //add 5 privates
    for (int i = 0; i < 5; i++) {
        createEntity("hidden", 0, 0, 1, 'b');
    }

    //extra spy
    createEntity("hidden", 0, 0, 14, 'b');

    //load white pieces last
    for (int i = 0; i <= 14; i++) {
        std::string str_i = std::to_string(i);
        createEntity(str_i, 0, 8, i, 'w');
    }

    for (int i = 0; i < 5; i++) {
        createEntity("1", 0, 8, 1, 'w');
    }

    createEntity("14", 0, 8, 14, 'w');

    //add grid here
    sf::RectangleShape* temp;
    for (int i = 0; i < 11; i++) {
        temp = new sf::RectangleShape;
        temp->setFillColor(sf::Color::White);
        temp->setPosition(i * 90, 0);
        temp->setSize(sf::Vector2f(5, 60 * 8));
        grid.push_back(temp);
    }
    for (int i = 0; i < 9; i++) {
        temp = new sf::RectangleShape;
        temp->setFillColor(sf::Color::White);
        temp->setPosition(0, i * 60);
        temp->setSize(sf::Vector2f(90 * 9, 5));
        grid.push_back(temp);
    }

    /*
    sf::Texture* mTexture = TextureManager::getInstance()->getTexture("quit");
    quitBox.setTexture(*mTexture);
    */
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
        switch (event.type) {
        case sf::Event::KeyPressed:
            handlePlayerInput(event.key.code);
            break;
        case sf::Event::Closed:
            mWindow.close();
            break;
        default:
            break;
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if(startPhase){
            sf::Vector2i mousePos = sf::Mouse::getPosition();
            if(mousePos.y < 480){
                sf::Vector2i indices = sf::Vector2i(mousePos.x / 90, mousePos.y / 60);
                
            }
        }

        else{
            sf::Vector2i mousePos = sf::Mouse::getPosition();
            if(mousePos.y < 480 && isPlayerTurn){
                sf::Vector2i indices = sf::Vector2i(mousePos.x / 90, mousePos.y / 60);
                for(int i = 0; i < entityList.size(); i++){
                    if(entityList[i]->boardPos==indices && entityList[i]->team=='w'){
                        selectedMode = true;
                        selectedPiece = entityList[i];
                    }
                }
            }
        }
        
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key) {
    if(isPlayerTurn && selectedMode){
        if (key == sf::Keyboard::W && selectedPiece->boardPos.y > 0) {
            selectedPiece->getSprite()->move(0, -1 * 60);
            selectedPiece->boardPos.y -= 1;

            isPlayerTurn = false;
            selectedPiece = nullptr;
            selectedMode = false;
        }

        if (key == sf::Keyboard::S && selectedPiece->boardPos.y < 7){
            selectedPiece->getSprite()->move(0, 1 * 60);
            selectedPiece->boardPos.y += 1;

            isPlayerTurn = false;
            selectedPiece = nullptr;
            selectedMode = false;
        }

        if (key == sf::Keyboard::A && selectedPiece->boardPos.x > 0){
            selectedPiece->getSprite()->move(-1 * 90, 0);
            selectedPiece->boardPos.x -= 1;

            isPlayerTurn = false;
            selectedPiece = nullptr;
            selectedMode = false;
        }

        if (key == sf::Keyboard::D && selectedPiece->boardPos.x < 8){
            selectedPiece->getSprite()->move(1 * 90, 0);
            selectedPiece->boardPos.x += 1;

            isPlayerTurn = false;
            selectedPiece = nullptr;
            selectedMode = false;
        }
        collisionCheck(selectedPiece);
    }

    if (key == sf::Keyboard::Backspace) {
        if (!quitMenu) quitMenu = true;
        else mWindow.close();
    }

    else quitMenu = false;
}

void Game::collisionCheck(Entity* a) {
    for (int i = 0; i < entityList.size(); i++) {
        if (a->boardPos == entityList[i]->boardPos) {
            handleCollision(a, entityList[i]);
        }
    }
}

void Game::update() {
    //check flag conditions here for game win or lose
}

void Game::handleCollision(Entity* aggressor, Entity* defender) {
    if (aggressor->rank == defender->rank) {
        if (aggressor->rank == 0) {
            //aggressor->eat(defender);
            defender->sendToGraveyard(&whiteDead, &blackDead);
        }
        aggressor->sendToGraveyard(&whiteDead, &blackDead);
        defender->sendToGraveyard(&whiteDead, &blackDead);
    }

    else if (aggressor->rank == 1 && defender->rank == 14) {
        //aggressor->eat(defender);
        defender->sendToGraveyard(&whiteDead, &blackDead);
    }

    else if (defender->rank == 1 && aggressor->rank == 14) {
        //defender->eat(aggressor);
        aggressor->sendToGraveyard(&whiteDead, &blackDead);
    }

    else if (aggressor->rank > defender->rank) {
        //aggressor->eat(defender);
        defender->sendToGraveyard(&whiteDead, &blackDead);
    }

    else {
        aggressor->sendToGraveyard(&whiteDead, &blackDead);
    }
}

void Game::render() {
    mWindow.clear();
    for (int i = 0; i < entityList.size(); i++) {
        mWindow.draw(*entityList[i]->getSprite());
    }
    for (int i = 0; i < grid.size(); i++) {
        mWindow.draw(*grid[i]);
    }
    // if (quitMenu) mWindow.draw(quitBox);
    mWindow.display();
}