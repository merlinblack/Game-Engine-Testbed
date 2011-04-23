class 'Text'

function Text:__init( layer, x, y, text, font, size, alignment )
    if alignment == nil then alignment = Gorilla.TextAlignment.Centre end
    if font == nil then font = 10 end
    self.layer = layer
    self.caption = layer:createCaption( font, x, y, text )
    self.caption . align = alignment
    self.caption . verticalAlign = Gorilla.VerticalAlignment.Middle
    self.caption . colour = ColourValue.White
    if size ~= nil then
        self.caption : size( size.x, size.y )
    end
    self.left = property( function(self) return self.caption.left end )
    self.top = property( function(self) return self.caption.top end )
end

function Text:destroy()
    self.layer:destroyCaption( self.caption )
end

function Text:move( x, y )
    self.caption.left = self.caption.left + x
    self.caption.top = self.caption.top + y
end
