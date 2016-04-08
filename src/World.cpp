#include "World.h"
#include "Game.h"

namespace bb {
    World::World(Game& game) : m_game(game), m_bWorld(b2Vec2(0.0f, -20.0f)) {
        m_font.loadFromFile("assets/system.otf");
        m_debug.init(m_font);

        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(0.0f, -1.0f);
        m_ground = m_bWorld.CreateBody(&groundBodyDef);

        b2PolygonShape groundBox;
        groundBox.SetAsBox(20.0f, 1.0F);
        m_ground->CreateFixture(&groundBox, 0.0f);

        m_player = new Player(*this);
        m_entities[0] = m_player;

        Enemy* enemy = new Enemy(*this, 5.0f, 2.5f, 0);
        m_entities[1] = enemy;
        enemy = new Enemy(*this, 5.0f, 3.5f, 0);
        m_entities[2] = enemy;
        enemy = new Enemy(*this, 8.0f, 2.5f, 1);
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
        for(auto entity = m_entities.cbegin(); entity != m_entities.cend();) {
            if(entity->second->update()) {
                ++entity;
            } else {
                m_bWorld.DestroyBody(entity->second->getBody());
                m_entities.erase(entity++);
            }
        }
        for(auto damage : m_damages) {
            if(m_entities.count(damage.first) > 0)
                m_entities[damage.first]->setHp(m_entities[damage.first]->getHp() - damage.second);
        }
        m_damages.clear();
        m_bWorld.Step(timeStep, velocityIterations, positionIterations);
        m_bWorld.ClearForces();
        return true;
    }

    void World::draw(const double dt) {
        for(auto& entity : m_entities) {
            entity.second->draw(dt);
        }
        m_debug.draw(m_game.getWindow());
    }

    sf::RenderWindow& World::getWindow() {
        return m_game.getWindow();
    }

    Debug& World::getDebug() {
        return m_debug;
    }

    b2World& World::getBWorld() {
        return m_bWorld;
    }

    void World::damage(int id, int damage) {
        m_damages.push_back({id, damage});
    }

    sf::Vector2f World::mapPixelToCoord(sf::Vector2i pixel) {
        return{pixel.x / 64.0f, (540 - pixel.y) / 64.0f};
    }

    int World::seekEntity(sf::Vector2f coord) {
        for(auto& entity : m_entities) {
            for(auto* f = entity.second->getBody()->GetFixtureList(); f; f = f->GetNext()) {
                if(f->TestPoint(b2Vec2(coord.x, coord.y)))
                    return entity.first;
            }
        }
        return -1;
    }
}