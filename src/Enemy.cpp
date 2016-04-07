#include "Enemy.h"
#include "World.h"

namespace bb {
    Enemy::Enemy(World& world, float coordX, float coordY) : Entity(world) {
        m_sprite.setFillColor(sf::Color::Blue);
        m_sprite.setSize({64.0f, 64.0f});
        m_sprite.setOrigin({32.0f, 32.0f});

        b2BodyDef bodyDef;
        //bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(coordX, coordY);
        m_body = m_world.getBWorld().CreateBody(&bodyDef);

        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(0.5f, 0.5f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 0.0f;
        fixtureDef.friction = 0.0f;
        m_body->CreateFixture(&fixtureDef);
    }

    void Enemy::update() {
        m_world.getDebug().addLine("Enemy: " + std::to_string(m_body->GetPosition().x) + " "
            + std::to_string(m_body->GetPosition().y));
    }

    void Enemy::draw(sf::RenderWindow& window, const double dt) {
        m_sprite.setPosition({64 * m_body->GetPosition().x, 540 - 64 * m_body->GetPosition().y});
        window.draw(m_sprite);
    }

    b2Body* Enemy::getBody() {
        return m_body;
    }
}