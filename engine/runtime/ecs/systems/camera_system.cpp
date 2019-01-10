#include "camera_system.h"
#include "../../system/events.h"
#include "../components/camera_component.h"
#include "../components/transform_component.h"

#include <core/system/subsystem.h>

namespace runtime
{
void camera_system::frame_update(delta_t)
{
	auto& ecs = core::get_subsystem<SpatialSystem>();

	ecs.view<transform_component, camera_component>().each(
		[](EntityType e, auto& transform, auto& camera) {
			camera.update(transform.get_transform());
		});
}

camera_system::camera_system()
{
	on_frame_update.connect(this, &camera_system::frame_update);
}

camera_system::~camera_system()
{
	on_frame_update.disconnect(this, &camera_system::frame_update);
}
}
