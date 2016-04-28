#include "Player.h"
#include "World.h"
#include "SkillHit.h"
#include "SkillProjectile.h"

namespace bb {
    Player::Player(World& world, int id) : Entity(world, id, Entity::PLAYER) {
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
        auto* playerFix = m_body->CreateFixture(&fixtureDef);
        EntityData* data = new EntityData;
        data->id = ID;
        data->subId = 0;
        playerFix->SetUserData(data);

        dynamicBox.SetAsBox(0.49f, 0.05f, b2Vec2(0, -0.5f), 0);
        fixtureDef.isSensor = true;
        b2Fixture* footSensorFixture = m_body->CreateFixture(&fixtureDef);
        data = new EntityData;
        data->id = ID;
        data->subId = 1;
        footSensorFixture->SetUserData(data);

        m_contactListener = std::unique_ptr<PlayerContactListener>(new PlayerContactListener(*this));
        m_contactListenerId = m_world.getContactListener()->addListener(m_contactListener.get());

        m_jumpState = JS_STOP;
        m_jumpTimeout = 0;
        m_isSprinting = false;
        m_isDodging = false;
        m_numFootContacts = 0;

        m_hp = 100;
        m_sp = 0;
        m_skillState = SS_NONE;
        m_skill = -1;
        m_skills.push_back(new SkillHit(m_world, sf::Keyboard::Num1));
        m_skills.push_back(new SkillProjectile(m_world, sf::Keyboard::Num2));
        m_killedBy = -1;
    }

    Player::~Player() {
        m_world.getContactListener()->removeListener(m_contactListenerId);
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

        if(m_skillState == SS_HOLD) {
            if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                m_skillState = SS_NONE;
                m_skill = -1;
                m_skillHold = 0;
            }
        } else if(m_skillState == SS_COUNT) {
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                m_skillState = SS_HOLD;
                m_skillCount = 0;
                m_skillHold = m_skills[m_skill]->getHold();
            }
        }
    }

    void Player::handleInput(sf::Event event) {
        if(event.type == sf::Event::KeyPressed) {
            switch(event.key.code) {
                case sf::Keyboard::W:
                    if(m_jumpState == JS_STOP) {
                        if(m_numFootContacts >= 1) {
                            m_jumpState = JS_JUMP;
                        } else {
                            m_jumpState = JS_D_JUMP;
                        }
                    } else if(m_jumpState == JS_JUMPED) {
                        m_jumpState = JS_D_JUMP;
                    }
                    break;
                default:
                    if(m_skillState == SS_NONE) {
                        for(auto s : m_skills) {
                            if(event.key.code == s->getKey() && m_sp >= s->getSp()) {
                                m_skill = std::find(m_skills.begin(), m_skills.end(), s)
                                    - m_skills.begin();
                                m_skillState = SS_COUNT;
                                m_skillCount = -50;
                                break;
                            }
                        }
                    }
                    break;
            }
        } else if(event.type == sf::Event::MouseButtonReleased) {
            if(event.mouseButton.button == sf::Mouse::Left) {
                auto coord = m_world.mapPixelToCoord(sf::Mouse::getPosition(m_world.getWindow()));
                if(m_skillState == SS_NONE) {
                    int entity = m_world.seekEntity(coord);
                    if(entity != -1 && entity != ID) {
                        double distance = std::sqrt(double(
                            (coord.x - m_body->GetPosition().x) * (coord.x - m_body->GetPosition().x) +
                            (coord.y - m_body->GetPosition().y) * (coord.y - m_body->GetPosition().y)));
                        if(distance < 2) {
                            m_world.damage(ID, entity, 10);
                            m_sp += 2;
                            m_sp = m_sp > 100 ? 100 : m_sp;
                        }
                    }
                } else if(m_skillState == SS_USE) {
                    if(m_sp > m_skills[m_skill]->getSp()) {
                        m_sp -= m_skills[m_skill]->getSp();
                        m_skills[m_skill]->use(this, coord);
                        m_skillState = SS_NONE;
                        m_skill = -1;
                        m_skillCount = 0;
                        m_skillHold = 0;
                        m_skillTimeout = 0;
                    }
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
            if(m_jumpTimeout <= 0) {
                if(!m_isDodging) {
                    desiredVelY = 10.0f;
                    m_jumpTimeout = 10;
                    m_jumpState = JS_JUMPED;
                }
            }
        } else if(m_jumpState == JS_D_JUMP) {
            m_jumpState = JS_JUMPED;
            if(!m_isDodging) {
                if(m_jumpTimeout <= 0) {
                    desiredVelY = 10.0f;
                    m_jumpState = JS_D_JUMPED;
                }
            }
        } else if(m_jumpState == JS_JUMPED || m_jumpState == JS_D_JUMPED) {
            int velY = int(m_body->GetLinearVelocity().y * 1000 + 0.5);
            if(m_numFootContacts >= 1 && velY == 0) m_jumpState = JS_STOP;
        }
        float velXChange = desiredVelX - vel.x;
        float velYChange = desiredVelY - vel.y;
        float impulseX = m_body->GetMass() * velXChange;
        float impulseY = m_body->GetMass() * velYChange;
        m_body->ApplyLinearImpulse(b2Vec2(impulseX, impulseY), m_body->GetWorldCenter(), true);

        switch(m_skillState) {
            case SS_COUNT:
                m_skillCount++;
                if(m_skillCount >= 0) {
                    m_skillState = SS_NONE;
                    m_skill = -1;
                    m_skillCount = 0;
                }
                break;
            case SS_HOLD:
                m_skillHold++;
                if(m_skillHold >= 0) {
                    m_skillState = SS_USE;
                    m_skillTimeout = m_skills[m_skill]->getTimeout();
                    m_skillHold = 0;
                }
                break;
            case SS_USE:
                m_skillTimeout++;
                if(m_skillTimeout >= 0) {
                    m_skillState = SS_NONE;
                    m_skill = -1;
                    m_skillTimeout = 0;
                }
                break;
        }

        debug.addLine("Coord:    " + std::to_string(coord.x) + " " + std::to_string(coord.y));
        debug.addLine("Velocity: " + std::to_string(vel.x) + " " + std::to_string(vel.y));
        debug.addLine("Dodge:    " + std::string(m_isDodging ? "True" : "False"));
        debug.addLine("Jump:     " + std::to_string(m_jumpState) + " " + std::string(m_jumpState == 4
            || m_jumpTimeout > 0 ? "Not OK" : "OK"));
        debug.addLine("Sprint:    " + std::to_string(m_sprintDuration));
        debug.addLine("Ability:   " + std::to_string(int(m_skillState)) + " ("
            + std::to_string(m_skill) + " " + std::to_string(m_skillCount) + " "
            + std::to_string(m_skillHold) + " " + std::to_string(m_skillTimeout) + ")");
        debug.addLine("Foots:     " + std::to_string(m_numFootContacts));
        if(m_hp <= 0) {
            if(m_killedBy == -1) {
                m_bp -= 10;
            } else {
                switch(m_world.getEntity(m_killedBy)->TYPE) {
                    case Entity::PLAYER: m_bp -= 10; break;
                    case Entity::OBJECT: m_bp -= 10; break;
                    case Entity::ENEMY: m_bp -= 10; break;
                }
            }
            return false;
        }
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

    void Player::setHp(int hp, int entity) {
        if(hp < m_hp) {
            m_sp += (m_hp - hp) * 2;
            m_sp = m_sp > 100 ? 100 : m_sp;
        }
        m_hp = hp;
        if(m_hp <= 0)
            m_killedBy = entity;
    }

    int Player::getSp() {
        return m_sp;
    }

    void Player::setBp(int bp) {
        m_bp = bp;
    }

    int Player::getBp() {
        return m_bp;
    }

    int Player::getJumpState() {
        return int(m_jumpState);
    }

    int Player::getSprintDuration() {
        return m_sprintDuration;
    }

    int Player::getSkillState() {
        return m_skillState;
    }

    int Player::getSkill() {
        return m_skill;
    }

    int Player::getSkillCount() {
        return m_skillCount;
    }

    int Player::getSkillHold() {
        return m_skillHold;
    }

    int Player::getSkillTimeout() {
        return m_skillTimeout;
    }

    std::vector<Skill*>& Player::getSkills() {
        return m_skills;
    }

    PlayerContactListener::PlayerContactListener(Player& player) : m_player(player) {
    }

    void PlayerContactListener::beginContact(EntityData* a, EntityData* b) {
        if(a->id == m_player.ID) {
            if(a->subId == 1)
                m_player.m_numFootContacts++;
        } else if(b->id == m_player.ID) {
            if(b->subId == 1)
                m_player.m_numFootContacts++;
        }
    }

    void PlayerContactListener::endContact(EntityData* a, EntityData* b) {
        if(a->id == m_player.ID) {
            if(a->subId == 1)
                m_player.m_numFootContacts--;
        } else if(b->id == m_player.ID) {
            if(b->subId == 1)
                m_player.m_numFootContacts--;
        }
    }
}