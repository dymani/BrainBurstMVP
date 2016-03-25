#include "Enemy.h"
#include "World.h"

namespace bb {
    Enemy::Enemy(World& world) : Entity(world) {
        m_coord = {10.0F, 0.0F};
        m_sprite.setFillColor(sf::Color::Blue);
        m_sprite.setSize({64.0F, 64.0F});
        m_sprite.setOrigin({0.0F, 64.0F});
        m_size = {64.0F, 64.0F};
        m_hitbox = {0.0F, 0.0F, 64.0F, 64.0F};
        m_isOnGround = false;
        m_isMovable = false;
    }

    void Enemy::update() {
    }

    void Enemy::draw(sf::RenderWindow& window, const double dt) {
        m_sprite.setPosition({64 * m_coord.x, 540 - 64 * m_coord.y});
        window.draw(m_sprite);
    }

    sf::Vector2f Enemy::getCoord() {
        return m_coord;
    }

    void Enemy::setCoord(float x, float y) {
        m_coord = {x, y};
    }

    sf::Vector2f Enemy::getVelocity() {
        return m_velocity;
    }

    void Enemy::setVelocity(float x, float y) {
        m_velocity = {x, y};
    }

    sf::FloatRect Enemy::getHitbox() {
        return m_hitbox;
    }
}