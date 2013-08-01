require 'gui/dialogpanel'
clock={}
clock.panel = DialogPanel( gui.mainLayer, 0, gui.screen.height - 30, 300, 30 )
clock.time = Text( gui.mainLayer, 150, gui.screen.height - 15, "Clock", 14 )
clock.panel:addChild( clock.time )

function clock.update()
    while clock.stop == false do
        clock.time.caption.text = os.date()
        wait(1)
    end
    events.unsubscribe( 'EVT_WINDOW_RESIZE', clock.resize )
    clock.panel:destroy()
end

clock.stop = false
createTask( clock.update )

function clock.resize( event )
    data = Engine.WindowEventData.downcast( event.data )
    clock.y = clock.panel:top()
    clock.desty = data.height - 30
    createTask( clock.animatePosition )
end

function clock.animatePosition()
    local speed = 5
    while clock.moving == true do wait(2) end
    clock.moving = true
    while math.abs(clock.y - clock.desty) > speed do
        if clock.y < clock.desty then
            clock.y = clock.y + speed
        else
            clock.y = clock.y - speed
        end
        clock.panel:move( 0, clock.y - clock.panel:top() )
        wait(.01)
    end
    clock.y = clock.desty
    clock.panel:move( 0, clock.y - clock.panel:top() )
    clock.moving = false
end

events.subscribe( 'EVT_WINDOW_RESIZE', clock.resize )
