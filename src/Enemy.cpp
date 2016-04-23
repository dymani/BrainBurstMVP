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
            case T_HOSTILE:
                bodyDef.type = b2_dynamicBody;
                bodyDef.position.Set(coordX, coordY);
                m_body = m_world.getBWorld().CreateBody(&bodyDef);

                dynamicBox.SetAsBox(0.5f, 0.5f);

                fixtureDef.shape = &dynamicBox;
                fixtureDef.density = 30.0f;
                fixtureDef.friction = 0.3f;
                auto* fix = m_body->CreateFixture(&fixtureDef);
                if(m_type == T_HOSTILE) {
                    EntityData* data = new EntityData;
                    data->id = float(ID);
                    fix->SetUserData(data);

                    dynamicBox.SetAsBox(0.4f, 0.05f, b2Vec2(0, -0.5f), 0);
                    fixtureDef.isSensor = true;
                    b2Fixture* footSensorFixture = m_body->CreateFixture(&fixtureDef);
                    data->id = float(ID + 0.1f);
                    footSensorFixture->SetUserData(data);

                    m_contactListener = std::unique_ptr<EnemyContactListener>(new
                        EnemyContactListener(*this));
                    m_contactListenerId = m_world.getContactListener()->addListener(m_contactListener.get());

                    m_numFootContacts = 0;
                }
                break;
        }

        m_hp = 5;
        m_hasHit = false;
        m_hitTimeout = 0;
    }

    Enemy::~Enemy() {
        m_world.getContactListener()->removeListener(m_contactListenerId);
    }

    bool Enemy::update() {
        if(m_type == T_HOSTILE) {
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
                    std::cout << "damage\n";
                }
                m_hasHit = false;
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
        if((a == float(m_enemy.ID) && b == 0) || (b == float(m_enemy.ID) && a == 0))
            m_enemy.m_hasHit = true;
        else if(a == float(m_enemy.ID + 0.1f) || b == float(m_enemy.ID + 0.1f))
            m_enemy.m_numFootContacts++;
    }

    void EnemyContactListener::endContact(b2Contact* contact) {
        auto* ptrA = static_cast<EntityData*>(contact->GetFixtureA()->GetUserData());
        auto* ptrB = static_cast<EntityData*>(contact->GetFixtureB()->GetUserData());
        float a = -1.0f, b = -1.0f;
        if(contact->GetFixtureA()->GetUserData() != NULL) a = ptrA->id;
        if(contact->GetFixtureB()->GetUserData() != NULL) b = ptrB->id;
        if(a == float(m_enemy.ID + 0.1f) || b == float(m_enemy.ID + 0.1f))
            m_enemy.m_numFootContacts--;
    }
}