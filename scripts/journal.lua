module( ... )

local quests = {}
local completed = {}

function addNewQuest( q )
    table.insert( quests, q )
    message( 'New quest: ' .. q.title )
end

function updateQuest( q )
end

function completeQuest( q )
    table.remove( quests, q )
    table.insert( completed, q )
    message( 'Quest ' .. q.title .. ' now complete' )
end

