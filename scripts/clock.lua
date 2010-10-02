clock={}
clock.panel = Panel( gui.mainLayer, 0, gui.screen.height - 30, 300, 30 )
clock.panel:background( gui.dialogBackground )
clock.text = Text( gui.mainLayer, 150, gui.screen.height - 15, "Clock", 14 )
clock.panel:addChild( clock.text )
function clock.update()
    while clock.stop == false do
        clock.text.caption.text = os.date()
        wait(1)
    end
    clock.panel:destroy()
end
clock.stop = false
createTask( clock.update )
