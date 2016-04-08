#include "Enemy.h"
#include "World.h"

namespace bb {
    Enemy::Enemy(World& world, float coordX, float coordY, int type) : Entity(world), m_type(Type(type)) {
        m_sprite.setSize({64.0f, 64.0f});
        m_sprite.setOrigin({32.0f, 32.0f});
        switch(m_type) {
            case T_STATIC:
                m_sprite.setFillColor(sf::Color::Blue);
                break;
            case T_MOBILE:
                m_sprite.setFillColor(sf::Color::Green);
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
                bodyDef.type = b2_dynamicBody;
                bodyDef.position.Set(coordX, coordY);
                m_body = m_world.getBWorld().CreateBody(&bodyDef);

                dynamicBox.SetAsBox(0.5f, 0.5f);

                fixtureDef.shape = &dynamicBox;
                fixtureDef.density = 30.0f;
                fixtureDef.friction = 1.0f;
                m_body->CreateFixture(&fixtureDef);
                break;
        }

        m_hp = 5;
    }

    bool Enemy::update() {
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
}