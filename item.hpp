#ifndef ITEM_H
#define ITEM_H

#include <SFML/Graphics.hpp>
#include "constants.hpp"

const int MAX_ITEMS = 10;

struct Item {
    bool being_carried;
    sf::Sprite sprite;
};

void spawn_item(std::vector<Item*>& items, std::vector<sf::Texture*> item_textures);

void remove_item(std::vector<Item*>& items, Item* item);

#endif /* ifndef ITEM_H */
