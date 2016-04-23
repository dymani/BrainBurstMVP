#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "ContactListener.h"

namespace bb {
    class World;

    class Enemy : public Entity{
        friend class EnemyContactListener;
    public:
        Enemy(World& world, int id, float coordX, float coordY, int type);
        ~Enemy();
        bool update();
        void draw(const double dt);
        b2Body* getBody();
        int getHp();
        void setHp(int hp);
    private:
        const enum Type {
            T_STATIC, T_MOBILE, T_HOSTILE
        } m_type;
        sf::RectangleShape m_sprite;
        std::unique_ptr<EnemyContactListener> m_contactListener;
        int m_contactListenerId;
        bool m_hasHit;
        int m_hitTimeout;
        int m_numFootContacts;
    };

    class EnemyContactListener : public ContactListener {
    public:
        EnemyContactListener(Enemy& enemy);
        void beginContact(b2Contact* contact);
        void endContact(b2Contact* contact);
    private:
        Enemy& m_enemy;
    };
}

#endif