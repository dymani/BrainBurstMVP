#include "IncarnateSkills.h"
#include "World.h"
#include "Player.h"

namespace bb {
    IncarnateSkillTest::IncarnateSkillTest(World& world) : IncarnateSkill(world) {
        m_keywords.push_back("atest");
        m_keywords.push_back("btest");
        m_keywords.push_back("ctest");
    }

    void IncarnateSkillTest::use(Player* player, sf::Vector2f coord) {
        player->setHp(100, player->ID);
    }

    IncarnateSkillTrial::IncarnateSkillTrial(World& world) : IncarnateSkill(world) {
        m_keywords.push_back("atrial");
        m_keywords.push_back("btrial");
    }

    void IncarnateSkillTrial::use(Player* player, sf::Vector2f coord) {
        player->setSp(100);
    }
}