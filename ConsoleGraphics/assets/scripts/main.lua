tex = Texture.new('assets/textures/bg.bmp')
bg = Sprite.new(tex)

offset = Vec2i.new(1, 0)

function setup()
    bg:setOrigin(Vec2i.new(0, 0))
end

function processEvents(e)
end

function update(dt)
end

function draw(r)
    r:drawSprite(bg)
end
