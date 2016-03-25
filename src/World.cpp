#include "World.h"

namespace bb {
    World::World() : m_physics(*this) {
        m_font.loadFromFile("assets/system.otf");
        m_debug.init(m_font);
        m_player = new Player(*this);
        m_player->setCoord(5, 0);
        m_entities[0] = m_player;
        auto* enemy = new Enemy(*this);
        enemy->setCoord(7, 4);
        m_entities[1] = enemy;

        /*enemy = new Enemy(*this);
        enemy->setCoord(7, 1);
        m_entities[2] = enemy;*/

        enemy = new Enemy(*this);
        enemy->setCoord(7, 0.71F);
        m_entities[3] = enemy;
    }

    void World::handleInput(sf::Event event) {
        m_player->handleInput(event);
        if(event.type == sf::Event::KeyPressed) {
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
        for(auto entity : m_entities) {
            entity.second->update();
        }
        m_physics.update(m_entities);
        return true;
    }

    void World::draw(sf::RenderWindow& window, const double dt) {
        m_player->draw(window, dt);
        for(auto entity : m_entities) {
            entity.second->draw(window, dt);
        }
        if(m_debug.isDebug()) m_physics.drawHitboxes(window, m_entities);
        m_debug.draw(window);
    }

    Debug& World::getDebug() {
        return m_debug;
    }
}