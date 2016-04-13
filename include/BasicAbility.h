#ifndef BASIC_ABILITY_H
#define BASIC_ABILITY_H

#include "Ability.h"

namespace bb {
    class BasicAbility : public Ability {
    public:
        BasicAbility(World& world, sf::Keyboard::Key key);
        void use(Player* player, sf::Vector2f coord);
        sf::Keyboard::Key getKey();
        int getHold();
        int getTimeout();
    private:

    };
}

#endif