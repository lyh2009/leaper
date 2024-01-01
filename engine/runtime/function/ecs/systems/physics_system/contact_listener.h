#pragma once
#include <function/ecs/scene.h>

#include <box2d/b2_world_callbacks.h>
#include <box2d/b2_contact.h>

namespace Leaper
{
    class ContactListener : public b2ContactListener
    {
    public:
        ContactListener() = default;
        ContactListener(Leaper::Scene* scene)
            : m_scene(scene) {}

        virtual void BeginContact(b2Contact* contact) override;
	    virtual void EndContact(b2Contact* contact) override;
    private:
        Leaper::Scene* m_scene;
    };

} // namespace Leaper
