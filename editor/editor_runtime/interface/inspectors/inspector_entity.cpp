#include "inspector_entity.h"
#include "inspectors.h"
#include <core/system/subsystem.h>
#include <runtime/ecs/components/relation.h>

#include <runtime/ecs/components/transform_component.h>
#include <runtime/ecs/components/camera_component.h>
#include <runtime/ecs/components/light_component.h>

template<typename C>
void try_inspect_component(const Registry& ecs, EntityType ent, bool& changed) {
	if (!ecs.has<C>(ent)) return;

	bool opened = true;
	// auto component = component_ptr.lock().get();
	auto& component = ecs.get<C>(ent);
	auto component_type = rttr::type::get(component);

	std::string name = component_type.get_name().data();
	auto meta_id = component_type.get_metadata("pretty_name");
	if(meta_id)
	{
		name = meta_id.to_string();
	}
	gui::PushID(&component);
	gui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
	if(gui::CollapsingHeader(name.c_str(), &opened))
	{
		gui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 8.0f);
		gui::TreePush(name.c_str());

		rttr::variant component_var = &component;
		changed |= inspect_var(component_var);

		gui::TreePop();
		gui::PopStyleVar();
	}
	gui::PopID();
	// if(!opened)
	// {
	// 	// component->get_entity().remove(component_ptr.lock());
	// }

}

bool inspector_entity::inspect(rttr::variant& var, bool read_only, const meta_getter& get_metadata)
{
	auto& ecs = core::get_subsystem<SpatialSystem>();
	assert(var.can_convert<EntityType>());
	EntityType data = var.get_value<EntityType>();
	if(!data || data == entt::null)
		return false;
	EntityType ent = data;
	bool changed = false;
	{
		property_layout prop_name("Name");
		// rttr::variant var_name = data.to_string();
		rttr::variant var_name = ecs.get<Name>(data);

		changed |= inspect_var(var_name);

		if(changed)
		{
			ecs.get<Name>(data).name = (var_name.to_string());
		}
	}
	ImGui::Separator();

	try_inspect_component<transform_component>(ecs, ent, changed);
	try_inspect_component<light_component>(ecs, ent, changed);
	try_inspect_component<camera_component>(ecs, ent, changed);


	gui::Separator();
	if(gui::Button("+COMPONENT"))
	{
		gui::OpenPopup("COMPONENT_MENU");
		gui::SetNextWindowPos(gui::GetCursorScreenPos());
	}

	if(gui::BeginPopup("COMPONENT_MENU"))
	{
		static ImGuiTextFilter filter;
		filter.Draw("Filter", 180);
		gui::Separator();
		gui::BeginChild("COMPONENT_MENU_CONTEXT", ImVec2(gui::GetContentRegionAvailWidth(), 200.0f));

		auto component_types = rttr::type::get<ent::component>().get_derived_classes();

		for(auto& component_type : component_types)
		{
			// If any constructors registered
			auto cstructor = component_type.get_constructor();
			if(cstructor)
			{
				std::string name = component_type.get_name().data();
				auto meta_id = component_type.get_metadata("pretty_name");
				if(meta_id)
				{
					name = meta_id.to_string();
				}
				if(!filter.PassFilter(name.c_str()))
					continue;

				if(gui::Selectable(name.c_str()))
				{
					auto c = cstructor.invoke();
					auto c_ptr = c.get_value<std::shared_ptr<ent::component>>();

					if(c_ptr) {
						// data.assign(c_ptr);
						// ecs.assign<
					}

					gui::CloseCurrentPopup();
				}
			}
		}

		gui::EndChild();
		gui::EndPopup();
	}

	if(changed)
	{
		var = data;
		return true;
	}

	return false;
}
