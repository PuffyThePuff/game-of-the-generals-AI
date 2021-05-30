#include "game.h"
#include <SFML/Graphics.hpp>
#include<iostream>

Game::Game() :
    mWindow(sf::VideoMode(660, 480), "Salpakan") {
    mWindow.setFramerateLimit(60);
    TextureManager::getInstance()->loadAll();

    // BLACK PIECES

    // Flag
    Piece* piece0 = new Piece(0, false, "hidden");
    piece0->isAlive = true;
    blackPieces.push_back(piece0);

    // Six privates
    for (int i = 0; i < 6; i++) {
        Piece* piece = new Piece(1, false, "hidden");
        piece->isAlive = true;
        blackPieces.push_back(piece);
    }

    // All other pieces
    for (int i = 2; i <= 14; i++) {
        Piece* piece = new Piece(i, false, "hidden");
        piece->isAlive = true;
        blackPieces.push_back(piece);
    }

    // Extra spy
    Piece* piece1 = new Piece(14, false, "hidden");
    piece1->isAlive = true;
    blackPieces.push_back(piece1);

    // WHITE PIECES

    //flag
    Piece* piece2 = new Piece(0, true, "flag_white");
    whitePieces.push_back(piece2);
    // Prepare for piece placement.
    whiteGraveyard.push_back(piece2);
    piece2->sprite->setPosition(10 * TILE_SIZE, 210);
    selectedIndex = 0;

    //six privates
    for (int i = 0; i < 6; i++) {
        Piece* piece = new Piece(1, true, "private_white");
        whitePieces.push_back(piece);
        whiteGraveyard.push_back(piece);
        piece->sprite->setPosition(660, 480);
    }

    Piece* piece = new Piece(2, true, "sergeant_white");
    whitePieces.push_back(piece);
    whiteGraveyard.push_back(piece);
    piece->sprite->setPosition(660, 480);

    piece = new Piece(3, true, "2ndlt_white");
    whitePieces.push_back(piece);
    whiteGraveyard.push_back(piece);
    piece->sprite->setPosition(660, 480);

    piece = new Piece(4, true, "1stlt_white");
    whitePieces.push_back(piece);
    whiteGraveyard.push_back(piece);
    piece->sprite->setPosition(660, 480);

    piece = new Piece(5, true, "captain_white");
    whitePieces.push_back(piece);
    whiteGraveyard.push_back(piece);
    piece->sprite->setPosition(660, 480);

    piece = new Piece(6, true, "major_white");
    whitePieces.push_back(piece);
    whiteGraveyard.push_back(piece);
    piece->sprite->setPosition(660, 480);

    piece = new Piece(7, true, "ltcolonel_white");
    whitePieces.push_back(piece);
    whiteGraveyard.push_back(piece);
    piece->sprite->setPosition(660, 480);

    piece = new Piece(8, true, "colonel_white");
    whitePieces.push_back(piece);
    whiteGraveyard.push_back(piece);
    piece->sprite->setPosition(660, 480);

    piece = new Piece(9, true, "1star_white");
    whitePieces.push_back(piece);
    whiteGraveyard.push_back(piece);
    piece->sprite->setPosition(660, 480);

    piece = new Piece(10, true, "2star_white");
    whitePieces.push_back(piece);
    whiteGraveyard.push_back(piece);
    piece->sprite->setPosition(660, 480);

    piece = new Piece(11, true, "3star_white");
    whitePieces.push_back(piece);
    whiteGraveyard.push_back(piece);
    piece->sprite->setPosition(660, 480);

    piece = new Piece(12, true, "4star_white");
    whitePieces.push_back(piece);
    whiteGraveyard.push_back(piece);
    piece->sprite->setPosition(660, 480);

    piece = new Piece(13, true, "5star_white");
    whitePieces.push_back(piece);
    whiteGraveyard.push_back(piece);
    piece->sprite->setPosition(660, 480);

    //two spies
    for (int i = 0; i < 2; i++) {
        Piece* piece3 = new Piece(14, true, "spy_white");
        whitePieces.push_back(piece3);
        whiteGraveyard.push_back(piece3);
        piece3->sprite->setPosition(660, 480);
    }

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

    if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if(startPhase){
            sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
            if(mousePos.y >= (5 * TILE_SIZE) && mousePos.y < 480 && mousePos.x >= 60 && mousePos.x < 660){
                int row = mousePos.y / TILE_SIZE;
                int col = (mousePos.x / TILE_SIZE) - 1;
                if (board[row][col].isOccupied == false) {
                    whiteGraveyard[selectedIndex]->place(row, col);
                    whiteGraveyard[selectedIndex]->isAlive = true;
                    board[row][col].isOccupied = true;
                    board[row][col].piece = whiteGraveyard[selectedIndex];

                    int current = selectedIndex;
                    if (whiteGraveyard.size() > 1) {
                        if (selectedIndex == whiteGraveyard.size() - 1) selectedIndex = 0;
                        else selectedIndex++;

                        whiteGraveyard[selectedIndex]->sprite->setPosition(10 * TILE_SIZE, 210);
                        whiteGraveyard.erase(
                            std::remove(
                                whiteGraveyard.begin(),
                                whiteGraveyard.end(),
                                whiteGraveyard[current]
                            ), whiteGraveyard.end()
                        );
                        whiteGraveyard.shrink_to_fit();
                        if (selectedIndex != 0) selectedIndex--;
                    }
                    
                    else {
                        whiteGraveyard.clear();
                    }
                }
            }

            if (whiteGraveyard.empty()) { 
                startPhase = false;
                std::cout << "end of placement\n";
            }
        }

        else{
            sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
            if(mousePos.y < 480 && mousePos.x < 660 && isPlayerTurn){
                int row = mousePos.y / TILE_SIZE;
                int col = (mousePos.x / TILE_SIZE) - 1;
                if(board[row][col].isOccupied && board[row][col].piece->team){
                    selectedMode = true;
                    if (selected != NULL) selected->deselect();
                    selected = board[row][col].piece;
                    board[row][col].piece->select();
                }
            }
        }
    }
}

void Game::handlePiecePlacement(int row, int col) {
    whiteGraveyard[selectedIndex]->place(row, col);
    whiteGraveyard[selectedIndex]->isAlive = true;
    board[row][col].isOccupied = true;


    int current = selectedIndex;
    if (selectedIndex == whiteGraveyard.size()) selectedIndex = 0;
    else selectedIndex++;
    whiteGraveyard[selectedIndex]->sprite->setPosition(10 * TILE_SIZE, 210);

    whiteGraveyard.erase(whiteGraveyard.begin() + current);

    if (whiteGraveyard.empty()) {
        startPhase = false;
        std::cout << "false\n";
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key) {
    if (startPhase) {
        if (key == sf::Keyboard::W) {
            whiteGraveyard[selectedIndex]->sprite->setPosition(660, 480);
            if (selectedIndex == whiteGraveyard.size() - 1) selectedIndex = 0;
            else selectedIndex++;
            whiteGraveyard[selectedIndex]->sprite->setPosition(10* TILE_SIZE, 210);
        }
        if (key == sf::Keyboard::S) {
            whiteGraveyard[selectedIndex]->sprite->setPosition(660, 480);
            if (selectedIndex == 0) selectedIndex = whiteGraveyard.size() - 1;
            else selectedIndex--;
            whiteGraveyard[selectedIndex]->sprite->setPosition(10 * TILE_SIZE, 210);
        }
    }

    else if(isPlayerTurn && selectedMode){
        if (key == sf::Keyboard::W && selected->currentRow > 0) {
            movePiece(selected, Piece::Up);
            selected->deselect();
            selectedMode = false;
            blackMove();
        }

        if (key == sf::Keyboard::S && selected->currentRow <= 7){
            movePiece(selected, Piece::Down);
            selected->deselect();
            selectedMode = false;
            blackMove();
        }

        if (key == sf::Keyboard::A && selected->currentCol > 0){
            movePiece(selected, Piece::Left);
            selected->deselect();
            selectedMode = false;
            blackMove();
        }

        if (key == sf::Keyboard::D && selected->currentCol <= 7){
            movePiece(selected, Piece::Right);
            selected->deselect();
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
            board[j][i].isOccupied = true;
            board[j][i].piece = blackPieces[k];
            k--;
        }
    }
    
    for(int i = 0; i < 3; i++){
        blackPieces[k]->place(2, i);
        board[2][i].isOccupied = true;
        board[2][i].piece = blackPieces[k];
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
            piece->sprite->move(0, -Game::TILE_SIZE);
            isPlayerTurn = !isPlayerTurn;
        }

        else if (piece->team != board[piece->currentRow - 1][piece->currentCol].piece->team) {
            board[piece->currentRow][piece->currentCol].isOccupied = false;
            board[piece->currentRow][piece->currentCol].piece = NULL;

            board[piece->currentRow - 1][piece->currentCol].piece = 
                determineWinner(piece, board[piece->currentRow - 1][piece->currentCol].piece);

            if (piece->isAlive) {
                piece->currentRow -= 1;
                piece->sprite->move(0, -Game::TILE_SIZE);
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
            piece->sprite->move(Game::TILE_SIZE, 0);
            isPlayerTurn = !isPlayerTurn;
        }

        else if (piece->team != board[piece->currentRow][piece->currentCol + 1].piece->team) {
            board[piece->currentRow][piece->currentCol].isOccupied = false;
            board[piece->currentRow][piece->currentCol].piece = NULL;

            board[piece->currentRow][piece->currentCol + 1].piece =
                determineWinner(piece, board[piece->currentRow][piece->currentCol + 1].piece);

            if (piece->isAlive) {
                piece->currentCol += 1;
                piece->sprite->move(Game::TILE_SIZE, 0);
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
            piece->sprite->move(0, Game::TILE_SIZE);
            isPlayerTurn = !isPlayerTurn;
        }

        else if (piece->team != board[piece->currentRow + 1][piece->currentCol].piece->team) {
            board[piece->currentRow][piece->currentCol].isOccupied = false;
            board[piece->currentRow][piece->currentCol].piece = NULL;

            board[piece->currentRow + 1][piece->currentCol].piece =
                determineWinner(piece, board[piece->currentRow + 1][piece->currentCol].piece);

            if (piece->isAlive) {
                piece->currentRow += 1;
                piece->sprite->move(0, Game::TILE_SIZE);
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
            piece->sprite->move(-Game::TILE_SIZE, 0);
            isPlayerTurn = !isPlayerTurn;
        }

        else if (piece->team != board[piece->currentRow][piece->currentCol - 1].piece->team) {
            board[piece->currentRow][piece->currentCol].isOccupied = false;
            board[piece->currentRow][piece->currentCol].piece = NULL;

            board[piece->currentRow][piece->currentCol - 1].piece =
                determineWinner(piece, board[piece->currentRow][piece->currentCol - 1].piece);

            if (piece->isAlive) {
                piece->currentCol -= 1;
                piece->sprite->move(-Game::TILE_SIZE, 0);
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
        piece->sprite->setPosition(10 * TILE_SIZE, 25 * whiteGraveyard.size());
        whiteGraveyard.push_back(piece);
    }
    else {
        piece->sprite->setPosition(0, 25 * blackGraveyard.size());
        blackGraveyard.push_back(piece);
    }
}

void Game::blackMove(){
    isPlayerTurn = true;
    selectedMode = false;
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
        if(whitePieces[i]->isAlive) mWindow.draw(*whitePieces[i]->sprite);
    }
    for (int i = 0; i < blackPieces.size(); i++) {
        if (blackPieces[i]->isAlive) mWindow.draw(*blackPieces[i]->sprite);
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