#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Entity.h"

namespace bb {
    class World;

    class Projectile : public Entity {
        friend class ProjectileContactListener;
    public:
        Projectile(World& world, float coordX, float coordY, float velX, float velY);
        bool update();
        void draw(const double dt);
        b2Body* getBody();
        int getHp();
        void setHp(int hp);
    private:
        sf::RectangleShape m_sprite;
        ProjectileContactListener* m_contactListener;
        bool m_hasHit;
    };

    class ProjectileContactListener : public b2ContactListener {
    public:
        ProjectileContactListener(Projectile& projectile);
        void BeginContact(b2Contact* contact);
        void EndContact(b2Contact* contact);
    private:
        Projectile& m_projectile;
    };
}

#endif