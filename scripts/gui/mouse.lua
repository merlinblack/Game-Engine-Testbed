mouse={}

function setupMouse()
    mouse.layer = gui.screen:createLayer(15)
    mouse.pointer = mouse.layer:createRectangle( 0, 0, 64, 48 )
    mouse.pointer : backgroundImage( 'yellow.arrow' )
    mouse.i = 1
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

function setMouseViewportSize( w, h )
    mouse.width = w
    mouse.height = h
end

function setMouseCursorPosition( x, y )
    mouse.x = x
    mouse.y = y
    mouse.pointer.left = x - 32
    mouse.pointer.top = y - 24
    if mouse.isDragging then
        mouse.draggedWindow:move( mouse.x - mouse.prev_x, mouse.y - mouse.prev_y )
        mouse.prev_x = mouse.x
        mouse.prev_y = mouse.y
    end
end

function startMouseDrag( window )
    mouse.isDragging = true
    mouse.draggedWindow = window
    mouse.prev_x = mouse.x
    mouse.prev_y = mouse.y
end

function stopMouseDrag()
    mouse.isDragging = false
end

