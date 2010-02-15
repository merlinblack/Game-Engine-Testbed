function dowidget()
    dialog = OverlayManager:createOverlay"mytestoverlay"

    local top = Panel( (1024-128)/1024, 0, 128/1024, 256/768, 'Dialog' )
    top.element:setMaterialName'gui/dialog.background'

    top:addChild(Button( 14/1024, 05/768, 100/1024, "Boing!" ))
    top:addChild(Button( 14/1024, 50/768, 100/1024, "Splat!" ))
    top:addChild(Button( 14/1024, 95/768, 100/1034, "Bounce" ))
    top:addChild(Button( 14/1024, 140/768, 100/1024, "KaChonk" ))
    console.log'Buttons created, adding top element to dialog overlay'
    dialog:add2D(top.element)
    dialog.top = top

    gui.addModeless( top )

    dialog:show()
    mouse.show()

    console.log'do widget completed'
end

function undowidget()
    gui.removeModeless( dialog )
    dialog = nil
    mouse.hide()
end

function testButtons()
    local testButtons = OverlayManager:createOverlay"testButtons"
    local top = Panel( (1024-128)/1024, (768-80)/768, 128/1024, 80/768 )
    testButtons:add2D( top.element )
    top.element:setMaterialName"gui/dialog.background"

    gui.addModeless( top )
    mouse.show()
    testButtons:show()

    function removeTestButtons()
        gui.removeModeless( testButtons )
        testButtons:hide()
        testButtons = nil
    end
    
    print'Creating buttons'

    local quitButton = Button( 14/1024, 5/768, 100/1024, "Quit" )
    local messageButton = Button( 14/1024, 40/768, 100/1024, "Message" )

    top:addChild( quitButton )
    top:addChild( messageButton )

    quitButton:setClickAction( quit )
    messageButton:setClickAction( function() message( "This is a test." ) end )

    quitButton:setKeyCode( KeyCodes.KC_Q )
    messageButton:setKeyCode( KeyCodes.KC_M )
end
