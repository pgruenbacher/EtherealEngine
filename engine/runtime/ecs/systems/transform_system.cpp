#include "transform_system.h"
#include "../../system/events.h"
#include "../components/relation.h"
#include "../components/transform_component.h"

#include <core/system/subsystem.h>

namespace runtime
{
void transform_system::frame_update(delta_t) {
  auto& ecs = core::get_subsystem<SpatialSystem>();

  ecs.view<transform_component>().each(
    [&ecs](EntityType e, auto& transform)->void {

      if (ecs.has<Relation>(e)) {
        // handle parent transform...
      }
      // transform.world_transform_ = transform.local_transform_;
      transform.set_world_transform(transform.get_local_transform());
    });
}

transform_system::transform_system() {
  on_frame_update.connect(this, &transform_system::frame_update);
}

transform_system::~transform_system() {
  on_frame_update.disconnect(this, &transform_system::frame_update);
}
}  // namespace runtime
