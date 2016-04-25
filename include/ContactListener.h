#ifndef CONTACT_LISTENER_H
#define CONTACT_LISTENER_H

#include <Box2D/Box2D.h>
#include <map>

namespace bb {
    struct EntityData {
        int id = -1;
        int subId = -1;
    };

    class ContactListener {
    public:
        virtual void beginContact(EntityData* a, EntityData* b) = 0;
        virtual void endContact(EntityData* a, EntityData* b) = 0;
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