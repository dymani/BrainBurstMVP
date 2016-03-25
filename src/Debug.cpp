#include "Debug.h"

namespace bb {
    Debug::Debug() {
    }

    void Debug::init(sf::Font& font) {
        m_text.setFont(font);
        m_text.setCharacterSize(15);
        m_text.setColor(sf::Color::White);
        m_text.setPosition({10.0F, 0.0F});
        reset();
    }

    void Debug::reset() {
        m_text.setString("");
    }

    void Debug::addLine(std::string text) {
        m_text.setString(m_text.getString() + '\n' + text);
    }

    void Debug::draw(sf::RenderWindow& window) {
        window.draw(m_text);
    }
}