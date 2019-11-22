function setup()
    textures = TextureHolder.new()
    textures:acquire('bg', Texture.new('assets/textures/bg.bmp'))
    textures:acquire('ptr', Texture.new('assets/textures/sprite.bmp'))

    bg = Sprite.new(textures['bg'])
    ptr = Sprite.new(textures['ptr'])

    bg:setOrigin(Vec2i.new(0, 0))

    ptr:setTransparent(true)
end

function processEvents(e)
    if e.type == "MouseMoved" then
        ptr:setPos(e.position)
    end
end

function update(dt)
end

function draw(r)
    r:drawSprite(bg)
    r:drawSprite(ptr)
end
