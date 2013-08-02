require 'gui/widget'

class 'Checkbox' (Button)

function Checkbox:__init( layer, x, y )
    guiLog 'creating Checkbox'

    Widget.__init( self, layer, x, y, 16, 16 )

    self:background "checkbox.false"

    self.state = "normal"
    self.oldstate = "normal"
    self.value = false
    self.x = x
    self.y = y

    guiLog 'Checkbox created'
end

function Checkbox:destroy()
    Widget.destroy(self)
end

function Checkbox:setState( state )
    self.value = state
    self:background("checkbox."..tostring(self.value))
end

function Checkbox:getState()
    return self.value
end

function Checkbox:onClick()
    if self.clickAction then self.clickAction() end
    self.value = not self.value
end

function Checkbox:mouseMoved( x, y, button )
    if self.rect:intersects( Ogre.Vector2(x, y) ) then
        if button == 0 and self.state == "click" then -- click release
            self:onClick()
            self:background("checkbox."..tostring(self.value))
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

function Checkbox:updateVisualState()
    if self.state ~= self.oldstate then
        if self.state == "normal" then
            self:background("checkbox."..tostring(self.value))
            self.rect.width = 16
            self.rect.height = 16
            self.rect.left = self.x
            self.rect.top = self.y
        elseif self.state == "hover" then
            self.rect.width = 20
            self.rect.height = 20
            self.rect.left = self.x-2
            self.rect.top =  self.y-2
        end
        self.oldstate = self.state
    end
end

function Checkbox:move( x, y )
    if Widget.move( self, x, y ) then return end
    for _,child in pairs( self.children ) do
        if child.move then child:move( x, y ) end
    end
end

function Checkbox:lostMouse()
    self.state = "normal"
    self:updateVisualState()

    for _,child in pairs( self.children ) do
        if child.lostMouse then child:lostMouse() end
    end
end

-----------------------------------------------------------------
function testCheckbox()
    createTask(testCheckboxDialog)
end

function testCheckboxDialog()
    console.setVisible(false)

    local w = 260
    local h = 150
    local x = gui.screen.width/2 - w/2
    local y = gui.screen.height/2 - h/2
    local l = gui.mainLayer

    local window = Widget( l, x, y, w, h )
    window:background( gui.dialogBackground )

    local text1 = Text( l, x + 130, y + 30, "Option 1:", 14 )
    local checkbox1 = Checkbox( l, x + 180, y + 22 )

    local text2 = Text( l, x + 130, y + 60, "Option 2:", 14 )
    local checkbox2 = Checkbox( l, x + 180, y + 52 )

    local icon = Widget( l, x + 8, y + 8, 32, 32 )
    icon:background "icon.info"

    local btnOK = Button( l, x + 20, y + 100, "Okay" )
    btnOK:setClickAction( function () window.OK=true end )

    local btnCancel = Button( l, x + 140, y + 100, "Cancel" )
    btnCancel:setClickAction( function () window.CANCEL=true end )

    window:addChild( text1 )
    window:addChild( checkbox1 )
    window:addChild( text2 )
    window:addChild( checkbox2 )
    window:addChild( icon )
    window:addChild( btnOK )
    window:addChild( btnCancel )

    mouse.show()

    gui.pushModal( window )

    window.OK, window.CANCEL = false, false
    while window.OK == false and window.CANCEL == false do
        yield()
    end

    gui.popModal()

    window:destroy()
    mouse.hide()

    print( "Setting #1: "..tostring(checkbox1:getState()))
    print( "Setting #2: "..tostring(checkbox2:getState()))
    print( "OK        : "..tostring(window.OK))
    print( "Cancel    : "..tostring(window.CANCEL))

    console.setVisible(true)
end
