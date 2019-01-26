#ifndef CONSTANTS
#define CONSTANTS

#include <SFML/Graphics.hpp>
#include "config.h"

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
const unsigned int WINDOW_MARGIN = 40;

struct KeyConfig {
    sf::Keyboard::Key up;
    sf::Keyboard::Key down;
    sf::Keyboard::Key left;
    sf::Keyboard::Key right;
};

struct PowerupTextures {
    sf::Texture* speed;
    sf::Texture* immunity;
    sf::Texture* fire;
    sf::Texture* stealing;
};

#endif /* ifndef CONSTANTS */

