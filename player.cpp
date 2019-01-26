#include "player.hpp"

Player::Player(
    unsigned int index,
    KeyConfig config,
    sf::Vector2f house_pos,
    sf::Texture* texture,
    sf::Texture* house_texture
) : key_config{config}, index{index}, direction{Down}, shape{PLAYER_RADIUS}, house{sf::Vector2f{HOUSE_WIDTH, HOUSE_HEIGHT}} {
    this->score = 0;
    this->stunned = false;
    this->moving = false;
    this->carried_item = nullptr;
    this->powerup = nullptr;

    this->house.setPosition(house_pos);
    this->house_sprite.setTexture(*house_texture);
    this->house_sprite.setPosition(house_pos);

    this->sprite.setTexture(*texture);
    this->sprite.setOrigin(PLAYER_RADIUS, PLAYER_RADIUS);
    this->shape.setOrigin(PLAYER_RADIUS, PLAYER_RADIUS);
    auto player_pos = house_pos + sf::Vector2f(HOUSE_WIDTH / 2, HOUSE_HEIGHT / 2);
    set_position(player_pos);

    const unsigned int item_spacing = 40;
    for (unsigned int i = 0; i < NUM_BOXES; i++) {
        boxes.push_back(Box{sf::Vector2f(20, 20)});
        boxes[i].shape.setFillColor(sf::Color(50, 50, 50));

        if (i < NUM_BOXES / 2) {
            sf::Vector2f offset(10 + i * item_spacing, 25);
            boxes[i].shape.setPosition(house_pos + offset);
        } else {
            sf::Vector2f offset(10 + (i - NUM_BOXES / 2) * item_spacing, HOUSE_HEIGHT - 45);
            boxes[i].shape.setPosition(house_pos + offset);
        }
    }
}

void Player::set_position(sf::Vector2f position) {
    // if (this->powerup != nullptr) {
    //     this->powerup->shape.setPosition(position);
    // }
    if (this->carried_item != nullptr) {
        this->carried_item->shape.setPosition(position);
    }
    this->sprite.setPosition(position);
    this->shape.setPosition(position);
}

void Player::move(float dx, float dy) {
    // if (this->powerup != nullptr) {
    //     this->powerup->shape.move(dx, dy);
    // }
    if (this->carried_item != nullptr) {
        this->carried_item->shape.move(dx, dy);
    }
    this->sprite.move(dx, dy);
    this->shape.move(dx, dy);
}

bool Player::is_home() const {
    auto house_bb = this->house.getGlobalBounds();
    return house_bb.intersects(this->sprite.getGlobalBounds());
}
