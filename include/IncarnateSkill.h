#ifndef INCARNATE_SKILL_H
#define INCARNATE_SKILL_H

#include "Skill.h"

namespace bb {
    class IncarnateSkill : public Skill {
    public:
        IncarnateSkill(World& world) : Skill(world, sf::Keyboard::Unknown) {
            m_sp = 0;
        }
        virtual ~IncarnateSkill() {
        }
        virtual void use(Player* player, sf::Vector2f coord) = 0;
        void setKey(sf::Keyboard::Key key) { m_key = key; }
        std::vector<std::string>& getKeywords() { return m_keywords; }
    protected:
        std::vector<std::string> m_keywords;
    };
}

#endif