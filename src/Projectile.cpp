#include "Projectile.h"
#include "World.h"

namespace bb {
    Projectile::Projectile(World& world, int id, float coordX, float coordY, float velX, float velY)
        : Entity(world, id) {
        m_sprite.setSize({6.4f, 6.4f});
        m_sprite.setOrigin({3.2f, 3.2f});
        m_sprite.setFillColor(sf::Color::Red);

        b2BodyDef bodyDef;
        b2PolygonShape dynamicBox;
        b2FixtureDef fixtureDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(coordX, coordY);
        m_body = m_world.getBWorld().CreateBody(&bodyDef);

        dynamicBox.SetAsBox(0.05f, 0.05f);

        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 1.0f;
        auto* fix = m_body->CreateFixture(&fixtureDef);
        EntityData* data = new EntityData;
        data->id = float(ID);
        fix->SetUserData(data);

        m_body->SetLinearVelocity(b2Vec2(velX, velY));
        m_body->SetGravityScale(0.0f);

        m_contactListener = std::unique_ptr<ProjectileContactListener>(new
            ProjectileContactListener(*this));
        m_contactListenerId = m_world.getContactListener()->addListener(m_contactListener.get());

        m_hasHit = false;
    }

    Projectile::~Projectile() {
        m_world.getContactListener()->removeListener(m_contactListenerId);
    }

    bool Projectile::update() {
        return !m_hasHit;
    }

    void Projectile::draw(const double dt) {
        m_sprite.setPosition({64 * m_body->GetPosition().x, 540 - 64 * m_body->GetPosition().y});
        m_world.getWindow().draw(m_sprite);
    }

    b2Body* Projectile::getBody() {
        return m_body;
    }

    int Projectile::getHp() {
        return -1;
    }

    void Projectile::setHp(int hp) {
    }

    ProjectileContactListener::ProjectileContactListener(Projectile& projectile) : m_projectile(projectile) {
    }

    void ProjectileContactListener::beginContact(b2Contact* contact) {
        auto* ptrA = static_cast<EntityData*>(contact->GetFixtureA()->GetUserData());
        auto* ptrB = static_cast<EntityData*>(contact->GetFixtureB()->GetUserData());
        float a = -1.0f, b = -1.0f;
        if(contact->GetFixtureA()->GetUserData() != NULL) a = ptrA->id;
        if(contact->GetFixtureB()->GetUserData() != NULL) b = ptrB->id;
        if(a == float(m_projectile.ID) || b == float(m_projectile.ID))
            m_projectile.m_hasHit = true;
    }

    void ProjectileContactListener::endContact(b2Contact* contact) {
        auto* ptrA = static_cast<EntityData*>(contact->GetFixtureA()->GetUserData());
        auto* ptrB = static_cast<EntityData*>(contact->GetFixtureB()->GetUserData());
        float a = -1.0f, b = -1.0f;
        if(contact->GetFixtureA()->GetUserData() != NULL) a = ptrA->id;
        if(contact->GetFixtureB()->GetUserData() != NULL) b = ptrB->id;
        if(a == float(m_projectile.ID) || b == float(m_projectile.ID))
            m_projectile.m_hasHit = true;
    }
}