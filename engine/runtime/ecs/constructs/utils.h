#pragma once

#include "runtime/ecs/ent.h"

#include <core/filesystem/filesystem.h>

#include <fstream>
#include <vector>

namespace ecs
{
namespace utils
{

EntityType clone_entity(const EntityType data);
//-----------------------------------------------------------------------------
//  Name : save_entity ()
/// <summary>
///
///
///
/// </summary>
//-----------------------------------------------------------------------------
void save_entity_to_file(const fs::path& full_path, const EntityType data);

//-----------------------------------------------------------------------------
//  Name : try_load_entity ()
/// <summary>
///
///
///
/// </summary>
//-----------------------------------------------------------------------------
bool try_load_entity_from_file(const fs::path& full_path, EntityType out_data);

//-----------------------------------------------------------------------------
//  Name : save_data ()
/// <summary>
///
///
///
/// </summary>
//-----------------------------------------------------------------------------
void save_entities_to_file(const fs::path& full_path, const std::vector<EntityType>& data);

//-----------------------------------------------------------------------------
//  Name : load_data ()
/// <summary>
///
///
///
/// </summary>
//-----------------------------------------------------------------------------
bool load_entities_from_file(const fs::path& full_path, std::vector<EntityType>& out_data);

//-----------------------------------------------------------------------------
//  Name : deserialize_data ()
/// <summary>
///
///
///
/// </summary>
//-----------------------------------------------------------------------------
bool deserialize_data(std::istream& stream, std::vector<EntityType>& out_data);
}
}
