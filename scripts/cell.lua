require "lualib"
cell={}
local x = gui.screen.width-300
cell.panel = Widget( gui.mainLayer, x, gui.screen.height-145, 300, 145 )
cell.panel:background( gui.dialogBackground )
cell.text = MarkupText( gui.mainLayer, x+25, gui.screen.height-140, "" )
cell.text2 = MarkupText( gui.mainLayer, x+135, gui.screen.height-140, "" )
cell.panel:addChild( cell.text )
cell.panel:addChild( cell.text2 )
cell.text.markup.text = "%9g Cost\nh Cost\npath\nOpen\nClosed"
cell.drag = DragButton( gui.mainLayer, x, gui.screen.height-145, cell.panel )
cell.panel:addChild( cell.drag )

function cell.update()
    local str
    local cellref
    local info
    while cell.stop == false do
        cellref = getCellUnderMouse()
        if cellref then
            info = cellref:info()
            str = ""
            str = str .. info.g .. "\n"
            str = str .. info.h .. "\n"
            str = str .. info.path .. "\n"
            str = str .. tostring(info.open) .. "\n"
            str = str .. tostring(info.closed)
            cell.text2.markup.text = str
        end
        wait(1)
    end
    gui.removeModeless(cell.panel)
    cell.panel:destroy()
end

cell.stop = false
createTask( cell.update )
gui.addModeless( cell.panel )
