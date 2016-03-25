#ifndef ENEMY_H
#define ENEMY_H

#include "IEntity.h"

namespace bb {
    class World;

    class Enemy : public IEntity{
    public:
        Enemy(World& world);
        void update();
        void draw(sf::RenderWindow& window, const double dt);
        sf::Vector2f getCoord();
        void setCoord(float x, float y);
    private:
        sf::RectangleShape m_sprite;
    };
}

#endif