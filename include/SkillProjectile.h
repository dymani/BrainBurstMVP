#ifndef SKILL_PROJECTILE_H
#define SKILL_PROJECTILE_H

#include "Skill.h"

namespace bb {
    class SkillProjectile : public Skill {
    public:
        SkillProjectile(World& world, sf::Keyboard::Key key);
        void use(Player* player, sf::Vector2f coord);
        sf::Keyboard::Key getKey();
        int getHold();
        int getTimeout();
        int getSp();
    private:

    };
}

#endif