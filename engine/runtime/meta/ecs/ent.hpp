#pragma once
#include "runtime/ecs/ent.h"

#include <core/reflection/reflection.h>
#include <core/serialization/serialization.h>

namespace runtime
{

std::map<std::uint64_t, EntityType>& get_serialization_map();

SAVE_EXTERN(EntityType);
LOAD_EXTERN(EntityType);
}
