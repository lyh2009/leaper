#include "contact_listener.h"
#include "lppch.h"

#include <function/ecs/systems/script_system/lua_script_system.h>
#include <function/ecs/components.hpp>

void Leaper::ContactListener::BeginContact(b2Contact *contact)
{
    b2Fixture* fixture1 = contact->GetFixtureA();
    b2Fixture* fixture2 = contact->GetFixtureB();

    FixtureUserData* data1 = reinterpret_cast<FixtureUserData*>(fixture1->GetUserData().pointer);
    FixtureUserData* data2 = reinterpret_cast<FixtureUserData*>(fixture2->GetUserData().pointer);

    m_scene->Reg().view<LuaScriptComponent>().each([&](auto other, LuaScriptComponent& lsc)
    {
        LuaScriptSystem::LuaCall(lsc, "OnTriggerEnter", data1->name, data2->name);
    });
}

void Leaper::ContactListener::EndContact(b2Contact *contact)
{
}
