#include "Projectile.h"
#include "World.h"

namespace bb {
    Projectile::Projectile(World& world, int id, float coordX, float coordY, float velX, float velY,
        int owner) : Entity(world, id, Entity::OBJECT) {
        m_owner = owner;

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
        data->id = ID;
        data->subId = 0;
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
        if(m_hasHit) {
            m_world.damage(m_owner, m_entityHit, 10);
        }
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

    void Projectile::setHp(int hp, int entity) {
    }

    ProjectileContactListener::ProjectileContactListener(Projectile& projectile) : m_projectile(projectile) {
    }

    void ProjectileContactListener::beginContact(EntityData* a, EntityData* b) {
        if(a->id == m_projectile.ID) {
            m_projectile.m_hasHit = true;
            m_projectile.m_entityHit = b->id;
        } else if(b->id == m_projectile.ID) {
            m_projectile.m_hasHit = true;
            m_projectile.m_entityHit = a->id;
        }
    }

    void ProjectileContactListener::endContact(EntityData* a, EntityData* b) {
        if(a->id == m_projectile.ID) {
            m_projectile.m_hasHit = true;
            m_projectile.m_entityHit = b->id;
        } else if(b->id == m_projectile.ID) {
            m_projectile.m_hasHit = true;
            m_projectile.m_entityHit = a->id;
        }
    }
}