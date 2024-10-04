#include "contact_listener.h"
#include "lppch.h"

#include <function/ecs/components.h>

void Leaper::ContactListener::BeginContact(b2Contact* contact)
{
    b2Fixture* fixture1 = contact->GetFixtureA();
    b2Fixture* fixture2 = contact->GetFixtureB();

    FixtureUserData* data1 = reinterpret_cast<FixtureUserData*>(fixture1->GetUserData().pointer);
    FixtureUserData* data2 = reinterpret_cast<FixtureUserData*>(fixture2->GetUserData().pointer);

    m_scene->Reg().view<LuaScriptComponent>().each([&](auto other, LuaScriptComponent& lsc) {

    });
}

void Leaper::ContactListener::EndContact(b2Contact* contact) {}
