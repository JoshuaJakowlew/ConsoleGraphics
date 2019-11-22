texBg  = Texture.new('assets/textures/bg.bmp')
texPtr = Texture.new('assets/textures/sprite.bmp')
bg     = Sprite.new(texBg)
ptr    = Sprite.new(texPtr)

function setup()
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
