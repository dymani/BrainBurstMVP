#include "SkillHit.h"
#include "Player.h"
#include "World.h"

namespace bb {
    SkillHit::SkillHit(World& world, sf::Keyboard::Key key) : Skill(world, key) {
        m_hold = -10;
        m_timeout = -100;
        m_sp = 10;
    }

    void SkillHit::use(Player* player, sf::Vector2f coord) {
        int entity = m_world.seekEntity(coord);
        m_world.damage(player->ID, entity, 10);
    }

    sf::Keyboard::Key SkillHit::getKey() {
        return m_key;
    }

    int SkillHit::getHold() {
        return m_hold;
    }

    int SkillHit::getTimeout() {
        return m_timeout;
    }

    int SkillHit::getSp() {
        return m_sp;
    }
}