#include "Player.h"
#include "World.h"

namespace bb {
    Player::Player(World& world) : Entity(world) {
        m_velocity = {0.0F, 0.0F};
        m_sprite.setFillColor(sf::Color::White);
        m_sprite.setSize({64.0F, 64.0F});
        m_sprite.setOrigin({0.0F, 64.0F});
        m_isDodging = false;
        m_doubleJump = true;
        m_isSprinting = false;
        m_sprint = 5;
        m_sprintCount = 0;
        m_size = {64.0F, 64.0F};
        m_hitbox = {0.0F, 0.0F, 64.0F, 64.0F};
        m_isOnGround = true;
        m_isMovable = true;
    }

    void Player::handleInput() {
        bool keyLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        bool keyRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        bool keyDodge = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
        bool keySprint = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
        m_isDodging = keyDodge;
        m_isSprinting = keySprint;
        float speed;
        if(m_isDodging) {
            speed = 0.05F;
            m_isSprinting = false;
        } else {
            if(m_sprint <= 0)
                m_isSprinting = false;
            if(m_isSprinting)
                speed = 0.8F;
            else
                speed = 0.1F;
        }
        if(keyLeft && !keyRight)
            m_velocity.x = -speed;
        else if(keyRight && !keyLeft)
            m_velocity.x = speed;
        if(m_velocity.x == 0) m_isSprinting = false;
    }

    void Player::handleInput(sf::Event event) {
        if(event.type == sf::Event::KeyPressed) {
            switch(event.key.code) {
                case sf::Keyboard::W:
                    if(m_isOnGround && !m_isDodging) {
                        m_velocity.y = 0.6F;
                        m_doubleJump = true;
                        m_isOnGround = false;
                    } else if(!m_isOnGround && m_doubleJump) {
                        m_velocity.y = 0.6F;
                        m_doubleJump = false;
                    }
                    break;
            }
        }
    }

    void Player::update() {
        auto& debug = m_world.getDebug();

        m_coord.x += m_velocity.x;
        m_coord.y += m_velocity.y;
        debug.addLine("Coord:    " + std::to_string(m_coord.x) + " " + std::to_string(m_coord.y));
        debug.addLine("Velocity: " + std::to_string(m_velocity.x) + " " + std::to_string(m_velocity.y));
        if(m_isSprinting) {
            if(m_velocity.x != 0) {
                m_sprint--;
            }
        } else if(m_velocity.x == 0) {
            m_sprintCount++;
            if(m_sprintCount >= 5) {
                m_sprintCount = 0;
                m_sprint = m_sprint < 5 ? m_sprint + 1 : 5;
            }
        }
        if(m_velocity.x > 0) {
            m_velocity.x -= 0.1F;
            if(m_velocity.x < 0)
                m_velocity.x = 0;
        } else if(m_velocity.x < 0) {
            m_velocity.x += 0.1F;
            if(m_velocity.x > 0)
                m_velocity.x = 0;
        }
        if(m_coord.y > 0) {
            if(m_isSprinting)
                m_velocity.y = 0;
            else
                m_velocity.y -= 0.05F;
            if(m_velocity.y < -3.0F) m_velocity.y = -3.0F;
        } else if(m_coord.y <= 0) {
            m_velocity.y = 0;
            m_coord.y = 0;
            m_doubleJump = true;
            m_isOnGround = true;
        }
        debug.addLine("Double jump: " + std::string(m_doubleJump ? "True" : "False"));
        debug.addLine((m_isSprinting ? "Spriting:     " : "Not sprinting: ") + std::to_string(m_sprint));
        debug.addLine(m_isDodging ? "Dodging" : "Not dodging");
    }

    void Player::draw(sf::RenderWindow & window, const double dt) {
        m_sprite.setPosition({64 * m_coord.x, 540 - 64 * m_coord.y});
        if(m_isDodging)
            m_sprite.setScale({1.0F, 0.7F});
        else
            m_sprite.setScale({1.0F, 1.0F});
        window.draw(m_sprite);
    }

    sf::Vector2f Player::getCoord() {
        return m_coord;
    }

    void Player::setCoord(float x, float y) {
        m_coord = {x, y};
    }

    sf::Vector2f Player::getVelocity() {
        return m_velocity;
    }

    void Player::setVelocity(float x, float y) {
        m_velocity = {x, y};
    }

    sf::FloatRect Player::getHitbox() {
        return m_hitbox;
    }
}