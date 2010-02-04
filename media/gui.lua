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
    mouseGui:show()
end

function setMouseCursorPosition( x, y )
    mousePointer:setPosition( x-32, y-24 )
end

function quitDialog()
    console.setVisible(false)
    gui = OverlayManager:createOverlay()
    local window = Panel( -(260/2), -(140/2), 260, 140 )
    window.element:setMaterialName "gui/dialog.background"
    window.element:setParameter("horz_align","center")
    window.element:setParameter("vert_align","center")

    local text = Text( 130, 30, "Are you sure?" )

    local btnOK = Button( 10, 70, 113, "Okay" )
    btnOK:setClickAction( function () OK=true end )

    local btnCANCEL = Button( 140, 70, 113, "Cancel" )
    btnCANCEL:setClickAction( function () CANCEL=true end )

    window:addChild( text )
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
    messageOverlay = OverlayManager:createOverlay()
    local window = Panel( -(task.data.width/2), -(task.data.height/2), task.data.width, task.data.height)
    window.element:setParameter("horz_align","center")
    window.element:setParameter("vert_align","center")
    window.element:setMaterialName "gui/dialog.background"

    local text = Text( task.data.width/2, 30, task.data.message )

    local btnOK = Button( (task.data.width/2)-57, task.data.height-50, 113, "OK" )
    btnOK:setClickAction( function () OK=true end )
    window:addChild( text )
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
    messageOverlay=nil

end

function message( mess )
    -- Figure out width and height needed.
    -- Split mess by newline.
    local m = split(mess)

    height = (#m * 24 ) + 100

    local maxline=0
    for i,str in ipairs(m) do
        if #str > maxline then
            maxline = #str
        end
    end

    maxline = (maxline * 13 ) + 50

    if maxline > 200 then
        width = maxline
    else
        width = 200
    end

    data = {}
    data.message = mess
    data.width = width
    data.height = height
    createTask( messageDialog, data )
end

function split( str )
    local t = {}
    for line in string.gmatch( str, '[ %a]+' ) do
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

class 'Panel'
Panel.count = 0

function Panel:__init( x,y,w,h,name )
    if name == nil then
        name = "Panel"..Panel.count
        Panel.count = Panel.count + 1
    end
    console.log( "Creating Panel: "..name )
    self.element = OverlayManager:createElement( "Panel", name, false )
    self.element:setParameter("left", ""..x)
    self.element:setParameter("top", ""..y)
    self.element:setParameter("width", ""..w)
    self.element:setParameter("height",""..h)
    self.element:setParameter("metrics_mode","pixels")
    self.children={}
    console.log"Panel Created"
end

function Panel:__finalize()
    print 'Destroying Panel.'
end

function Panel:mouseMoved( x, y, button )
    for _,child in pairs(self.children) do
        if child.mouseMoved then child:mouseMoved( x, y, button ) end
    end
end

function Panel:addChild( widget )
    console.log'Adding child to Panel'
    if widget.element then
        table.insert(self.children, widget)
        self.element:addChild( widget.element )
    else
        error( 'Not a valid widget' )
    end
    console.log'Child now added to Panel'
end

class 'Text'
Text.count = 0

function Text:__init( x, y, text, alignment, size )
    if alignment == nil then alignment = "center" end
    if size == nil then size = 24 end
    self.element = OverlayManager:createElement( "TextArea", "Text"..Text.count, false )
    self.element:setParameter("left", ""..x)
    self.element:setParameter("top", ""..y)
    self.element:setParameter("width", "0")
    self.element:setParameter("height","0")
    self.element:setParameter("metrics_mode","pixels")
    self.element:setParameter('font_name','Console')
    self.element:setParameter('char_height',""..size)
    self.element:setParameter('colour_top','1 1 1')
    self.element:setParameter('colour_bottom','0.5 0.5 0.5')
    self.element:setParameter('alignment',alignment)
    self.element:setParameter('caption',text)
    Text.count=Text.count+1
end

class 'Button' (Panel)
Button.count = 0

function Button:__init( x, y, width, caption )
    console.log'creating Button'

    Panel.__init( self, x, y, width, 29, "Button"..Button.count )

    self.element:setMaterialName"gui/button.normal"

    self.txt = OverlayManager:createElement( "TextArea", "ButtonText"..Button.count, false )
    self.txt:setParameter("left", ""..width/2)
    self.txt:setParameter("top", "5")
    self.txt:setParameter("width", "0")
    self.txt:setParameter("height","0")
    self.txt:setParameter("metrics_mode","pixels")
    self.txt:setParameter('font_name','Console')
    self.txt:setParameter('char_height','20')
    self.txt:setParameter('colour','0 0 1')
    self.txt:setParameter('alignment','center')
    self.txt:setParameter('caption',caption)

    self.element:addChild(self.txt)

    self.state = "normal"
    self.oldstate = "normal"

    Button.count = Button.count+1
    console.log'Button created'
end

function Button:__finalize()
    print 'Destroying Button.'
end

function Button:changeCaption( caption )
    self.txt:setParameter('caption', caption )
end

function Button:setClickAction( action )
    if type(action) ~= "function" then
        error( "Click Action can only be set to a function." )
        return
    end
    self.clickAction = action
end

function Button:mouseMoved( x, y, button )
    if self.element:contains( x/_WIDTH, y/_HEIGHT ) then
        if button == 0 and self.state == "click" then -- click release
            if self.clickAction then self.clickAction() end
        end
        self.state = "hover"
    else
        self.state = "normal"
    end
    if self.state == "hover" and button == 1 then
        self.state = "click"
    end
    if self.state ~= self.oldstate then
        if self.state == "normal" then
            self.element:setMaterialName("gui/button.normal")
        elseif self.state == "hover" then
            self.element:setMaterialName("gui/button.hover")
        else
            self.element:setMaterialName("gui/button.pressed")
        end
        self.oldstate = self.state
    end
end

function dowidget()
    dialog = OverlayManager:createOverlay"mytestoverlay"

    local top = Panel( 1024-128, 0, 128, 256, 'Dialog' )
    top.element:setMaterialName'girls/3'

    top:addChild(Button( 14, 05, 100, "Boing!" ))
    top:addChild(Button( 14, 50, 100, "Splat!" ))
    top:addChild(Button( 14, 95, 100, "Bounce" ))
    top:addChild(Button( 14, 140, 100, "KaChonk" ))
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
