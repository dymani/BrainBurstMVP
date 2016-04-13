#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Debug.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"

namespace bb {
    class Game;

    class World {
    public:
        World(Game& game);
        void handleInput(sf::Event event);
        void handleInput();
        bool update();
        void draw(const double dt);
        sf::RenderWindow& getWindow();
        Debug& getDebug();
        b2World& getBWorld();
        void damage(int id, int damage);
        sf::Vector2f mapPixelToCoord(sf::Vector2i pixel);
        int seekEntity(sf::Vector2f coord);
        Entity* getEntity(int id);
    private:
        Game& m_game;
        float32 timeStep = 1.0F / 50.0F;
        int32 velocityIterations = 8;
        int32 positionIterations = 3;
        sf::Font m_font;
        Debug m_debug;
        b2World m_bWorld;
        Player* m_player;
        b2Body* m_ground;
        b2Body* m_borderL;
        b2Body* m_borderR;
        std::map<int, Entity*> m_entities;
        std::vector<std::pair<int, int>> m_damages;
    };

    class SeekEntityCallback : public b2QueryCallback {
    public:
        SeekEntityCallback(World& world) {
        }
        bool ReportFixture(b2Fixture* fixture) {
            m_body = fixture->GetBody();
            return false;
        }
        b2Body* getBody() {
            return m_body;
        }
        b2Body* m_body;
    };
}

#endif