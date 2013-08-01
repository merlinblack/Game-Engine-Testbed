class 'DragButton' (Widget)

function DragButton:__init( layer, x, y, parentToDrag )
    guiLog 'creating DragButton'

    if parentToDrag == nil then
        error( 'You must give a gui object to drag.' )
    end

    Widget.__init( self, layer, x, y, 16, 16 )

    self.parent = parentToDrag

    self:background( 'drag.normal' )

    self.state = "normal"
    self.oldstate = "normal"

    guiLog 'DragButton created'
end

function DragButton:mouseMoved( x, y, button )
    if self.rect:intersects( Ogre.Vector2(x, y) ) then
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

    self:updateVisualState()
end

function DragButton:updateVisualState()
    if self.state ~= self.oldstate then
        if self.state == "normal" then
            self.rect:backgroundImage("drag.normal")
        elseif self.state == "hover" then
            self.rect:backgroundImage("drag.hover")
        else
            self.rect:backgroundImage("drag.pressed")
        end
        self.oldstate = self.state
    end
end

function DragButton:lostMouse()
    if self.state == "click" then
        stopMouseDrag()
    end
    self.state = "normal"
    self:updateVisualState()

    for _,child in pairs( self.children ) do
        if child.lostMouse then child:lostMouse() end
    end
end
