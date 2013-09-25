scene = Ogre.getSceneManager()
root = scene:getRootSceneNode()

gm = Engine.GameEntityManager.getSingleton()

function createGameEntity( parentNode, name, mesh )
    local e = Engine.GameEntity()

    e.name = name
    e.mesh = scene:createEntity( mesh )
    e.node = parentNode:createChildSceneNode( name .. '_Node' )
    e.node:attachObject( e.mesh )
    gm:add( e )

    return e
end

mouse:show()

function test()
    local ret = gm:mousePick(mouse.x/mouse.width, mouse.y / mouse.height)
    t = ret.entities
    d = ret.distances
    print(#t)
    for i=1,#t do
        print( t[i].name, d[i] )
    end
    return ret
end

infVector = Ogre.Vector3( math.huge, math.huge, math.huge )

function getWalkableEntityHitPosition( x, y )
    local ret = gm:mousePick( x, y )
    local geTable = ret.entites

    if #geTable == 0 then
        return infVector
    end

    -- Get the game entiy where the mouse ray intersects the nearest game object,
    -- that has 'walkable' set to true.
    local ge
    for index = 1, #geTable do
        if geTable[index].walkable == true then
            ge = geTable[index]
            break
        end
    end

    if ge then
        return ge:hitPosition( x, y )
    else
        return infVector
    end
end


function teleport()

    -- Teleport to where the mouse ray intersects the nearest game object,
    -- that is walkable.

    local p = getWalkableEntityHitPosition( mouse.x / mouse.width, mouse.y / mouse.height )
    if p ~= infVector then
        player.node:setPosition( p )
    else
        print 'Nothing under the mouse cursor.'
    end
end

function getCellUnderMouse()

    -- Grab the Navigation cell where the mouse ray intersects the nearest game object,
    -- that is walkable.

    local p = getWalkableEntityHitPosition( mouse.x / mouse.width, mouse.y / mouse.height )
    if p ~= infVector then
        return nv:getCellAtPoint( p )
    end
    return nil
end
