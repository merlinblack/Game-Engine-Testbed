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

function clock.resize( event )
    clock.x = clock.panel.rect.top
    clock.destx = event.data.height - 30
    createTask( clock.animatePosition )
end

function clock.animatePosition()
    local speed = 25
    while clock.moving == true do wait(1) end
    clock.moving = true
    while math.abs(clock.x - clock.destx) > speed do
        if clock.x < clock.destx then
            clock.x = clock.x + speed
        else
            clock.x = clock.x - speed
        end
        clock.panel:move( 0, clock.x - clock.panel.rect.top )
        wait(.1)
    end
    clock.x = clock.destx
    clock.panel:move( 0, clock.x - clock.panel.rect.top )
    clock.moving = false
end

events.subscribe( 'EVT_WINDOW_RESIZE', clock.resize )
