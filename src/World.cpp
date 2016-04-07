#include "World.h"

namespace bb {
    World::World() : m_bWorld(b2Vec2(0.0f, -20.0f)) {
        m_font.loadFromFile("assets/system.otf");
        m_debug.init(m_font);

        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(0.0f, -1.0f);
        m_ground = m_bWorld.CreateBody(&groundBodyDef);

        b2PolygonShape groundBox;
        groundBox.SetAsBox(20.0f, 1.0F);
        m_ground->CreateFixture(&groundBox, 0.0f);

        m_player = new Player(*this);
        m_entities.push_back(m_player);

        Enemy* enemy = new Enemy(*this, 5.0f, 0.5f);
        m_entities.push_back(enemy);

    }

    void World::handleInput(sf::Event event) {
        if(event.type == sf::Event::KeyPressed) {
            m_player->handleInput(event);
            switch(event.key.code) {
                case sf::Keyboard::F3:
                    m_debug.toggle();
                    break;
            }
        }
    }

    void World::handleInput() {
        m_player->handleInput();
    }

    bool World::update() {
        m_debug.reset();
        for(auto* entity : m_entities) {
            entity->update();
        }
        m_bWorld.Step(timeStep, velocityIterations, positionIterations);
        m_bWorld.ClearForces();
        return true;
    }

    void World::draw(sf::RenderWindow& window, const double dt) {
        for(auto* entity : m_entities) {
            entity->draw(window, dt);
        }
        m_debug.draw(window);
    }

    Debug& World::getDebug() {
        return m_debug;
    }

    b2World& World::getBWorld() {
        return m_bWorld;
    }
}