keybindings = keybindings or { up = {}, down = {} }
keybinder = {}

function keybinder.bind( key, func, func2 )
    if type(func) ~= 'function' then
        error 'Second parameter (key down func) must be a function'
    end
    if func2 and type(func2) ~= 'function' then
        error 'Thrid parameter (key release func) must be a function or nil )'
    end

    keybindings.down[key] = func
    keybindings.up[key] = func2
end

function keybinder.keypressed( key )
    if keybindings.down[key] then
        keybindings.down[key](key)
    end
end

function keybinder.keyreleased( key )
    if keybindings.up[key] then
        keybindings.up[key]( key )
    end
end

function keybinder.unbind( key )
    keybindings.down[key] = nil
    keybindings.up[key] = nil
end

function keybinder.list()
    local skiplist={}
    skiplist[keybindings] = true
    skiplist[gui] = true
    print 'Key down list'
    for k,v in pairs( keybindings.down ) do
        key = table.keyOf( KeyCodes, k )
        print( key, getName(v, skiplist) )
    end

    print 'Key up list'
    for k,v in pairs( keybindings.up ) do
        key = table.keyOf( KeyCodes, k )
        print( key, getName(v, skiplist) )
    end
end



bind = keybinder.bind
unbind = keybinder.unbind
