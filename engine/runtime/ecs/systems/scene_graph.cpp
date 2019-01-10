#include "scene_graph.h"
#include "../../system/events.h"
#include "../components/transform_component.h"
#include "../components/relation.h"

#include <core/system/subsystem.h>

namespace runtime
{

void scene_graph::frame_update(delta_t dt)
{
	auto& ecs = core::get_subsystem<SpatialSystem>();
	roots_.clear();

	auto view = ecs.view<Relation>();
	for (auto ent : view) {
		const auto& relation = view.get(ent);
		if (relation.parent == entt::null) {
			roots_.push_back(ent);
		}
	}
}

scene_graph::scene_graph()
{
	runtime::on_frame_update.connect(this, &scene_graph::frame_update);

	transform_component::static_id();
}

scene_graph::~scene_graph()
{
	runtime::on_frame_update.disconnect(this, &scene_graph::frame_update);
}
}
