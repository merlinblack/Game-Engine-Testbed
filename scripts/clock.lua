clock={}
clock.overlay = OverlayManager:createOverlay()
clock.panel = Panel( 0, 738/768, 300/1024, 30/768 )
clock.panel.element:setMaterialName( "gui/dialog.background" )
clock.overlay:add2D( clock.panel.element )
clock.text = Text( 150/1024, 5/786, "Clock" )
clock.panel:addChild( clock.text )
function clock.update()
    while clock.stop == false do
        clock.text.element:setParameter('caption', os.date() )
        wait(1)
    end
end
clock.stop = false
createTask( clock.update )
clock.overlay:show()
