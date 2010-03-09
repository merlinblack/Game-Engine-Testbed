root = Ogre.getRootSceneNode()

robotNode = root:createChildSceneNode'Robot'
floorNode = root:createChildSceneNode'Floor'

robot = Ogre.createEntity('Robot', 'robot.mesh')
floor = Ogre.createEntity('Floor', 'Floor.mesh')

robotNode:attachObject( robot )
floorNode:attachObject( floor )

robotNode:yaw(180)
robotNode:scale(.25,.25,.25)
robotNode:setPosition(0,25,0)


gm = GameEntityManager.getSingleton()

player = GameEntity()
player.name = "Mike"
player.mesh = robot;
player.node = robotNode;

gm:add( player )

base = GameEntity()
base.name = "Floor"
base.mesh = floor
base.node = floorNode
gm:add(base)

cam = Ogre.getCamera()
cam:setPosition(-100,70,-100)
cam:lookAt(0,25,0)
cam:setNearClipDistance(1)

mouse:show()

function test()
    t,d = gm:mousePick(_X,_Y)
    print(#t)
    for i=1,#t do
        print( t[i].name, d[i] )
    end
end
