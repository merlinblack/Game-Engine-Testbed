-- quest.lua
-- Class for a single quest.

class 'Quest'

function Quest:__init( QuestID )
    self.questID = QuestID
    self.finished = false

    -- Load quest scripts
    self.scripts = require( 'quests.' .. QuestID )

    self.scripts.init(self)
end

function Quest:update()
    return self.scripts.update( self )
end
