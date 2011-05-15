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
#include <eventmanager.h>
#include <boost/functional/hash.hpp>
#include <iostream>

#define EVENT_POOL_INCREMENT 10
#define EVENT_POOL_INITIAL_SIZE 50

std::size_t Event::hash( std::string hashstr )
{
    static boost::hash<std::string> hasher;
    return hasher( hashstr );
}

EventManager::EventManager()
{
    // Create initial event pool.
    for( int i = 0; i < EVENT_POOL_INITIAL_SIZE; i++ )
        pool.push_back( new Event() );
}

EventManager::~EventManager()
{
    std::cout
        << "Shuting down Event Manager" << std::endl
        << "Event pool reached a size of " << pool.size() << " events." << std::endl
        << "Initial size of " << EVENT_POOL_INITIAL_SIZE << std::endl
        << "Pool memory:" << std::endl
        << "\tEvents   - " << (sizeof(Event) * pool.size()) << " bytes" << std::endl
        << "\tOverhead - " << sizeof(pool) << " bytes" << std::endl;

    while( ! pool.empty() )
    {
        Event* event = pool.front();
        pool.pop_front();
        delete event;
    }
}

void EventManager::addListener( EventListenerSender* listener )
{
    listeners.push_back( listener );
    listener->setEventManager( this );
}

void EventManager::removeListener( EventListenerSender* listener )
{
    listeners.remove( listener );
    listener->setEventManager( 0 );
}

void EventManager::queueEvent( EventPtr event )
{
    current.push_back( event );
}

void EventManager::processEvents()
{
    while( ! active.empty() )
    {
        EventPtr event = active.front();

        std::list<EventListenerSender*>::iterator iter = listeners.begin();

        while( iter != listeners.end() && (!(*iter)->EventNotification( event )) )
            iter++;

        active.pop_front();
    }

    swap( active, current );
}

EventPtr EventManager::newEventFromPool( size_t type_hash )
{
    if( pool.empty() )
    {
        //std::cout << "Event Manager adding to event pool " << EVENT_POOL_INCREMENT << " events." << std::endl;

        for( int i = 0; i < EVENT_POOL_INCREMENT; i++ )
            pool.push_back( new Event() );
    }
    
    EventPtr event;
    event.reset(  pool.front(), PooledEventDeleter(this) );
    pool.pop_front();

    event->type = type_hash;

    return event;
}

void EventManager::returnEventToPool( Event* event )
{
    //std::cout << "Event Manager returning event to pool. Pool size is " << pool.size() << std::endl;
    pool.push_back( event );
}

void PooledEventDeleter::operator() ( Event* event )
{
    //std::cout << "Shared pointer functor called to return event to pool." << std::endl;
    mManager->returnEventToPool( event );
}
