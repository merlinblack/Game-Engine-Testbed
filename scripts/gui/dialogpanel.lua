require 'gui/widget'

class 'DialogPanel' (Widget)

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
    Widget.__init( self, layer, x, y, w, h ) -- use invisible rect for checking intersection
    self.rect:noBackground()
    self.rects = {}
    self.rects[1] = layer:createRectangle( x,        y+8,    w,      h-16 )
    self.rects[2] = layer:createRectangle( x+8,      y,      w-16,   8    )
    self.rects[3] = layer:createRectangle( x+8,      y+h-8,  w-16,   8    )
    self.rects[4] = layer:createRectangle( x,        y,      8,      8    )
    self.rects[5] = layer:createRectangle( x+w-8,    y,      8,      8    )
    self.rects[6] = layer:createRectangle( x+w-8,    y+h-8,  8,      8    )
    self.rects[7] = layer:createRectangle( x,        y+h-8,  8,      8    )
    for i = 1,3 do
        self.rects[i]:backgroundColour( gui.dialogBackground )
    end
    self.rects[4]:backgroundImage( 'nw' )
    self.rects[5]:backgroundImage( 'ne' )
    self.rects[6]:backgroundImage( 'se' )
    self.rects[7]:backgroundImage( 'sw' )
    guiLog "DialogPanel Created"
end

function DialogPanel:destroy()
    guiLog 'Destroying DialogPanel.'
    if self.rects[1] then
        for i = 1, 7 do
            self.layer:destroyRectangle( self.rects[i] )
        end
    end
    self.rects = {}
    Widget.destroy( self )
end

function DialogPanel:background( back )
    -- No op
end

-- Return true to cancel move, when it would move off view
function DialogPanel:move( x, y )
    if Widget.move( self, x, y ) ~= true then
        for i = 1,7 do
            self.rects[i].left = self.rects[i].left + x
            self.rects[i].top = self.rects[i].top + y
        end
    end
end
