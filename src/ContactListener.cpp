#include "ContactListener.h"
#include <iostream>

namespace bb {
    GameContactListener::GameContactListener() {
        m_listeners.clear();
        m_currId = 0;
    }

    int GameContactListener::addListener(ContactListener* listener) {
        m_listeners[m_currId] = listener;
        return m_currId++;
    }

    void GameContactListener::removeListener(int id) {
        m_listeners.erase(id);
    }

    void GameContactListener::BeginContact(b2Contact* contact) {
        for(auto& listener : m_listeners) {
            listener.second->beginContact(contact);
        }
    }

    void GameContactListener::EndContact(b2Contact* contact) {
        for(auto& listener : m_listeners) {
            listener.second->endContact(contact);
        }
    }
}