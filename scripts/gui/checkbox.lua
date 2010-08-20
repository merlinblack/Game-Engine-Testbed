function testCheckbox()
    createTask(testCheckboxDialog)
end

function testCheckboxDialog()
    console.setVisible(false)

    local dialog = OverlayManager:createOverlay()
    local window = Panel( -(260/1024/2), -(150/768/2), 260/1024, 150/768 )
    window.element:setMaterialName "gui/dialog.background"
    window.element:setParameter("horz_align","center")
    window.element:setParameter("vert_align","center")

    local text1 = Text( 130/1024, 30/768, "Option 1:" )
    local checkbox1 = Checkbox( 180/1024,30/768 )

    local text2 = Text( 130/1024, 60/768, "Option 2:" )
    local checkbox2 = Checkbox( 180/1024, 60/768 )

    local icon = Panel( 0, 0, 32/1024, 32/768 )
    icon.element:setMaterialName "gui/icon.info"

    local btnOK = Button( 15/1024, 100/768, 0.1003, "Okay" )
    btnOK:setClickAction( function () dialog.OK=true end )

    local btnCancel = Button( 140/1024, 100/768, 0.1003, "Cancel" )
    btnCancel:setClickAction( function () dialog.CANCEL=true end )

    window:addChild( text1 )
    window:addChild( checkbox1 )
    window:addChild( text2 )
    window:addChild( checkbox2 )
    window:addChild( icon )
    window:addChild( btnOK )
    window:addChild( btnCancel )
    dialog:add2D(window.element)
    dialog:show()

    mouse.show()

    gui.pushModal( window )

    dialog.OK, dialog.CANCEL = false, false
    while dialog.OK == false and dialog.CANCEL == false do
        yield()
    end

    gui.popModal()

    dialog:hide()
    mouse.hide()
    dialog=nil

    guiLog( "Setting #1: "..tostring(checkbox1:getState()))
    guiLog( "Setting #2: "..tostring(checkbox2:getState()))
end

class 'Checkbox' (Button)

function Checkbox:__init( x, y )
    guiLog 'creating Button'

    Panel.__init( self, x, y, 16/1024, 16/768, "Button"..Button.count )

    self.element:setMaterialName"gui/checkbox.false"

    self.state = "normal"
    self.oldstate = "normal"
    self.value = false
    self.x = x
    self.y = y

    Button.count = Button.count+1
    guiLog 'Checkbox created'
end

function Checkbox:setState( state )
    self.value = state
    self.element:setMaterialName("gui/checkbox."..tostring(self.value))
end

function Checkbox:getState()
    return self.value
end

function Checkbox:onClick()
    if self.clickAction then self.clickAction() end
    self.value = not self.value
end

function Checkbox:mouseMoved( x, y, button )
    if self.element:contains( x/_WIDTH, y/_HEIGHT ) then
        if button == 0 and self.state == "click" then -- click release
            self:onClick()
            self.element:setMaterialName("gui/checkbox."..tostring(self.value))
        end
        self.state = "hover"
    else
        self.state = "normal"
    end
    if self.state == "hover" and button == 1 then
        self.state = "click"
    end
    if self.state ~= self.oldstate then
        if self.state == "normal" then
            self.element:setMaterialName("gui/checkbox."..tostring(self.value))
            self.element:setDimensions( 16/1024, 16/768 )
            self.element:setPosition( self.x, self.y )
        elseif self.state == "hover" then
            self.element:setDimensions( 20/1024, 20/768 )
            self.element:setPosition( self.x-(2/1024), self.y-(2/768) )
        end
        self.oldstate = self.state
    end
end
