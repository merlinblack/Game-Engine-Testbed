require 'gui/widget'

class 'Text'

function Text:__init( layer, x, y, text, font, size, alignment )
    if alignment == nil then alignment = Gorilla.Enum.TextAlign.Centre end
    if font == nil then font = 10 end
    self.layer = layer
    self.caption = layer:createCaption( font, x, y, text )
    self.caption : setAlign( alignment )
    self.caption : setVerticalAlign( Gorilla.Enum.VerticalAlign.Middle )
    self.caption . colour = Ogre.ColourValue.White
    if size ~= nil then
        self.caption : size( size.x, size.y )
    end
    self.left = function(self) return self.caption.left end
    self.top = function(self) return self.caption.top end
end

function Text:destroy()
    if self.caption then
        self.layer:destroyCaption( self.caption )
        self.caption = nil
    end
end

function Text:move( x, y )
    self.caption.left = self.caption.left + x
    self.caption.top = self.caption.top + y
end
