#include "editing_system.h"

#include <core/graphics/texture.h>
#include <core/system/subsystem.h>

#include <runtime/assets/asset_manager.h>
#include <runtime/ecs/components/audio_listener_component.h>
#include <runtime/ecs/components/camera_component.h>
#include <runtime/ecs/components/transform_component.h>
#include <runtime/ecs/components/relation.h>
#include <runtime/ecs/constructs/utils.h>
#include <runtime/rendering/material.h>
#include <runtime/rendering/mesh.h>
#include <runtime/rendering/render_window.h>
#include <runtime/system/events.h>

namespace editor
{

editing_system::editing_system()
{
	auto& am = core::get_subsystem<runtime::asset_manager>();

	icons["translate"] = am.load<gfx::texture>("editor:/data/icons/translate.png").get();
	icons["rotate"] = am.load<gfx::texture>("editor:/data/icons/rotate.png").get();
	icons["scale"] = am.load<gfx::texture>("editor:/data/icons/scale.png").get();
	icons["local"] = am.load<gfx::texture>("editor:/data/icons/local.png").get();
	icons["global"] = am.load<gfx::texture>("editor:/data/icons/global.png").get();
	icons["play"] = am.load<gfx::texture>("editor:/data/icons/play.png").get();
	icons["pause"] = am.load<gfx::texture>("editor:/data/icons/pause.png").get();
	icons["stop"] = am.load<gfx::texture>("editor:/data/icons/stop.png").get();
	icons["next"] = am.load<gfx::texture>("editor:/data/icons/next.png").get();
	icons["material"] = am.load<gfx::texture>("editor:/data/icons/material.png").get();
	icons["mesh"] = am.load<gfx::texture>("editor:/data/icons/mesh.png").get();
	icons["export"] = am.load<gfx::texture>("editor:/data/icons/export.png").get();
	icons["grid"] = am.load<gfx::texture>("editor:/data/icons/grid.png").get();
	icons["wireframe"] = am.load<gfx::texture>("editor:/data/icons/wireframe.png").get();
	icons["prefab"] = am.load<gfx::texture>("editor:/data/icons/prefab.png").get();
	icons["scene"] = am.load<gfx::texture>("editor:/data/icons/scene.png").get();
	icons["shader"] = am.load<gfx::texture>("editor:/data/icons/shader.png").get();
	icons["loading"] = am.load<gfx::texture>("editor:/data/icons/loading.png").get();
	icons["folder"] = am.load<gfx::texture>("editor:/data/icons/folder.png").get();
	icons["animation"] = am.load<gfx::texture>("editor:/data/icons/animation.png").get();
	icons["sound"] = am.load<gfx::texture>("editor:/data/icons/sound.png").get();
}

void editing_system::save_editor_camera()
{
	if(camera != entt::null)
		ecs::utils::save_entity_to_file(fs::resolve_protocol("app:/settings/editor_camera.cfg"), camera);
}

void editing_system::load_editor_camera()
{
	EntityType object;
	if(!ecs::utils::try_load_entity_from_file(fs::resolve_protocol("app:/settings/editor_camera.cfg"),
											  object))
	{
		auto& ecs = core::get_subsystem<SpatialSystem>();
		auto factory = ecs::utils::get_default_ent_factory(ecs);
		object = factory.create();
	}
	auto& ecs = core::get_subsystem<SpatialSystem>();
	ecs.assign<Name>(object, "EDITOR CAMERA");

	if(!ecs.has<transform_component>(object))
	{
		auto& transform_comp = ecs.assign<transform_component>(object);
		transform_comp.set_local_position({0.0f, 2.0f, -5.0f});
	}
	if(!ecs.has<camera_component>(object))
	{
		ecs.assign<camera_component>(object);
	}
	if(!ecs.has<audio_listener_component>(object))
	{
		ecs.assign<audio_listener_component>(object);
	}

	camera = object;
}


void editing_system::select_ent(EntityType id) {
	selection_data.reset();
	selection_data.id = id;
}
void editing_system::select_variant(rttr::variant obj) {
	selection_data.reset();
	selection_data.obj = obj;
}

// void editing_system::select(EntityType object)
// {
// 	selection_data.object = object;
// }

void editing_system::unselect()
{
	selection_data.reset();
	imguizmo::enable(false);
	imguizmo::enable(true);
}

void editing_system::close_project()
{
	save_editor_camera();
	unselect();
	scene.clear();
}
}
