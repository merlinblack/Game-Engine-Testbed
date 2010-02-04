#include <eventmanager.h>
#include <boost/functional/hash.hpp>
#include <iostream>

std::size_t Event::hash( std::string hashstr )
{
    static boost::hash<std::string> hasher;
#if 0
    std::size_t ret;
    ret = hasher( hashstr );
    std::cout << "Hash code for: " << hashstr << " = " << ret << std::endl;
    return ret;
#else
    return hasher( hashstr );
#endif
}

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
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
    current.push( event );
//    std::cout << "Event Queued: " << event->type << std::endl;
}

void EventManager::processEvents()
{
    while( ! active.empty() )
    {
        EventPtr event = active.front();

        std::list<EventListenerSender*>::iterator iter = listeners.begin();

        while( iter != listeners.end() && (!(*iter)->EventNotification( event )) )
            iter++;

        active.pop();
    }

    swap( active, current );
}
