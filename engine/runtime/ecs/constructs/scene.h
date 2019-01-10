#pragma once

#include "runtime/ecs/ent.h"
#include <fstream>
#include <memory>

struct scene
{
	enum class mode
	{
		standard,
		additive,
	};
	std::vector<EntityType> instantiate(mode mod);
	std::shared_ptr<std::istream> data;
};
