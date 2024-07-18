local sound = 
{
    this,
    sound_path
}
function sound.OnAttach()
    sound.this:GetSoundComponent().path = "F:\\Repo\\leaper\\bin\\Debug\\assets\\ikun2.mp3"
end

function sound.OnUpdate()

end

return sound;