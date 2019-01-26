#include <math.h>
#include "item.hpp"

const int SPAWN_RADIUS = 150;

void spawn_item(std::vector<Item*>& items) {
    if (items.size() < MAX_ITEMS) {
        Item* item = new Item;

        sf::Vector2f center(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        float random_angle = float(rand() % 180) / M_PI;
        int random_radius = rand() % SPAWN_RADIUS;
        sf::Vector2f offset(
            random_radius * cos(random_angle),
            random_radius * sin(random_angle)
        );

        sf::CircleShape shape(10, 3);
        shape.setOrigin(10, 10);
        shape.setPosition(center + offset);
        shape.setFillColor(sf::Color(200, 255, 255));

        item->shape = shape;
        item->being_carried = false;

        items.push_back(item);
    }
}

void remove_item(std::vector<Item*>& items, Item* item) {
    size_t index = 0;
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] == item) {
            index = i;
            break;
        }
    }
    items.erase(items.begin() + index);
    delete item;
}
