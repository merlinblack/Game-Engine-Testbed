require "lualib"
fps={}
fps.panel = Panel( gui.mainLayer, 0, gui.screen.height-145, 300, 110 )
fps.panel:background( gui.dialogBackground )
fps.text = MarkupText( gui.mainLayer, 25, gui.screen.height-130, "" )
fps.text2 = MarkupText( gui.mainLayer, 125, gui.screen.height-130, "" )
fps.panel:addChild( fps.text )
fps.text.markup.text = "%6Average FPS\nBest FPS\nWorst\nTriangles\nBatch Count"
fps.logo = Panel( gui.mainLayer, 210, gui.screen.height-125, 64, 64 )
fps.logo:background( 'ogrehead' )
function fps.update()
    local stats
    local str
    while fps.stop == false do
        stats = Ogre.getStats()
        str = ""
        str = str .. round(stats.avgFPS,1) .. "\n"
        str = str .. round(stats.bestFPS,1) .. "\n"
        str = str .. round(stats.worstFPS,1) .. "\n"
        str = str .. stats.triangleCount .. "\n"
        str = str .. stats.batchCount 
        fps.text2.markup.text = str
        wait(1)
    end
    fps.panel:destroy()
end
fps.stop = false
createTask( fps.update )
