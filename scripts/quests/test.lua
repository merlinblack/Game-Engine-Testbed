-- Import required Gloabls
--
local print = print
local player = player
local Vector3 = Vector3
local _NAME = ...

-- Modulize
module( _NAME )

-- Quest Globals
--
local GoalPosition = Vector3( 0, 15, 0 )

function init()
    print( 'Initialising as ' .. _NAME )
end

function update()
    print 'Checking current goal for completion...'
    print 'Nope'
    return false
end

local function Goal_GetToPoint()
end
