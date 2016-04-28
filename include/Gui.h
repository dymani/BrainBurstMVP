#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>

namespace bb {
    class World;

    class Gui {
    public:
        Gui(World& world);
        void setFont(sf::Font& font);
        void handleInput();
        void handleInput(sf::Event event);
        void update();
        void draw(const double dt);
    private:
        World& m_world;
        sf::RectangleShape m_cursor;
        sf::CircleShape m_playerJump, m_playerSprint;
        std::vector<sf::RectangleShape> m_playerSkills;
        sf::RectangleShape m_hp, m_hpBar;
        sf::RectangleShape m_sp, m_spBar;
        sf::Text m_bp;
        bool m_abilityCreated = false;
    };
}

#endif