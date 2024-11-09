player = {this, aaa}

local rb2d;

function player:OnAttach()
    rb2d = player.this:GetRigdbody2DComponent();
    print("hello")
end

function player:OnUpdate()

    rb2d.velocity.x = 0.0;
    if (Input.IsKeyDown(Key.Left)) then
        rb2d.velocity.x = -1;
    end

    if (Input.IsKeyDown(Key.Right)) then
        rb2d.velocity.x = 1;
    end

end

return player
