require 'scheduler'
require 'keys'
--require 'gui/gui'
--require 'autocomplete'
require 'events'

function version()
--    message( '%@14%' .. versionString()..'\n\n' )
end

local old_dofile = dofile
function dofile( f )
    if f == nil then
        print 'stdin dofile tomfoolery not availible.'
        return
    end
    old_dofile( f )
end

function getCounter()
    local count=0
    return function()
        count=count+1
        return count
    end
end

timeSinceStart = 0

yield = coroutine.yield

function FrameEnded( timeSinceLastFrame )
    timeSinceStart = timeSinceStart + timeSinceLastFrame
    currentTaskId = runNextTask( currentTaskId )
end

function quitProgram()
    Engine.queueEvent( Engine.Event( "MSG_QUIT" ) )
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
    if type(t) == 'table' then
        table.foreach(t,print)
    else
        --table.foreach( class_info( t ), print )
        print( 'Not a table' )
    end
end

--[[
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
        t[v]=classInstance[v]
    end

    return t
end
--]]

function quitKeyListener( event )
    if event.data.key == KeyCodes.KC_ESCAPE and #gui.modal == 0 then
        quit()
        return true
    end
end

function resizeListener( event )
    print( 'resizeListener' )
    --setViewportSize( event.data.width, event.data.height )
end

function setup()
    oldprint = print
    print = console.print
    log = console.log
    clear = console.clear

    --local sb = Gorilla.Silverback.getSingleton()
    --gui.screen = sb:createScreen( Ogre.getViewport( 0 ), 'atlas' )
    --gui.mainLayer = gui.screen:createLayer(0)
    --gui.dialogBackground = ColourValue( .999, .999, .878, .6 )
    --gui.dialogBackground = ColourValue( 11/255, 34/255, 35/255, .9 )

    --setupMouse()

    --events.subscribe( 'EVT_MOUSEMOVE',     mouseMovedEventListener )
    --events.subscribe( 'EVT_MOUSEDOWN',     mouseMovedEventListener )
    --events.subscribe( 'EVT_MOUSEUP',       mouseMovedEventListener )
    --events.subscribe( 'EVT_KEYUP',         quitKeyListener )
    --events.subscribe( 'EVT_KEYDOWN',       keyPressedEventListener )
    --events.subscribe( 'EVT_KEYUP',         keyReleasedEventListener )
    events.subscribe( 'EVT_WINDOW_RESIZE', resizeListener )

    --Something to look at...
    --require 'fps'
    --require 'clock'
    --require 'cameracontrol'
    --require 'walk'
    --require 'test'
    --require 'mydebug'
    --require 'loadlevel'

    --player = createGameEntity( root, 'Robot', 'robot.mesh' )
    --player.node:scale( .5, .5, .5 )

    --loadLevel 'levels/level2'

    print "Setup task completed"
end

createTask( setup )

print " *** main.lua processed. *** "
