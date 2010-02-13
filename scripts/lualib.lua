-- A collection of usefull Lua functions for various things.
-- Some are mine, others are collected from the internet.
--

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
