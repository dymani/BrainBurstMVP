#ifndef SKILL_HIT_H
#define SKILL_HIT_H

#include "Skill.h"

namespace bb {
    class SkillHit : public Skill {
    public:
        SkillHit(World& world, sf::Keyboard::Key key);
        void use(Player* player, sf::Vector2f coord);
        sf::Keyboard::Key getKey();
        int getHold();
        int getTimeout();
        int getSp();
    private:

    };
}

#endif