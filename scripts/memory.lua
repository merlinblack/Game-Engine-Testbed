require 'gui/dialogpanel'
memorypanel={}
memorypanel.panel = DialogPanel( gui.mainLayer, 0, gui.screen.height - 30, 300, 30 )
memorypanel.memoryuse = Text( gui.mainLayer, 150, gui.screen.height - 15, "MEM", 14 )
memorypanel.panel:addChild( memorypanel.text )

function memorypanel.update()
    while memorypanel.stop == false do
        memorypanel.memoryuse.caption.text = collectgarbage( 'count' ) .. ' KB'
        wait(1)
    end
    memorypanel.panel:destroy()
    events.unsubscribe( 'EVT_WINDOW_RESIZE', memorypanel.resize )
end

memorypanel.stop = false
createTask( memorypanel.update )
