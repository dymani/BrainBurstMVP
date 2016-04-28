#ifndef INCARNATE_H
#define INCARNATE_H

#include <SFML/Graphics.hpp>
#include "IncarnateSkill.h"

namespace bb {
    class Player;

    class Incarnate {
    public:
        Incarnate(Player& player);
        void addSkill(IncarnateSkill* skill);
        bool checkInput(int code);
        std::string getKeyword();
    private:
        Player& m_player;
        char m_keyword[10] = {0};
        std::vector<IncarnateSkill*> m_skills;
    };
}

#endif