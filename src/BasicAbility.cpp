#include "BasicAbility.h"
#include "Player.h"
#include "World.h"

namespace bb {
    BasicAbility::BasicAbility(World& world, sf::Keyboard::Key key) : Ability(world, key) {
        m_hold = -50;
        m_timeout = -100;
    }

    void BasicAbility::use(Player* player, sf::Vector2f coord) {
        int entity = m_world.seekEntity(coord);
        m_world.damage(entity, 1);
    }

    sf::Keyboard::Key BasicAbility::getKey() {
        return m_key;
    }

    int BasicAbility::getHold() {
        return m_hold;
    }

    int BasicAbility::getTimeout() {
        return m_timeout;
    }
}