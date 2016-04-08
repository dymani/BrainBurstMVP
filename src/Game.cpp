#include "Game.h"

namespace bb {
    Game::Game() : m_world(*this) {

    }

    Game::~Game(){

    }

    int Game::run(){
        m_window.create(sf::VideoMode(960, 540), "Brain Burst [MVP]");
        clock_t previous, lag, current, elapsed;
        previous = clock();
        lag = 0;
        bool isRunning = true;
        while(isRunning) {
            current = clock();
            elapsed = current - previous;
            previous = current;
            lag += elapsed;

            handleInput();

            while(lag >= MS_PER_UPDATE) {
                isRunning = update();
                lag -= MS_PER_UPDATE;
            }

            draw(double(lag) / double(MS_PER_UPDATE));
        }
    #if _DEBUG
        system("PAUSE");
    #endif
        return 0;
    }

    void Game::handleInput(){
        sf::Event windowEvent;
        m_world.handleInput();
        while(m_window.pollEvent(windowEvent)) {
            m_world.handleInput(windowEvent);
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

    bool Game::update(){
        if(!m_world.update()) m_state = QUIT;
        if(m_state == QUIT) {
            m_window.close();
            return false;
        }
        return true;
    }

    void Game::draw(const double dt){
        m_window.clear();
        m_world.draw(dt);
        m_window.display();
    }

    sf::RenderWindow& Game::getWindow() {
        return m_window;
    }
}