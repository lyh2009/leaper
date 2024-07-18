#include "navite_script_system.h"
#include "lppch.h"

#include "function/ecs/components.h"
#include "function/ecs/scene.h"

void Leaper::NaviteScriptSystem::OnUpdate()
{
    m_scene->Reg().view<Leaper::NaviteScriptComponent>().each([=](auto other, Leaper::NaviteScriptComponent& nsc) {
        if (!nsc.instance)
        {
            nsc.instantiate_function();
            nsc.instance->m_entity = { other, m_scene };
            nsc.on_create_function(nsc.instance);
        }

        nsc.on_update_function(nsc.instance);
    });
}
