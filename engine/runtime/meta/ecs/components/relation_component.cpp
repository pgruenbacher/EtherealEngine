#include "./relation_component.hpp"
#include "../../core/math/transform.hpp"
#include "../../core/math/vector.hpp"

#include <core/serialization/types/vector.hpp>

REFLECT(Relation)
{
  rttr::registration::class_<Relation>("Relation")();
}

SAVE(Relation)
{
  try_save(ar, cereal::make_nvp("parent", obj.parent));
}
SAVE_INSTANTIATE(Relation, cereal::oarchive_associative_t);
SAVE_INSTANTIATE(Relation, cereal::oarchive_binary_t);

LOAD(Relation)
{
  try_load(ar, cereal::make_nvp("parent", obj.parent));
}
LOAD_INSTANTIATE(Relation, cereal::iarchive_associative_t);
LOAD_INSTANTIATE(Relation, cereal::iarchive_binary_t);

/*----------  Name  ----------*/


REFLECT(Name)
{
  rttr::registration::class_<Name>("Name")();
}

SAVE(Name)
{
  ar(obj.name);
}
SAVE_INSTANTIATE(Name, cereal::oarchive_associative_t);
SAVE_INSTANTIATE(Name, cereal::oarchive_binary_t);

LOAD(Name)
{
  ar(obj.name);
}
LOAD_INSTANTIATE(Name, cereal::iarchive_associative_t);
LOAD_INSTANTIATE(Name, cereal::iarchive_binary_t);
