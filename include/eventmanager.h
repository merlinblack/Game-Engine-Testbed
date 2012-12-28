/*
-----------------------------------------------------------------------------

Copyright (c) 2010 Nigel Atkinson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __EVENT_MANAGER_H
#define __EVENT_MANAGER_H

#include <queue>
#include <list>
#include <OIS.h>
#include <boost/shared_ptr.hpp>
#include <RefCountedObject.h>

struct EventData : public RefCountedObjectType<size_t>
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

    RefCountedObjectPtr<EventData> data;
};

class EventListenerSender;

typedef boost::shared_ptr<Event> EventPtr;
typedef RefCountedObjectPtr<EventData> EventDataPtr;

class EventManager
{
    std::deque<Event*> pool;
    std::deque<EventPtr> active;
    std::deque<EventPtr> current;
    std::list<EventListenerSender*> listeners;

public:
    EventManager();
    ~EventManager();

    void addListener( EventListenerSender* listener );
    void removeListener( EventListenerSender* listener );
    void queueEvent( EventPtr event );
    void processEvents();

    EventPtr newEventFromPool( size_t type_hash );
    void returnEventToPool( Event* ptr );
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

    EventPtr newEvent( size_t type_hash )
    {
        return eventManager->newEventFromPool( type_hash );
    }

    EventPtr newEvent( const char *type_str )
    {
        return newEvent( Event::hash(type_str) );
    }

    void setEventManager( EventManager* em )
    {
        eventManager = em;
    }
};

class PooledEventDeleter
{
    EventManager* mManager;

    public:
    PooledEventDeleter( EventManager* manager ) : mManager(manager) { }

    void operator() ( Event* event );
};

#endif // __EVENT_MANAGER_H
