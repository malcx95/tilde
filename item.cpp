#include <math.h>
#include "item.hpp"

const int SPAWN_RADIUS = 150;

unsigned int num_stationary_items(std::vector<Item*>& items) {
    unsigned int res = 0;
    for (Item* it : items) {
        if (!it->being_carried) res++;
    }
    return res;
}

void spawn_item(std::vector<Item*>& items, std::vector<sf::Texture*> item_textures) {
    if (num_stationary_items(items) < MAX_ITEMS) {
        Item* item = new Item;

        sf::Vector2f center(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        float random_angle = float(rand() % 180) / M_PI;
        int random_radius = rand() % SPAWN_RADIUS;
        sf::Vector2f offset(
            random_radius * cos(random_angle),
            random_radius * sin(random_angle)
        );

        int random_item = int(rand() % item_textures.size());
        item->sprite.setTexture(*item_textures[random_item]);
        item->sprite.setOrigin(10,10);
        item->sprite.setPosition(center + offset);

        item->being_carried = false;
        item->in_box = false;

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
