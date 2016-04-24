#include "View.h"

namespace bb {
    View::View(sf::Vector2f size, sf::Vector2f coord) {
        m_view.setSize(size);
        m_coord = coord;
        m_view.setCenter({64 * coord.x, 540 - 64 * coord.y});
    }

    void View::setCoord(sf::Vector2f coord) {
        m_coord = coord;
        m_view.setCenter({64 * coord.x, 540 - 64 * coord.y});
    }

    sf::Vector2f View::getCoord() {
        return m_coord;
    }

    sf::View& View::getView() {
        return m_view;
    }
}