class 'DialogPanel'

    --  +----+------------------+----+
    --  | nw |       top        | ne |
    --  +----+------------------+----+
    --  |                            |
    --  |           window           |
    --  |                            |
    --  +----+------------------+----+
    --  | sw |       bot        | se |
    --  +----+------------------+----+

function DialogPanel:__init( layer, x, y, w, h )
    guiLog ( "Creating DialogPanel" )
    self.layer = layer
    self.rect = {}
    self.rect[1] = layer:createRectangle( x,        y+8,    w,      h-16 )
    self.rect[2] = layer:createRectangle( x+8,      y,      w-16,   8    )
    self.rect[3] = layer:createRectangle( x+8,      y+h-8,  w-16,   8    )
    self.rect[4] = layer:createRectangle( x,        y,      8,      8    )
    self.rect[5] = layer:createRectangle( x+w-8,    y,      8,      8    )
    self.rect[6] = layer:createRectangle( x+w-8,    y+h-8,  8,      8    )
    self.rect[7] = layer:createRectangle( x,        y+h-8,  8,      8    )
    for i = 1,3 do
        self.rect[i]:backgroundColour( gui.dialogBackground )
    end
    self.rect[4]:backgroundImage( 'nw' )
    self.rect[5]:backgroundImage( 'ne' )
    self.rect[6]:backgroundImage( 'se' )
    self.rect[7]:backgroundImage( 'sw' )
    self.children={}
    self.left = property( function( self ) return self.rect[4].left end )
    self.top = property( function( self ) return self.rect[4].top end )
    self.width = property( function( self ) return self.rect[1].width end )
    self.height = property( function( self ) return self.rect[1].height+16 end )
    guiLog "DialogPanel Created"
end

function DialogPanel:destroy()
    guiLog 'Destroying DialogPanel.'
    for i = 1, 7 do
        self.layer:destroyRectangle( self.rect[i] )
    end
    for _,child in pairs(self.children) do
        child:destroy()
    end
end

function DialogPanel:mouseMoved( x, y, button )
    if self.rect[1]:intersects( Vector2( x, y ) ) then
        for _,child in pairs(self.children) do
            if child.mouseMoved then child:mouseMoved( x, y, button ) end
        end
    else
        self:lostMouse()
    end
end

function DialogPanel:keypressed( key )
    for _,child in pairs(self.children) do
        if child.keypressed then child:keypressed( key ) end
    end
end

function DialogPanel:addChild( widget )
    table.insert(self.children, widget)
end

--[[ This doesn't quite make sense at the moment...
function DialogPanel:background( back )
    if type(back) == 'string' then
        self.rect:backgroundImage( back )
    else
        self.rect:backgroundColour( back )
    end
end
--]]

-- Return true to cancel move, when it would move off view
function DialogPanel:move( x, y )
    local left = self.left
    local top = self.top
    local width = self.width
    local height = self.height

    if left + x < 0 or top + y < 0 then return true end
    if left + width + x > mouse.width or top + height + y > mouse.height then
        return true
    end

    for i = 1,7 do
        self.rect[i].left = self.rect[i].left + x
        self.rect[i].top = self.rect[i].top + y
    end

    for _,child in pairs( self.children ) do
        if child.move then child:move( x, y ) end
    end
end

function DialogPanel:lostMouse()
    for _,child in pairs( self.children ) do
        if child.lostMouse then child:lostMouse() end
    end
end
