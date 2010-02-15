function quitDialogTask()
    console.setVisible(false)
    mouse.show()

    local quitDialog = OverlayManager:createOverlay()
    local window = Panel( -(0.2539/2), -(0.1692/2), 0.2539, 0.1692 )
    window.element:setMaterialName "gui/dialog.background"
    window.element:setParameter("horz_align","center")
    window.element:setParameter("vert_align","center")

    local text = Text( 0.1269, 0.0381, "Are you sure?" )
    local icon = Panel( 0, 0, 32/1024, 32/768 )
    icon.element:setMaterialName "gui/icon.exit"

    local btnOK = Button( 0.0146, 0.0890, 0.1003, "Okay" )
    btnOK:setClickAction( function () quitDialog.OK=true end )
    btnOK:setKeyCode( KeyCodes.KC_RETURN )

    local btnCancel = Button( 0.1360, 0.0890, 0.1003, "Cancel" )
    btnCancel:setClickAction( function () quitDialog.Cancel=true end )
    btnCancel:setKeyCode( KeyCodes.KC_ESCAPE )

    window:addChild( text )
    window:addChild( icon )
    window:addChild( btnOK )
    window:addChild( btnCancel )
    quitDialog:add2D(window.element)
    quitDialog:show()
    
    gui.pushModal( window )

    quitDialog.OK, quitDialog.Cancel = false, false
    while quitDialog.OK == false and quitDialog.Cancel == false do
        yield()
    end

    gui.popModal()

    quitDialog:hide()
    mouse.hide()
    guiDialog=nil

    if quitDialog.OK==true then
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

