#include "Enemy.h"
#include "World.h"

namespace bb {
    Enemy::Enemy(World& world, int id, float coordX, float coordY, int type) : Entity(world, id),
        m_type(Type(type)) {
        m_sprite.setSize({64.0f, 64.0f});
        m_sprite.setOrigin({32.0f, 32.0f});
        switch(m_type) {
            case T_STATIC:
                m_sprite.setFillColor(sf::Color::Blue);
                break;
            case T_MOBILE:
                m_sprite.setFillColor(sf::Color::Yellow);
                break;
            case T_HOSTILE:
                m_sprite.setFillColor(sf::Color::Red);
                break;
        }

        m_contactListenerId = -1;

        b2BodyDef bodyDef;
        b2PolygonShape dynamicBox;
        b2FixtureDef fixtureDef;
        switch(m_type) {
            case T_STATIC:
                bodyDef.position.Set(coordX, coordY);
                m_body = m_world.getBWorld().CreateBody(&bodyDef);

                dynamicBox.SetAsBox(0.5f, 0.5f);

                fixtureDef.shape = &dynamicBox;
                fixtureDef.density = 0.0f;
                fixtureDef.friction = 0.0f;
                m_body->CreateFixture(&fixtureDef);
                break;
            case T_MOBILE:
                m_moveState = MoveState(rand() % 3);
                m_moveTime = rand() % 100 + 150;
            case T_HOSTILE:
                bodyDef.type = b2_dynamicBody;
                bodyDef.position.Set(coordX, coordY);
                m_body = m_world.getBWorld().CreateBody(&bodyDef);

                dynamicBox.SetAsBox(0.5f, 0.5f);

                fixtureDef.shape = &dynamicBox;
                fixtureDef.density = 30.0f;
                fixtureDef.friction = 0.3f;
                m_body->CreateFixture(&fixtureDef);

                if(m_type == T_HOSTILE) {
                    dynamicBox.SetAsBox(0.4f, 0.05f, b2Vec2(0, -0.5f), 0);
                    fixtureDef.isSensor = true;
                    auto* sensorFixture = m_body->CreateFixture(&fixtureDef);
                    auto* data = new EntityData;
                    data->id = float(ID + 0.2f);
                    sensorFixture->SetUserData(data);

                    dynamicBox.SetAsBox(0.55f, 0.55f, b2Vec2(0, 0), 0);
                    fixtureDef.isSensor = true;
                    sensorFixture = m_body->CreateFixture(&fixtureDef);
                    data = new EntityData;
                    data->id = float(ID + 0.1f);
                    sensorFixture->SetUserData(data);

                    m_contactListener = std::unique_ptr<EnemyContactListener>(new
                        EnemyContactListener(*this));
                    m_contactListenerId = m_world.getContactListener()->addListener(m_contactListener
                        .get());
                    m_numFootContacts = 0;
                    m_hasHit = false;
                }
                break;
        }

        m_hp = 5;
        m_hitTimeout = 0;
    }

    Enemy::~Enemy() {
        if(m_contactListenerId != -1)
            m_world.getContactListener()->removeListener(m_contactListenerId);
    }

    bool Enemy::update() {
        if(m_type == T_MOBILE) {
            auto vel = m_body->GetLinearVelocity();
            float desiredVelX = 0.0f;
            m_moveTime--;
            switch(m_moveState) {
                case MS_LEFT:
                    desiredVelX = -1.0f;
                    break;
                case MS_RIGHT:
                    desiredVelX = 1.0f;
                    break;
            }
            if(m_moveTime <= 0 || int(vel.x * 1000 + 0.5f) == 0) {
                int newState = rand() % 3;
                if(int(m_moveState) == newState)
                    newState = (newState + rand() % 2 + 1) % 3;
                m_moveState = MoveState(newState);
                m_moveTime = rand() % 50 + 150;
            }
            float velXChange = desiredVelX - vel.x;
            float impulseX = m_body->GetMass() * velXChange;
            m_body->ApplyLinearImpulse(b2Vec2(impulseX, 0), m_body->GetWorldCenter(), true);
        } else if(m_type == T_HOSTILE) {
            auto playerPos = m_world.getEntity(0)->getBody()->GetPosition();
            auto vel = m_body->GetLinearVelocity();
            float desiredVelX = 0.0f;
            float desiredVelY = vel.y;
            if(playerPos.x > m_body->GetPosition().x) {
                desiredVelX = 3.0f;
            } else {
                desiredVelX = -3.0f;
            }
            float xDiff = m_body->GetPosition().x - playerPos.x;
            if(xDiff < 0.5f && xDiff > -0.5f)
                desiredVelX = 0.0f;
            if(int(vel.x * 1000 + 0.5f) == 0 && m_numFootContacts >= 1 && vel.y == 0.0f) {
                desiredVelY = 10.0f;
            }
            m_hitTimeout--;
            if(m_hasHit) {
                if(m_hitTimeout < 0) {
                    m_world.damage(0, 1);
                    m_hitTimeout = 50;
                }
            }
            float velXChange = desiredVelX - vel.x;
            float velYChange = desiredVelY - vel.y;
            float impulseX = m_body->GetMass() * velXChange;
            float impulseY = m_body->GetMass() * velYChange;
            m_body->ApplyLinearImpulse(b2Vec2(impulseX, impulseY), m_body->GetWorldCenter(), true);
        }
        if(m_hp <= 0) return false;
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

    void Enemy::setHp(int hp) {
        m_hp = hp;
    }

    EnemyContactListener::EnemyContactListener(Enemy& enemy) : m_enemy(enemy) {
    }

    void EnemyContactListener::beginContact(b2Contact* contact) {
        auto* ptrA = static_cast<EntityData*>(contact->GetFixtureA()->GetUserData());
        auto* ptrB = static_cast<EntityData*>(contact->GetFixtureB()->GetUserData());
        float a = -1.0f, b = -1.0f;
        if(contact->GetFixtureA()->GetUserData() != NULL) a = ptrA->id;
        if(contact->GetFixtureB()->GetUserData() != NULL) b = ptrB->id;
        if(a == float(m_enemy.ID) + 0.1f || b == float(m_enemy.ID) + 0.1f) {
            if(b == 0.0f || a == 0.0f) {
                m_enemy.m_hasHit = true;
            }
        }
        if(a == float(m_enemy.ID) + 0.2f || b == float(m_enemy.ID) + 0.2f)
            m_enemy.m_numFootContacts++;
    }

    void EnemyContactListener::endContact(b2Contact* contact) {
        auto* ptrA = static_cast<EntityData*>(contact->GetFixtureA()->GetUserData());
        auto* ptrB = static_cast<EntityData*>(contact->GetFixtureB()->GetUserData());
        float a = -1.0f, b = -1.0f;
        if(contact->GetFixtureA()->GetUserData() != NULL) a = ptrA->id;
        if(contact->GetFixtureB()->GetUserData() != NULL) b = ptrB->id;
        if(a == float(m_enemy.ID) + 0.1f || b == float(m_enemy.ID) + 0.1f) {
            if(b == 0.0f || a == 0.0f) {
                m_enemy.m_hasHit = false;
            }
        }
        if(a == float(m_enemy.ID) + 0.2f || b == float(m_enemy.ID) + 0.2f)
            m_enemy.m_numFootContacts--;
    }
}