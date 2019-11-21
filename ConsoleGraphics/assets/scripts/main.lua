function sleep(s)
    local ntime = os.time() + s
    repeat until os.time() > ntime
end

print('Vec2<T> test')
vec1 = Vec2u.new(1, 1)
vec2 = Vec2u.new(1, 1)
vec1 = vec1 + vec2
print(vec1.x, vec1.y)

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