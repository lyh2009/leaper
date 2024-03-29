#pragma once

#include "scriptable_entity.h"
#include "function/render/vertex_array.h"
#include "function/render/buffers.h"
#include "function/render/texture.h"
#include "function/render/orthographic_camera.h"
#include "core/math/uuid.h"
#include "core/base.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <sol/sol.hpp>

#include <string>
#include <functional>


namespace Leaper
{	
	struct TagComponent
	{
		TagComponent(std::string _tag)
			: tag(_tag) {}

		inline std::string &Tag() { return tag; }

		std::string tag;
	};

	struct UUIDComponent
	{
		UUIDComponent(Leaper::UUID _uuid)
			: uuid(_uuid) {}

		Leaper::UUID uuid;
	};

	struct TransformComponent
	{
		glm::vec3 position = {0, 0, 0},
				  rotation = {0, 0, 0},
				  scale = {1, 1, 1};

		TransformComponent() : transform(glm::mat4(1.0f)){};
		TransformComponent(const TransformComponent &) = default;
		TransformComponent(const glm::mat4 &_transform)
			: transform(_transform){};

		glm::mat4& GetTransform()
		{
			transform = glm::translate(glm::mat4(1.0f), position)
				*glm::toMat4(glm::quat(rotation))
				*glm::scale(glm::mat4(1.0f), scale);

			return transform;
		}

		operator glm::mat4() const { return transform; };

	private:
		glm::mat4 transform = glm::mat4(1.0f);
	};

	struct Animation2DComponent
	{
		int current_frame = 0;
		int row, col;
		int speed;
		glm::vec2 texture_coords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	};

	struct SpriteRendererComponent
	{
		SpriteRendererComponent()
		 : color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)){};
		SpriteRendererComponent(const SpriteRendererComponent &) = default;
		SpriteRendererComponent(const glm::vec4 &_color)
		 : color(_color) {};

		Leaper::Ref<Leaper::Texture> m_texture = nullptr;
		Animation2DComponent anim;
		glm::vec4 color;
		std::string texture_path;
	};

	struct CameraComponent
	{
		CameraComponent(float left, float right, float bottom, float top)
			: camera(left, right, bottom, top) {};
			
		OrthgraphicCamera camera;
	};

	struct NaviteScriptComponent
	{
		Leaper::ScriptableEntity* instance = nullptr;

		std::function<void ()> instantiate_function;
		std::function<void ()> destory_instance_function;
		std::function<void (Leaper::ScriptableEntity*)> on_create_function;
		std::function<void (Leaper::ScriptableEntity*)> on_update_function;

		template<typename T>
		void Bind()
		{
			instantiate_function = [&]() { instance = new T(); };
			destory_instance_function = [&]() { delete instance; instance = nullptr; };
			on_create_function = [](Leaper::ScriptableEntity* script) { ((T*)script)->OnCreate(); };
			on_update_function = [](Leaper::ScriptableEntity* script) { ((T*)script)->OnUpdate(); };
		}
	};

	struct Rigidbody2DComponent
	{
		enum BodyType
		{
			Static = 0,
			Kinematic,
			Dynamic
		};
		BodyType body_type = Dynamic;

		glm::vec2 velocity = glm::vec2(0, 0);
		void* runtime_body = nullptr;
	};

	struct FixtureUserData
	{
		FixtureUserData()
			: name("default") {}
		FixtureUserData(std::string user_name)
			: name(user_name) {}
		std::string name;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 size = glm::vec2(0.5f, 0.5f);

		float friction = 0.3f;
		float restitution = 0.0f;
		float density = 1.0f;

		bool is_trigger = false;

		FixtureUserData* user_data = new FixtureUserData();
	};

	struct LuaScriptComponent
	{
		LuaScriptComponent()
			: path("") {}
		LuaScriptComponent(const std::string& script_path)
			: path(script_path) {}
			
		std::string path;
		sol::table self;

	};

} // namespace Leaper



