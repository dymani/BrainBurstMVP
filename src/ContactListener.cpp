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
        auto* ptrA = static_cast<EntityData*>((void*)contact->GetFixtureA()->GetUserData());
        auto* ptrB = static_cast<EntityData*>((void*)contact->GetFixtureB()->GetUserData());
        if(ptrA == nullptr) ptrA = new EntityData;
        if(ptrB == nullptr) ptrB = new EntityData;
        for(auto& listener : m_listeners) {
            listener.second->beginContact(ptrA, ptrB);
        }
    }

    void GameContactListener::EndContact(b2Contact* contact) {
        auto* ptrA = static_cast<EntityData*>((void*)contact->GetFixtureA()->GetUserData());
        auto* ptrB = static_cast<EntityData*>((void*)contact->GetFixtureB()->GetUserData());
        if(ptrA == nullptr) ptrA = new EntityData;
        if(ptrB == nullptr) ptrB = new EntityData;
        for(auto& listener : m_listeners) {
            listener.second->endContact(ptrA, ptrB);
        }
    }
}