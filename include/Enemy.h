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
        void setHp(int hp, int entity);
    private:
        const enum EnemyType {
            T_FRIENDLY, T_PASSIVE , T_HOSTILE
        } m_type;
        sf::RectangleShape m_sprite;
        std::unique_ptr<EnemyContactListener> m_contactListener;
        int m_contactListenerId;
        bool m_hasHit;
        int m_hitTimeout;
        int m_numFootContacts;
        enum MoveState {
            MS_IDLE, MS_LEFT, MS_RIGHT
        } m_moveState;
        int m_moveTime;
        int m_killedBy;
        bool m_needJump;
        int m_jumpTimeout;
        int m_aggro, m_aggroTimeout;
        bool m_hitLeft;
        sf::Color m_colour;
    };

    class EnemyContactListener : public ContactListener {
    public:
        EnemyContactListener(Enemy& enemy);
        void beginContact(EntityData* a, EntityData* b);
        void endContact(EntityData* a, EntityData* b);
    private:
        Enemy& m_enemy;
    };
}

#endif