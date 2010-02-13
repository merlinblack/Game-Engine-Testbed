require 'lualib'

gui = gui or {}
gui.modeless = gui.modeless or {}
gui.modal = gui.modal or {}

function gui.pushModal( obj )
	if obj.mouseMoved and obj.keypressed then
		table.insert( gui.modal, 1, obj )
	else
		error "gui.pushModal given an invalid object."
	end
end

function gui.popModal()
	table.remove( gui.modal, 1 )
end

function gui.addModeless( obj )
	if obj.mouseMoved and obj.keypressed then
		table.insert( gui.modeless, obj )
	else
		error "gui.pushModeless given an invalid object."
	end
end

function gui.removeModeless( obj )
	local i = table.indexOf( gui.modeless, obj )
	if i then
		table.remove( gui.modeless, i )
	end
end

function gui.mouseMoved( x, y, buttons )
--	print( 'gui.mouseMoved', #gui.modal )
	if #gui.modal > 0 then
		-- Send event to first (on top) modal overlay/dialog.
		gui.modal[1].mouseMoved( x, y, buttons )
	else
		-- Send event to all modless overlays/dialogs.
		table.foreach( gui.modeless, function( i,v ) v.mouseMoved( x, y, buttons ) end )
	end
end
