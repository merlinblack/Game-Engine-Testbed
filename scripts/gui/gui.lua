-- Basic GUI parts.
require 'gui/dialogpanel'
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
--require 'gui/guitest'

function setViewportSize( w, h )
    setMouseViewportSize( w, h )
end

function mouseMovedEventListener( event )
    data = Engine.InputEventData.downcast( event.data )
    if not data then
        return
    end
    setMouseCursorPosition(data.x,data.y)
    gui.mouseMoved(data.x, data.y, data.buttons)
    return false
end

function keyPressedEventListener( event )
    if not console.isVisible() then
        data = Engine.InputEventData.downcast( event.data )
        if not data then
            return
        end
        keybinder.keypressed( data.key )
        gui.keypressed( key )
    end
    return false
end

function keyReleasedEventListener( event )
    if not console.isVisible() then
        data = Engine.InputEventData.downcast( event.data )
        if not data then
            return
        end
        keybinder.keyreleased( data.key )
        gui.keyreleased( data.key )
    end
    return false
end

function guiLog( str )
    --log( os.date() .. ' - ' .. str )
end
