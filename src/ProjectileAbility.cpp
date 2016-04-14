#include "ProjectileAbility.h"
#include "World.h"
#include "Player.h"
#include "Projectile.h"
#include <math.h>
#define PI 3.14159265

namespace bb {
    ProjectileAbility::ProjectileAbility(World& world, sf::Keyboard::Key key) : Ability(world, key) {
        m_hold = -50;
        m_timeout = -100;
    }

    void ProjectileAbility::use(Player* player, sf::Vector2f coord) {
        auto pCoord = player->getBody()->GetPosition();
        float velX, velY;
        float x = coord.x - pCoord.x;
        float y = coord.y - pCoord.y;
        double angle = std::atan2(y, x) * 180 / PI;
        velX = float(5 * std::cos(angle * PI / 180));
        velY = float(5 * std::sin(angle * PI / 180));
        pCoord.x += float(0.8 * std::cos(angle * PI / 180));
        pCoord.y += float(0.8 * std::sin(angle * PI / 180));
        Projectile* projectile = new Projectile(m_world, pCoord.x, pCoord.y, velX, velY);
        m_world.addEntity(projectile);
    }

    sf::Keyboard::Key ProjectileAbility::getKey() {
        return m_key;
    }

    int ProjectileAbility::getHold() {
        return m_hold;
    }

    int ProjectileAbility::getTimeout() {
        return m_timeout;
    }
}