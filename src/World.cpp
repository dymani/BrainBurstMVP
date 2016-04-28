#include "World.h"
#include "Game.h"

namespace bb {
    World::World(Game& game) : m_game(game), m_bWorld(b2Vec2(0.0f, -20.0f)), m_gui(*this),
        m_view({960, 540}, {7.5f, 4.2f}) {
        srand(unsigned int(time(NULL)));

        m_paused = false;

        m_font.loadFromFile("assets/system.otf");
        m_debug.init(m_font);
        m_gui.setFont(m_font);

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
        borderBodyDef.position.Set(0.0f, 20.0f);
        m_borderU = m_bWorld.CreateBody(&borderBodyDef);

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

        Entity* entity;
        entity = new Object(*this, getNewId(), 8.0f, 0.5f);
        addEntity(entity);
        entity = new Object(*this, getNewId(), 5.0f, 2.5f);
        addEntity(entity);
        entity = new Object(*this, getNewId(), 8.0f, 4.5f);
        addEntity(entity);
        entity = new Object(*this, getNewId(), 5.0f, 6.5f);
        addEntity(entity);
        entity = new Object(*this, getNewId(), 8.0f, 8.5f);
        addEntity(entity);
        entity = new Object(*this, getNewId(), 13.0f, 0.5f);
        addEntity(entity);
        entity = new Enemy(*this, getNewId(), 3.0f, 2.5f, 0);
        addEntity(entity);
        entity = new Enemy(*this, getNewId(), 10.0f, 2.5f, 2);
        addEntity(entity);
        entity = new Enemy(*this, getNewId(), 12.0f, 2.5f, 0);
        addEntity(entity);
        entity = new Enemy(*this, getNewId(), 15.0f, 2.5f, 1);
        addEntity(entity);
        entity = new Enemy(*this, getNewId(), 17.0f, 2.5f, 1);
        addEntity(entity);
        entity = new Enemy(*this, getNewId(), 19.0f, 2.5f, 1);
        addEntity(entity);
    }

    void World::handleInput(sf::Event event) {
        m_player->handleInput(event);
        m_gui.handleInput(event);
        if(event.type == sf::Event::KeyPressed) {
            switch(event.key.code) {
                case sf::Keyboard::F3:
                    m_debug.toggle();
                    break;
                case sf::Keyboard::R:
                    if(!sf::Mouse::isButtonPressed(sf::Mouse::Right))
                        m_restart = true;
                    break;
                case sf::Keyboard::P:
                    if(!sf::Mouse::isButtonPressed(sf::Mouse::Right))
                        togglePause();
                    break;
            }
        }
    }

    void World::handleInput() {
        m_player->handleInput();
        m_gui.handleInput();
    }

    int World::update() {
        if(!m_paused) {
            m_debug.reset();
            for(auto entity = m_entities.cbegin(); entity != m_entities.cend();) {
                if(entity->second->update()) {
                    ++entity;
                } else {
                    if(entity->first == 0) {
                        return 1;
                    } else {
                        m_bWorld.DestroyBody(entity->second->getBody());
                        m_entities.erase(entity++);
                    }
                }
            }
            for(auto damage : m_damages) {
                if(m_entities.count(damage.y) > 0)
                    m_entities[damage.y]->setHp(m_entities[damage.y]->getHp() - damage.z, damage.x);
            }
            m_damages.clear();
            m_bWorld.Step(timeStep, velocityIterations, positionIterations);
            m_bWorld.ClearForces();
        }
        m_gui.update();
        auto playerPos = m_player->getBody()->GetPosition();
        sf::Vector2f player = {playerPos.x, playerPos.y};
        sf::Vector2f view = m_view.getCoord();
        float viewXDest = view.x;
        float viewMaxVel, viewVel = 0.0f;
        if(player.x - view.x > 5.0f)
            viewXDest = player.x - 5.0f;
        else if(view.x - player.x > 5.0f)
            viewXDest = player.x + 5.0f;
        if(viewXDest > view.x) {
            viewMaxVel = 1.0f * (viewXDest - view.x);
            viewVel += 0.5F;
            viewVel = viewVel > viewMaxVel ? viewMaxVel : viewVel;
        } else if(viewXDest < view.x) {
            viewMaxVel = 1.0f * (viewXDest - view.x);
            viewVel -= 0.5F;
            viewVel = viewVel < viewMaxVel ? viewMaxVel : viewVel;
        }
        view.x += viewVel;
        if(view.x < 7.5f) view.x = 7.5f;
        if(view.x > 12.5f) view.x = 12.5f;
        m_view.setCoord(view);
        view = m_view.getCoord();
        float viewYDest = player.y;
        viewVel = 0.0f;
        if(viewYDest < 4.2f) viewYDest = 4.2f;
        if(viewYDest > view.y) {
            viewMaxVel = 1.0f * (viewYDest - view.y);
            viewVel += 0.5F;
            viewVel = viewVel > viewMaxVel ? viewMaxVel : viewVel;
        } else if(viewYDest < view.y) {
            viewMaxVel = 1.0f * (viewYDest - view.y);
            viewVel -= 0.5F;
            viewVel = viewVel < viewMaxVel ? viewMaxVel : viewVel;
        }
        view.y += viewVel;
        if(view.y < 4.2f) view.y = 4.2f;
        m_view.setCoord(view);
        if(m_restart)
            return 1;
        return 0;
    }

    void World::draw(const double dt) {
        m_game.getWindow().setView(m_view.getView());
        for(auto& entity : m_entities) {
            entity.second->draw(dt);
        }
        m_game.getWindow().setView(m_game.getWindow().getDefaultView());
        m_gui.draw(dt);
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

    GameContactListener* World::getContactListener() {
        return m_contactListener;
    }

    Gui& World::getGui() {
        return m_gui;
    }

    View& World::getView() {
        return m_view;
    }

    void World::damage(int id, int target, int damage) {
        m_damages.push_back({id, target, damage});
    }

    sf::Vector2f World::mapPixelToCoord(sf::Vector2i pixel) {
        sf::Vector2f coord = {pixel.x / 64.0f, (540 - pixel.y) / 64.0f};
        coord.x += m_view.getCoord().x - 7.5f;
        coord.y += m_view.getCoord().y - 4.2f;
        return coord;
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

    void World::togglePause() {
        m_paused = !m_paused;
    }

    bool World::isPaused() {
        return m_paused;
    }
}