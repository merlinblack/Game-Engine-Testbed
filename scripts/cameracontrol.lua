local scene = Ogre.getSceneManager()

if scene:hasSceneNode 'Pivot' then
    scene:destroySceneNodeByName 'Pivot'
end

if scene:hasSceneNode 'CameraNode' then
    scene:destroySceneNodeByName 'CameraNode'
end

if scene:hasSceneNode 'CameraPitch' then
    scene:destroySceneNodeByName 'CameraPitch'
end

if cameraControl then
    gui.removeModeless( cameraControl )
end

cameraControl = {}
local root = scene:getRootSceneNode()
cameraControl.pivotNode = root:createChildSceneNode "Pivot"
cameraControl.mainNode = cameraControl.pivotNode:createChildSceneNode "CameraNode"
cameraControl.pitchNode = cameraControl.mainNode:createChildSceneNode "CameraPitch"
local cam = scene:getCamera("MainCamera")
cam:setNearClipDistance(0.1)
cameraControl.pitchNode:attachObject(cam)
cameraControl.autoRotateSpeed = .05
cameraControl.autoRotate = false
cameraControl.forward = false
cameraControl.backward = false
cameraControl.left = false
cameraControl.right = false
cameraControl.stop = false;

function cameraControl.update()
    while cameraControl.stop == false do
        if cameraControl.autoRotate == true then
            cameraControl.pivotNode:yaw(cameraControl.autoRotateSpeed)
        else
            if cameraControl.forward == true then
                cameraControl.mainNode:translate( Ogre.Vector3( 0, 0, -2 ), Ogre.SceneNode.TS_LOCAL )
            end
            if cameraControl.backward == true then
                cameraControl.mainNode:translate( Ogre.Vector3( 0, 0, 2 ), Ogre.SceneNode.TS_LOCAL )
            end
            if cameraControl.left == true then
                cameraControl.mainNode:yaw( 2 );
            end
            if cameraControl.right == true then
                cameraControl.mainNode:yaw( -2 );
            end
        end

        wait(.01)
    end
end

function cameraControl.forwardKey() cameraControl.forward = not cameraControl.forward end
function cameraControl.backwardKey() cameraControl.backward = not cameraControl.backward end
function cameraControl.leftKey() cameraControl.left = not cameraControl.left end
function cameraControl.rightKey() cameraControl.right = not cameraControl.right end
function cameraControl.autoRotateKey() cameraControl.autoRotate = not cameraControl.autoRotate end

bind( KeyCodes.KC_UP,    cameraControl.forwardKey,    cameraControl.forwardKey )
bind( KeyCodes.KC_DOWN,  cameraControl.backwardKey,   cameraControl.backwardKey )
bind( KeyCodes.KC_LEFT,  cameraControl.leftKey,       cameraControl.leftKey )
bind( KeyCodes.KC_RIGHT, cameraControl.rightKey,      cameraControl.rightKey )

bind( KeyCodes.KC_W,     cameraControl.forwardKey,    cameraControl.forwardKey )
bind( KeyCodes.KC_S,     cameraControl.backwardKey,   cameraControl.backwardKey )
bind( KeyCodes.KC_A,     cameraControl.leftKey,       cameraControl.leftKey )
bind( KeyCodes.KC_D,     cameraControl.rightKey,      cameraControl.rightKey )

bind( KeyCodes.KC_R,     cameraControl.autoRotateKey, cameraControl.autoRotateKey )

function cameraControl:keypressed( key )
    if key == KeyCodes.KC_R then
        cameraControl.autoRotate = not cameraControl.autoRotate
    end
    if cameraControl.autoRotate == false then
        if key == KeyCodes.KC_HOME then
            cameraControl.pitchNode:pitch( -1 );
        end
        if key == KeyCodes.KC_END then
            cameraControl.pitchNode:pitch( 1 );
        end
        if key == KeyCodes.KC_PGUP then
            cameraControl.mainNode:translate( Ogre.Vector3( 0, 5, 0 ), Ogre.SceneNode.TS_WORLD )
        end
        if key == KeyCodes.KC_PGDOWN then
            cameraControl.mainNode:translate( Ogre.Vector3( 0, -5, 0 ), Ogre.SceneNode.TS_WORLD )
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
