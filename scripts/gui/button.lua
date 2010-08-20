class 'Button' (Panel)
Button.count = 0

function Button:__init( x, y, width, caption )
    guiLog 'creating Button'

    Panel.__init( self, x, y, width, 0.03689, "Button"..Button.count )

    self.element:setMaterialName"gui/button.normal"

    self.txt = OverlayManager:createElement( "TextArea", "ButtonText"..Button.count, false )
    self.txt:setParameter("left", ""..width/2)
    self.txt:setParameter("top", "0.0063")
    self.txt:setParameter("width", "0")
    self.txt:setParameter("height","0")
    self.txt:setParameter('font_name','Console')
    self.txt:setParameter('char_height','.025')
    self.txt:setParameter('colour','1 1 1')
    self.txt:setParameter('alignment','center')
    self.txt:setParameter('caption',caption)

    self.element:addChild(self.txt)

    self.state = "normal"
    self.oldstate = "normal"
    self.key = KeyCodes.KC_UNKNOWN

    Button.count = Button.count+1
    guiLog 'Button created'
end

function Button:__finalize()
    guiLog 'Destroying Button.'
end

function Button:changeCaption( caption )
    self.txt:setParameter('caption', caption )
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
            self:onClick()
        end
    end
end

function Button:onClick()
    if self.clickAction then self.clickAction() end
end

function Button:mouseMoved( x, y, button )
    if self.element:contains( x/_WIDTH, y/_HEIGHT ) then
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
    if self.state ~= self.oldstate then
        if self.state == "normal" then
            self.element:setMaterialName("gui/button.normal")
        elseif self.state == "hover" then
            self.element:setMaterialName("gui/button.hover")
        else
            self.element:setMaterialName("gui/button.pressed")
        end
        self.oldstate = self.state
    end
end
