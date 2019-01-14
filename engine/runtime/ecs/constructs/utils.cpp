#include "utils.h"
// #include "../../meta/ecs/entity.hpp"
#include "../../meta/ecs/ent.hpp"

#include <core/serialization/associative_archive.h>
#include <core/serialization/binary_archive.h>
#include <core/serialization/serialization.h>
#include <core/system/subsystem.h>
#include <runtime/ecs/components/relation.h>
#include "./snapshots.cpp"

namespace ecs
{
namespace utils
{

// template <typename OArchive>
static void serialize_t(std::ostream& stream, const std::vector<EntityType>& data)
{
	// OArchive ar(stream);

	const auto& ecs = core::get_subsystem<SpatialSystem>();
	serialize(stream, ecs, data);

	// try_save(ar, cereal::make_nvp("data", data));

	// runtime::get_serialization_map().clear();
}

// template <typename IArchive>
static bool deserialize_t(std::istream& stream, std::vector<EntityType>& out_data)
{
	// get length of file:
	// runtime::get_serialization_map().clear();
	stream.seekg(0, stream.end);
	std::streampos length = stream.tellg();
	stream.seekg(0, stream.beg);
	if(length > 0)
	{
		// IArchive ar(stream);
		auto& ecs = core::get_subsystem<SpatialSystem>();
		deserialize(stream, ecs, out_data);
		// try_load(ar, cereal::make_nvp("data", out_data));

		stream.clear();
		stream.seekg(0);
		// runtime::get_serialization_map().clear();
		return true;
	}
	return false;
}

/*----------  Interface methods  ----------*/


void save_entity_to_file(const fs::path& full_path, const EntityType data)
{
	save_entities_to_file(full_path, {data});
}

bool try_load_entity_from_file(const fs::path& full_path, EntityType out_data)
{
	std::vector<EntityType> out_data_vec;
	if(!load_entities_from_file(full_path, out_data_vec))
		return false;

	if(!out_data_vec.empty())
		out_data = out_data_vec[0];

	return true;
}

void save_entities_to_file(const fs::path& full_path, const std::vector<EntityType>& data)
{
	std::ofstream os(full_path.string(), std::fstream::binary | std::fstream::trunc);
	serialize_t(os, data);
}

bool load_entities_from_file(const fs::path& full_path, std::vector<EntityType>& out_data)
{
	std::ifstream is(full_path.string(), std::fstream::binary);
	// std::cout << "LOADIN " << full_path.string() << " " << is.rdbuf() << std::endl;
	return deserialize_t(is, out_data);
}

EntityType clone_entity(const EntityType data)
{
	auto& ecs = core::get_subsystem<SpatialSystem>();
	return clone(ecs, data);
}

bool deserialize_data(std::istream& stream, std::vector<EntityType>& out_data)
{
	return deserialize_t(stream, out_data);
}


entt::prototype<EntityType> get_default_ent_factory(Registry& reg) {
  entt::prototype proto{reg};
  proto.set<Name>();
  proto.set<Relation>();
  proto.set<MarkDelete>();
  return proto;
}

} // namespace utils
} // namespace ecs
