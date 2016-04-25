#ifndef PROJECTILE_ABILITY_H
#define PROJECTILE_ABILITY_H

#include "Ability.h"

namespace bb {
    class ProjectileAbility : public Ability {
    public:
        ProjectileAbility(World& world, sf::Keyboard::Key key);
        void use(Player* player, sf::Vector2f coord);
        sf::Keyboard::Key getKey();
        int getHold();
        int getTimeout();
        int getAp();
    private:

    };
}

#endif