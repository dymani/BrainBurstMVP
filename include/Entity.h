#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

namespace bb {
    class World;

    class Entity {
    public:
        const int ID;
        const enum Type {
            PLAYER, OBJECT, ENEMY
        } TYPE;
        Entity(World& world, int id, int type) : m_world(world), ID(id), TYPE(Type(type)) {
        }
        virtual ~Entity() {
        }
        virtual bool update() = 0;
        virtual void draw(const double dt) = 0;
        virtual b2Body* getBody() = 0;
        virtual int getHp() = 0;
        virtual void setHp(int hp, int entity) = 0;
    protected:
        World& m_world;
        b2Body* m_body;
        int m_hp;
    };
}

#endif