#ifndef INCARNATE_SKILLS_H
#define INCARNATE_SKILLS_H

#include "IncarnateSkill.h"

namespace bb {
    class IncarnateSkillTest : public IncarnateSkill {
    public:
        IncarnateSkillTest(World& world);
        void use(Player* player, sf::Vector2f coord);
    };

    class IncarnateSkillTrial : public IncarnateSkill {
    public:
        IncarnateSkillTrial(World& world);
        void use(Player* player, sf::Vector2f coord);
    };
}

#endif