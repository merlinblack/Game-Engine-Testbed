function keydownlistener( event )
    ed = event.data
    inputdata = Engine.InputEventData.downcast( ed )
    if( inputdata ) then
        print( inputdata.text )
        print( string.byte(inputdata.text) )
    end
end
