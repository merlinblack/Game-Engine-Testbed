require 'class'

class 'Widget'

function Widget:__init( layer, x, y, w, h )
    guiLog "Creating Widget"
    self.layer = layer
    self.rect = layer:createRectangle( x, y, w, h )
    self.children={}
    self.left =   function( self ) return self.rect.left end
    self.top =    function( self ) return self.rect.top end
    self.width =  function( self ) return self.rect.width end
    self.height = function( self ) return self.rect.height end
    guiLog "Widget Created"
end

-- Can be called, directly or by garbage collection
-- if garbage collection runs it, it must not error!!
function Widget:destroy()
    guiLog 'Destroying Widget.'
    if self.rect then
        self.layer:destroyRectangle( self.rect )
    end
    self.rect = nil
    if type(self.children) == 'table' then
        for _,child in pairs(self.children) do
            child:destroy()
        end
    end
end

function Widget:mouseMoved( x, y, button )
    if not self.rect then
        print( 'No rect!', self.__type )
        return
    end
    if self.rect:intersects( Ogre.Vector2( x, y ) ) then
        for _,child in pairs(self.children) do
            if child.mouseMoved then child:mouseMoved( x, y, button ) end
        end
    else
        self:lostMouse()
    end
end

function Widget:keypressed( key )
    for _,child in pairs(self.children) do
        if child.keypressed then child:keypressed( key ) end
    end
end

function Widget:keyreleased( key )
    for _,child in pairs(self.children) do
        if child.keyreleased then child:keyreleased( key ) end
    end
end

function Widget:addChild( widget )
    table.insert(self.children, widget)
end

function Widget:background( back )
    if type(back) == 'string' then
        self.rect:backgroundImage( back )
    else
        self.rect:backgroundColour( back )
    end
end

-- Return true to cancel move, when it would move off view
function Widget:move( x, y )
    local left = self.rect.left
    local top = self.rect.top
    local width = self.rect.width
    local height = self.rect.height

    if left + x < 0 or top + y < 0 then return true end
    if left + width + x > mouse.width or top + height + y > mouse.height then
        return true
    end

    self.rect.left = self.rect.left + x
    self.rect.top = self.rect.top + y

    for _,child in pairs( self.children ) do
        if child.move then child:move( x, y ) end
    end
end

function Widget:lostMouse()
    for _,child in pairs( self.children ) do
        if child.lostMouse then child:lostMouse() end
    end
end
