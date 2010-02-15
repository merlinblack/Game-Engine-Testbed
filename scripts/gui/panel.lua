class 'Panel'
Panel.count = 0

function Panel:__init( x,y,w,h,name )
    if name == nil then
        name = "Panel"..Panel.count
        Panel.count = Panel.count + 1
    end
    console.log( "Creating Panel: "..name )
    self.element = OverlayManager:createElement( "Panel", name, false )
    self.element:setParameter("left", ""..x)
    self.element:setParameter("top", ""..y)
    self.element:setParameter("width", ""..w)
    self.element:setParameter("height",""..h)
    self.children={}
    console.log"Panel Created"
end

function Panel:__finalize()
    print 'Destroying Panel.'
end

function Panel:mouseMoved( x, y, button )
    for _,child in pairs(self.children) do
        if child.mouseMoved then child:mouseMoved( x, y, button ) end
    end
end

function Panel:keypressed( key )
    for _,child in pairs(self.children) do
        if child.keypressed then child:keypressed( key ) end
    end
end

function Panel:addChild( widget )
    console.log'Adding child to Panel'
    if widget.element then
        table.insert(self.children, widget)
        self.element:addChild( widget.element )
    else
        error( 'Not a valid widget' )
    end
    console.log'Child now added to Panel'
end

class 'BorderPanel'( Panel )

function BorderPanel:__init( x,y,w,h,name )
    if name == nil then
        name = "BorderPanel"..Panel.count
        Panel.count = Panel.count + 1
    end
    console.log( "Creating BorderPanel: "..name )
    self.element = OverlayManager:createElement( "BorderPanel", name, false )
    self.element:setParameter("left", ""..x)
    self.element:setParameter("top", ""..y)
    self.element:setParameter("width", ""..w)
    self.element:setParameter("height",""..h)
    self.element:setParameter('border_material','gui/dialog.border')
    self.element:setParameter('border_size',           '0.0146484375 0.0146484375 0.019083969465649 0.019083969465649')
--    self.element:setParameter('border_size',           '.0146 .0146 .0190 .0190')
    self.element:setParameter('border_topleft_uv',     '.0000 .0000 .5859375 .5859375' )
    self.element:setParameter('border_topright_uv',    '.94140625 .0000 1.000 .5859375' )
    self.element:setParameter('border_bottomleft_uv',  '.0000 .94140625 .5859375 1.000' )
    self.element:setParameter('border_bottomright_uv', '.94140625 .94140625 1.000 1.000' )
    self.element:setParameter('border_top_uv',         '.5859375 .0000 .94140625 .5859375' )
    self.element:setParameter('border_left_uv',        '.0000 .5859375 .5859375 .94140625' )
    self.element:setParameter('border_bottom_uv',      '.5859375 .94140625 .94140625 1.000' )
    self.element:setParameter('border_right_uv',       '.94140625 .5859375 1.000 .94140625' )
    self.element:setMaterialName'gui/dialog.background'
    self.children={}
    console.log"BorderPanel Created"
end

