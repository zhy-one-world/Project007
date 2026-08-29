#pragma once

#include "ecs.h"
namespace faith
{
	using namespace ECS;

	class ecs_world
	{
	public:
		static ecs_world& get_instance()
		{
			static ecs_world instance;
			return instance;
		}
	public:
		void init()
		{
			world = World::createWorld();
			auto world_guid = guid_gen::make_guid();
			world_entity = world->create(world_guid);
		}
		World* get_world()
		{
			return world;
		}
		Entity* get_world_ent()
		{
			return world_entity;
		}
	private:
		World* world;
		Entity* world_entity;
	};

#define g_ecs ecs_world::get_instance().get_world()
#define g_world_ent ecs_world::get_instance().get_world_ent()

Entity* create_entity(const guid_64 entity_id = 0);
Entity* get_entity(const guid_64& entity_id);
void destroy_entity(Entity* entity);
}

