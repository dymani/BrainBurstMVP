#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <memory>
#include "Entity.h"
#include "Skill.h"
#include "ContactListener.h"
#include "Incarnate.h"

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
        void setSp(int sp);
        int getSp();
        void setBp(int bp);
        int getBp();
        int getJumpState();
        int getSprintDuration();
        int getSkillState();
        int getSkill();
        int getSkillCount();
        int getSkillHold();
        int getSkillTimeout();
        std::vector<Skill*>& getSkills();
        std::string getNewSkillText();
        void setNewIncarnateSkill(IncarnateSkill* skill);
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
        enum SkillState {
            SS_NONE, SS_COUNT, SS_HOLD, SS_USE
        } m_skillState;
        int m_skill, m_skillCount, m_skillHold, m_skillTimeout;
        std::vector<Skill*> m_skills;
        int m_sp, m_bp;
        int m_killedBy;
        Incarnate m_incarnate;
        IncarnateSkill* m_newSkill;
        enum NewSkillState {
            NSS_NONE, NSS_CHOOSE, NSS_CHOSE
        } m_nssState;
        int m_nssTimeout;
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