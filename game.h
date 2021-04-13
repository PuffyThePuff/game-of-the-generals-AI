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
    void createEntity(std::string, float, float);
private:
    sf::RenderWindow mWindow;
    std::vector<Entity*> entityList;
};