-- debug.lua
-- Routines for debugging!

function getCells( startPoint )

    createTask( function() clist=getConnectedCells( startPoint ) end )

end

function getConnectedCells( startPoint )
    local startCell = nv:getCellAtPoint( startPoint )
    local list = {}

    addCellAndNeighboursToList( list, startCell )

    return list
end

function addCellAndNeighboursToList( list, cell )
    if arghstop == true then return end

    if list[tostring(cell)] ~= nil then -- we've been here already.
        return
    end

    list[tostring(cell)] = cell:info()
    list[tostring(cell)].name = tostring(cell)
    list[tostring(cell)].cell = cell

    adjustCellVertices( list[tostring(cell)] )
    drawCell( list[tostring(cell)] )

    yield()

    for index = 1,3 do
        if list[tostring(cell)].links[index] ~= nil then
            addCellAndNeighboursToList( list, list[tostring(cell)].links[index] )
        end
    end
end

function adjustCellVertices( cellinfo )
    local centre = cellinfo.vertices[1]

    centre = centre + cellinfo.vertices[2]
    centre = centre + cellinfo.vertices[3]

    centre = centre / 3

    cellinfo.centre = centre

    for index = 1,3 do
        cellinfo.vertices[index] = adjustPointTowardsPoint( cellinfo.vertices[index], centre, 0.9 )
        cellinfo.vertices[index].y = cellinfo.vertices[index].y + 1
    end
end

function adjustPointTowardsPoint( point, anchor, percentage )
    -- Move 'point' towards 'anchor' so that the distance is
    -- 'percentage' of the original.
    local vector = ( point - anchor ) * percentage
    return anchor + vector
end

function drawCell( cellinfo )
    local mo = Ogre.ManualObject( cellinfo.name )
    local material = 'debug/yellow'
    if cellinfo.open then
        material = 'debug/cyan'
    end
    if cellinfo.closed then
        material = 'debug/red'
    end
    mo:begin( material, 4, 'General' )
    mo:position( cellinfo.vertices[1] )
    mo:position( cellinfo.vertices[2] )
    mo:position( cellinfo.vertices[3] )
    mo:finish()
    root:attachObject(mo)
    cellinfo.mo=mo
end

function clearDraw( list )
    if type(list) ~= 'table' then
        return
    end

    for _,v in pairs(list) do
        if type(v) == 'table' and v.mo ~= nil then
            v.mo:detachFromParent()
            v.mo = nil
        end
    end
end

function walkAndShow()
    local p = player.node:getPosition()
    walk()
    clearDraw(clist)
    getCells(p)
end

bind( KeyCodes.KC_P, walkAndShow )
