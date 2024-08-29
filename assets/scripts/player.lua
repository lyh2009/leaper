local player
{
    this,
    rb2d
}

function player.OnAttach()
    self.rb2d = self.this:GetRigdbody2DComponent();
    print("hello")
end

function player.OnUpdate()
    
    if(Input.IsKeyDown(Key.UP) and isGround)
    then
        self.rb2d.velocity.y = 2;
        isGround = false;
    end

    if(Input.IsKeyDown(Key.Left))
    then
        self.rb2d.velocity.x = -0.05;
    end

    if(Input.IsKeyDown(Key.Right))
    then
        self.rb2d.velocity.x = 0.05;
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
