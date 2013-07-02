-- Lua Class
--
-- Creates a class definition to have members added to
--
-- Example use:
--
-- class 'base'
--
-- function base:__init()       -- will be called on construction
--   self.xyz = 42
-- end
--
-- class 'derived' (base)       -- will inheirit base's members unless overridden
--
-- function derived:something( x )
--   self.xyz = x
--   self.abc = 'Hello World'
-- end
--
-- d=derived()
-- d:something()
-- print( d.xyz, d.abc )
--


function class( name )
    _G[name] = {
        __type = name,
        __luaclass = true
    }

    local newclass = _G[name]

    local metatable =
    {
        __call = function(...)
            local obj = {}

            setmetatable( obj, { __index = newclass } )

            if obj.__init ~= nil then
                obj:__init( select(2, ...) )
            end

            return obj
        end
    }

    setmetatable( newclass, metatable )

    return function( base ) metatable['__index'] = base end
end
