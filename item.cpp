#include "item.hpp"

void spawn_item(std::vector<Item*>& items) {
    if (items.size() < MAX_ITEMS) {
        Item* item = new Item;

        sf::CircleShape shape(10, 3);
        shape.setOrigin(10, 10);
        shape.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
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
