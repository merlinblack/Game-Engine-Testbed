require 'gui/dialogpanel'

function messageDialog(task)
    console.setVisible(false)
    mouse.show()

    local l = gui.mainLayer

    local h = (task.data.lines * 20) + 100
    local y = gui.screen.height/2 - h/2

    local text = MarkupText( l, 0, y + 48, task.data.message )
    text.markup.left = gui.screen.width/2 - round(text.markup.maxTextWidth/2, 0)

    local w = text.markup.maxTextWidth + 100
    if w < 300 then w = 300 end
    local x = gui.screen.width/2 - w/2

    local window = DialogPanel( l, x, y, w, h )

    local icon = Widget( l, x + 8, y + 8, 32, 32 )
    icon:background( "icon.info" )

    local btnOK = Button( l, x + w/2 - 40, y + h * 0.70, "ok" )
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
    local data = {}
    data.message = mess
    data.lines = countCR( mess )
    return createTask( messageDialog, data )
end

function countCR( str )
    local pos
    local count = 1
    pos = string.find( str, '\n' )
    while pos do
        pos = string.find( str, '\n', pos+1 )
        count = count + 1
    end
    return count
end
