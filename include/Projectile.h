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
        Projectile(World& world, int id, float coordX, float coordY, float velX, float velY, int owner);
        ~Projectile();
        bool update();
        void draw(const double dt);
        b2Body* getBody();
        int getHp();
        void setHp(int hp, int entity);
    private:
        sf::RectangleShape m_sprite;
        std::unique_ptr<ProjectileContactListener> m_contactListener;
        int m_contactListenerId;
        int m_owner;
        bool m_hasHit;
        int m_entityHit;
    };

    class ProjectileContactListener : public ContactListener {
    public:
        ProjectileContactListener(Projectile& projectile);
        void beginContact(EntityData* a, EntityData* b);
        void endContact(EntityData* a, EntityData* b);
    private:
        Projectile& m_projectile;
    };
}

#endif