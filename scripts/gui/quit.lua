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

    local btnCANCEL = Button( 0.1360, 0.0890, 0.1003, "Cancel" )
    btnCANCEL:setClickAction( function () quitDialog.CANCEL=true end )

    window:addChild( text )
    window:addChild( icon )
    window:addChild( btnOK )
    window:addChild( btnCANCEL )
    quitDialog:add2D(window.element)
    quitDialog.window = window
    quitDialog:show()
    
    function quitDialog.mouseMoved( x, y, button )
        quitDialog.window:mouseMoved( x, y, button )
    end
    function quitDialog.keypressed( key )
    end

    gui.pushModal( quitDialog )

    quitDialog.OK, quitDialog.CANCEL = false, false
    while quitDialog.OK == false and quitDialog.CANCEL == false do
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

