-- Some amusing things with positioning.
-- Animating the position of a dialog in various
-- ways.
--
dowidget()
e = dialog.top.element
left = 0
gx=-10

dialog.top.children[1]:setClickAction( function() createTask( bounce ) end )
dialog.top.children[3]:setClickAction( function() createTask( wizzbounce ) end )

function bounce()
    local dx = -80
    local x = _WIDTH
    local bounces = 0

    while bounces < 4 do
        x = x + dx
        dx = dx + gx
        if x < (left) and dx < 0 then
            -- Do not cross left side
            x = left
            -- Quarter speed and change direction.
            dx = dx / 4 
            dx = dx * -1
            bounces = bounces + 1
        end
        e:setPosition(x/_WIDTH,200/768)
        wait(0.00001)
    end
    e:setPosition(left/_WIDTH, 200/768)
end

function wizz()
    local x = left
    local speed = 1

    while x < _WIDTH do
        x = x + (speed * speed)
        speed = speed + 1
        e:setPosition( x/_WIDTH, 200/768 )
        wait(0.00001)
    end
end

function wizzbounce()
    wizz()
    wait(2)
    bounce()
end
