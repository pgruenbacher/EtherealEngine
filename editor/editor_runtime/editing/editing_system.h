#pragma once
#include "../interface/docks/imguidock.h"

#include <core/math/math_includes.h>

#include <runtime/assets/asset_handle.h>
#include <runtime/ecs/ent.h>

class render_window;

namespace gfx
{
struct texture;
}

namespace editor
{
struct editing_system
{
	// struct selection
	// {
	// 	rttr::variant object;
	// };
	struct selection {
		rttr::variant obj;
		EntityType id;
		bool is_any_selected() {
			return is_ent_selected() || obj;
		}
		bool is_ent_selected() {
			return id != entt::null;
		}
		void reset() {
			obj = {};
			id = entt::null;
		}
	};

	struct snap
	{
		///
		math::vec3 translation_snap = {1.0f, 1.0f, 1.0f};
		///
		float rotation_degree_snap = 15.0f;
		///
		float scale_snap = 0.1f;
	};
	editing_system();
	~editing_system() = default;

	//-----------------------------------------------------------------------------
	//  Name : save_editor_camera ()
	/// <summary>
	///
	///
	///
	/// </summary>
	//-----------------------------------------------------------------------------
	void save_editor_camera();

	//-----------------------------------------------------------------------------
	//  Name : load_editor_camera ()
	/// <summary>
	///
	///
	///
	/// </summary>
	//-----------------------------------------------------------------------------
	void load_editor_camera();

	//-----------------------------------------------------------------------------
	//  Name : select ()
	/// <summary>
	/// Selects an object. Can be anything.
	/// </summary>
	//-----------------------------------------------------------------------------
	void select_ent(EntityType id);
	void select_variant(rttr::variant obj);
	//-----------------------------------------------------------------------------
	//  Name : unselect ()
	/// <summary>
	/// Clears the selection data.
	/// </summary>
	//-----------------------------------------------------------------------------
	void unselect();

	//-----------------------------------------------------------------------------
	//  Name : try_unselect ()
	/// <summary>
	/// Clears the selection data if it maches the type.
	/// </summary>
	//-----------------------------------------------------------------------------
	template <typename T>
	void try_unselect()
	{
		if(selection_data.obj.is_type<T>())
		{
			unselect();
		}
	}

	void close_project();

	/// editor camera
	EntityType camera{entt::null};
	/// current scene
	std::string scene;
	/// enable editor grid
	bool show_grid = true;
	/// enable wireframe selection
	bool wireframe_selection = true;
	/// current manipulation gizmo operation.
	imguizmo::operation operation = imguizmo::translate;
	/// current manipulation gizmo space.
	imguizmo::mode mode = imguizmo::local;
	/// selection data containing selected object
	selection selection_data;
	/// snap data containging various snap options
	snap snap_data;
	/// editor icons lookup map
	std::unordered_map<std::string, asset_handle<gfx::texture>> icons;
};
}
