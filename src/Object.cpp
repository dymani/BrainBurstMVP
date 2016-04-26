#include "Object.h"
#include "World.h"

namespace bb {
    Object::Object(World& world, int id, float coordX, float coordY) : Entity(world, id, Entity::OBJECT) {
        m_sprite.setSize({64.0f, 64.0f});
        m_sprite.setOrigin({32.0f, 32.0f});
        m_sprite.setFillColor(sf::Color(128, 128, 128));

        b2BodyDef bodyDef;
        b2PolygonShape dynamicBox;
        b2FixtureDef fixtureDef;
        bodyDef.position.Set(coordX, coordY);
        m_body = m_world.getBWorld().CreateBody(&bodyDef);

        dynamicBox.SetAsBox(0.5f, 0.5f);

        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 0.0f;
        fixtureDef.friction = 0.0f;
        m_body->CreateFixture(&fixtureDef);

        m_hp = 50;
    }

    bool Object::update() {
        return m_hp > 0;
    }

    void Object::draw(const double dt) {
        m_sprite.setPosition({64 * m_body->GetPosition().x, 540 - 64 * m_body->GetPosition().y});
        m_world.getWindow().draw(m_sprite);
    }

    b2Body* Object::getBody() {
        return m_body;
    }

    int Object::getHp() {
        return m_hp;
    }

    void Object::setHp(int hp, int entity) {
        m_hp = hp;
        int colour = 128 * hp / 50;
        m_sprite.setFillColor(sf::Color(colour, colour, colour));
    }
}