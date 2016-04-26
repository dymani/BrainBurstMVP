#ifndef OBJECT_H
#define OBJECT_H

#include "Entity.h"

namespace bb {
    class Object : public Entity {
    public:
        Object(World& world, int id, float coordX, float coordY);
        bool update();
        void draw(const double dt);
        b2Body* getBody();
        int getHp();
        void setHp(int hp, int entity);
    private:
        sf::RectangleShape m_sprite;
    };
}

#endif