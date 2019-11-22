function makeSprite(id)
    t = textures[id]
    return Sprite.new(t)
end

function setup(params)
    console = params['console']

    textures = TextureHolder.new()
    textures:acquire('bg', Texture.new('assets/textures/bg.bmp'))
    textures:acquire('ptr', Texture.new('assets/textures/sprite.bmp'))
    textures:acquire("animation", Texture.new('assets/textures/animation.bmp'))
    
    local t = 1
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

    print(Palette.size)
    print(Palette.default)
    
    pnew = {}
    for i = 1, #Palette.default do
        pnew[i] = Palette.default[i]
    end
    pnew[1] = Palette.rgb(255, 255, 255)
    
    for i = 1, #pnew do
        print(pnew[i])
    end

    pnew = Palette.new(pnew)
    print(pnew)

    clock = Clock.new()
    time = clock:getElapsed()
    frame = 1;
end

function processEvents(e)
    if e.type == "MouseMoved" then
        ptr:setPos(e.position)
    end
end

function update(dt)
    animator:update(a, dt)

    console:setPalette(pnew)
end

function draw(r)
    --collectgarbage("collect")
    for i = 1, 100 do
        r:drawSprite(bg)
    end
    r:drawSprite(ptr)
    r:drawSprite(a)

    frame = frame + 1
    if frame == 250 then
        print(clock:getElapsed() - time)
        c:cx()
    end
end
