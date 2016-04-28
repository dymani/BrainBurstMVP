#include "Gui.h"
#include "World.h"

namespace bb {
    Gui::Gui(World& world) : m_world(world) {
        m_cursor.setSize({10.0f, 10.0f});
        m_cursor.setFillColor(sf::Color::White);

        m_playerJump.setRadius(15.0f);
        m_playerJump.setPosition({10.0f, 500.0f});
        m_playerJump.setFillColor(sf::Color(128, 128, 128));

        m_playerSprint.setRadius(15.0f);
        m_playerSprint.setPosition({10.0f, 460.0f});
        m_playerSprint.setFillColor(sf::Color(128, 128, 128));

        m_hp.setPosition({10.0f, 10.0f});
        m_hp.setSize({100.0f, 15.0f});
        m_hp.setFillColor(sf::Color::Blue);

        m_hpBar.setPosition({10.0f, 10.0f});
        m_hpBar.setSize({100.0f, 15.0f});
        m_hpBar.setFillColor(sf::Color(128, 0, 0));

        m_sp.setPosition({10.0f, 35.0f});
        m_sp.setSize({100.0f, 15.0f});
        m_sp.setFillColor(sf::Color::Green);

        m_spBar.setPosition({10.0f, 35.0f});
        m_spBar.setSize({100.0f, 15.0f});
        m_spBar.setFillColor(sf::Color(50, 50, 50));

        m_bp.setCharacterSize(15);
        m_bp.setColor(sf::Color::White);
        m_bp.setPosition({10.0f, 60.0f});

        m_world.getWindow().setMouseCursorVisible(false);
    }

    void Gui::setFont(sf::Font& font) {
        m_bp.setFont(font);
    }

    void Gui::handleInput() {
        m_cursor.setPosition(sf::Vector2f(sf::Mouse::getPosition(m_world.getWindow())));
    }

    void Gui::handleInput(sf::Event event) {
    }

    void Gui::update() {
        auto* player = dynamic_cast<Player*>(m_world.getEntity(0));
        switch(player->getJumpState()) {
            case 0:
                m_playerJump.setFillColor(sf::Color(0, 255, 0));
                break;
            case 1:
            case 2:
                m_playerJump.setFillColor(sf::Color(128, 255, 128));
                break;
            case 3:
            case 4:
                m_playerJump.setFillColor(sf::Color(128, 128, 128));
                break;
        }
        int sprint = player->getSprintDuration();
        if(sprint >= 0)
            m_playerSprint.setFillColor(sf::Color(
                int(128 - 12.8 * sprint),
                int(128 + 12.7 * sprint),
                int(128 - 12.8 * sprint)));
        else
            m_playerSprint.setFillColor(sf::Color(
                int(128 - 12.7 * sprint),
                int(128 + 12.8 * sprint),
                int(128 + 12.8 * sprint)));
        if(!m_abilityCreated) {
            sf::RectangleShape rect;
            rect.setSize({30.0f, 30.0f});
            rect.setFillColor(sf::Color::White);
            for(unsigned int i = 0; i < player->getSkills().size(); i++) {
                rect.setPosition({70.0f + i * 40, 500});
                m_playerSkills.push_back(rect);
            }
            m_abilityCreated = true;
        }
        switch(int(player->getSkillState())) {
            case 0:
                m_cursor.setFillColor(sf::Color::White);
                m_cursor.setSize({10.0f, 10.0f});
                for(auto& ability : m_playerSkills) {
                    ability.setFillColor(sf::Color::White);
                }
                break;
            case 1:
                m_playerSkills[player->getSkill()].setFillColor(sf::Color(
                    int(255 / 50 * (player->getSkillCount() + 50)),
                    255,
                    int(255 / 50 * (player->getSkillCount() + 50))
                ));
                break;
            case 2:
            {
                int total = -player->getSkills().at(player->getSkill())->getHold();
                m_cursor.setFillColor(sf::Color(
                    int(128 - 128 / total * (player->getSkillHold() + total)),
                    255,
                    int(128 - 128 / total * (player->getSkillHold() + total))
                ));
                m_playerSkills[player->getSkill()].setFillColor(sf::Color::Green);
            }
            break;
            case 3:
            {
                int total = -player->getSkills().at(player->getSkill())->getTimeout();
                m_cursor.setFillColor(sf::Color(
                    int(255 / total * (player->getSkillTimeout() + total)),
                    255,
                    int(255 / total * (player->getSkillTimeout() + total))
                ));
                m_cursor.setSize({15.0f, 15.0f});
                m_playerSkills[player->getSkill()].setFillColor(sf::Color::Green);
            }
            break;
        }
        m_hp.setSize({float(player->getHp()), 15.0f});
        m_sp.setSize({float(player->getSp()), 15.0f});
        m_bp.setString("BP: " + std::to_string(player->getBp()));
    }

    void Gui::draw(const double dt) {
        auto& window = m_world.getWindow();
        window.draw(m_playerJump);
        window.draw(m_playerSprint);
        for(auto& ability : m_playerSkills) {
            window.draw(ability);
        }
        window.draw(m_hpBar);
        window.draw(m_hp);
        window.draw(m_spBar);
        window.draw(m_sp);
        window.draw(m_bp);
        window.draw(m_cursor);
    }
}