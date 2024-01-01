local player = 
{ 
    this,
    rb2d,
}

function player.OnAttach()
    player.rb2d = player.this:GetRigdbody2DComponent();
    print("hello world")
end

function player.OnUpdate()
    player.rb2d.velocity.x = 0.0;
    if(Input.IsKeyDown(Key.Left))
    then
        player.rb2d.velocity.x = player.rb2d.velocity.x - 0.1;
    end

    if(Input.IsKeyDown(Key.Right))
    then
        player.rb2d.velocity.x = player.rb2d.velocity.x + 0.1;
    end
end

function player.OnTriggerEnter(data1, data2)
    
end

return player
