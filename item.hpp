#ifndef ITEM_H
#define ITEM_H

#include <SFML/Graphics.hpp>
#include "constants.hpp"

const int MAX_ITEMS = 10;

struct Item {
    sf::CircleShape shape;
};

void spawn_item(std::vector<Item*>& items);

void remove_item(std::vector<Item*>& items, Item* item);

#endif /* ifndef ITEM_H */
