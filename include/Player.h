#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

namespace bb {
    class World;

    class Player : public Entity {
    public:
        Player(World& world);
        void handleInput();
        void handleInput(sf::Event event);
        void update();
        void draw(sf::RenderWindow& window, const double dt);
        sf::Vector2f getCoord();
        void setCoord(float x, float y);
        sf::Vector2f getVelocity();
        void setVelocity(float x, float y);
        sf::FloatRect getHitbox();
    private:
        sf::RectangleShape m_sprite;
        bool m_isDodging, m_doubleJump, m_isSprinting;
        int m_sprint, m_sprintCount;
    };
}

#endif