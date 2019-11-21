function sleep(s)
    local ntime = os.time() + s
    repeat until os.time() > ntime
end

print('Vec2<T> test')
vec = Vec2u.new(2, 2)
vec1 = Vec2u.new(4, 4)
print(vec.x, vec.y)

vec.x = 5.4
vec.y = vec.x * 2
print(vec.x, vec.y)

vec = vec.add(vec, vec1)
print(vec.x, vec.y)

vec = vec.sub(vec, vec1)
print(vec.x, vec.y)

vec = vec.mul(2, vec)
print(vec.x, vec.y)

vec = vec.mul(vec, 2)
print(vec.x, vec.y)

print(vec == vec1)
print(vec ~= vec1)

vec2 = Vec2f.new(2, 2.5)
print(vec2.x, vec2.y)

print('Clock test')
clock = Clock.new()

sleep(1)
print(clock:getElapsed())

sleep(1)
print(clock:restart())

sleep(1)
print(clock:getElapsed())