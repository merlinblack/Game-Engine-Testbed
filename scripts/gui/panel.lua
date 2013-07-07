require 'gui/widget'

class 'Panel' (Widget)

function Panel:__init( layer, x, y, w, h )
    guiLog "Creating Panel"
    Widget.__init( self, layer, x, y, w, h )
end
