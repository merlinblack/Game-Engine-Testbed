require 'scheduler'
require 'keys'
require 'gui/gui'

local old_dofile = dofile
function dofile( f )
    if f == nil then
        print 'stdin dofile tomfoolery not availible.'
        return
    end
    old_dofile( f )
end

function EventNotification( event )
    --print "Scripting System Received an Event"

    if event.type == Event.hash "EVT_KEYDOWN"  then
        if event.data.key == KeyCodes.KC_ESCAPE and #gui.modal == 0 then
            quit()
            return true
        end
    end

    if event.type == Event.hash "EVT_WINDOW_RESIZE" then
        setViewportSize( event.data.width, event.data.height )
    end

    if event.type == Event.hash "EVT_MOUSEMOVE" then
        mouseMoved( event.data.x, event.data.y, event.data.buttons )
    end

    if event.type == Event.hash "EVT_MOUSEDOWN" then
        mouseMoved( event.data.x, event.data.y, event.data.buttons )
    end

    if event.type == Event.hash "EVT_MOUSEUP" then
        mouseMoved( event.data.x, event.data.y, event.data.buttons )
    end

    if event.type == Event.hash "EVT_KEYDOWN" then
        keypressed( event.data.key )
    end

    return false
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

function setup()
    oldprint = print
    print = console.print
    log = console.log
    clear = console.clear

    --Something to look at...
    require 'fps'
    require 'clock'
    require 'cameracontrol'
    require 'walk'
    require 'test'

    print "Setup task completed"
end

createTask( setup )

print "main.lua processed."
