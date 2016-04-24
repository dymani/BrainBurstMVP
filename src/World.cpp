#include "World.h"
#include "Game.h"

namespace bb {
    World::World(Game& game) : m_game(game), m_bWorld(b2Vec2(0.0f, -20.0f)), m_gui(*this) {
        srand(unsigned int(time(NULL)));

        m_font.loadFromFile("assets/system.otf");
        m_debug.init(m_font);

        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(0.0f, -1.0f);
        m_ground = m_bWorld.CreateBody(&groundBodyDef);

        b2PolygonShape groundBox;
        groundBox.SetAsBox(20.0f, 1.0f);
        b2FixtureDef groundFix;
        groundFix.friction = 0.6f;
        groundFix.shape = &groundBox;
        groundFix.density = 0.0f;
        m_ground->CreateFixture(&groundFix);

        b2BodyDef borderBodyDef;
        borderBodyDef.position.Set(-1.0f, 0.0f);
        m_borderL = m_bWorld.CreateBody(&borderBodyDef);
        borderBodyDef.position.Set(21.0f, 0.0f);
        m_borderR = m_bWorld.CreateBody(&borderBodyDef);
        borderBodyDef.position.Set(0.0f, 11.0f);
        m_borderU= m_bWorld.CreateBody(&borderBodyDef);

        b2PolygonShape borderBox;
        borderBox.SetAsBox(1.0f, 10.0f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &borderBox;
        fixtureDef.density = 0.0f;
        fixtureDef.friction = 0.0f;
        m_borderL->CreateFixture(&fixtureDef);
        m_borderR->CreateFixture(&fixtureDef);
        fixtureDef.shape = &groundBox;
        m_borderU->CreateFixture(&fixtureDef);

        m_contactListener = new GameContactListener();
        m_bWorld.SetContactListener(m_contactListener);

        m_player = new Player(*this, 0);
        addEntity(m_player);

        Entity* enemy;
        enemy = new Enemy(*this, getNewId(), 5.0f, 2.5f, 0);
        addEntity(enemy);
        enemy = new Enemy(*this, getNewId(), 8.0f, 0.5f, 0);
        addEntity(enemy);
        enemy = new Enemy(*this, getNewId(), 8.0f, 1.5f, 0);
        addEntity(enemy);
        enemy = new Enemy(*this, getNewId(), 8.0f, 2.5f, 0);
        addEntity(enemy);
        enemy = new Enemy(*this, getNewId(), 10.0f, 2.5f, 1);
        addEntity(enemy);
    }

    void World::handleInput(sf::Event event) {
        m_player->handleInput(event);
        m_gui.handleInput(event);
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
        m_gui.handleInput();
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
        m_gui.update();
        return true;
    }

    void World::draw(const double dt) {
        for(auto& entity : m_entities) {
            entity.second->draw(dt);
        }
        m_debug.draw(m_game.getWindow());
        m_gui.draw(dt);
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

    GameContactListener* World::getContactListener() {
        return m_contactListener;
    }

    Gui& World::getGui() {
        return m_gui;
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

    Entity* World::getEntity(int id) {
        return m_entities[id].get();
    }

    int World::getNewId() {
        int id = rand() % 100;
        while(m_entities.find(id) != m_entities.end()) {
            id = rand() % 100;
        }
        return id;
    }

    void World::addEntity(Entity* entity) {
        m_entities[entity->ID] = std::unique_ptr<Entity>(entity);
    }
}