#include "Entity.h"
#include "Piece.h"
#include"Space.h"
#include <SFML/Graphics.hpp>
#include <vector>
#pragma once
using namespace std;

class Game {
public:
    Game();
    void run();

    static const int TILE_SIZE = 60;
    const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
private:
    void processEvents(sf::Time);
    void update(sf::Time);
    void render();

    void handlePlayerInput(sf::Keyboard::Key);
    void movePiece(Piece* piece, Piece::MoveType moveType);
    void setBlack();
    
    Piece* determineWinner(Piece*, Piece*);
    void sendToGraveyard(Piece*);
    void blackMove();
    void checkFlags();
private:
    sf::RenderWindow mWindow;
    std::vector<sf::RectangleShape*> grid;
    sf::Sprite quitBox;
    sf::Sprite resultScreen;
    sf::Sprite titleScreen;
    Piece* selected = NULL;
    int selectedIndex;

    Space board[8][9];
    vector<Piece*> whitePieces;
    vector<Piece*> blackPieces;
    vector<Piece*> whiteGraveyard;
    vector<Piece*> blackGraveyard;

    float ticks = 0.0f;

    bool isClicking = false;
    bool startPhase = true;
    bool isPlayerTurn = true;
    bool selectedMode = false;
    bool quitMenu = false;
    bool startMenu = true;
    bool win = false;
    bool lose = false;
};