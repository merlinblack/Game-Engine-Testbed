function doLuaLogo()
    lualogo = Widget( gui.mainLayer, mouse.width - 64, mouse.height - 64, 64, 64 )
    lualogo.rect:backgroundImage 'lua.sm'
end

-- do as a task to wait for mouse width and height to be set.
-- doLuaLogo will run next frame.
createTask( doLuaLogo )
