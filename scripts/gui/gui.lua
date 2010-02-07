require 'gui/panel'
require 'gui/text'
require 'gui/button'
require 'gui/checkbox'


function setViewportSize( w, h )
    _WIDTH = w
    _HEIGHT = h
end

function mouseMoved()
end

function setupMouse()
    local mgr=getOverlayManager()
    mouseGui=mgr:createOverlay('MouseOverlay')
    mousePointer=mgr:createElement('Panel','MouseCursor',false)
    mousePointer:setParameter('material','mouse/arrow')
    mousePointer:setParameter('metrics_mode','pixels')
    mousePointer:setParameter('width','64')
    mousePointer:setParameter('height','48')
    mouseGui:add2D(mousePointer)
    function mouseMoved(x,y)
        setMouseCursorPosition(x,y)
    end
    mouseGui:setZOrder(650)
    --mouseGui:show()
end

function setMouseCursorPosition( x, y )
    mousePointer:setPosition( x-32, y-24 )
end

function quitDialog()
    console.setVisible(false)
	mouseGui:show()
    gui = OverlayManager:createOverlay()
    local window = Panel( -(0.2539/2), -(0.1692/2), 0.2539, 0.1692 )
    window.element:setMaterialName "gui/dialog.background"
    window.element:setParameter("horz_align","center")
    window.element:setParameter("vert_align","center")

    local text = Text( 0.1269, 0.0381, "Are you sure?" )
    local icon = Panel( 0, 0, 32/1024, 32/768 )
    icon.element:setMaterialName "gui/icon.exit"

    local btnOK = Button( 0.0146, 0.0890, 0.1003, "Okay" )
    btnOK:setClickAction( function () OK=true end )

    local btnCANCEL = Button( 0.1360, 0.0890, 0.1003, "Cancel" )
    btnCANCEL:setClickAction( function () CANCEL=true end )

    window:addChild( text )
    window:addChild( icon )
    window:addChild( btnOK )
    window:addChild( btnCANCEL )
    gui:add2D(window.element)
    gui.window = window
    gui:show()

    oldmouse = mouseMoved

    function mouseMoved( x, y, button )
        gui.window:mouseMoved( x, y, button )
        setMouseCursorPosition(x,y)
    end

    OK, CANCEL = false, false
    while OK == false and CANCEL == false do
        yield()
    end

    mouseMoved = oldmouse

    gui:hide()
	mouseGui:hide()
    gui=nil

    if OK==true then
        quitProgram()
    end
end

function quit()
    createTask(quitDialog)
end

function messageDialog(task)
    console.setVisible(false)
	mouseGui:show()
    messageOverlay = OverlayManager:createOverlay()
    local window = Panel( -(task.data.width/2), -(task.data.height/2), task.data.width, task.data.height)
    window.element:setParameter("horz_align","center")
    window.element:setParameter("vert_align","center")
    window.element:setMaterialName "gui/dialog.background"

    local text = Text( task.data.width/2, 0.0381, task.data.message )

    local icon = Panel( 0, 0, 32/1024, 32/768 )
    icon.element:setMaterialName "gui/icon.info"

    local btnOK = Button( (task.data.width/2)-0.0556, task.data.height-0.0636, 0.1103, "OK" )
    btnOK:setClickAction( function () OK=true end )
    window:addChild( text )
    window:addChild( icon )
    window:addChild( btnOK )
    messageOverlay.window = window
    messageOverlay:add2D( window.element )
    messageOverlay:show()

    local oldmouse = mouseMoved
    function mouseMoved( x, y, button )
        messageOverlay.window:mouseMoved( x, y, button )
        setMouseCursorPosition(x,y)
    end

    OK = false
    while OK == false do
        yield()
    end

    mouseMoved = oldmouse

    messageOverlay:hide()
	mouseGui:hide()
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
    createTask( messageDialog, data )
end

function split( str )
    local t = {}
    for line in string.gmatch( str, '[ %w]+' ) do
        table.insert( t, line )
    end
    return t
end

function setupOverlayManager()
    OverlayManager = getOverlayManager()
    OverlayManager.count = 0
    OverlayManager.createOverlay2 = OverlayManager.createOverlay
    function OverlayManager:createOverlay( name )
        if name == nil then
            name = "Overlay"..OverlayManager.count
            self.count=self.count+1
        end
        return self:createOverlay2( name )
    end
end

createTask( setupOverlayManager )
createTask( setupMouse )

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

    dialog:show()
    console.log'completed'
end

function domouse()
    oldmouse = mouseMoved
    function mouseMoved(x,y,button)
        oldmouse(x,y,button)
        dialog.top:mouseMoved(x,y,button)
    end
end
