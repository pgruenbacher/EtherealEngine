#pragma once

#include "runtime/ecs/ent.h"
#include <fstream>
#include <memory>

struct prefab
{
	EntityType instantiate();
	std::shared_ptr<std::istream> data;
};
