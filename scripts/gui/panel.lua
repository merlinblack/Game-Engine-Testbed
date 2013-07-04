require 'gui/widget'

class 'Panel' (Widget)

function Panel:__init( layer, x, y, w, h )
    guiLog "Creating Panel"
    self.__base.__init( self, layer, x, y, w, h )
    guiLog "Panel Created"
end
