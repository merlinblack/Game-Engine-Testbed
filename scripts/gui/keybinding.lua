keybindings = keybindings or {}
keybinder = {}

function keybinder.bind( key, func )
    if type(func) ~= 'function' then
        error 'You can only bind a key to a function'
    end

    keybindings[key] = func
end

function keybinder.keypressed( key )
    if keybindings[key] then
        keybindings[key](key)
    end
end

function keybinder.unbind( key )
    keybindings[key] = nil
end

bind = keybinder.bind
unbind = keybinder.unbind
