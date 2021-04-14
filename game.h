#include "Entity.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Game {
public:
    Game();
    void run();
private:
    void processEvents();
    void update();
    void render();
    void createEntity(std::string, float, float, int, char);
    void collisionCheck(Entity*);
    void handleCollision(Entity*, Entity*);
    void handlePlayerInput(sf::Keyboard::Key);
private:
    sf::RenderWindow mWindow;
    std::vector<Entity*> entityList;
    std::vector<sf::RectangleShape*> grid;
    // sf::Sprite quitBox;
    Entity* selectedPiece;

    int placeIndex = 20;
    int whiteDead = 0;
    int blackDead = 0;

    bool startPhase = true;
    bool isPlayerTurn = true;
    bool selectedMode = false;
    bool quitMenu = false;
};