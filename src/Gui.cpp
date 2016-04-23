#include "Gui.h"
#include "World.h"

namespace bb {
    Gui::Gui(World& world) : m_world(world) {
        m_playerJump.setRadius(25.0f);
        m_playerJump.setPosition({10.0f, 480.0f});
        m_playerJump.setFillColor(sf::Color(128, 128, 128));

        m_playerSprint.setRadius(25.0f);
        m_playerSprint.setPosition({10.0f, 420.0f});
        m_playerSprint.setFillColor(sf::Color(128, 128, 128));
    }

    void Gui::handleInput() {
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
    }

    void Gui::draw(const double dt) {
        m_world.getWindow().draw(m_playerJump);
        m_world.getWindow().draw(m_playerSprint);
    }
}