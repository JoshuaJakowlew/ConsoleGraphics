function setup()
    textures = TextureHolder.new()
    textures:acquire('bg', Texture.new('assets/textures/bg.bmp'))
    textures:acquire('ptr', Texture.new('assets/textures/sprite.bmp'))
    textures:acquire("animation", Texture.new('assets/textures/animation.bmp'))

    bg = Sprite.new(textures['bg'])
    ptr = Sprite.new(textures['ptr'])
    a = Sprite.new(textures["animation"], Vec2u.new(0, 0), Vec2u.new(15, 15))
    
    bg:setOrigin(Vec2i.new(0, 0))
    ptr:setTransparent(true)

    anim = FrameAnimation.new()
    anim:addFrame(0.5, Vec2u.new(0, 0), Vec2u.new(15, 15))
    anim:addFrame(0.5, Vec2u.new(16, 0), Vec2u.new(31, 15))

    animator = FrameAnimator.new()
    animator:addAnimation("idle", anim)
    animator:play()
end

function processEvents(e)
    if e.type == "MouseMoved" then
        ptr:setPos(e.position)
    end
end

function update(dt)
    animator:update(a, dt)
end

function draw(r)
    r:drawSprite(bg)
    r:drawSprite(ptr)
    r:drawSprite(a)
end
