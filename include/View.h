#ifndef VIEW_H
#define VIEW_H

#include <SFML/Graphics.hpp>

namespace bb {
    class View {
    public:
        View(sf::Vector2f size, sf::Vector2f coord);
        void setCoord(sf::Vector2f coord);
        sf::Vector2f getCoord();
        sf::View& getView();
    private:
        sf::View m_view;
        sf::Vector2f m_coord;
    };
}

#endif