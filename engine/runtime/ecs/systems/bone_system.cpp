#include "bone_system.h"
#include "../../rendering/mesh.h"
#include "../../system/events.h"
#include "../components/model_component.h"
#include "../components/transform_component.h"

#include <core/system/subsystem.h>

namespace runtime
{

void process_node(const std::unique_ptr<mesh::armature_node>& node, const skin_bind_data& bind_data,
				  EntityType parent, std::vector<EntityType>& entity_nodes,
				  SpatialSystem& ecs)
{
	// if(!parent.valid())
	// 	return;

	auto entity_node = ecs.create();
	// entity_node.set_name(node->name);

	auto& transf_comp = ecs.assign<transform_component>(entity_node);
	// transf_comp->set_parent(parent);
	transf_comp.set_local_transform(node->local_transform);

	auto bone = bind_data.find_bone_by_id(node->name);
	if(bone)
	{
		entity_nodes.push_back(entity_node);
	}

	for(auto& child : node->children)
	{
		process_node(child, bind_data, entity_node, entity_nodes, ecs);
	}
}

static std::vector<math::transform>
get_transforms_for_bones(const std::vector<EntityType>& bone_entities)
{
	auto& ecs = core::get_subsystem<SpatialSystem>();

	std::vector<math::transform> result;
	if(!bone_entities.empty())
	{
		result.reserve(bone_entities.size());
		for(const auto& e : bone_entities)
		{
			// if(e.valid())
			if (true)
			{
				const auto& bone_transform = ecs.get<transform_component>(e);
				// if(!bone_transform.expired())
				if (true)
				{
					result.emplace_back(bone_transform.get_transform());
				}
				else
				{
					result.emplace_back();
				}
			}
			else
			{
				result.emplace_back();
			}
		}
	}

	return result;
}

void bone_system::frame_update(delta_t)
{
	auto& ecs = core::get_subsystem<SpatialSystem>();
	ecs.view<model_component>().each([&ecs](EntityType e, auto& model_comp) {

		const auto& model = model_comp.get_model();
		auto mesh = model.get_lod(0);

		// If mesh isnt loaded yet skip it.
		if(!mesh)
			return;

		const auto& skin_data = mesh->get_skin_bind_data();

		// Has skinning data?
		if(skin_data.has_bones())
		{
			if(model_comp.get_bone_entities().size() <= 1)
			{
				const auto& armature = mesh->get_armature();
				std::vector<EntityType> be;
				process_node(armature, skin_data, e, be, ecs);
				model_comp.set_bone_entities(be);
				model_comp.set_static(false);
			}

			const auto& bone_entities = model_comp.get_bone_entities();
			auto transforms = get_transforms_for_bones(bone_entities);
			model_comp.set_bone_transforms(std::move(transforms));
		}

	});
}

bone_system::bone_system()
{
	runtime::on_frame_update.connect(this, &bone_system::frame_update);
}

bone_system::~bone_system()
{
	runtime::on_frame_update.disconnect(this, &bone_system::frame_update);
}
}
