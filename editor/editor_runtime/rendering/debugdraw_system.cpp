#include "debugdraw_system.h"
#include "../editing/editing_system.h"

#include <core/graphics/debugdraw.h>
#include <core/graphics/render_pass.h>
#include <core/system/subsystem.h>

#include <runtime/assets/asset_manager.h>
#include <runtime/ecs/components/camera_component.h>
#include <runtime/ecs/components/light_component.h>
#include <runtime/ecs/components/model_component.h>
#include <runtime/ecs/components/reflection_probe_component.h>
#include <runtime/ecs/components/transform_component.h>
#include <runtime/rendering/camera.h>
#include <runtime/rendering/gpu_program.h>
#include <runtime/rendering/mesh.h>
#include <runtime/rendering/model.h>
#include <runtime/system/events.h>

namespace editor
{
void debugdraw_system::frame_render(delta_t)
{
	auto& es = core::get_subsystem<editing_system>();
	auto& ecs = core::get_subsystem<SpatialSystem>();
	auto& editor_camera = es.camera;
	bool selected = es.selection_data.is_ent_selected();
	EntityType selected_entity = es.selection_data.id;
	if (editor_camera != entt::null || !ecs.has<camera_component>(editor_camera))
		return;

	auto& camera_comp = ecs.get<camera_component>(editor_camera);
	// const auto camera_comp_ptr = camera_comp.lock().get();
	auto& render_view = camera_comp.get_render_view();
	auto& camera = camera_comp.get_camera();
	const auto& view = camera.get_view();
	const auto& proj = camera.get_projection();
	const auto& viewport_size = camera.get_viewport_size();
	const auto surface = render_view.get_output_fbo(viewport_size);
	const auto camera_posiiton = camera.get_position();

	gfx::render_pass pass("debug_draw_pass");
	pass.bind(surface.get());
	pass.set_view_proj(view, proj);
	gfx::dd_raii dd(pass.id);

	if(es.show_grid)
	{
		auto draw_grid = [&](std::uint32_t grid_color, float height, float height_intervals,
							 std::uint32_t size_intervals, std::uint32_t iteration,
							 std::uint32_t max_iterations) {
			bool should_render = true;
			if(iteration + 1 != max_iterations)
			{
				const auto iterationHeight = height_intervals * float(iteration + 1);
				const float factor = math::clamp(height, 0.0f, iterationHeight) / iterationHeight;
				std::uint32_t r = (grid_color)&0xff;
				std::uint32_t g = (grid_color >> 8) & 0xff;
				std::uint32_t b = (grid_color >> 16) & 0xff;
				std::uint32_t a = (grid_color >> 24) & 0xff;
				a = static_cast<std::uint32_t>(math::lerp(255.0f, 0.0f, factor));
				if(a < 10)
					should_render = false;

				grid_color = r + (g << 8) + (b << 16) + (a << 24);
			}

			if(should_render)
			{
				const auto step = static_cast<std::uint32_t>(
					math::pow<int>(static_cast<int>(size_intervals), static_cast<int>(iteration)));
				const auto grid_size = static_cast<std::uint32_t>(math::pow(size_intervals, max_iterations));
				const auto sz = grid_size / step;

				dd.encoder.push();
				dd.encoder.setState(true, false, true);
				dd.encoder.setColor(grid_color);
				math::vec3 center = {0.0f, 0.0f, 0.0f};
				math::vec3 normal = {0.0f, 1.0f, 0.0f};

				dd.encoder.drawGrid({normal.x, normal.y, normal.z}, {center.x, center.y, center.z}, sz,
									float(step));
				dd.encoder.pop();
			}
		};

		static const auto divison = 5;
		static const auto iterations = 3;
		static const auto height = 40.0f;

		for(std::uint32_t i = 0; i < iterations; ++i)
		{
			draw_grid(0xff808080, math::abs(camera_posiiton.y), height, divison, i, iterations);
		}
	}

	if(!selected)
		return;

	// auto selected_entity = selected.get_value<EntityType>();

	if(!ecs.has<transform_component>(selected_entity))
		return;

	const auto& transform_comp = ecs.get<transform_component>(selected_entity);
	// const auto transform_comp_ptr = transform_comp.get();
	const auto& world_transform = transform_comp.get_transform();

	if(ecs.has<camera_component>(selected_entity) && selected_entity != editor_camera)
	{
		auto& selected_camera_comp = ecs.get<camera_component>(selected_entity);
		// const auto selected_camera_comp_ptr = selected_camera_comp.lock().get();
		auto& selected_camera = selected_camera_comp.get_camera();
		const auto view_proj = selected_camera.get_view_projection();
		const auto bounds = selected_camera.get_local_bounding_box();
		dd.encoder.push();
		dd.encoder.setColor(0xffffffff);
		dd.encoder.setWireframe(true);
		if(selected_camera.get_projection_mode() == projection_mode::perspective)
		{
			dd.encoder.drawFrustum(&view_proj);
		}
		else
		{
			Aabb aabb;
			aabb.m_min.x = bounds.min.x;
			aabb.m_min.y = bounds.min.y;
			aabb.m_min.z = bounds.min.z;
			aabb.m_max.x = bounds.max.x;
			aabb.m_max.y = bounds.max.y;
			aabb.m_max.z = bounds.max.z;
			dd.encoder.pushTransform(&world_transform);
			dd.encoder.draw(aabb);
			dd.encoder.popTransform();
		}

		dd.encoder.pop();
	}

	if(ecs.has<light_component>(selected_entity))
	{
		const auto& light_comp = ecs.get<light_component>(selected_entity);
		// const auto light_comp_ptr = light_comp.lock().get();
		const auto& light = light_comp.get_light();
		if(light.type == light_type::spot)
		{
			auto adjacent = light.spot_data.get_range();
			{
				auto tan_angle = math::tan(math::radians(light.spot_data.get_outer_angle() * 0.5f));
				// oposite = tan * adjacent
				auto oposite = tan_angle * adjacent;
				dd.encoder.push();
				dd.encoder.setColor(0xff00ff00);
				dd.encoder.setWireframe(true);
				dd.encoder.setLod(3);
				math::vec3 from = transform_comp.get_position();
				math::vec3 to = from + transform_comp.get_z_axis() * adjacent;
				dd.encoder.drawCone({to.x, to.y, to.z}, {from.x, from.y, from.z}, oposite);
				dd.encoder.pop();
			}
			{
				auto tan_angle = math::tan(math::radians(light.spot_data.get_inner_angle() * 0.5f));
				// oposite = tan * adjacent
				auto oposite = tan_angle * adjacent;
				dd.encoder.push();
				dd.encoder.setColor(0xff00ffff);
				dd.encoder.setWireframe(true);
				dd.encoder.setLod(3);
				math::vec3 from = transform_comp.get_position();
				math::vec3 to = from + transform_comp.get_z_axis() * adjacent;
				dd.encoder.drawCone({to.x, to.y, to.z}, {from.x, from.y, from.z}, oposite);
				dd.encoder.pop();
			}
		}
		else if(light.type == light_type::point)
		{
			auto radius = light.point_data.range;
			dd.encoder.push();
			dd.encoder.setColor(0xff00ff00);
			dd.encoder.setWireframe(true);
			math::vec3 center = transform_comp.get_position();
			dd.encoder.drawCircle(Axis::X, center.x, center.y, center.z, radius);
			dd.encoder.drawCircle(Axis::Y, center.x, center.y, center.z, radius);
			dd.encoder.drawCircle(Axis::Z, center.x, center.y, center.z, radius);
			dd.encoder.pop();
		}
		else if(light.type == light_type::directional)
		{
			dd.encoder.push();
			dd.encoder.setLod(255);
			dd.encoder.setColor(0xff00ff00);
			dd.encoder.setWireframe(true);
			math::vec3 from1 = transform_comp.get_position();
			math::vec3 to1 = from1 + transform_comp.get_z_axis() * 2.0f;
			dd.encoder.drawCylinder({from1.x, from1.y, from1.z}, {to1.x, to1.y, to1.z}, 0.1f);
			math::vec3 from2 = to1;
			math::vec3 to2 = from2 + transform_comp.get_z_axis() * 1.5f;
			dd.encoder.drawCone({from2.x, from2.y, from2.z}, {to2.x, to2.y, to2.z}, 0.5f);
			dd.encoder.pop();
		}
	}

	if(ecs.has<reflection_probe_component>(selected_entity))
	{
		const auto& probe_comp = ecs.get<reflection_probe_component>(selected_entity);
		// const auto probe_comp_ptr = probe_comp.lock().get();
		const auto& probe = probe_comp.get_probe();
		if(probe.type == probe_type::box)
		{
			dd.encoder.push();
			dd.encoder.setColor(0xff00ff00);
			dd.encoder.setWireframe(true);
			dd.encoder.pushTransform(&world_transform);
			Aabb aabb;
			aabb.m_min.x = -probe.box_data.extents.x;
			aabb.m_min.y = -probe.box_data.extents.y;
			aabb.m_min.z = -probe.box_data.extents.z;
			aabb.m_max.x = probe.box_data.extents.x;
			aabb.m_max.y = probe.box_data.extents.y;
			aabb.m_max.z = probe.box_data.extents.z;
			dd.encoder.draw(aabb);
			dd.encoder.popTransform();
			dd.encoder.pop();
		}
		else
		{
			auto radius = probe.sphere_data.range;
			dd.encoder.push();
			dd.encoder.setColor(0xff00ff00);
			dd.encoder.setWireframe(true);
			math::vec3 center = transform_comp.get_position();
			dd.encoder.drawCircle(Axis::X, center.x, center.y, center.z, radius);
			dd.encoder.drawCircle(Axis::Y, center.x, center.y, center.z, radius);
			dd.encoder.drawCircle(Axis::Z, center.x, center.y, center.z, radius);
			dd.encoder.pop();
		}
	}

	if(ecs.has<model_component>(selected_entity))
	{
		const auto& model_comp = ecs.get<model_component>(selected_entity);
		// const auto model_comp_ptr = model_comp.lock().get();
		const auto& model = model_comp.get_model();
		if(!model.is_valid())
			return;

		const auto mesh = model.get_lod(0);
		if(!mesh)
			return;
		const auto& frustum = camera.get_frustum();
		const auto& bounds = mesh->get_bounds();
		// Test the bounding box of the mesh
		if(math::frustum::test_obb(frustum, bounds, world_transform))
		{
			// if(es->wireframe_selection)
			//{
			//	const float u_params[8] =
			//	{
			//		1.0f, 1.0f, 0.0f, 0.7f, //r,g,b,a
			//		1.0f, 0.0f, 0.0f, 0.0f  //thickness, unused, unused, unused
			//	};
			//	if (!_program)
			//		return;
			//
			//	model.render(
			//		pass.id,
			//		world_transform,
			//		false,
			//		false,
			//		false,
			//		BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA),
			//		0,
			//		_program.get(), [&u_params](program& p)
			//	{
			//		p.set_uniform("u_params", &u_params, 2);
			//	});
			//}
			// else
			{
				dd.encoder.push();
				dd.encoder.setColor(0xff00ff00);
				dd.encoder.setWireframe(true);
				dd.encoder.pushTransform(&world_transform);
				Aabb aabb;
				aabb.m_min.x = bounds.min.x;
				aabb.m_min.y = bounds.min.y;
				aabb.m_min.z = bounds.min.z;
				aabb.m_max.x = bounds.max.x;
				aabb.m_max.y = bounds.max.y;
				aabb.m_max.z = bounds.max.z;
				dd.encoder.draw(aabb);
				dd.encoder.popTransform();
				dd.encoder.pop();
			}
		}
	}
}

debugdraw_system::debugdraw_system()
{
	runtime::on_frame_render.connect(this, &debugdraw_system::frame_render);

	auto& ts = core::get_subsystem<core::task_system>();
	auto& am = core::get_subsystem<runtime::asset_manager>();

	auto vs_wf_wireframe = am.load<gfx::shader>("editor:/data/shaders/vs_wf_wireframe.sc");
	auto fs_wf_wireframe = am.load<gfx::shader>("editor:/data/shaders/fs_wf_wireframe.sc");
	vs_wf_wireframe.wait();
	fs_wf_wireframe.wait();
	ts.push_or_execute_on_owner_thread(
		[this](asset_handle<gfx::shader> vs, asset_handle<gfx::shader> fs) {
			program_ = std::make_unique<gpu_program>(vs, fs);
		},
		vs_wf_wireframe, fs_wf_wireframe);

	ddInit();
}

debugdraw_system::~debugdraw_system()
{
	ddShutdown();
	runtime::on_frame_render.disconnect(this, &debugdraw_system::frame_render);
}
} // namespace editor
