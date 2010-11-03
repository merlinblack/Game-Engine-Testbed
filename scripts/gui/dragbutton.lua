class 'DragButton' (Panel)

function DragButton:__init( layer, x, y, parentToDrag )
    guiLog 'creating DragButton'

    if parentToDrag == nil then
        error( 'You must give a gui object to drag.' )
    end

    Panel.__init( self, layer, x, y, 24, 24 )

    self.parent = parentToDrag

    self:background( 'button.normal' )

    self.state = "normal"
    self.oldstate = "normal"

    guiLog 'DragButton created'
end

function DragButton:destroy()
    guiLog 'Destroying DragButton.'
    Panel.destroy(self)
end

function DragButton:keypressed( key )
end

function DragButton:mouseMoved( x, y, button )
    if self.rect:intersects( Vector2(x, y) ) then
        if button == 0 and self.state == "click" then -- click release
            stopMouseDrag()
        end
        self.state = "hover"
    else
        self.state = "normal"
        stopMouseDrag()
    end
    if self.state == "hover" and button == 1 then
        self.state = "click"
        startMouseDrag(self.parent)
    end
    if self.state ~= self.oldstate then
        if self.state == "normal" then
            self.rect:backgroundImage("button.normal")
        elseif self.state == "hover" then
            self.rect:backgroundImage("button.hover")
        else
            self.rect:backgroundImage("button.pressed")
        end
        self.oldstate = self.state
    end
end

function DragButton:move( x, y )
    if Panel.move( self, x, y ) then return end
    for _,child in pairs( self.children ) do
        if child.move then child:move( x, y ) end
    end
end
