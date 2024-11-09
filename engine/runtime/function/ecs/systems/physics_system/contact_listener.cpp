#include "contact_listener.h"
#include "function/ecs/systems/script_system/lua_script_system.h"
#include "lppch.h"

#include <function/ecs/components.h>

void Leaper::ContactListener::BeginContact(b2Contact* contact)
{
    b2Fixture* fixture1 = contact->GetFixtureA();
    b2Fixture* fixture2 = contact->GetFixtureB();

    UUID id1 = (UUID)fixture1->GetUserData().pointer;
    UUID id2 = (UUID)fixture2->GetUserData().pointer;

    Entity a = m_scene->GetEntityByUUID(id1);
    Entity b = m_scene->GetEntityByUUID(id2);

    auto callOnCollision2DBegin = [](Entity entity, Entity other) {
        if (!entity.HasComponent<LuaScriptComponent>())
            return;
        auto& script = entity.GetComponent<LuaScriptComponent>();
        LuaScriptSystem::LuaCall(script, "OnCollision2DBegin", other);
    };
    callOnCollision2DBegin(a, b);
    callOnCollision2DBegin(b, a);
}

void Leaper::ContactListener::EndContact(b2Contact* contact)
{
    b2Fixture* fixture1 = contact->GetFixtureA();
    b2Fixture* fixture2 = contact->GetFixtureB();

    UUID id1 = (UUID)fixture1->GetUserData().pointer;
    UUID id2 = (UUID)fixture2->GetUserData().pointer;

    Entity a = m_scene->GetEntityByUUID(id1);
    Entity b = m_scene->GetEntityByUUID(id2);

    auto callOnCollision2DEnd = [](Entity entity, Entity other) {
        if (!entity.HasComponent<LuaScriptComponent>())
            return;
        auto& script = entity.GetComponent<LuaScriptComponent>();
        LuaScriptSystem::LuaCall(script, "OnCollision2DEnd", other);
    };
    callOnCollision2DEnd(a, b);
    callOnCollision2DEnd(b, a);
}
