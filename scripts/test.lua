scene = Ogre.getSceneManager()
root = scene:getRootSceneNode()

gm = GameEntityManager.getSingleton()

function createGameEntity( parentNode, name, mesh )
    local e = GameEntity()

    e.name = name
    e.mesh = scene:createEntity( mesh )
    e.node = parentNode:createChildSceneNode( name .. '_Node' )
    e.node:attachObject( e.mesh )
    gm:add( e )

    return e
end

mouse:show()

function test()
    t,d = gm:mousePick(mouse.x/mouse.width, mouse.y / mouse.height)
    print(#t)
    for i=1,#t do
        print( t[i].name, d[i] )
    end
    return { t, d }
end

infVector = Vector3( math.huge, math.huge, math.huge )

function getWalkableEntityHitPosition( x, y )
    local geTable = gm:mousePick( x, y )

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
