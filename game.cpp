#include "game.h"
#include "TextureManager.h"
#include <SFML/Graphics.hpp>

Game::Game() :
    mWindow(sf::VideoMode(900, 600), "Salpakan") {
    mWindow.setFramerateLimit(5);
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

    setBlack();

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

    sf::Texture* mTexture = TextureManager::getInstance()->getTexture("quit");
    quitBox.setTexture(*mTexture);
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
            sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
            if(mousePos.y < 480 && mousePos.x < 810){
                sf::Vector2i indices = sf::Vector2i(mousePos.x / 90, mousePos.y / 60);
                entityList[placeIndex]->boardPos = indices;
                entityList[placeIndex]->getSprite()->setPosition(
                    entityList[placeIndex]->boardPos.x * 90,
                    entityList[placeIndex]->boardPos.y * 60
                );
                placeIndex--;
            }

            if (placeIndex < 21) startPhase = false;
        }

        else{
            sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
            if(mousePos.y < 480 && mousePos.x < 810 && isPlayerTurn){
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
            collisionCheck(selectedPiece, sf::Vector2i(selectedPiece->boardPos.x, selectedPiece->boardPos.y - 1));
            selectedPiece->getSprite()->move(0, -1 * 60);
            selectedPiece->boardPos.y -= 1;

            isPlayerTurn = false;
            selectedMode = false;
            blackMove();
        }

        if (key == sf::Keyboard::S && selectedPiece->boardPos.y < 7){
            collisionCheck(selectedPiece, sf::Vector2i(selectedPiece->boardPos.x, selectedPiece->boardPos.y + 1));
            selectedPiece->getSprite()->move(0, 1 * 60);
            selectedPiece->boardPos.y += 1;

            isPlayerTurn = false;
            selectedMode = false;
            blackMove();
        }

        if (key == sf::Keyboard::A && selectedPiece->boardPos.x > 0){
            collisionCheck(selectedPiece, sf::Vector2i(selectedPiece->boardPos.x - 1, selectedPiece->boardPos.y));
            selectedPiece->getSprite()->move(-1 * 90, 0);
            selectedPiece->boardPos.x -= 1;

            isPlayerTurn = false;
            selectedMode = false;
            blackMove();
        }

        if (key == sf::Keyboard::D && selectedPiece->boardPos.x < 8){
            collisionCheck(selectedPiece, sf::Vector2i(selectedPiece->boardPos.x + 1, selectedPiece->boardPos.y));
            selectedPiece->getSprite()->move(1 * 90, 0);
            selectedPiece->boardPos.x += 1;

            isPlayerTurn = false;
            selectedMode = false;
            blackMove();
        }
    }

    if (key == sf::Keyboard::Backspace) {
        if (!quitMenu) quitMenu = true;
        else mWindow.close();
    }

    else quitMenu = false;
}

void Game::collisionCheck(Entity* a, sf::Vector2i targetPos) {
    for (int i = 0; i < entityList.size(); i++) {
        if (targetPos == entityList[i]->boardPos) {
            handleCollision(a, entityList[i]);
        }
    }
}

void Game::setBlack() {
    int k = 20;
    for (int i = 0; i < 9; i++) {
        for(int j = 0; j < 2; j++){
            entityList[k]->boardPos = sf::Vector2i(i, j);
            entityList[k]->getSprite()->setPosition(i * 90, j * 60);
            k--;
        }
    }
    
    for(int i = 0; i < 3; i++){
        entityList[k]->boardPos = sf::Vector2i(i, 2);
        entityList[k]->getSprite()->setPosition(i * 90, 2 * 60);
        k--;
    }
}

//temp function for testing
void Game::blackMove(){
    if(entityList[0]->boardPos==sf::Vector2i(2, 2)){
        collisionCheck(entityList[0], sf::Vector2i(entityList[0]->boardPos.x + 1, entityList[0]->boardPos.y));
        entityList[0]->getSprite()->move(1 * 90, 0);
        entityList[0]->boardPos.x += 1;

        isPlayerTurn = true;
        selectedMode = false;
    }

    else {
        collisionCheck(entityList[0], sf::Vector2i(entityList[0]->boardPos.x - 1, entityList[0]->boardPos.y));
        entityList[0]->getSprite()->move(-1 * 90, 0);
        entityList[0]->boardPos.x -= 1;

        isPlayerTurn = true;
        selectedMode = false;
    }
}

void Game::update() {
    //check flag conditions here for game win or lose
}

void Game::handleCollision(Entity* aggressor, Entity* defender) {
    //same rank, both die
    if (aggressor->rank == defender->rank) {
        //special condition for flag aggressor eating other flag
        if (aggressor->rank == 0) {
            //aggressor->eat(defender);
            defender->sendToGraveyard(&whiteDead, &blackDead);
        }
        aggressor->sendToGraveyard(&whiteDead, &blackDead);
        defender->sendToGraveyard(&whiteDead, &blackDead);
    }

    //private aggressor, spy defender
    else if (aggressor->rank == 1 && defender->rank == 14) {
        //aggressor->eat(defender);
        defender->sendToGraveyard(&whiteDead, &blackDead);
    }

    //private defender, spy aggressor
    else if (defender->rank == 1 && aggressor->rank == 14) {
        //defender->eat(aggressor);
        aggressor->sendToGraveyard(&whiteDead, &blackDead);
    }

    //standard rank
    else if (aggressor->rank > defender->rank) {
        //aggressor->eat(defender);
        defender->sendToGraveyard(&whiteDead, &blackDead);
    }

    //lost the challenge
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
    if (quitMenu) mWindow.draw(quitBox);
    mWindow.display();
}