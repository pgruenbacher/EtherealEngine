#pragma once
//-----------------------------------------------------------------------------
// light_component Header Includes
//-----------------------------------------------------------------------------
#include "../../rendering/light.h"
#include "runtime/ecs/ent.h"

#include <core/common/basetypes.hpp>
//-----------------------------------------------------------------------------
// Forward Declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Main Class Declarations
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Name : light_component (Class)
/// <summary>
/// Class that contains our core light data, used for rendering and other
/// things.
/// </summary>
//-----------------------------------------------------------------------------
class light_component : public ent::component_impl<light_component>
{
	SERIALIZABLE(light_component)
	REFLECTABLEV(light_component, ent::component)
public:
	//-------------------------------------------------------------------------
	// Public Methods
	//-------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	//  Name : get_light ()
	/// <summary>
	///
	///
	///
	/// </summary>
	//-----------------------------------------------------------------------------
	inline const light& get_light() const
	{
		return light_;
	}

	//-----------------------------------------------------------------------------
	//  Name : set_light ()
	/// <summary>
	///
	///
	///
	/// </summary>
	//-----------------------------------------------------------------------------
	inline void set_light(const light& l)
	{
		light_ = l;
	}

	//-----------------------------------------------------------------------------
	//  Name : compute_projected_sphere_rect ()
	/// <summary>
	///
	///
	///
	/// </summary>
	//-----------------------------------------------------------------------------
	int compute_projected_sphere_rect(irect32_t& rect, const math::vec3& light_position,
									  const math::vec3& light_direction, const math::transform& view,
									  const math::transform& proj);

private:
	//-------------------------------------------------------------------------
	// Private Member Variables.
	//-------------------------------------------------------------------------
	/// The light object this component represents
	light light_;
};


class light_component2 : public ent::component_impl<light_component2>
{
	SERIALIZABLE(light_component2)
	REFLECTABLEV(light_component2, ent::component)
public:
	//-------------------------------------------------------------------------
	// Public Methods
	//-------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	//  Name : get_light ()
	/// <summary>
	///
	///
	///
	/// </summary>
	//-----------------------------------------------------------------------------
	inline const light& get_light() const
	{
		return light_;
	}

	//-----------------------------------------------------------------------------
	//  Name : set_light ()
	/// <summary>
	///
	///
	///
	/// </summary>
	//-----------------------------------------------------------------------------
	inline void set_light(const light& l)
	{
		light_ = l;
	}

	//-----------------------------------------------------------------------------
	//  Name : compute_projected_sphere_rect ()
	/// <summary>
	///
	///
	///
	/// </summary>
	//-----------------------------------------------------------------------------
	int compute_projected_sphere_rect(irect32_t& rect, const math::vec3& light_position,
									  const math::vec3& light_direction, const math::transform& view,
									  const math::transform& proj);

private:
	//-------------------------------------------------------------------------
	// Private Member Variables.
	//-------------------------------------------------------------------------
	/// The light object this component represents
	light light_;
};
