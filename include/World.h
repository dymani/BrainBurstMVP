#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Debug.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"

namespace bb {
    class World {
    public:
        World();
        void handleInput(sf::Event event);
        void handleInput();
        bool update();
        void draw(sf::RenderWindow& window, const double dt);
        Debug& getDebug();
        b2World& getBWorld();
    private:
        sf::Font m_font;
        std::vector<Entity*> m_entities;
        Debug m_debug;
        b2World m_bWorld;
        Player* m_player;
        b2Body* m_ground;
        float32 timeStep = 1.0F / 50.0F;
        int32 velocityIterations = 8;
        int32 positionIterations = 3;
    };
}

#endif