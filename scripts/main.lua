require 'scheduler'
require 'keys'
require 'gui/gui'

function version()
    message( 'Game Engine Testbed %3v0.2%r\nOgre 3d %3v1.7.2%r\nLua %3v5.1.4%r\nLuabind %3v0.9git%r' )
end

local old_dofile = dofile
function dofile( f )
    if f == nil then
        print 'stdin dofile tomfoolery not availible.'
        return
    end
    old_dofile( f )
end

-- Cache hash values.
Event.keydown = Event.hash "EVT_KEYDOWN"
Event.mousemove = Event.hash "EVT_MOUSEMOVE"
Event.mousedown = Event.hash "EVT_MOUSEDOWN"
Event.mouseup = Event.hash "EVT_MOUSEUP"
Event.windowresize = Event.hash "EVT_WINDOW_RESIZE"

function EventNotification( event )
    --print "Scripting System Received an Event"

    if event.type == Event.keydown  then
        if event.data.key == KeyCodes.KC_ESCAPE and #gui.modal == 0 then
            quit()
            return true
        end
    end

    if event.type == Event.windowresize then
        setViewportSize( event.data.width, event.data.height )
        if type(resizeEventListen) == 'table' then
            table.foreach( resizeEventListen,
            function(k,v)
                v( event.data.width, event.data.height )
            end
            )
        end
    end

    if event.type == Event.mousemove then
        mouseMoved( event.data.x, event.data.y, event.data.buttons )
    end

    if event.type == Event.mousedown then
        mouseMoved( event.data.x, event.data.y, event.data.buttons )
    end

    if event.type == Event.mouseup then
        mouseMoved( event.data.x, event.data.y, event.data.buttons )
    end

    if event.type == Event.keydown then
        keypressed( event.data.key )
    end

    return false
end

function addResizeListener( func )
    if type(func) ~= 'function' then
        error( 'You must supply a function to addResizeListener' )
    end
    if resizeEventListen == nil then
        resizeEventListen = {}
    end
    if type(resizeEventListen) ~= 'table' then
        error( 'resizeEventListen is not a table! Has it been redefined?' )
    end
    table.insert( resizeEventListen, func )
end

timeSinceStart = 0

yield = coroutine.yield

function FrameEnded( timeSinceLastFrame )
    timeSinceStart = timeSinceStart + timeSinceLastFrame
    currentTaskId = runNextTask( currentTaskId )
end

function quitProgram()
    queueEvent( Event( "MSG_QUIT" ) )
end

function wait( seconds )
    local endTime = timeSinceStart + seconds
    while timeSinceStart < endTime do
        yield()
    end
end

function waitForTask( task )
    while task.status() ~= 'dead' do
        yield()
    end
end

function testWait()
    print' Createing task.'
    local t = createTask( function() wait(10) end )
    print' Waiting for task to complete.'
    waitForTask( t )
    print' Task finished, continuing.'
end

function dump(t)
    table.foreach(t,print)
end

function info( classInstance )
    local ci = class_info( classInstance )
    local str=''

    print( 'Name: '..ci.name )

    for k,v in pairs( ci.methods ) do
        str = str..' '..k
    end

    print( 'Methods:'..str )
    str=''

    for k,v in pairs( ci.attributes ) do
        str = str..' '..v
    end

    print( 'Attributes:'..str )
end

function infotable( classInstance )
    local t={}
    local ci = class_info( classInstance )

    for k,v in pairs( ci.methods ) do
        t[k]=v
    end

    for k,v in pairs( ci.attributes ) do
        t[v]=k
    end

    return t
end


function setup()
    oldprint = print
    print = console.print
    log = console.log
    clear = console.clear

    local sb = Gorilla.Silverback.getSingleton()
    gui.screen = sb:createScreen( Ogre.getViewport( 0 ), 'atlas' )
    gui.mainLayer = gui.screen:createLayer(0)
    gui.dialogBackground = ColourValue( 0, 0, 0, .5 )

    setupMouse()

    --Something to look at...
    require 'fps'
    require 'clock'
    require 'cameracontrol'
    require 'walk'
    require 'test'

    print "Setup task completed"
end

createTask( setup )

print " *** main.lua processed. *** "
