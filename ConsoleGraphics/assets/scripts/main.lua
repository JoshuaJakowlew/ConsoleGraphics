function sleep(s)
    local ntime = os.time() + s
    repeat until os.time() > ntime
end


print('Color test')
-- color = Color.new()
-- color = Color.new(string.byte('1'), 12)
color = Color.new()
color.glyph = string.byte('#')
color.bgColor = 10
color.fgColor = 12
print(color.glyph, color.bgColor, color.fgColor)
print(color:toCharInfo())

color = makeColor(string.byte('@'), 2, 5)
print(color.glyph, color.bgColor, color.fgColor)
print(makeCharInfo(string.byte('@'), 2, 5))
print(combine(1, 1))
print(getBgColor(32))
print(getFgColor(33))

print('Texture test')
tex = Texture.new('assets/textures/sprite.bmp')
sz = tex:getSize()
print(sz.x, sz.y)

print('Sprite test')
sprite = Sprite.new(tex, Vec2u.new(0, 0), Vec2u.new(7, 7))
print(sprite:getSize().x, sprite:getSize().y)