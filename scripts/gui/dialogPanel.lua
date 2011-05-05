function makeDialogPanel( layer, x, y, w, h )

    --  +----+------------------+----+
    --  | nw |       top        | ne |
    --  +----+------------------+----+
    --  |                            |
    --  |           window           |
    --  |                            |
    --  +----+------------------+----+
    --  | sw |       bot        | se |
    --  +----+------------------+----+

    local window = Panel( layer, x,     y+8,   w,    h-16 )
    local top =    Panel( layer, x+8,   y,     w-16, 8 )
    local bot =    Panel( layer, x+8,   y+h-8, w-16, 8 )
    local nw =     Panel( layer, x,     y,     8,    8 )
    local ne =     Panel( layer, x+w-8, y,     8,    8 )
    local se =     Panel( layer, x+w-8, y+h-8, 8,    8 )
    local sw =     Panel( layer, x,     y+h-8, 8,    8 )

    window:background( gui.dialogBackground )
    top:background( gui.dialogBackground )
    bot:background( gui.dialogBackground )
    nw:background('nw')
    ne:background('ne')
    se:background('se')
    sw:background('sw')

    window:addChild(top)
    window:addChild(bot)
    window:addChild(nw)
    window:addChild(ne)
    window:addChild(se)
    window:addChild(sw)

    return window
end
