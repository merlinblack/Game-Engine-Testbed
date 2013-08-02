layout_def = {
    id = 'layout_test',
    {
        type = 'panel',
        position = { 10, 10 },
        size = { 500, 100 },
        background = gui.dialogBackground,
        id = 'mainpanel',
        children = {
            {
                type = 'dragbutton'
            },
            {
                type = 'text',
                position = { 250, 20 },
                alignment = Gorilla.TextAlignment.Centre,
                font = 10,
                text = 'Hello World'
            },
            {
                type = 'button',
                position = { 210, -30 },
                caption = 'ok',
                id = 'ok_button',
                keycode = KeyCodes.KC_RETURN,
                action = function() log'layout_test:button press' end
            }
        }
    }
}

guiFactories = {}

function createLayout( layout )

    if layout.id == nil then
        error 'No id given for layout.'
    end

    _G[layout.id] = {}
    local ptr = _G[layout.id]
    ptr.left = 0
    ptr.top = 0
    ptr.width = gui.screen.width
    ptr.height = gui.screen.height

    for k,v in pairs( layout ) do
        print( 'Layout factory:', v.type )
        if type(guiFactories[v.type]) == 'function' then
            guiFactories[v.type]( ptr, v )
        else
            print( 'Ignored.' )
        end
    end

end

function getAbsolutePosition( parent, x, y )
    if x < 0 then
        x = parent.left + parent.width + x
    else
        x = parent.left + x
    end
    if y < 0 then
        y = parent.top + parent.height + y
    else
        y = parent.top + y
    end

    return x, y
end

function panelFactory( parent, parameters )
    print 'Widget Factory'
    local x, y, w, h, panel
    x = parameters.position[1]
    y = parameters.position[2]
    w = parameters.size[1]
    h = parameters.size[2]
    x, y = getAbsolutePosition( parent, x, y )
    print( x, y, w, h )
    panel =  Widget( gui.mainLayer, x, y, w, h )
    panel:background( parameters.background )
    parent[parameters.id] = panel
    panel.parent = parent

    for k,v in pairs( parameters.children ) do
        if type(guiFactories[v.type]) == 'function' then
            guiFactories[v.type]( panel, v )
        end
    end

end

function dragbuttonFactory( parent, parameters )
    print 'Drag Button Factory'
    print( parent.left, parent.top )
    local drag = DragButton( gui.mainLayer, parent.left, parent.top, parent )
    parent:addChild( drag )
end

function textFactory( parent, parameters )
    print 'Text Factory'
    local x, y, text, font, size, alignment
    x = parameters.position[1]
    y = parameters.position[2]
    x, y = getAbsolutePosition( parent, x, y )
    text = Text( gui.mainLayer, x, y,
        parameters.text,
        parameters.font,
        parameters.size,
        parameters.alignment )
    parent:addChild( text )
end

function buttonFactory( parent, parameters )
    print 'Button Factory'
    local x, y, button
    x = parameters.position[1]
    y = parameters.position[2]
    x, y = getAbsolutePosition( parent, x, y )
    button = Button( gui.mainLayer, x, y, parameters.caption )
    parent:addChild( button )
    if parameters.id then
        parent[parameters.id] = button
    end
    if parameters.action then
        button:setClickAction( parameters.action )
    end
    if parameters.keycode then
        button:setKeyCode( parameters.keycode )
    end
end

guiFactories['panel'] = panelFactory
guiFactories['dragbutton'] = dragbuttonFactory
guiFactories['text'] = textFactory
guiFactories['button'] = buttonFactory
