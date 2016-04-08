#ifndef GAME_H
#define GAME_H

#include <time.h>
#include <stack>
#include <windows.h>
#include <SFML/Graphics.hpp>
#include "World.h"

namespace bb {
    class Game {
    public:
        static int const MS_PER_UPDATE = 20;
        Game();
        ~Game();
        int run();
        void handleInput();
        bool update();
        void draw(const double dt);
        sf::RenderWindow& getWindow();
    private:
        enum State {
            RUNNING, QUIT
        } m_state;
        sf::RenderWindow m_window;
        World m_world;
    };
}

#endif