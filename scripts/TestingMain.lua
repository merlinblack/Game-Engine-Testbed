print 'TestingMain.lua'

function dump(t)
    if type(t) == 'table' then
        for k,v in pairs(t) do
            print( k, v )
        end
    else
        print( 'Not a table' )
    end
end

gm = Engine.GameEntityManager.getSingleton()

function createGameEntity( name )
    print 'Create'
    local e = Engine.GameEntity()

    e.name = name
    print 'Add to manager'
    gm:add( e )

    print 'Return'
    return e
end

--a={}
--for i = 1,10 do
    --a[i] = createGameEntity( 'Player ' .. i )
--end

player = createGameEntity( 'Player' )

print 'Garbage collection'
collectgarbage();
dump( gm:getEntityList() )
print 'Garbage collection'
collectgarbage();
dump( gm:getEntityList() )
