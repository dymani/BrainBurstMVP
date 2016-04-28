#ifndef SKILLS_H
#define SKILLS_H

#include "Skill.h"

namespace bb {
    class SkillHit : public Skill {
    public:
        SkillHit(World& world, sf::Keyboard::Key key);
        void use(Player* player, sf::Vector2f coord);
    };

    class SkillProjectile : public Skill {
    public:
        SkillProjectile(World& world, sf::Keyboard::Key key);
        void use(Player* player, sf::Vector2f coord);
    };
}

#endif