require 'gui/widget'

class 'MarkupText' (Widget)

function MarkupText:__init( layer, x, y, text, font, size )
    if font == nil then font = 10 end
    self.layer = layer
    self.markup = layer:createMarkupText( font, x, y, text )
    if size ~= nil then
        self.markup : size( size.x, size.y )
    end
    children={}
end

function MarkupText:destroy()
    if self.markup then
        self.layer:destroyMarkupText( self.markup )
        self.markup = nil
    end
end

function MarkupText:move( x, y )
    self.markup.left = self.markup.left + x
    self.markup.top = self.markup.top + y
end
