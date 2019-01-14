#include "./scene.h"
#include "./utils.h"

#include <core/system/subsystem.h>

std::vector<EntityType> scene::instantiate(mode mod)
{
	if(mod == mode::standard)
	{
		auto& ecs = core::get_subsystem<SpatialSystem>();
		// reset the ecs, remove all existing entities and components
		ecs.reset();
	}

	std::vector<EntityType> out_vec;
	if(!data)
		return out_vec;

	ecs::utils::deserialize_data(*data, out_vec);

	return out_vec;
}
