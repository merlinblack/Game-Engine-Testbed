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

base = createGameEntity( root, 'Floor', 'Floor.mesh' )

mouse:show()

function test()
    t,d = gm:mousePick(mouse.x/mouse.width, mouse.y / mouse.height)
    print(#t)
    for i=1,#t do
        print( t[i].name, d[i] )
    end
end
