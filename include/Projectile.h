#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <memory>
#include "Entity.h"
#include "ContactListener.h"

namespace bb {
    class World;

    class Projectile : public Entity {
        friend class ProjectileContactListener;
    public:
        Projectile(World& world, int id, float coordX, float coordY, float velX, float velY);
        ~Projectile();
        bool update();
        void draw(const double dt);
        b2Body* getBody();
        int getHp();
        void setHp(int hp);
    private:
        sf::RectangleShape m_sprite;
        std::unique_ptr<ProjectileContactListener> m_contactListener;
        int m_contactListenerId;
        bool m_hasHit;
    };

    class ProjectileContactListener : public ContactListener {
    public:
        ProjectileContactListener(Projectile& projectile);
        void beginContact(b2Contact* contact);
        void endContact(b2Contact* contact);
    private:
        Projectile& m_projectile;
    };
}

#endif