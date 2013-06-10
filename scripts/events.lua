-- Cache hash values.
events = events or {}

events.keydown      = Engine.Event.hash "EVT_KEYDOWN"
events.mousemove    = Engine.Event.hash "EVT_MOUSEMOVE"
events.mousedown    = Engine.Event.hash "EVT_MOUSEDOWN"
events.mouseup      = Engine.Event.hash "EVT_MOUSEUP"
events.windowresize = Engine.Event.hash "EVT_WINDOW_RESIZE"

events.subscribers = events.subscribers or {}

function events.subscribe( eventType, listener )
    if type(listener) ~= 'function' then
        error 'listener must be a function.'
    end

    if type(eventType) == 'string' then
        eventType = Engine.Event.hash( eventType )
    end

    events.subscribers[eventType] = events.subscribers[eventType] or {}

    table.insert( events.subscribers[eventType], listener )
end

function events.unsubscribe( eventType, listener )

    if type(eventType) == 'string' then
        eventType = Engine.Event.hash( eventType )
    end

    index = table.indexOf( events.subscribers[eventType], listener )
    if index then
        table.remove( events.subscribers[eventType], index )
    end
end

function events.route( event )
    listeners = events.subscribers[event.type]

    if type(listeners) == 'table' then
        for _, v in ipairs( listeners ) do
            if v( event ) then
                return true
            end
        end
    end
    return false
end

function EventNotification( event )
    LastEvent = event
    return events.route( event )
end
