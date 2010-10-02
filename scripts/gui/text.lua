class 'Text'

function Text:__init( layer, x, y, text, font, size, alignment )
    if alignment == nil then alignment = Gorilla.TextAlignment.Centre end
    if font == nil then font = 9 end
    self.layer = layer
    self.caption = layer:createCaption( font, x, y, text )
    self.caption . align = alignment
    self.caption . verticalAlign = Gorilla.VerticalAlignment.Middle
    if size ~= nil then
        self.caption : size( size.x, size.y )
    end
end

function Text:destroy()
    self.layer:destroyCaption( self.caption )
end

