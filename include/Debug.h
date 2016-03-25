#ifndef DEGUB_H
#define DEBUG_H

#include <SFML/Graphics.hpp>

namespace bb {
    class Debug {
    public:
        Debug();
        void init(sf::Font& font);
        void reset();
        void addLine(std::string text);
        void draw(sf::RenderWindow& window);
    private:
        sf::Text m_text;
    };
}

#endif