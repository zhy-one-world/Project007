#pragma once

#include "ecs_world.h"
#include "server_log.hpp"
namespace hld
{
	using namespace ECS;

	Entity* create_entity(const guid_64 entity_id)
	{
		if (entity_id.server_64 == 0)
		{
			auto new_guid = guid_gen::make_guid();
			return g_ecs->create(new_guid);
		}
		else
		{
			return g_ecs->create(entity_id);
		}
	}
	Entity* get_entity(const guid_64& entity_id)
	{
		if (entity_id.server_64 == 0)
		{
			CONSOLE_ERROR(" entity_id is 0");
			return nullptr;
		}
		return g_ecs->getEntity(entity_id.server_64);
	}

	void destroy_entity(Entity* entity)
	{
		if (nullptr == entity)
		{
			CONSOLE_ERROR("entity is nullprt");
			return;
		}
		g_ecs->destroy(entity);
	}
}

