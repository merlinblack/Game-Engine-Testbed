require 'gui/widget'

class 'Button' (Widget)

function Button:__init( layer, x, y, caption )
    guiLog 'creating Button'

    Widget.__init( self, layer, x, y, 80, 24 )

    self:background( 'button.normal' )

    self.text = layer:createCaption( 10, x, y, caption )
    self.text : setAlign( Gorilla.Enum.TextAlign.Centre )
    self.text : setVerticalAlign( Gorilla.Enum.VerticalAlign.Middle )
    self.text : size( 80, 24 )

    self.state = "normal"
    self.oldstate = "normal"
    self.key = KeyCodes.KC_UNKNOWN

    guiLog 'Button created'
end

function Button:destroy()
    guiLog 'Destroying Button.'
    Widget.destroy(self)
    if self.text then
        self.layer:destroyCaption(self.text)
        self.text = nil
    end
end

function Button:changeCaption( caption )
    self.text.text = caption
end

function Button:setClickAction( action )
    if type(action) ~= "function" then
        error( "Click Action can only be set to a function." )
    end
    self.clickAction = action
end

function Button:setKeyCode( key )
    self.key = key
end

function Button:keypressed( key )
    if self.key ~= KeyCodes.KC_UNKNOWN then
        if key == self.key then
            self.state = "click"
            self:updateVisualState()
        end
    end
end

function Button:keyreleased( key )
    if self.key ~= KeyCodes.KC_UNKNOWN then
        if key == self.key then
            self:onClick()
        end
    end
end

function Button:onClick()
    if self.clickAction then self.clickAction() end
end

function Button:mouseMoved( x, y, button )
    if self.rect:intersects( Ogre.Vector2(x, y) ) then
        if button == 0 and self.state == "click" then -- click release
            self:onClick()
        end
        self.state = "hover"
    else
        self.state = "normal"
    end
    if self.state == "hover" and button == 1 then
        self.state = "click"
    end

    self:updateVisualState()
end

function Button:updateVisualState()
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

function Button:move( x, y )
    if Widget.move( self, x, y ) then return end
    self.text.left = self.text.left + x
    self.text.top = self.text.top + y
    for _,child in pairs( self.children ) do
        if child.move then child:move( x, y ) end
    end
end

function Button:lostMouse()
    self.state = "normal"
    self:updateVisualState()

    for _,child in pairs( self.children ) do
        if child.lostMouse then child:lostMouse() end
    end
end
