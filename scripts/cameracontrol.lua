local scene = Ogre.getSceneManager()

if scene:hasSceneNode 'Pivot' then
    scene:destroySceneNode'Pivot'
end

if scene:hasSceneNode 'CameraNode' then
    scene:destroySceneNode'CameraNode'
end

if scene:hasSceneNode 'CameraPitch' then
    scene:destroySceneNode'CameraPitch'
end

if cameraControl then
    gui.removeModeless( cameraControl )
end

cameraControl = {}
local root = scene:getRootSceneNode()
cameraControl.pivotNode = root:createChildSceneNode"Pivot"
cameraControl.mainNode = cameraControl.pivotNode:createChildSceneNode"CameraNode"
cameraControl.pitchNode = cameraControl.mainNode:createChildSceneNode"CameraPitch"
local cam = scene:getCamera("MainCamera")
cam:setNearClipDistance(0.1)
cameraControl.pitchNode:attachObject(cam)
cameraControl.pitchNode:pitch(-25)
cameraControl.mainNode:translate( Vector3( -80, 75, 0 ) )
cameraControl.mainNode:yaw( -90 )
cameraControl.autoRotateSpeed = .05
cameraControl.autoRotate = false
cameraControl.stop = false;

function cameraControl.update()
    while cameraControl.stop == false do
        if cameraControl.autoRotate == true then
            cameraControl.pivotNode:yaw(cameraControl.autoRotateSpeed)
        end
        wait(.01)
    end
end

function cameraControl:keypressed( key )
    if key == KeyCodes.KC_R then
        cameraControl.autoRotate = not cameraControl.autoRotate
    end
    if cameraControl.autoRotate == false then
        if key == KeyCodes.KC_LEFT or key == KeyCodes.KC_A then
            cameraControl.mainNode:yaw( 2 );
        end
        if key == KeyCodes.KC_RIGHT or key == KeyCodes.KC_D then
            cameraControl.mainNode:yaw( -2 );
        end
        if key == KeyCodes.KC_UP or key == KeyCodes.KC_W then
            cameraControl.mainNode:translate( Vector3( 0, 0, -2 ), SceneNode.TS_LOCAL )
        end
        if key == KeyCodes.KC_DOWN or key == KeyCodes.KC_S then
            cameraControl.mainNode:translate( Vector3( 0, 0, 2 ), SceneNode.TS_LOCAL )
        end
        if key == KeyCodes.KC_HOME then
            cameraControl.pitchNode:pitch( -1 );
        end
        if key == KeyCodes.KC_END then
            cameraControl.pitchNode:pitch( 1 );
        end
        if key == KeyCodes.KC_PGUP then
            cameraControl.mainNode:translate( Vector3( 0, 5, 0 ))
        end
        if key == KeyCodes.KC_PGDOWN then
            cameraControl.mainNode:translate( Vector3( 0, -5, 0 ))
        end
    end
end

function cameraControl:mouseMoved( x, y, buttons )
    if buttons == 2 then
        oldx = oldx or x
        oldy = oldy or y
        local dx = x - oldx
        local dy = y - oldy

        cameraControl.mainNode:yaw( dx/10 )
        cameraControl.pitchNode:pitch( dy/10 )

        oldx = x
        oldy = y
    else
        oldx = nil
        oldy = nil
    end
end

gui.addModeless( cameraControl )

createTask(cameraControl.update)
