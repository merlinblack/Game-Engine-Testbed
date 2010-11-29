class 'MarkupText'

function MarkupText:__init( layer, x, y, text, font, size )
    if font == nil then font = 10 end
    self.layer = layer
    self.markup = layer:createMarkupText( font, x, y, '%9' .. text )
    if size ~= nil then
        self.markup : size( size.x, size.y )
    end
end

function MarkupText:destroy()
    self.layer:destroyMarkupText( self.markup )
end

function MarkupText:move( x, y )
    self.markup.left = self.markup.left + x
    self.markup.top = self.markup.top + y
end
