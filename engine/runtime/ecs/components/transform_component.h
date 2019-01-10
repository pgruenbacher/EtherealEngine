#pragma once

// #include "runtime/ecs/ent.h"
#include "runtime/ecs/ent.h"
#include <core/math/math_includes.h>

//-----------------------------------------------------------------------------
// Main Class Declarations
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Name : transform_component (Class)
/// <summary>
/// Class containing transformation data and functionality. It represents
/// an object's state in a 3D setup. Provides functionality for manipulating
/// that state.
/// </summary>
//-----------------------------------------------------------------------------
class transform_component : public ent::component_impl<transform_component>
{
	SERIALIZABLE(transform_component)
	REFLECTABLEV(transform_component, ent::component)

public:
	// transform_component() = default;
	~transform_component();
	void resolve(bool force = false);
	void set_dirty(bool dirty);
	bool is_dirty() const;
	// virtual void on_entity_set() override;
	const math::transform& get_local_transform() const;
	const math::transform& get_transform() const;
	const math::vec3& get_position() const;
	math::quat get_rotation() const;
	math::vec3 get_x_axis() const;
	math::vec3 get_y_axis() const;
	math::vec3 get_z_axis() const;

	//-----------------------------------------------------------------------------
	//  Name : get_scale()
	/// <summary>
	/// Retrieve the current scale of the node along its world space axes.
	/// </summary>
	//-----------------------------------------------------------------------------
	math::vec3 get_scale() const;
	const math::vec3& get_local_position() const;
	math::quat get_local_rotation() const;
	math::vec3 get_local_x_axis() const;
	math::vec3 get_local_y_axis() const;
	math::vec3 get_local_z_axis() const;
	math::vec3 get_local_scale() const;
	void look_at(float x, float y, float z);
	void look_at(const math::vec3& point);
	void set_position(const math::vec3& position);
	void set_local_position(const math::vec3& position);
	void move(const math::vec3& amount);
	void move_local(const math::vec3& amount);
	void rotate(float x, float y, float z);
	void rotate_local(float x, float y, float z);
	void rotate_axis(float degrees, const math::vec3& axis);
	void set_scale(const math::vec3& s);
	void set_local_scale(const math::vec3& scale);
	void set_rotation(const math::quat& rotation);
	void set_local_rotation(const math::quat& rotation);
	void reset_rotation();
	void reset_scale();
	void reset_local_rotation();
	void reset_local_scale();
	void reset_pivot();
	void set_local_transform(const math::transform& trans);
	void set_transform(const math::transform& trans);
	void look_at(const math::vec3& eye, const math::vec3& at);
	void look_at(const math::vec3& eye, const math::vec3& at, const math::vec3& up);
	bool can_adjust_pivot() const;
	bool can_scale() const;
	bool can_rotate() const;
	// void set_parent(ActorType parent, bool world_position_stays, bool local_position_stays);
	// void set_parent(ActorType parent);
	// const EntityType& get_parent() const;
	// const std::vector<EntityType>& get_children() const;
	// void attach_child(const EntityType& child);
	// void remove_child(const EntityType& child);
	// void cleanup_dead_children();

protected:
    void apply_transform(math::transform& trans);
    void apply_local_transform(const math::transform& trans);

	//-------------------------------------------------------------------------
	// Protected Member Variables
	//-------------------------------------------------------------------------
	/// Parent object.
	EntityType parent_;
	/// Children objects.
	// std::vector<EntityType> children_;
	/// Local transformation relative to the parent
	math::transform local_transform_;
	/// Cached world transformation at pivot point.
	math::transform world_transform_;
	/// Should recalc world transform.
	bool dirty_ = true;
};
