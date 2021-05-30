#include "game.h"
#include <SFML/Graphics.hpp>

Game::Game() :
    mWindow(sf::VideoMode(660, 480), "Salpakan") {
    mWindow.setFramerateLimit(60);
    TextureManager::getInstance()->loadAll();

    // BLACK PIECES

    // Flag
    Piece* piece0 = new Piece(0, false);
    blackPieces.push_back(piece0);

    // Six privates
    for (int i = 0; i < 6; i++) {
        Piece* piece = new Piece(1, false);
        blackPieces.push_back(piece);
    }

    // All other pieces
    for (int i = 2; i <= 14; i++) {
        Piece* piece = new Piece(i, false);
        blackPieces.push_back(piece);
    }

    // Extra spy
    Piece* piece1 = new Piece(14, false);
    blackPieces.push_back(piece1);

    // WHITE PIECES

    Piece* piece2 = new Piece(0, true);
    whitePieces.push_back(piece2);
    // Prepare for piece placement.
    whiteGraveyard.push_back(piece2);
    piece2->sprite->setPosition(10 * TILE_SIZE, 210);
    selectedIndex = 0;
    piece2->select();

    for (int i = 0; i < 6; i++) {
        Piece* piece = new Piece(1, true);
        whitePieces.push_back(piece);
        whiteGraveyard.push_back(piece);
        piece->sprite->setPosition(660, 480);
    }

    for (int i = 2; i <= 14; i++) {
        Piece* piece = new Piece(i, true);
        whitePieces.push_back(piece);
        whiteGraveyard.push_back(piece);
        piece->sprite->setPosition(660, 480);
    }

    Piece* piece3 = new Piece(14, true);
    whitePieces.push_back(piece3);
    whiteGraveyard.push_back(piece3);
    piece3->sprite->setPosition(10 * TILE_SIZE, 110);
    
    whiteGraveyard[1]->sprite->setPosition(10 * TILE_SIZE, 310);

    setBlack();

    //add grid here
    sf::RectangleShape* temp;
    for (int i = 1; i < 11; i++) {
        temp = new sf::RectangleShape;
        temp->setFillColor(sf::Color::White);
        temp->setPosition(i * TILE_SIZE, 0);
        temp->setSize(sf::Vector2f(5, 480));
        grid.push_back(temp);
    }
    for (int i = 0; i < 9; i++) {
        temp = new sf::RectangleShape;
        temp->setFillColor(sf::Color::White);
        temp->setPosition(TILE_SIZE, i * TILE_SIZE);
        temp->setSize(sf::Vector2f(540, 5));
        grid.push_back(temp);
    }

    sf::Texture* mTexture = TextureManager::getInstance()->getTexture("quit");
    quitBox.setTexture(*mTexture);
};

void Game::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen()) {
        processEvents(TimePerFrame);
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > TimePerFrame) {
            timeSinceLastUpdate -= TimePerFrame;
            processEvents(TimePerFrame);
            update(TimePerFrame);
        }
        render();
    }
}

void Game::processEvents(sf::Time deltaTime) {
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
        ticks += deltaTime.asSeconds();
        if(startPhase && ticks > 0.03f){
            ticks = 0.0;
            sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
            if(mousePos.y >= 0 && mousePos.y < 480 && mousePos.x >= 0 && mousePos.x < 660){
                sf::Vector2i indices = sf::Vector2i(mousePos.x / TILE_SIZE, mousePos.y / TILE_SIZE);
                entityList[placeIndex]->boardPos = indices;
                entityList[placeIndex]->getSprite()->setPosition(
                    entityList[placeIndex]->boardPos.x * 90,
                    entityList[placeIndex]->boardPos.y * 60
                );
                placeIndex--;
            }

            if (whiteGraveyard.empty()) startPhase = false;
        }

        else{
            sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
            if(mousePos.y < 480 && mousePos.x < 660 && isPlayerTurn){
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
    if (startPhase) {
        if (key == sf::Keyboard::W) {
            whiteGraveyard[selectedIndex]->deselect();
            if (selectedIndex == 0) whiteGraveyard[whiteGraveyard.size() - 1]->sprite->setPosition(660, 480);
            else whiteGraveyard[selectedIndex - 1]->sprite->setPosition(660, 480);
            whiteGraveyard[selectedIndex]->sprite->setPosition(10 * TILE_SIZE, 110);

            if (selectedIndex == whiteGraveyard.size() - 1) selectedIndex = 0;
            else selectedIndex++;

            whiteGraveyard[selectedIndex]->select();
            whiteGraveyard[selectedIndex]->sprite->setPosition(10 * TILE_SIZE, 210);
            if (selectedIndex == whiteGraveyard.size() - 1) whiteGraveyard[0]->sprite->setPosition(10 * TILE_SIZE, 310);
            else whiteGraveyard[selectedIndex + 1]->sprite->setPosition(10 * TILE_SIZE, 310);
        }
        if (key == sf::Keyboard::S) {
            whiteGraveyard[selectedIndex]->deselect();
            if (selectedIndex == whiteGraveyard.size() - 1) whiteGraveyard[0]->sprite->setPosition(660, 480);
            else whiteGraveyard[selectedIndex + 1]->sprite->setPosition(660, 480);
            whiteGraveyard[selectedIndex]->sprite->setPosition(10 * TILE_SIZE, 310);

            if (selectedIndex == 0) selectedIndex = whiteGraveyard.size() - 1;
            else selectedIndex--;

            whiteGraveyard[selectedIndex]->select();
            whiteGraveyard[selectedIndex]->sprite->setPosition(10 * TILE_SIZE, 210);
            if (selectedIndex == 0) whiteGraveyard[whiteGraveyard.size() - 1]->sprite->setPosition(10 * TILE_SIZE, 310);
            else whiteGraveyard[selectedIndex - 1]->sprite->setPosition(10 * TILE_SIZE, 110);
        }
    }

    else if(isPlayerTurn && selectedMode){
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
    int k = blackPieces.size() - 1;
    for (int i = 0; i < 9; i++) {
        for(int j = 0; j < 2; j++){
            blackPieces[k]->place(j, i);
            k--;
        }
    }
    
    for(int i = 0; i < 3; i++){
        blackPieces[k]->place(2, i);
        k--;
    }
}

void Game::movePiece(Piece* piece, Piece::MoveType moveType) {
    switch (moveType) {
    case Piece::Up:
        if (!board[piece->currentRow - 1][piece->currentCol].isOccupied) {
            board[piece->currentRow][piece->currentCol].isOccupied = false;
            board[piece->currentRow][piece->currentCol].piece = NULL;

            board[piece->currentRow - 1][piece->currentCol].isOccupied = true;
            board[piece->currentRow - 1][piece->currentCol].piece = piece;

            piece->currentRow -= 1;
            piece->sprite->move(-Game::TILE_SIZE, 0);
            isPlayerTurn = !isPlayerTurn;
        }

        else if (piece->team != board[piece->currentRow - 1][piece->currentCol].piece->team) {
            board[piece->currentRow][piece->currentCol].isOccupied = false;
            board[piece->currentRow][piece->currentCol].piece = NULL;

            board[piece->currentRow - 1][piece->currentCol].piece = 
                determineWinner(piece, board[piece->currentRow - 1][piece->currentCol].piece);

            if (piece->isAlive) {
                piece->currentRow -= 1;
                piece->sprite->move(-Game::TILE_SIZE, 0);
            }
            isPlayerTurn = !isPlayerTurn;
        }

        break;
    case Piece::Right:
        if (!board[piece->currentRow][piece->currentCol + 1].isOccupied) {
            board[piece->currentRow][piece->currentCol].isOccupied = false;
            board[piece->currentRow][piece->currentCol].piece = NULL;

            board[piece->currentRow][piece->currentCol + 1].isOccupied = true;
            board[piece->currentRow][piece->currentCol + 1].piece = piece;

            piece->currentCol += 1;
            piece->sprite->move(0, Game::TILE_SIZE);
            isPlayerTurn = !isPlayerTurn;
        }

        else if (piece->team != board[piece->currentRow][piece->currentCol + 1].piece->team) {
            board[piece->currentRow][piece->currentCol].isOccupied = false;
            board[piece->currentRow][piece->currentCol].piece = NULL;

            board[piece->currentRow][piece->currentCol + 1].piece =
                determineWinner(piece, board[piece->currentRow][piece->currentCol + 1].piece);

            if (piece->isAlive) {
                piece->currentCol += 1;
                piece->sprite->move(0, Game::TILE_SIZE);
            }
            isPlayerTurn = !isPlayerTurn;
        }

        break;
    case Piece::Down:
        if (!board[piece->currentRow + 1][piece->currentCol].isOccupied) {
            board[piece->currentRow][piece->currentCol].isOccupied = false;
            board[piece->currentRow][piece->currentCol].piece = NULL;

            board[piece->currentRow + 1][piece->currentCol].isOccupied = true;
            board[piece->currentRow + 1][piece->currentCol].piece = piece;

            piece->currentRow += 1;
            piece->sprite->move(Game::TILE_SIZE, 0);
            isPlayerTurn = !isPlayerTurn;
        }

        else if (piece->team != board[piece->currentRow + 1][piece->currentCol].piece->team) {
            board[piece->currentRow][piece->currentCol].isOccupied = false;
            board[piece->currentRow][piece->currentCol].piece = NULL;

            board[piece->currentRow + 1][piece->currentCol].piece =
                determineWinner(piece, board[piece->currentRow + 1][piece->currentCol].piece);

            if (piece->isAlive) {
                piece->currentRow += 1;
                piece->sprite->move(Game::TILE_SIZE, 0);
            }
            isPlayerTurn = !isPlayerTurn;
        }

        break;
    case Piece::Left:
        if (!board[piece->currentRow][piece->currentCol - 1].isOccupied) {
            board[piece->currentRow][piece->currentCol].isOccupied = false;
            board[piece->currentRow][piece->currentCol].piece = NULL;

            board[piece->currentRow][piece->currentCol - 1].isOccupied = true;
            board[piece->currentRow][piece->currentCol - 1].piece = piece;

            piece->currentCol -= 1;
            piece->sprite->move(0, -Game::TILE_SIZE);
            isPlayerTurn = !isPlayerTurn;
        }

        else if (piece->team != board[piece->currentRow][piece->currentCol - 1].piece->team) {
            board[piece->currentRow][piece->currentCol].isOccupied = false;
            board[piece->currentRow][piece->currentCol].piece = NULL;

            board[piece->currentRow][piece->currentCol - 1].piece =
                determineWinner(piece, board[piece->currentRow][piece->currentCol - 1].piece);

            if (piece->isAlive) {
                piece->currentCol -= 1;
                piece->sprite->move(0, -Game::TILE_SIZE);
            }
            isPlayerTurn = !isPlayerTurn;
        }

        break;
    }
}

Piece* Game::determineWinner(Piece* aggressor, Piece* defender) {
    //same rank, both die
    if (aggressor->rank == defender->rank) {
        //special condition for flag aggressor eating other flag
        if (aggressor->rank == 0) {
            sendToGraveyard(defender);
            return aggressor;
        }
        sendToGraveyard(aggressor);
        sendToGraveyard(defender);
        return NULL;
    }

    //private aggressor, spy defender
    else if (aggressor->rank == 1 && defender->rank == 14) {
        sendToGraveyard(defender);
        return aggressor;
    }

    //private defender, spy aggressor
    else if (defender->rank == 1 && aggressor->rank == 14) {
        sendToGraveyard(aggressor);
        return defender;
    }

    //standard rank
    else if (aggressor->rank > defender->rank) {
        sendToGraveyard(defender);
        return aggressor;
    }

    //lost the challenge
    else {
        sendToGraveyard(aggressor);
        return defender;
    }
}

void Game::sendToGraveyard(Piece* piece) {
    piece->isAlive = false;

    if (piece->team) {
        piece->sprite->setPosition(8 * TILE_SIZE, 25 * whiteGraveyard.size());
        whiteGraveyard.push_back(piece);
    }
    else {
        piece->sprite->setPosition(0, 25 * blackGraveyard.size());
        blackGraveyard.push_back(piece);
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

void Game::update(sf::Time deltaTime) {
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
    for (int i = 0; i < grid.size(); i++) {
        mWindow.draw(*grid[i]);
    }
    for (int i = 0; i < whitePieces.size(); i++) {
        mWindow.draw(*whitePieces[i]->sprite);
    }
    for (int i = 0; i < blackPieces.size(); i++) {
        mWindow.draw(*blackPieces[i]->sprite);
    }
    for (int i = 0; i < whiteGraveyard.size(); i++) {
        mWindow.draw(*whiteGraveyard[i]->sprite);
    }
    for (int i = 0; i < blackGraveyard.size(); i++) {
        mWindow.draw(*blackGraveyard[i]->sprite);
    }

    if (quitMenu) mWindow.draw(quitBox);
    mWindow.display();
}