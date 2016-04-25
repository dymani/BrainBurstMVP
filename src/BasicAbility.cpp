#include "BasicAbility.h"
#include "Player.h"
#include "World.h"

namespace bb {
    BasicAbility::BasicAbility(World& world, sf::Keyboard::Key key) : Ability(world, key) {
        m_hold = -20;
        m_timeout = -100;
        m_ap = 10;
    }

    void BasicAbility::use(Player* player, sf::Vector2f coord) {
        int entity = m_world.seekEntity(coord);
        m_world.damage(player->ID, entity, 10);
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

    int BasicAbility::getAp() {
        return m_ap;
    }
}