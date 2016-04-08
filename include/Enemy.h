#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

namespace bb {
    class World;

    class Enemy : public Entity{
    public:
        Enemy(World& world, float coordX, float coordY, int type);
        bool update();
        void draw(const double dt);
        b2Body* getBody();
        int getHp();
        void setHp(int hp);
    private:
        const enum Type {
            T_STATIC, T_MOBILE
        } m_type;
        sf::RectangleShape m_sprite;
    };
}

#endif