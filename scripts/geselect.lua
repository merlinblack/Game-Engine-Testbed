-- Init
--
require 'gui/control'
require 'gui/mouse'

geSelect = geSelect or {}
geSelect.selected = geSelect.selected or {}
geSelect.lx = 0
geSelect.ly = 0

-- x and y are in pixels

function geSelect:mouseMoved( x, y, buttons )
    -- If the mouse has moved more than 5px in either direction...
    if math.abs(x-self.lx) > 5 or math.abs( y-self.ly ) > 5 then
        self.lx = x
        self.ly = y

        local _X = mouse.x / mouse.width
        local _Y = mouse.y / mouse.height

        local hit = gm:mousePick( _X, _Y )

        -- Unhighlight any selected entities.
        table.foreach( self.selected, 
            function( i, v )
                v:highlight( false )
            end
        )
        self.selected = {}

        -- Select any hit entities that can be selected.
        table.foreach( hit,
            function( i, v )
                if v.selectable then
                    if v:hitCheck( _X, _Y ) then
                        v:highlight( true )
                        table.insert( self.selected, v )
                    end
                end
            end
        )
    end
end

function geSelect:keypressed( key )
end

gui.removeModeless( geSelect )
gui.addModeless( geSelect )
