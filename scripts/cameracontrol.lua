if Ogre.sceneNodeExists 'Pivot' then
    Ogre.getSceneNode'Pivot' : destroy()
end

if Ogre.sceneNodeExists 'CameraNode' then
    Ogre.getSceneNode'CameraNode' : destroy()
end

if Ogre.sceneNodeExists 'CameraPitch' then
    Ogre.getSceneNode'CameraPitch' : destroy()
end

if cameraControl then
    gui.removeModeless( cameraControl )
end

cameraControl = {}
local root = Ogre.getRootSceneNode()
cameraControl.pivotNode = root:createChildSceneNode"Pivot"
cameraControl.mainNode = cameraControl.pivotNode:createChildSceneNode"CameraNode"
cameraControl.pitchNode = cameraControl.mainNode:createChildSceneNode"CameraPitch"
local cam = Ogre.getCamera()
cam:setNearClipDistance(0.1)
cameraControl.pitchNode:attachObject(cam)
cameraControl.pitchNode:pitch(25)
cameraControl.mainNode:translate( Vector3( -80, 75, 0 ) )
cameraControl.autoRotateSpeed = .05
cameraControl.autoRotate = true

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
            cameraControl.mainNode:yaw( 1 );
        end
        if key == KeyCodes.KC_RIGHT or key == KeyCodes.KC_D then
            cameraControl.mainNode:yaw( -1 );
        end
        if key == KeyCodes.KC_UP or key == KeyCodes.KC_W then
            cameraControl.mainNode:translate( Vector3( 0, 0, -1 ), SceneNode.TS_LOCAL )
        end
        if key == KeyCodes.KC_DOWN or key == KeyCodes.KC_S then
            cameraControl.mainNode:translate( Vector3( 0, 0, 1 ), SceneNode.TS_LOCAL )
        end
        if key == KeyCodes.KC_HOME then
            cameraControl.pitchNode:pitch( -1 );
        end
        if key == KeyCodes.KC_END then
            cameraControl.pitchNode:pitch( 1 );
        end
    end
end

function cameraControl:mouseMoved( x, y, buttons )
end

gui.addModeless( cameraControl )

createTask(cameraControl.update)
