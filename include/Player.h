#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

namespace bb {
    class World;
    class PlayerContactListener;

    class Player : public Entity {
        friend class PlayerContactListener;
    public:
        Player(World& world);
        void handleInput();
        void handleInput(sf::Event event);
        void update();
        void draw(sf::RenderWindow& window, const double dt);
        b2Body* getBody();
    private:
        sf::RectangleShape m_sprite;
        enum MoveState {
            MS_LEFT, MS_RIGHT, MS_STOP
        } m_moveState;
        enum JumpState {
            JS_JUMP, JS_DOUBLE_JUMP, JS_STOP, JS_DOUBLE_STOP
        } m_jumpState;
        int m_jumpTimeout, m_numFootContacts;
        bool m_isSprinting, m_isDodging;
        int m_sprintDuration;
        PlayerContactListener* m_contactListener;
    };

    class PlayerContactListener : public b2ContactListener {
    public:
        PlayerContactListener(Player& player);
        void BeginContact(b2Contact* contact);
        void EndContact(b2Contact* contact);
    private:
        Player& m_player;
    };
}

#endif