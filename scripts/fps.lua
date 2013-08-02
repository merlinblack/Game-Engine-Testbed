require "lualib"
require "gui/dialogpanel"
fps={}
fps.panel = DialogPanel( gui.mainLayer, 0, gui.screen.height-145, 300, 110 )
fps.text = MarkupText( gui.mainLayer, 25, gui.screen.height-133, "" )
fps.text2 = MarkupText( gui.mainLayer, 135, gui.screen.height-133, "" )
fps.panel:addChild( fps.text )
fps.panel:addChild( fps.text2 )
fps.text.markup.text = "%3Average FPS\nBest FPS\nWorst\nTriangles\nBatch Count"
fps.logo = Widget( gui.mainLayer, 210, gui.screen.height-125, 64, 64 )
fps.logo:background( 'ogrehead' )
fps.panel:addChild( fps.logo )
fps.drag = DragButton( gui.mainLayer, 8, gui.screen.height-137, fps.panel )
fps.panel:addChild( fps.drag )

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
    gui.removeModeless(fps.panel)
    fps.panel:destroy()
end

fps.stop = false
createTask( fps.update )
gui.addModeless( fps.panel )

function fps.resize( event )
    fps.x = fps.panel.top
    fps.destx = event.data.height - 145
    createTask( fps.animatePosition )
end

function fps.animatePosition()
    local speed = 25
    while fps.moving == true do wait(1) end
    fps.moving = true
    while math.abs(fps.x - fps.destx) > speed do
        if fps.x < fps.destx then
            fps.x = fps.x + speed
        else
            fps.x = fps.x - speed
        end
        fps.panel:move( 0, fps.x - fps.panel.top )
        wait(.1)
    end
    fps.x = fps.destx
    fps.panel:move( 0, fps.x - fps.panel.top )
    fps.moving = false
end
