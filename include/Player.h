#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <memory>
#include "Entity.h"
#include "Ability.h"
#include "ContactListener.h"

namespace bb {
    class World;
    class PlayerContactListener;

    class Player : public Entity {
        friend class PlayerContactListener;
    public:
        Player(World& world, int id);
        ~Player();
        void handleInput();
        void handleInput(sf::Event event);
        bool update();
        void draw(const double dt);
        b2Body* getBody();
        int getHp();
        void setHp(int hp, int entity);
        int getAp();
        void setBp(int bp);
        int getBp();
        int getJumpState();
        int getSprintDuration();
        int getAbilityState();
        int getAbility();
        int getAbilityCount();
        int getAbilityHold();
        int getAbilityTimeout();
        std::vector<Ability*>& getAbilities();
    private:
        sf::RectangleShape m_sprite;
        enum MoveState {
            MS_LEFT, MS_RIGHT, MS_STOP
        } m_moveState;
        enum JumpState {
            JS_STOP, JS_JUMP, JS_JUMPED, JS_D_JUMP, JS_D_JUMPED
        } m_jumpState;
        int m_jumpTimeout, m_numFootContacts;
        bool m_isSprinting, m_isDodging;
        int m_sprintDuration;
        std::unique_ptr<PlayerContactListener> m_contactListener;
        int m_contactListenerId;
        enum AbilityState {
            AS_NONE, AS_COUNT, AS_HOLD, AS_USE
        } m_abilityState;
        int m_ability, m_abilityCount, m_abilityHold, m_abilityTimeout;
        std::vector<Ability*> m_abilities;
        int m_ap, m_bp;
        int m_killedBy;
    };

    class PlayerContactListener : public ContactListener {
    public:
        PlayerContactListener(Player& player);
        void beginContact(EntityData* a, EntityData* b);
        void endContact(EntityData* a, EntityData* b);
    private:
        Player& m_player;
    };
}

#endif