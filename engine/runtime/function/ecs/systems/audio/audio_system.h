#include "function/ecs/scene.h"
#include <fmod.hpp>
#include <function/ecs/systems/system.h>

namespace Leaper
{
class AudioSystem : public Leaper::System
{
public:
    AudioSystem(Leaper::Scene* scene) : Leaper::System(scene) {}

    virtual void OnGameStart() override;
    virtual void OnGameUpdate() override;
    virtual void OnGameStop() override;

private:
    FMOD::System* m_fmod_system;
};

}  // namespace Leaper