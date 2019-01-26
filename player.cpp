#include "player.hpp"

Player::Player(unsigned int index,
        sf::Color color,
        KeyConfig config,
        sf::Vector2f house_pos) : key_config{config}, index{index} { 
    this->score = 0;
    this->stunned = false;
    this->carried_item = nullptr;
    this->powerup = nullptr;

    this->house = sf::RectangleShape{sf::Vector2f{HOUSE_WIDTH, HOUSE_HEIGHT}};
    this->house.setPosition(house_pos);
    this->house.setFillColor(sf::Color(100, 100, 100));

    this->shape = sf::CircleShape{PLAYER_RADIUS};
    this->shape.setOrigin(10, 10);
    this->shape.setFillColor(color);
    auto player_pos = house_pos + sf::Vector2f(HOUSE_WIDTH / 2, HOUSE_HEIGHT / 2);
    this->shape.setPosition(player_pos);
}

void Player::set_position(sf::Vector2f position) {
    // if (this->powerup != nullptr) {
    //     this->powerup->shape.setPosition(position);
    // }
    if (this->carried_item != nullptr) {
        this->carried_item->shape.setPosition(position);
    }
    this->shape.setPosition(position);
}

void Player::move(float dx, float dy) {
    // if (this->powerup != nullptr) {
    //     this->powerup->shape.move(dx, dy);
    // }
    if (this->carried_item != nullptr) {
        this->carried_item->shape.move(dx, dy);
    }
    this->shape.move(dx, dy);
}

bool Player::is_home() const {
    auto house_bb = this->house.getGlobalBounds();
    return house_bb.intersects(this->shape.getGlobalBounds());
}

