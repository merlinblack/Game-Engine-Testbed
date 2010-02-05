local root = Ogre.getRootSceneNode()
cameraNode = root:createChildSceneNode"CameraNode"
local cam = Ogre.getCamera()
cameraNode:attachObject(cam)
camerarotatespeed = .05

function updateCamera()
	while stopcamera == false do
		cameraNode:yaw(camerarotatespeed)
		wait(.01)
	end
end

stopcamera = false;

createTask(updateCamera)
