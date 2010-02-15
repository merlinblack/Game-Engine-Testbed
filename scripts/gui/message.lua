function messageDialog(task)
    console.setVisible(false)
    mouse.show()
    local messageOverlay = OverlayManager:createOverlay()
    local window = Panel( -(task.data.width/2), -(task.data.height/2), task.data.width, task.data.height)
    window.element:setParameter("horz_align","center")
    window.element:setParameter("vert_align","center")
    window.element:setMaterialName "gui/dialog.background"

    local text = Text( task.data.width/2, 0.0381, task.data.message )

    local icon = Panel( 0, 0, 32/1024, 32/768 )
    icon.element:setMaterialName "gui/icon.info"

    local btnOK = Button( (task.data.width/2)-0.0556, task.data.height-0.0636, 0.1103, "OK" )
    btnOK:setClickAction( function () messageOverlay.OK=true end )
    window:addChild( text )
    window:addChild( icon )
    window:addChild( btnOK )
    messageOverlay.window = window
    messageOverlay:add2D( window.element )
    messageOverlay:show()

    function messageOverlay.mouseMoved( x, y, button )
        messageOverlay.window:mouseMoved( x, y, button )
    end
    function messageOverlay.keypressed( key )
        if key == KeyCodes.KC_ENTER then
            messageOverlay.OK = true
        end
    end
    
    gui.pushModal( messageOverlay )

    messageOverlay.OK = false
    while messageOverlay.OK == false do
        yield()
    end

    gui.popModal()

    messageOverlay:hide()
    mouse.hide()
    messageOverlay=nil

end

function message( mess )
    -- Figure out width and height needed.
    -- Split mess by newline.
    local m = split(mess)

    height = (#m * 0.025 ) + 0.1272

    print( 'Lines:',#m, height )

    local maxline=0
    for i,str in ipairs(m) do
        if #str > maxline then
            maxline = #str
        end
    end

    maxline = (maxline * 0.0127 ) + 0.0636

    if maxline > 0.1953 then
        width = maxline
    else
        width = 0.1953
    end

    data = {}
    data.message = mess
    data.width = width
    data.height = height
    return createTask( messageDialog, data )
end

function split( str )
    local t = {}
    for line in string.gmatch( str, '[ %w]+' ) do
        table.insert( t, line )
    end
    return t
end
