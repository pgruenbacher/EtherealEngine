#pragma once

#include "../../../ecs/components/relation.h"
#include <core/reflection/reflection.h>
#include <core/serialization/serialization.h>

REFLECT_EXTERN(Relation);
SAVE_EXTERN(Relation);
LOAD_EXTERN(Relation);

REFLECT_EXTERN(Name);
SAVE_EXTERN(Name);
LOAD_EXTERN(Name);


#include <core/serialization/associative_archive.h>
#include <core/serialization/binary_archive.h>

CEREAL_REGISTER_TYPE(Relation)
CEREAL_REGISTER_TYPE(Name)
