if tasks == nil then
    tasks = {}
end

function createTask( fn, data )
	local task = {}
	taskId = ( taskId or 0 ) + 1

	task.fn = fn
	task.id = taskId
	task.co = coroutine.create( fn )
	task.status = function () return coroutine.status( task.co ) end
	task.data = data

    tasks[task.id] = task

	return task
end

function runTask( id )
	local task = tasks[id]

	if task == nil then
		return
	end

	coroutine.resume( task.co, task )

	if task.status() == 'dead' then
		tasks[task.id] = nil
		return
	end
end

function runNextTask( prevId )
    local nextId

    tasksPerFrame = tasksPerFrame or 100

    for i=1,tasksPerFrame do

        nextId = next( tasks, prevId )

        if nextId == nil then break end

        runTask( nextId )

    end

	return nextId
end

function showTasks()
	print( 'Id','Function         ','Thread          ','Status' )
	for k,v in pairs(tasks) do
		print( k, v.fn, v.co, v.status() )
	end
end


