#include "physics2d_system.h"
#include "lppch.h"

#include "core/log.h"
#include "core/time.h"
#include "function/ecs/components.h"


#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_world.h>


using namespace Leaper;

static b2BodyType Rigidbody2DTypeToBox2DBodyType(Rigidbody2DComponent::BodyType body_type)
{
    switch (body_type)
    {
    case Rigidbody2DComponent::BodyType::Static:
        return b2_staticBody;
    case Rigidbody2DComponent::BodyType::Kinematic:
        return b2_kinematicBody;
    case Rigidbody2DComponent::BodyType::Dynamic:
        return b2_dynamicBody;
    }
}

void Leaper::Physics2D_System::OnGameStart()
{
    m_physics_world = new b2World(b2Vec2(m_gravity.x, m_gravity.y));
    m_scene->Reg().view<Rigidbody2DComponent>().each([=](auto other, Rigidbody2DComponent& rb2d) {
        Entity entity = { other, m_scene };
        auto& trans   = entity.GetComponent<TransformComponent>();
        auto& tag     = entity.GetComponent<TagComponent>();

        b2BodyDef body_def;
        body_def.type = Rigidbody2DTypeToBox2DBodyType(rb2d.body_type);
        body_def.position.Set(trans.position.x, trans.position.y);
        body_def.angle = trans.rotation.z;

        b2Body* body      = m_physics_world->CreateBody(&body_def);
        rb2d.runtime_body = body;

        // box collider2d
        if (entity.HasComponent<BoxCollider2DComponent>())
        {
            auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
            // shape
            b2PolygonShape box_shape;
            box_shape.SetAsBox(trans.scale.x * bc2d.size.x, trans.scale.y * bc2d.size.y);
            // def fixture
            b2FixtureDef fixture_def;
            fixture_def.shape       = &box_shape;
            fixture_def.friction    = bc2d.friction;
            fixture_def.restitution = bc2d.restitution;
            fixture_def.density     = bc2d.density;

            fixture_def.userData.pointer = reinterpret_cast<uintptr_t>(bc2d.user_data);

            body->CreateFixture(&fixture_def);
        }
    });

    m_physics_world->SetContactListener(&m_contact_listener);
}

void Leaper::Physics2D_System::OnGameUpdate()
{
    int32 velocity_iterations = 6;
    int32 position_iterations = 2;
    m_physics_world->Step(Time::GetDeltaTime(), velocity_iterations, position_iterations);

    m_scene->Reg().view<Rigidbody2DComponent>().each([=](auto other, Rigidbody2DComponent& rb2d) {
        Entity entity = { other, m_scene };
        b2Body* body  = (b2Body*)rb2d.runtime_body;
        auto& trans   = entity.GetComponent<TransformComponent>();

        trans.position   = glm::vec3(body->GetPosition().x, body->GetPosition().y, trans.position.z);
        trans.rotation.z = body->GetAngle();

        body->ApplyForceToCenter(b2Vec2(rb2d.velocity.x, rb2d.velocity.y), true);
        const int32_t velocity_iterations = 6;
        const int32_t position_iterations = 2;
    });
}

void Leaper::Physics2D_System::OnGameStop()
{
    delete m_physics_world;
    m_physics_world = nullptr;
}
