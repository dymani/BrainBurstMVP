#ifndef CONTACT_LISTENER_H
#define CONTACT_LISTENER_H

#include <Box2D/Box2D.h>
#include <map>

namespace bb {
    class ContactListener {
    public:
        virtual void beginContact(b2Contact* contact) = 0;
        virtual void endContact(b2Contact* contact) = 0;
    };

    class GameContactListener : public b2ContactListener {
    public:
        GameContactListener();
        int addListener(ContactListener* listener);
        void removeListener(int id);
        void BeginContact(b2Contact* contact);
        void EndContact(b2Contact* contact);
    private:
        std::map<int, ContactListener*> m_listeners;
        int m_currId;
    };
}

#endif