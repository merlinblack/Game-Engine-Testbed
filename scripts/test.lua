scene = Ogre.getSceneManager()
root = scene:getRootSceneNode()

gm = GameEntityManager.getSingleton()

function createGameEntity( parentNode, name, mesh )
    local e = GameEntity()
    
    e.name = name
    e.mesh = scene:createEntity( mesh )
    e.node = parentNode:createChildSceneNode( name .. 'Node' )
    e.node:attachObject( e.mesh )
    gm:add( e )

    return e
end

player = createGameEntity( root, 'Robot', 'robot.mesh' )

player.node:yaw(180)
player.node:scale(.25,.25,.25)
player.node:setPosition(0,25,0)

--base = createGameEntity( root, 'A Strange base.', 'floor.mesh' )
--door = createGameEntity( root, 'Door of Death', 'door.mesh' )
island = createGameEntity( root, 'Island of Dark Green Colour', 'level2.mesh' )
island.walkable = true
--[[
function door:close()
    local path = { Vector3( 25, 0, 0 ), Vector3( 0, 0, 0 ) }
    createTask( door.followPathTask, path )
end

function door:open()
    local path = { Vector3( 25, 0, 0 ), Vector3( 25, 50, 0 ) }
    createTask( door.followPathTask, path )
end

bind( KeyCodes.KC_O, function() door:open() end )
bind( KeyCodes.KC_C, function() door:close() end )

function door.followPathTask(task)
    while door.isMoving == true do wait(1) end

    if door.speed == nil then door.speed = 5 end

    door.isMoving = true

    for _,v in pairs(task.data) do
        local m = MovementAnimation( door.node, v, door.speed )
        am:add(m)
        m:start()
        while not m:isFinished() do wait(2) end
    end

    door.isMoving = false
end
--]]
mouse:show()

function test()
    t,d = gm:mousePick(mouse.x/mouse.width, mouse.y / mouse.height)
    print(#t)
    for i=1,#t do
        print( t[i].name, d[i] )
    end
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

function loadBridge()
    bridge = createGameEntity( root, 'A bridge too far', 'bridge.mesh' )
    bridge2 = createGameEntity( root, 'Another bridge too far', 'bridge.mesh' )
    bridge.walkable = true
    bridge2.walkable = true
    bridge_walkable = scene:createEntity( 'bridge_walk.mesh' )
    local n = bridge.node
    n:setPosition( -32, 0, -96 )
    nv:addFromEntity( bridge_walkable, n:getPosition(), n:getOrientation(), Vector3.UNIT_SCALE )
    n = bridge2.node
    n:setPosition( -256, 0, 96 )
    n:yaw( 90 )
    nv:addFromEntity( bridge_walkable, n:getPosition(), n:getOrientation(), Vector3.UNIT_SCALE )
    nv:computeNeighbours()
    nv.show = true
end
