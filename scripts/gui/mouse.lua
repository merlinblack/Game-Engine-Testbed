mouse={}

function setupMouse()
    mouse.layer = gui.screen:createLayer(15)
    mouse.pointer = mouse.layer:createRectangle( 0, 0, 64, 48 )
    mouse.pointer : backgroundImage( 'yellow.arrow' )
end

function mouse.show()
    mouse.i = (mouse.i or 0 ) + 1

    if mouse.i > 0 then
        mouse.layer:show()
    end
end

function mouse.hide()
    mouse.i = (mouse.i or 1) - 1
    if mouse.i < 0 then 
        mouse.i = 0
    end
    if mouse.i == 0 then 
        mouse.layer:hide()
    end
end
    

function setMouseCursorPosition( x, y )
    mouse.pointer.left = x - 32
    mouse.pointer.top = y - 24
end
