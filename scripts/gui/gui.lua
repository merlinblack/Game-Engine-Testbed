-- Basic GUI parts.
require 'gui/panel'
require 'gui/text'
require 'gui/markup'
require 'gui/button'
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

function mouseMoved(x, y, buttons)
    setMouseCursorPosition(x,y)
    gui.mouseMoved(x, y, buttons)
end

function keypressed( key )
    keybinder.keypressed( key )
    gui.keypressed( key )
end

function guiLog( str )
--    log( str )
end
