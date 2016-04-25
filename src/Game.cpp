#include "Game.h"

namespace bb {
    Game::Game() {

    }

    Game::~Game() {

    }

    int Game::run() {
        m_window.create(sf::VideoMode(960, 540), "Brain Burst [MVP]", sf::Style::Close);
        clock_t previous, lag, current, elapsed;
        previous = clock();
        lag = 0;
        bool isRunning = true;
        int restart = -1;
        while(isRunning) {
            if(restart != 0) {
                int bp = 100;
                if(restart == 1) {
                    bp = dynamic_cast<Player*>(m_world->getEntity(0))->getBp();
                    if(bp <= 0) isRunning = false;
                }
                m_world.reset(new World(*this));
                dynamic_cast<Player*>(m_world->getEntity(0))->setBp(bp);
                restart = 0;
            }
            current = clock();
            elapsed = current - previous;
            previous = current;
            lag += elapsed;

            handleInput();

            while(lag >= MS_PER_UPDATE) {
                int i = update();
                if(i == -1)
                    isRunning = false;
                else if(i == 1)
                    restart = 1;
                lag -= MS_PER_UPDATE;
            }

            draw(double(lag) / double(MS_PER_UPDATE));
        }
    #if _DEBUG
        system("PAUSE");
    #endif
        return 0;
    }

    void Game::handleInput() {
        sf::Event windowEvent;
        m_world->handleInput();
        while(m_window.pollEvent(windowEvent)) {
            m_world->handleInput(windowEvent);
            if(windowEvent.type == sf::Event::Closed) {
                m_state = QUIT;
                return;
            } else if(windowEvent.type == sf::Event::KeyPressed) {
                switch(windowEvent.key.code) {
                    case sf::Keyboard::Escape:
                        m_state = QUIT;
                        break;
                }
            }
        }
    }

    int Game::update() {
        int update = m_world->update();
        if(update == 1) return 1;
        if(m_state == QUIT || update == -1) {
            m_window.close();
            return -1;
        }
        return 0;
    }

    void Game::draw(const double dt) {
        m_window.clear();
        m_world->draw(dt);
        m_window.display();
    }

    sf::RenderWindow& Game::getWindow() {
        return m_window;
    }
}