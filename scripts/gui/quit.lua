function quitDialogTask()
    console.setVisible(false)
    mouse.show()
    
    local w = 300
    local h = 140
    local x = gui.screen.width/2 - w/2
    local y = gui.screen.height/2 - h/2

    local layer = gui.mainLayer

    local window = Panel( layer, x, y, w, h )
    window:background( gui.dialogBackground )

    local text = Text( layer, x + w/2, y+h*0.40, "Are you sure?", 14  )
    local icon = Panel( layer, x + 8, y + 8, 32, 32 )
    icon:background "icon.exit"

    local by = y + h * 0.6

    local btnOK = Button( layer, x + (w*2/8)-40, by, "Okay" )
    btnOK:setClickAction( function () window.OK=true end )
    btnOK:setKeyCode( KeyCodes.KC_RETURN )

    local btnCancel = Button( layer, x + (w*6/8)-40, by, "Cancel" )
    btnCancel:setClickAction( function () window.Cancel=true end )
    btnCancel:setKeyCode( KeyCodes.KC_ESCAPE )

    window:addChild( text )
    window:addChild( icon )
    window:addChild( btnOK )
    window:addChild( btnCancel )
    
    gui.pushModal( window )

    window.OK, window.Cancel = false, false
    while window.OK == false and window.Cancel == false do
        yield()
    end

    gui.popModal()

    window:destroy()

    mouse.hide()

    if window.OK==true then
        quitProgram()
    end

    quitting = false
end

function quit()
    if quitting == true then 
        return 
    end
    quitting = true
    createTask(quitDialogTask)
end

