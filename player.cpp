#include "player.hpp"

Player::Player(
    unsigned int index,
    sf::Color color,
    KeyConfig config,
    sf::Vector2f house_pos,
    sf::Texture* texture
) : key_config{config}, index{index}, house{sf::Vector2f{HOUSE_WIDTH, HOUSE_HEIGHT}} { 
    this->score = 0;
    this->stunned = false;
    this->carried_item = nullptr;

    this->house.setPosition(house_pos);
    this->house.setFillColor(
        sf::Color(100 + color.r * 0.2, 100 + color.g * 0.2, 100 + color.b * 0.2));

    this->sprite.setTexture(*texture);
    this->sprite.setOrigin(8, 8);
    this->sprite.setTextureRect(sf::IntRect(0, 18 * 2, 16, 18));
    //this->shape.setFillColor(color);
    auto player_pos = house_pos + sf::Vector2f(HOUSE_WIDTH / 2, HOUSE_HEIGHT / 2);
    this->sprite.setPosition(player_pos);

    const unsigned int item_spacing = 40;
    for (unsigned int i = 0; i < NUM_BOXES; i++) {
        boxes.push_back(Box{sf::Vector2f(20, 20)});
        boxes[i].shape.setFillColor(sf::Color(50, 50, 50));

        if (i < NUM_BOXES / 2) {
            sf::Vector2f offset(10 + i * item_spacing, 10);
            boxes[i].shape.setPosition(house_pos + offset);
        } else {
            sf::Vector2f offset(10 + (i - NUM_BOXES / 2) * item_spacing, HOUSE_HEIGHT - 30);
            boxes[i].shape.setPosition(house_pos + offset);
        }
    }
}

void Player::set_position(sf::Vector2f position) {
    if (this->carried_item != nullptr) {
        this->carried_item->shape.setPosition(position);
    }
    this->sprite.setPosition(position);
}

void Player::move(float dx, float dy) {
    if (this->carried_item != nullptr) {
        this->carried_item->shape.move(dx, dy);
    }
    this->sprite.move(dx, dy);
}

bool Player::is_home() const {
    auto house_bb = this->house.getGlobalBounds();
    return house_bb.intersects(this->sprite.getGlobalBounds());
}

