#include "World.h"

namespace bb {
    World::World() {
        m_font.loadFromFile("assets/system.otf");
        m_debug.init(m_font);
        m_player = new Player(*this);
        m_player->setCoord(5, 0);

        auto* enemy = new Enemy(*this);
        enemy->setCoord(7, 0);
        m_entities[0] = enemy;
    }

    void World::handleInput(sf::Event event) {
        m_player->handleInput(event);
        if(event.type == sf::Event::KeyPressed) {
            switch(event.key.code) {
                case sf::Keyboard::F3:
                    m_debugShown = !m_debugShown;
                    break;
            }
        }
    }

    void World::handleInput() {
        m_player->handleInput();
    }

    bool World::update() {
        m_debug.reset();
        m_player->update();
        for(auto entity : m_entities) {
            entity.second->update();
        }
        return true;
    }

    void World::draw(sf::RenderWindow& window, const double dt) {
        m_player->draw(window, dt);
        for(auto entity : m_entities) {
            entity.second->draw(window, dt);
        }
        if(m_debugShown)
            m_debug.draw(window);
    }

    Debug& World::getDebug() {
        return m_debug;
    }
}