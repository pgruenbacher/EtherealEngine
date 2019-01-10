#include "audio_system.h"
#include "../../system/events.h"
#include "../components/audio_listener_component.h"
#include "../components/audio_source_component.h"
#include "../components/transform_component.h"

#include <core/system/subsystem.h>

namespace runtime
{
void audio_system::frame_update(delta_t dt)
{
	auto& ecs = core::get_subsystem<SpatialSystem>();

	ecs.view<transform_component, audio_source_component>().each(
		[](EntityType e, auto& transform, auto& source) {
			source.update(transform.get_transform());
		});
	ecs.view<transform_component, audio_listener_component>().each(
		[](EntityType e, auto& transform, auto& listener) {
			listener.update(transform.get_transform());
		});
}

audio_system::audio_system()
{
	on_frame_update.connect(this, &audio_system::frame_update);
}

audio_system::~audio_system()
{
	on_frame_update.disconnect(this, &audio_system::frame_update);
}
}
