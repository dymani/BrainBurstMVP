#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "Entity.h"
#include "Ability.h"

namespace bb {
    class World;
    class PlayerContactListener;

    class Player : public Entity {
        friend class PlayerContactListener;
    public:
        Player(World& world);
        void handleInput();
        void handleInput(sf::Event event);
        bool update();
        void draw(const double dt);
        b2Body* getBody();
        int getHp();
        void setHp(int hp);
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
        enum AbilityState {
            AS_NONE, AS_COUNT, AS_HOLD, AS_USE
        } m_abilityState;
        int m_ability, m_abilityCount, m_abilityHold, m_abilityTimeout;
        std::vector<Ability*> m_abilities;
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