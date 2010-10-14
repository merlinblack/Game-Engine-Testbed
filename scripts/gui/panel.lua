class 'Panel'

function Panel:__init( layer, x, y, w, h )
    guiLog ( "Creating Panel" )
    self.layer = layer
    self.rect = layer:createRectangle( x, y, w, h )
    self.children={}
    guiLog "Panel Created"
end

function Panel:destroy()
    guiLog 'Destroying Panel.'
    self.layer:destroyRectangle( self.rect )
    for _,child in pairs(self.children) do
        child:destroy()
    end
end

function Panel:mouseMoved( x, y, button )
    if self.rect:intersects( Vector2( x, y ) ) then
        for _,child in pairs(self.children) do
            if child.mouseMoved then child:mouseMoved( x, y, button ) end
        end
    end
end

function Panel:keypressed( key )
    for _,child in pairs(self.children) do
        if child.keypressed then child:keypressed( key ) end
    end
end

function Panel:addChild( widget )
    table.insert(self.children, widget)
end

function Panel:background( back )
    if type(back) == 'string' then
        self.rect:backgroundImage( back )
    else
        self.rect:backgroundColour( back )
    end
end

function Panel:move( x, y )
    self.rect.left = self.rect.left + x
    self.rect.top = self.rect.top + y
    for _,child in pairs( self.children ) do
        if child.move then child:move( x, y ) end
    end
end
