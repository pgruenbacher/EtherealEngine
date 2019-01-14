#pragma once
#include "runtime/ecs/ent.h"

#include <core/reflection/reflection.h>
#include <core/serialization/serialization.h>

namespace ent
{
REFLECT_EXTERN(component);
SAVE_EXTERN(component);
LOAD_EXTERN(component);
}
