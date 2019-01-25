#include "player.hpp"

Player::Player(unsigned int index,
        sf::Color color,
        sf::Vector2f house_pos) : index{index} { 
    this->score = 0;
    this->stunned = false;

    this->house = sf::RectangleShape{sf::Vector2f{HOUSE_WIDTH, HOUSE_HEIGHT}};
    this->house.setPosition(house_pos);
    this->house.setFillColor(sf::Color(100, 100, 100));

    this->shape = sf::CircleShape{PLAYER_RADIUS};
    this->shape.setOrigin(10, 10);
    this->shape.setFillColor(color);
    auto player_pos = house_pos + sf::Vector2f(HOUSE_WIDTH / 2, HOUSE_HEIGHT / 2);
    this->shape.setPosition(player_pos);
}

