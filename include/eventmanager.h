#ifndef __EVENT_MANAGER_H
#define __EVENT_MANAGER_H

#include <queue>
#include <list>
#include <OIS.h>
#include <boost/shared_ptr.hpp>

struct EventData
{
    virtual ~EventData() {}
};

struct Event
{
    size_t type;

    Event()                     {}
    Event( size_t t ) : type(t) {}
    Event( const char *strt )   { type = hash( strt ); }

    static std::size_t hash( std::string hashstr );

    boost::shared_ptr<EventData> data;
};

class EventListenerSender;

typedef boost::shared_ptr<Event> EventPtr;

class EventManager
{
    std::queue<EventPtr> active;
    std::queue<EventPtr> current;
    std::list<EventListenerSender*> listeners;

public:
    EventManager();
    ~EventManager();

    void addListener( EventListenerSender* listener );
    void removeListener( EventListenerSender* listener );
    void queueEvent( EventPtr event );
    void processEvents();
};

class EventListenerSender
{
    EventManager* eventManager;
public:
    virtual bool EventNotification( EventPtr event ) = 0;

    void queueEvent( EventPtr event )
    {
        if( eventManager )
            eventManager->queueEvent( event );
    }

    void setEventManager( EventManager* em )
    {
        eventManager = em;
    }
};

#endif // __EVENT_MANAGER_H
