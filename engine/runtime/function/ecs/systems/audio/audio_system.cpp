#include "audio_system.h"
#include "function/ecs/components.h"
#include "function/ecs/entity.h"
#include "function/ecs/systems/system.h"

void Leaper::AudioSystem::OnGameStart()
{
    FMOD::System_Create(&m_fmod_system);
    m_fmod_system->init(32, FMOD_INIT_NORMAL, 0);

    auto view = m_scene->Reg().view<Leaper::TransformComponent, Leaper::SoundComponent>();
    for (auto e : view)
    {
        Leaper::Entity entity = { e, m_scene };
        auto& sc              = entity.GetComponent<Leaper::SoundComponent>();
        m_fmod_system->createSound(sc.path.c_str(), FMOD_DEFAULT, 0, &sc.sound);

        m_fmod_system->playSound(sc.sound, nullptr, false, &sc.channel);
    }
}

void Leaper::AudioSystem::OnGameUpdate()
{
    m_fmod_system->update();
}

void Leaper::AudioSystem::OnGameStop()
{
    m_fmod_system->close();
    m_fmod_system->release();
}