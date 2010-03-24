require "lualib"
fps={}
fps.overlay = OverlayManager:createOverlay()
fps.panel = Panel( 0, 623/768, 300/1024, 110/768 )
fps.panel.element:setMaterialName( "gui/dialog.background" )
fps.overlay:add2D( fps.panel.element )
fps.text = Text( 5/1024, 10/786, "Clock", "left" )
fps.panel:addChild( fps.text )
function fps.update()
    local stats
    local str
    while fps.stop == false do
        stats = Ogre.getStats()
        str = ""
        str = str .. "Average FPS:\t" .. round(stats.avgFPS,1) .. "\n"
        str = str .. "Best    FPS:\t" .. round(stats.bestFPS,1) .. "\n"
        str = str .. "Worst   FPS:\t" .. round(stats.worstFPS,1) .. "\n"
        str = str .. "Triangles  :\t" .. stats.triangleCount .. "\n"
        str = str .. "Batch Count:\t" .. stats.batchCount 
        fps.text.element:setParameter('caption', str )
        wait(1)
    end
end
fps.stop = false
createTask( fps.update )
fps.overlay:show()
