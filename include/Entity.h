#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

namespace bb {
    class World;

    class Entity {
    public:
        Entity(World& world, int id) : m_world(world), ID(id) {
        }
        virtual ~Entity() {
        }
        virtual bool update() = 0;
        virtual void draw(const double dt) = 0;
        virtual b2Body* getBody() = 0;
        virtual int getHp() = 0;
        virtual void setHp(int hp) = 0;
        const int ID;
    protected:
        World& m_world;
        b2Body* m_body;
        int m_hp;
    };
}

#endif