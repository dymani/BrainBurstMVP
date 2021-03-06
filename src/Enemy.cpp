#include "Enemy.h"
#include "World.h"

namespace bb {
    Enemy::Enemy(World& world, int id, float coordX, float coordY, int type) : Entity(world, id,
        Entity::ENEMY), m_type(EnemyType(type)) {
        m_sprite.setSize({64.0f, 64.0f});
        m_sprite.setOrigin({32.0f, 32.0f});
        switch(m_type) {
            case T_FRIENDLY:
                m_colour = sf::Color::Yellow;
                m_sprite.setFillColor(sf::Color::Yellow);
                break;
            case T_PASSIVE:
                m_colour = sf::Color(255, 128, 0);
                break;
            case T_HOSTILE:
                m_colour = sf::Color::Red;
                break;
        }
        m_sprite.setFillColor(m_colour);

        b2BodyDef bodyDef;
        b2PolygonShape dynamicBox;
        b2FixtureDef fixtureDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(coordX, coordY);
        bodyDef.fixedRotation = true;
        m_body = m_world.getBWorld().CreateBody(&bodyDef);

        dynamicBox.SetAsBox(0.5f, 0.5f);

        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 30.0f;
        fixtureDef.friction = 0.3f;
        m_body->CreateFixture(&fixtureDef);

        dynamicBox.SetAsBox(0.49f, 0.05f, b2Vec2(0, -0.5f), 0);
        fixtureDef.isSensor = true;
        b2Fixture* footSensorFixture = m_body->CreateFixture(&fixtureDef);
        auto* data = new EntityData;
        data->id = ID;
        data->subId = 1;
        footSensorFixture->SetUserData(data);

        dynamicBox.SetAsBox(0.55f, 0.55f, b2Vec2(0, 0), 0);
        fixtureDef.shape = &dynamicBox;
        auto* sensorFixture = m_body->CreateFixture(&fixtureDef);
        data = new EntityData;
        data->id = ID;
        data->subId = 2;
        sensorFixture->SetUserData(data);

        dynamicBox.SetAsBox(0.05f, 0.4f, b2Vec2(-0.5, 0), 0);
        fixtureDef.shape = &dynamicBox;
        sensorFixture = m_body->CreateFixture(&fixtureDef);
        data = new EntityData;
        data->id = ID;
        data->subId = 3;
        sensorFixture->SetUserData(data);

        dynamicBox.SetAsBox(0.05f, 0.4f, b2Vec2(0.5, 0), 0);
        fixtureDef.shape = &dynamicBox;
        sensorFixture = m_body->CreateFixture(&fixtureDef);
        data = new EntityData;
        data->id = ID;
        data->subId = 4;
        sensorFixture->SetUserData(data);

        switch(m_type) {
            case T_FRIENDLY:
            case T_PASSIVE:
                m_moveState = MoveState(rand() % 3);
                m_moveTime = rand() % 100 + 150;
                break;
            case T_HOSTILE:
                break;
        }

        m_contactListener = std::unique_ptr<EnemyContactListener>(new EnemyContactListener(*this));
        m_contactListenerId = m_world.getContactListener()->addListener(m_contactListener.get());

        m_numFootContacts = 0;
        m_hasHit = false;
        m_needJump = false;
        m_jumpTimeout = 0;
        m_aggro = 0;
        m_aggroTimeout = 0;
        m_hp = 50;
        m_hitTimeout = 0;
        m_killedBy = -1;
    }

    Enemy::~Enemy() {
        m_world.getContactListener()->removeListener(m_contactListenerId);
    }

    bool Enemy::update() {
        if(m_type == T_FRIENDLY || m_type == T_PASSIVE) {
            auto playerPos = m_world.getEntity(0)->getBody()->GetPosition();
            auto vel = m_body->GetLinearVelocity();
            float desiredVelX = 0.0f;
            float desiredVelY = vel.y;
            m_moveTime--;
            bool jump = false;
            if(m_type == T_FRIENDLY || (m_type == T_PASSIVE && m_aggro == 0)) {
                switch(m_moveState) {
                    case MS_LEFT:
                        desiredVelX = -1.0f;
                        break;
                    case MS_RIGHT:
                        desiredVelX = 1.0f;
                        break;
                }
                if(m_needJump) {
                    if(m_hitLeft)
                        m_moveState = MS_RIGHT;
                    else
                        m_moveState = MS_LEFT;
                    m_moveTime = rand() % 50 + 150;
                }
                if(m_moveTime <= 0) {
                    int newState = rand() % 3;
                    if(int(m_moveState) == newState)
                        newState = (newState + rand() % 2 + 1) % 3;
                    m_moveState = MoveState(newState);
                    m_moveTime = rand() % 50 + 150;
                }
            } else {
                m_aggroTimeout--;
                desiredVelX = playerPos.x > m_body->GetPosition().x ? float(2 * m_aggro) : float(-2 * m_aggro);
                float xDiff = m_body->GetPosition().x - playerPos.x;
                if(xDiff < 0.5f && xDiff > -0.5f)
                    desiredVelX = 0.0f;
                jump = m_needJump || desiredVelX == 0.0f;
                m_hitTimeout--;
                if(m_hasHit)
                    if(m_hitTimeout < 0) {
                        m_world.damage(ID, 0, m_aggro);
                        m_hitTimeout = 10;
                        m_aggroTimeout -= 10;
                    }
            }
            m_jumpTimeout--;
            int velY = int(vel.y * 1000 + 0.5);
            if(jump && m_numFootContacts >= 1 && m_jumpTimeout < 0 && velY == 0) {
                desiredVelY = 10.0f;
                m_jumpTimeout = 10;
            }
            float velXChange = desiredVelX - vel.x;
            float velYChange = desiredVelY - vel.y;
            float impulseX = m_body->GetMass() * velXChange;
            float impulseY = m_body->GetMass() * velYChange;
            m_body->ApplyLinearImpulse(b2Vec2(impulseX, impulseY), m_body->GetWorldCenter(), true);
            if(m_aggroTimeout <= 0)
                m_aggro = 0;
        } else if(m_type == T_HOSTILE) {
            m_aggroTimeout--;
            auto playerPos = m_world.getEntity(0)->getBody()->GetPosition();
            auto vel = m_body->GetLinearVelocity();
            float desiredVelX = playerPos.x > m_body->GetPosition().x ? 3.0f : -3.0f;
            float desiredVelY = vel.y;
            float xDiff = m_body->GetPosition().x - playerPos.x;
            if(xDiff < 0.5f && xDiff > -0.5f)
                desiredVelX = 0.0f;
            m_jumpTimeout--;
            int velY = int(vel.y * 1000 + 0.5);
            if((m_needJump || desiredVelX == 0.0f) && m_numFootContacts >= 1 && m_jumpTimeout < 0
                && velY == 0) {
                desiredVelY = 10.0f;
                m_jumpTimeout = 10;
            }
            m_world.getDebug().addLine("Aggro: " + std::to_string(m_aggro) + " " + std::to_string(m_aggroTimeout));
            m_hitTimeout--;
            if(m_hasHit)
                if(m_hitTimeout < 0) {
                    m_world.damage(ID, 0, 1 + m_aggro);
                    m_hitTimeout = 10;
                }
            float impulseX = m_body->GetMass() * (desiredVelX - vel.x);
            float impulseY = m_body->GetMass() * (desiredVelY - vel.y);
            m_body->ApplyLinearImpulse(b2Vec2(impulseX, impulseY), m_body->GetWorldCenter(), true);
            if(m_aggroTimeout <= 0)
                m_aggro = 0;
        }
        if(m_hp <= 0) {
            if(m_killedBy == 0) {
                auto* player = dynamic_cast<Player*>(m_world.getEntity(0));
                player->setBp(player->getBp() + 10);
            }
            return false;
        }
        return true;
    }

    void Enemy::draw(const double dt) {
        m_sprite.setPosition({64 * m_body->GetPosition().x, 540 - 64 * m_body->GetPosition().y});
        m_world.getWindow().draw(m_sprite);
    }

    b2Body* Enemy::getBody() {
        return m_body;
    }

    int Enemy::getHp() {
        return m_hp;
    }

    void Enemy::setHp(int hp, int entity) {
        if(m_type != T_FRIENDLY && hp < m_hp && entity == 0) {
            m_aggro = m_aggro >= 3 ? 3 : m_aggro + 1;
            m_aggroTimeout = rand() % 100 + 150;
        }
        m_hp = hp;
        m_sprite.setFillColor(sf::Color(m_colour.r * hp / 50, m_colour.g * hp / 50, m_colour.b * hp / 50));
        if(m_hp <= 0) m_killedBy = entity;
    }

    EnemyContactListener::EnemyContactListener(Enemy& enemy) : m_enemy(enemy) {
    }

    void EnemyContactListener::beginContact(EntityData* a, EntityData* b) {
        if(a->id == m_enemy.ID) {
            if(a->subId == 1) {
                m_enemy.m_numFootContacts++;
                return;
            } else if(a->subId == 2) {
                if(b->id == 0) {
                    m_enemy.m_hasHit = true;
                    return;
                }
            } else if(a->subId == 3 || a->subId == 4) {
                if(m_enemy.m_type == Enemy::T_PASSIVE && b->id == 0 && m_enemy.m_aggro == 0) {
                    m_enemy.m_aggro = 1;
                    m_enemy.m_aggroTimeout = rand() % 300 + 500;
                }
                m_enemy.m_hitLeft = a->subId == 3;
                if(b->id != 0)
                    m_enemy.m_needJump = true;
                return;
            }
        }
        if(b->id == m_enemy.ID) {
            if(b->subId == 1) {
                m_enemy.m_numFootContacts++;
                return;
            } else if(b->subId == 2) {
                if(a->id == 0) {
                    m_enemy.m_hasHit = true;
                    return;
                }
            } else if(b->subId == 3 || b->subId == 4) {
                if(m_enemy.m_type == Enemy::T_PASSIVE && a->id == 0 && m_enemy.m_aggro == 0) {
                    m_enemy.m_aggro = 1;
                    m_enemy.m_aggroTimeout = rand() % 300 + 500;
                }
                m_enemy.m_hitLeft = b->subId == 3;
                if(b->id != 0)
                    m_enemy.m_needJump = true;
                return;
            }
        }
    }

    void EnemyContactListener::endContact(EntityData* a, EntityData* b) {
        if(a->id == m_enemy.ID) {
            if(a->subId == 1) {
                m_enemy.m_numFootContacts--;
                return;
            } else if(a->subId == 2) {
                if(b->id == 0) {
                    m_enemy.m_hasHit = false;
                    return;
                }
            } else if(a->subId == 3 || a->subId == 4) {
                if(b->id != 0)
                    m_enemy.m_needJump = false;
                return;
            }
        }
        if(b->id == m_enemy.ID) {
            if(b->subId == 1) {
                m_enemy.m_numFootContacts--;
                return;
            } else if(b->subId == 2) {
                if(a->id == 0) {
                    m_enemy.m_hasHit = false;
                    return;
                }
            } else if(b->subId == 3 || b->subId == 4) {
                if(b->id != 0)
                    m_enemy.m_needJump = false;
                return;
            }
        }
    }
}