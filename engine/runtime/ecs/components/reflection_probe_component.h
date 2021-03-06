#pragma once
//-----------------------------------------------------------------------------
// reflection_probe_component Header Includes
//-----------------------------------------------------------------------------
#include "../../rendering/reflection_probe.h"
// #include "runtime/ecs/ent.h"
#include "runtime/ecs/ent.h"

#include <core/common/basetypes.hpp>
#include <core/graphics/render_pass.h>
#include <core/graphics/render_view.h>
#include <array>

class reflection_probe_component : public ent::component_impl<reflection_probe_component>
{
	SERIALIZABLE(reflection_probe_component)
	REFLECTABLEV(reflection_probe_component, ent::component)
public:
	inline const reflection_probe& get_probe() const
	{
		return probe_;
	}
	void set_probe(const reflection_probe& probe);

	int compute_projected_sphere_rect(irect32_t& rect, const math::vec3& position,
									  const math::transform& view, const math::transform& proj);

	inline gfx::render_view& get_render_view(size_t idx)
	{
		return render_view_[idx];
	}

	std::shared_ptr<gfx::texture> get_cubemap();

	std::shared_ptr<gfx::frame_buffer> get_cubemap_fbo();

	void update();

	bool is_touched() { return true; }

	reflection_probe_component() {};
	reflection_probe_component(const reflection_probe_component& p) {};
	// void operator=(const reflection_probe_component& p) {};

private:
	//-------------------------------------------------------------------------
	// Private Member Variables.
	//-------------------------------------------------------------------------
	/// The probe object this component represents
	reflection_probe probe_;
	/// The render view for this component
	std::array<gfx::render_view, 6> render_view_;
};

