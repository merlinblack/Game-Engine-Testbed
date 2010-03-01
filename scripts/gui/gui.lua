-- Basic GUI parts.
require 'gui/panel'
require 'gui/text'
require 'gui/button'
require 'gui/checkbox'
require 'gui/overlay'
require 'gui/control'
require 'gui/mouse'

-- Higher level parts.
require 'gui/quit'
require 'gui/message'

-- Testing
require 'gui/guitest'


-- High level switching and misc.
function setViewportSize( w, h )
    _WIDTH = w
    _HEIGHT = h
end

function mouseMoved(x, y, buttons)
    setMouseCursorPosition(x,y)
    gui.mouseMoved(x, y, buttons)
end

function keypressed( key )
    gui.keypressed( key )
end
