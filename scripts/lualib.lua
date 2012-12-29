-- A collection of usefull Lua functions for various things.
-- Some are mine, others are collected from the internet.
--

if table.foreach == nil then
    function table.foreach( tbl, func )
        for k,v in pairs( tbl ) do
            func( k, v )
        end
    end
end

function table.set(t) -- set of list
    local u = { }
    for _, v in ipairs(t) do u[v] = true end
    return u
end

function table.find(f, l) -- find element v of l satisfying f(v)
    for _, v in ipairs(l) do
        if f(v) then
            return v
        end
    end
    return nil
end

function table.indexOf( t, e )
    for i, v in ipairs(t) do
        if v == e then
            return i
        end
    end
    return nil
end

function table.keyOf( t, e )
    for k, v in pairs(t) do
        if v == e then
            return k
        end
    end
    return nil
end

function round( num, digits )
    local shift = 10 ^ digits
    return math.floor( num * shift + 0.5 ) / shift
end

-- Given a reference (usally a function) find the first
-- "name" in the Global name space.
function getName( ref, skip )
	local visited = {}
	if type(skip)=='table' then -- copy skip table
		for k,v in pairs(skip) do
			visited[k]=v
		end
	end
	return searchTableHier( ref, visited, _G )
end

function searchTableHier( ref, visited, t )

	visited[t] = true

	-- Search table for function
	for k,v in pairs(t) do
		if v == ref then
			return k
		end
	end

	-- Not found, try each table (if any) within t recursively
	for k,v in pairs(t) do
		if type(v) == 'table' and visited[v] ~= true then
			local ret = searchTableHier( ref, visited, v )
			if ret then
				return k .. '.' .. ret
			end
		end
	end

	return nil
end
