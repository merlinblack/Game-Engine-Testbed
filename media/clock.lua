clock={}
clock.overlay = OverlayManager:createOverlay()
clock.panel = Panel( 0, 686, 400, 100 )
clock.panel.element:setMaterialName( "gui/dialog.background" )
clock.overlay:add2D( clock.panel.element )
clock.text = Text( 64, 30, "Clock", "left" )
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
