#include "Debug.h"

namespace bb {
    Debug::Debug() {
        m_shown = false;
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
        if(m_shown) window.draw(m_text);
    }

    void Debug::toggle() {
        m_shown = !m_shown;
    }
}