#pragma once

#include "runtime/ecs/ent.h"

struct Name {
  std::string name;
};

struct Relation {
  EntityType parent{entt::null};
};

struct MarkDelete {
  bool destroy{false};
  void markDelete() {
    destroy = true;
  }
};
