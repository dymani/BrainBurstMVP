#ifndef GAME_H
#define GAME_H

#include <time.h>
#include <stack>
#include <windows.h>
#include <memory>
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
        int update();
        void draw(const double dt);
        sf::RenderWindow& getWindow();
    private:
        enum State {
            RUNNING, QUIT
        } m_state;
        sf::RenderWindow m_window;
        std::unique_ptr<World> m_world;
    };
}

#endif