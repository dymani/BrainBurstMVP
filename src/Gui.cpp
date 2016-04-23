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

        m_world.getWindow().setMouseCursorVisible(false);
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
            for(unsigned int i = 0; i < player->getAbilities().size(); i++) {
                rect.setPosition({70.0f + i * 40, 500});
                m_playerAbilities.push_back(rect);
            }
            m_abilityCreated = true;
        }
        switch(int(player->getAbilityState())) {
            case 0:
                m_cursor.setFillColor(sf::Color::White);
                m_cursor.setSize({10.0f, 10.0f});
                for(auto& ability : m_playerAbilities) {
                    ability.setFillColor(sf::Color::White);
                }
                break;
            case 1:
                m_playerAbilities[player->getAbility()].setFillColor(sf::Color(
                    int(255 / 50 * (player->getAbilityCount() + 50)),
                    255,
                    int(255 / 50 * (player->getAbilityCount() + 50))
                ));
                break;
            case 2:
            {
                int total = -player->getAbilities().at(player->getAbility())->getHold();
                m_cursor.setFillColor(sf::Color(
                    int(128 - 128 / total * (player->getAbilityHold() + total)),
                    255,
                    int(128 - 128 / total * (player->getAbilityHold() + total))
                ));
                m_playerAbilities[player->getAbility()].setFillColor(sf::Color::Green);
            }
            break;
            case 3:
            {
                int total = -player->getAbilities().at(player->getAbility())->getTimeout();
                m_cursor.setFillColor(sf::Color(
                    int(255 / total * (player->getAbilityTimeout() + total)),
                    255,
                    int(255 / total * (player->getAbilityTimeout() + total))
                ));
                m_cursor.setSize({15.0f, 15.0f});
                m_playerAbilities[player->getAbility()].setFillColor(sf::Color::Green);
            }
            break;
        }
    }

    void Gui::draw(const double dt) {
        m_world.getWindow().draw(m_cursor);
        m_world.getWindow().draw(m_playerJump);
        m_world.getWindow().draw(m_playerSprint);
        for(auto& ability : m_playerAbilities) {
            m_world.getWindow().draw(ability);
        }
    }
}