#pragma once
#include "contact_listener.h"
#include "function/ecs/systems/system.h"

#include <glm/glm.hpp>

class b2World;
namespace Leaper
{
    class Physics2D_System : public Leaper::System
    {
    public:
        Physics2D_System() = delete;
        Physics2D_System(Leaper::Scene* scene)
            : Leaper::System(scene), m_contact_listener(scene) {}
        
        virtual void OnGameStart() override;
        virtual void OnGameUpdate() override;
        virtual void OnGameStop() override;

    private:
        b2World* m_physics_world;
		glm::vec2 m_gravity = glm::vec2( 0.0f, -9.8f );

        ContactListener m_contact_listener;
    };

} // namespace Leaper