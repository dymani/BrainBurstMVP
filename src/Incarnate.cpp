#include "Incarnate.h"
#include "Player.h"

namespace bb {
    Incarnate::Incarnate(Player& player) : m_player(player) {
    }

    void Incarnate::addSkill(IncarnateSkill* skill) {
        m_skills.push_back(skill);
    }

    bool Incarnate::checkInput(int code) {
        if(code >= 65 && code <= 90)
            code += 32;
        if(code < 97 || code > 122)
            return false;
        for(int i = 8; i >= 0; i--) {
            m_keyword[i + 1] = m_keyword[i];
        }
        m_keyword[0] = char(code);
        for(auto& s : m_skills) {
            for(auto& word : s->getKeywords()) {
                for(int i = word.size() - 1; i < 10; i++) {
                    std::string input = std::string(m_keyword).substr(0, i);
                    std::reverse(input.begin(), input.end());
                    if(input == word) {
                        m_player.setNewIncarnateSkill(s);
                        auto skill = std::find(m_skills.begin(), m_skills.end(), s);
                        m_skills.erase(skill);
                        return true;
                    }
                }
            }
        }
        return false;
    }
    std::string Incarnate::getKeyword() {
        std::string keyword;
        for(int i = 9; i >= 0; i--)
            if(m_keyword[i])
                keyword.push_back(m_keyword[i]);
        return keyword;
    }
}