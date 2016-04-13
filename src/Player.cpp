#include "Player.h"
#include "World.h"
#include "BasicAbility.h"

namespace bb {
    Player::Player(World& world) : Entity(world) {
        m_sprite.setFillColor(sf::Color::White);
        m_sprite.setSize({64.0f, 64.0f});
        m_sprite.setOrigin({32.0f, 32.0f});

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(5.0f, 5.0f);
        bodyDef.fixedRotation = true;
        m_body = m_world.getBWorld().CreateBody(&bodyDef);

        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(0.5f, 0.5f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 50.0f;
        fixtureDef.friction = 0.3f;
        m_body->CreateFixture(&fixtureDef);

        dynamicBox.SetAsBox(0.4f, 0.05f, b2Vec2(0, -0.5f), 0);
        fixtureDef.isSensor = true;
        b2Fixture* footSensorFixture = m_body->CreateFixture(&fixtureDef);
        footSensorFixture->SetUserData((void*)3);

        m_contactListener = new PlayerContactListener(*this);
        m_world.getBWorld().SetContactListener(m_contactListener);

        m_jumpState = JS_STOP;
        m_jumpTimeout = 0;
        m_isSprinting = false;
        m_isDodging = false;

        m_hp = -1;
        m_abilityState = AS_NONE;
        m_ability = -1;
        m_abilities.push_back(new BasicAbility(m_world, sf::Keyboard::Num1));
    }

    void Player::handleInput() {
        bool keyLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        bool keyRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        bool keyDodge = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
        bool keySprint = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
        m_isDodging = keyDodge;
        m_isSprinting = keySprint;
        m_moveState = MS_STOP;
        if(keyLeft && !keyRight)
            m_moveState = MS_LEFT;
        else if(keyRight && !keyLeft)
            m_moveState = MS_RIGHT;
        else
            m_isSprinting = false;

        if(m_abilityState == AS_HOLD) {
            if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                m_abilityState = AS_NONE;
                m_ability = -1;
                m_abilityHold = 0;
            }
        } else if(m_abilityState == AS_COUNT) {
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                m_abilityState = AS_HOLD;
                m_abilityCount = 0;
                m_abilityHold = m_abilities[m_ability]->getHold();
            }
        }
    }

    void Player::handleInput(sf::Event event) {
        if(event.type == sf::Event::KeyPressed) {
            switch(event.key.code) {
                case sf::Keyboard::W:
                    if(m_body->GetLinearVelocity().y == 0) {
                        m_jumpState = JS_JUMP;
                    } else {
                        if(m_jumpState == JS_STOP)
                            m_jumpState = JS_DOUBLE_JUMP;
                    }
                    break;
            }
            if(m_abilityState == AS_NONE) {
                for(auto a : m_abilities) {
                    if(event.key.code == a->getKey()) {
                        m_ability = std::find(m_abilities.begin(), m_abilities.end(), a) - m_abilities.begin();
                        m_abilityState = AS_COUNT;
                        m_abilityCount = -50;
                        break;
                    }
                }
            }
        } else if(event.type == sf::Event::MouseButtonPressed) {
            if(event.mouseButton.button == sf::Mouse::Left) {
                auto coord = m_world.mapPixelToCoord(sf::Mouse::getPosition(m_world.getWindow()));
                if(m_abilityState == AS_NONE) {
                    int entity = m_world.seekEntity(coord);
                    double distance = std::sqrt(double(
                        (coord.x - m_body->GetPosition().x) * (coord.x - m_body->GetPosition().x) +
                        (coord.y - m_body->GetPosition().y) * (coord.y - m_body->GetPosition().y)));
                    if(distance < 5)
                        m_world.damage(entity, 1);
                } else if(m_abilityState == AS_USE) {
                    m_abilities[m_ability]->use(this, coord);
                    std::cout << m_ability << std::endl;
                    m_abilityState = AS_NONE;
                    m_ability = -1;
                    m_abilityCount = 0;
                    m_abilityHold = 0;
                    m_abilityTimeout = 0;
                }
            }
        }
    }

    bool Player::update() {
        m_jumpTimeout--;
        auto& debug = m_world.getDebug();
        auto coord = m_body->GetPosition();
        auto vel = m_body->GetLinearVelocity();
        float desiredVelX = 0;
        float desiredVelY = vel.y;
        float desiredSpeed = 5.0f;
        if(m_moveState == MS_STOP && m_numFootContacts >= 1) {
            m_sprintDuration++;
            m_sprintDuration = m_sprintDuration > 10 ? 10 : m_sprintDuration;
        }
        if(m_isDodging) {
            desiredSpeed = 1.0f;
        } else {
            if(m_isSprinting) {
                m_sprintDuration--;
                m_sprintDuration = m_sprintDuration < -10 ? -10 : m_sprintDuration;
                if(m_sprintDuration > 0) {
                    desiredSpeed = 30.0f;
                    if(vel.y != 0)
                        m_body->ApplyForce(m_body->GetMass() * -m_world.getBWorld().GetGravity(),
                            m_body->GetWorldCenter(), true);
                }
            }
        }
        switch(m_moveState) {
            case MS_LEFT:  desiredVelX = -desiredSpeed; break;
            case MS_STOP:  desiredVelX = 0; break;
            case MS_RIGHT: desiredVelX = desiredSpeed; break;
        }
        if(m_jumpState == JS_JUMP) {
            m_jumpState = JS_STOP;
            if(m_numFootContacts >= 1) {
                if(m_jumpTimeout <= 0) {
                    if(!m_isDodging) {
                        desiredVelY = 10.0f;
                        m_jumpTimeout = 15;
                    }
                }
            }
        } else if(m_jumpState == JS_DOUBLE_JUMP) {
            m_jumpState = JS_DOUBLE_STOP;
            if(!m_isDodging) {
                desiredVelY = 10.0f;
            }
        } else if(m_jumpState == JS_DOUBLE_STOP) {
            if(m_numFootContacts >= 1) m_jumpState = JS_STOP;
        }
        float velXChange = desiredVelX - vel.x;
        float velYChange = desiredVelY - vel.y;
        float impulseX = m_body->GetMass() * velXChange;
        float impulseY = m_body->GetMass() * velYChange;
        m_body->ApplyLinearImpulse(b2Vec2(impulseX, impulseY), m_body->GetWorldCenter(), true);

        switch(m_abilityState) {
            case AS_COUNT:
                m_abilityCount++;
                if(m_abilityCount >= 0) {
                    m_abilityState = AS_NONE;
                    m_ability = -1;
                    m_abilityCount = 0;
                }
                break;
            case AS_HOLD:
                m_abilityHold++;
                if(m_abilityHold >= 0) {
                    m_abilityState = AS_USE;
                    m_abilityTimeout = m_abilities[m_ability]->getTimeout();
                    m_abilityHold = 0;
                }
                break;
            case AS_USE:
                m_abilityTimeout++;
                if(m_abilityTimeout >= 0) {
                    m_abilityState = AS_NONE;
                    m_ability = -1;
                    m_abilityTimeout = 0;
                }
                break;
        }

        debug.addLine("Coord:    " + std::to_string(coord.x) + " " + std::to_string(coord.y));
        debug.addLine("Velocity: " + std::to_string(vel.x) + " " + std::to_string(vel.y));
        debug.addLine("Dodge:    " + std::string(m_isDodging ? "True" : "False"));
        switch(m_jumpState) {
            case JS_JUMP:
                debug.addLine("Jump:     0 OK");
                break;
            case JS_DOUBLE_JUMP:
                debug.addLine("Jump:     1 OK");
                break;
            case JS_STOP:
                debug.addLine("Jump:     2 OK");
                break;
            case JS_DOUBLE_STOP:
                debug.addLine("Jump:     3 Not OK");
                break;

        }
        debug.addLine("Sprint:    " + std::to_string(m_sprintDuration));
        debug.addLine("Ability:   " + std::to_string(int(m_abilityState)) + " (" + std::to_string(m_ability)
            + " " + std::to_string(m_abilityCount) + " " + std::to_string(m_abilityHold)
            + " " + std::to_string(m_abilityTimeout) + ")");
        return true;
    }

    void Player::draw(const double dt) {
        auto coord = m_body->GetPosition();
        m_sprite.setPosition({64 * coord.x, 540 - 64 * coord.y});
        m_world.getWindow().draw(m_sprite);
    }

    b2Body* Player::getBody() {
        return m_body;
    }

    int Player::getHp() {
        return m_hp;
    }

    void Player::setHp(int hp) {
        m_hp = hp;
    }

    PlayerContactListener::PlayerContactListener(Player& player) : m_player(player) {
    }

    void PlayerContactListener::BeginContact(b2Contact* contact) {
        void* fixtureUserData = contact->GetFixtureA()->GetUserData();
        if((int)fixtureUserData == 3)
            m_player.m_numFootContacts++;
        fixtureUserData = contact->GetFixtureB()->GetUserData();
        if((int)fixtureUserData == 3)
            m_player.m_numFootContacts++;
    }

    void PlayerContactListener::EndContact(b2Contact* contact) {
        void* fixtureUserData = contact->GetFixtureA()->GetUserData();
        if((int)fixtureUserData == 3)
            m_player.m_numFootContacts--;
        fixtureUserData = contact->GetFixtureB()->GetUserData();
        if((int)fixtureUserData == 3)
            m_player.m_numFootContacts--;
    }
}