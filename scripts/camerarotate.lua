local root = Ogre.getRootSceneNode()
cameraNode = root:createChildSceneNode"CameraNode"
cameraPitch = cameraNode:createChildSceneNode"CameraPitch"
local cam = Ogre.getCamera()
cameraPitch:attachObject(cam)
camerarotatespeed = .05
cameraPitch:pitch(25)

function updateCamera()
	while stopcamera == false do
		cameraNode:yaw(camerarotatespeed)
		wait(.01)
	end
end

stopcamera = false;

createTask(updateCamera)
