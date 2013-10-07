
function loadLevel( name )
    local level = require( name )

    fixWalkmeshLinks( level )

    currentEntities = createEntities( level )
    currentLights = createLights( level )

    moveEntityToWaypoint( player, level.points.player_spawn )

    local loc = level.cameras.Camera_001.loc
    local rot = level.cameras.Camera_001.rot

    cameraControl.mainNode:setPosition( loc[1], loc[2], loc[3] )
    cameraControl.mainNode:setOrientation( Quaternion(rot[1], rot[2], rot[3], rot[4]) )
    cameraControl.mainNode:pitch(-90)
    cameraControl.pitchNode:setOrientation( Quaternion() )

    print 'Finished loading level'

end

function fixWalkmeshLinks( level )
    for _,mesh in pairs( level.meshes ) do
        if mesh.walkmesh ~= nil then
            if type(mesh.walkmesh) == 'string' then
                mesh.walkmesh = level.meshes[mesh.walkmesh]
            end
        end
    end
end

function moveEntityToWaypoint( entity, waypoint )
    local location = Ogre.Vector3( waypoint.loc[1], waypoint.loc[2], waypoint.loc[3] )
    local rotation = Ogre.Quaternion( waypoint.rot[1], waypoint.rot[2], waypoint.rot[3], waypoint.rot[4] )

    entity.node:setPositionV3( location )
    entity.node:setOrientation( rotation )
end

function createLights( level )
    local scene = Ogre.getSceneManager()
    local currentLights = {}
    for _,light in pairs( level.lights ) do
        local l = scene:createLightByName( light.name )
        l:setDiffuse( Ogre.ColourValue( light.colour[1], light.colour[2], light.colour[3], 1 ) )
        l:setPosition( Ogre.Vector3( light.loc[1], light.loc[2], light.loc[3] ) )
        currentLights[light.name] = l
    end
    return currentLights
end

function createEntities( level )
    local scene = Ogre.getSceneManager()
    local root  = scene:getRootSceneNode()
    local gm    = Engine.GameEntityManager.getSingleton()
    local entities = {}
    nv = Engine.NavigationMesh( Ogre.Vector3.ZERO, Ogre.Quaternion.IDENTITY, Ogre.Vector3.ZERO )

    for _,mesh in pairs( level.meshes ) do
        print( mesh.name )
        if mesh.isEntity == true then
            print 'Creating entity'
            local e = Engine.GameEntity()

            e.name = mesh.name
            e.mesh = scene:createEntity( mesh.filename )
            e.node = root:createChildSceneNode( e.name .. '_Node' )
            local pos = Ogre.Vector3( mesh.loc[1], mesh.loc[2], mesh.loc[3] )
            local rot = Ogre.Quaternion( mesh.rot[1], mesh.rot[2], mesh.rot[3], mesh.rot[4] )
            print( pos, rot )
            e.node:setPositionV3( pos )
            e.node:setOrientation( rot )
            e.node:attachObject( e.mesh )
            entities[e.name] = e
            gm:add(e)

            if mesh.walkmesh then
                local nm = scene:createEntity( mesh.walkmesh.filename )
                local pos = Ogre.Vector3( mesh.walkmesh.loc[1], mesh.walkmesh.loc[2], mesh.walkmesh.loc[3] )
                local rot = Ogre.Quaternion( mesh.walkmesh.rot[1], mesh.walkmesh.rot[2], mesh.walkmesh.rot[3], mesh.walkmesh.rot[4] )
                local scale = Ogre.Vector3( mesh.walkmesh.scale[1], mesh.walkmesh.scale[2], mesh.walkmesh.scale[3] )
                local tag = mesh.tag or 0
                print( 'Navmesh:', pos,rot,scale,tag )
                nv:addFromEntity( nm, pos, rot, scale, tag )
                scene:destroyEntity( nm )
                e.walkable = true
            end
        end
    end

    nv:computeNeighbours()

    print 'Finished loading entites'

    return entities

end
