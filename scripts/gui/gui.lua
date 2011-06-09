-- Basic GUI parts.
require 'gui/panel'
require 'gui/text'
require 'gui/markup'
require 'gui/button'
require 'gui/dragbutton'
require 'gui/checkbox'
require 'gui/control'
require 'gui/mouse'
require 'gui/keybinding'

-- Higher level parts.
require 'gui/quit'
require 'gui/message'

-- Testing
require 'gui/guitest'


-- High level switching and misc.
function setViewportSize( w, h )
    setMouseViewportSize( w, h )
end

function mouseMovedEventListener( event )
    x = event.data.x
    y = event.data.y
    buttons = event.data.buttons
    setMouseCursorPosition(x,y)
    gui.mouseMoved(x, y, buttons)
    return false
end

function keyPressedEventListener( event )
    if not console.isVisible() then
        key = event.data.key
        keybinder.keypressed( key )
        gui.keypressed( key )
    end
    return false
end

function keyReleasedEventListener( event )
    if not console.isVisible() then
        key = event.data.key
        keybinder.keyreleased( key )
        gui.keyreleased( key )
    end
    return false
end

function guiLog( str )
--    log( str )
end
