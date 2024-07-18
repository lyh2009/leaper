local player = 
{ 
    this,
    rb2d,
    isGround
}

function player.OnAttach()
    player.rb2d = player.this:GetRigdbody2DComponent();
end

function player.OnUpdate()
    player.rb2d.velocity.x = 0.0;
    player.rb2d.velocity.y = 0.0;
    
    if(Input.IsKeyDown(Key.UP) and isGround)
    then
        player.rb2d.velocity.y = 2;
        isGround = false;
    end

    if(Input.IsKeyDown(Key.Left))
    then
        player.rb2d.velocity.x = -0.05;
    end

    if(Input.IsKeyDown(Key.Right))
    then
        player.rb2d.velocity.x = 0.05;
    end
end

function player.OnTriggerEnter(data1, data2)
    if(data2 == "player" and data1 == "ground")
    then
       isGround = true;
    end

    if(data2 == "player" and data1 == "coin")
    then
       print("died")
    end
end

return player
