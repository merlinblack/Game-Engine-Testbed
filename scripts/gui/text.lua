class 'Text'
Text.count = 0

function Text:__init( x, y, text, alignment, size )
    if alignment == nil then alignment = "center" end
    if size == nil then size = .025 end
    self.element = OverlayManager:createElement( "TextArea", "Text"..Text.count, false )
    self.element:setParameter("left", ""..x)
    self.element:setParameter("top", ""..y)
    self.element:setParameter("width", "0")
    self.element:setParameter("height","0")
    self.element:setParameter('font_name','Console')
    self.element:setParameter('char_height',""..size)
    self.element:setParameter('colour_top','1 1 1')
    self.element:setParameter('colour_bottom','0.5 0.5 0.5')
    self.element:setParameter('alignment',alignment)
    self.element:setParameter('caption',text)
    Text.count=Text.count+1
end

