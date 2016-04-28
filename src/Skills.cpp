#include "Skills.h"
#include "World.h"
#include "Player.h"
#include "Projectile.h"
#include <math.h>
#define PI 3.14159265

namespace bb {
    SkillHit::SkillHit(World& world, sf::Keyboard::Key key) : Skill(world, key) {
        m_sp = 10;
    }

    void SkillHit::use(Player* player, sf::Vector2f coord) {
        int entity = m_world.seekEntity(coord);
        m_world.damage(player->ID, entity, 10);
    }

    SkillProjectile::SkillProjectile(World& world, sf::Keyboard::Key key) : Skill(world, key) {
        m_sp = 20;
    }

    void SkillProjectile::use(Player* player, sf::Vector2f coord) {
        auto pCoord = player->getBody()->GetPosition();
        float velX, velY;
        float x = coord.x - pCoord.x;
        float y = coord.y - pCoord.y;
        double angle = std::atan2(y, x) * 180 / PI;
        velX = float(5 * std::cos(angle * PI / 180));
        velY = float(5 * std::sin(angle * PI / 180));
        pCoord.x += float(0.8 * std::cos(angle * PI / 180));
        pCoord.y += float(0.8 * std::sin(angle * PI / 180));
        Projectile* projectile = new Projectile(m_world, m_world.getNewId(), pCoord.x, pCoord.y, velX,
            velY, player->ID);
        m_world.addEntity(projectile);
    }
}