function messageDialog(task)
    console.setVisible(false)
    mouse.show()
    
    local l = gui.mainLayer

    local h = (task.data.lines * 20) + 100
    local y = gui.screen.height/2 - h/2

    local text = MarkupText( l, 0, y + 32, task.data.message )
    text.markup.left = gui.screen.width/2 - round(text.markup.maxTextWidth/2, 0)

    local w = text.markup.maxTextWidth + 100
    if w < 300 then w = 300 end
    local x = gui.screen.width/2 - w/2

    local window = Panel( l, x, y, w, h )
    window:background( gui.dialogBackground ) 

    local icon = Panel( l, x + 8, y + 8, 32, 32 )
    icon:background( "icon.info" )

    local btnOK = Button( l, x + w/2 - 40, y + h * 0.70, "OK" )
    btnOK:setClickAction( function () window.OK=true end )
    btnOK:setKeyCode( KeyCodes.KC_RETURN )
    window:addChild( text )
    window:addChild( icon )
    window:addChild( btnOK )

    gui.pushModal( window )

    window.OK = false
    while window.OK == false do
        yield()
    end

    gui.popModal()

    window:destroy()
    mouse.hide()

end

function message( mess )
    -- Figure out width and height needed.
    -- Split mess by newline.
    local m = split(mess)

    local lines = #m 

    print( 'Lines:',#m )

    data = {}
    data.message = mess
    data.lines = lines
    return createTask( messageDialog, data )
end

function split( str )
    local t = {}
    for line in string.gmatch( str, '[ %w]+' ) do
        table.insert( t, line )
    end
    return t
end
