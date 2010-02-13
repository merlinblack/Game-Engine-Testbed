function setupOverlayManager()
    OverlayManager = getOverlayManager()
    OverlayManager.count = 0
    OverlayManager.createOverlay2 = OverlayManager.createOverlay
    function OverlayManager:createOverlay( name )
        if name == nil then
            name = "Overlay"..OverlayManager.count
            self.count=self.count+1
        end
        return self:createOverlay2( name )
    end
end

createTask( setupOverlayManager )
