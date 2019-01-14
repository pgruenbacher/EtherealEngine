#pragma once

#include "runtime/ecs/ent.h"

struct Name : public ent::component_impl<Name>{
  SERIALIZABLE(Name)
  REFLECTABLEV(Name, ent::component)

  std::string name;
  Name(const char* n) : name(n) {};
  Name() {};
};

struct Relation : public ent::component_impl<Relation>{
  SERIALIZABLE(Relation)
  REFLECTABLEV(Relation, ent::component)

  EntityType parent{entt::null};
};

struct MarkDelete {
  bool should_destroy() { return destroy; }
  bool destroy{false};
  void markDelete() {
    destroy = true;
  }
};

