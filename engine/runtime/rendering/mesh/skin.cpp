#include "./skin.h"


///////////////////////////////////////////////////////////////////////////////
// skin_bind_data Member Definitions
///////////////////////////////////////////////////////////////////////////////
void skin_bind_data::add_bone(const bone_influence& bone)
{
  bones_.push_back(bone);
}

void skin_bind_data::remove_empty_bones()
{
  for(size_t i = 0; i < bones_.size();)
  {
    if(bones_[i].influences.empty())
    {
      bones_.erase(bones_.begin() + static_cast<int>(i));

    } // End if empty
    else
    {
      ++i;
    }

  } // Next Bone
}

void skin_bind_data::clear_vertex_influences()
{
  for(size_t i = 0; i < bones_.size(); ++i)
  {
    bones_[i].influences.clear();
  }
}

void skin_bind_data::clear()
{
  bones_.clear();
}

void skin_bind_data::remap_vertices(const std::vector<std::uint32_t>& remap)
{
  // Iterate through all bone information and remap vertex indices.
  for(size_t i = 0; i < bones_.size(); ++i)
  {
    vertex_influence_array_t new_influences;
    vertex_influence_array_t& influences = bones_[i].influences;
    new_influences.reserve(influences.size());
    for(size_t j = 0; j < influences.size(); ++j)
    {
      std::uint32_t new_index = remap[influences[j].vertex_index];
      if(new_index != 0xFFFFFFFF)
      {
        // Insert an influence at the new index
        new_influences.push_back(vertex_influence(new_index, influences[j].weight));

        // If the vertex was split into two, we want to retain an
        // influence to the original index too.
        if(new_index >= remap.size())
        {
          new_influences.push_back(
            vertex_influence(influences[j].vertex_index, influences[j].weight));
        }

      } // End if !removed

    } // Next source influence
    bones_[i].influences = new_influences;

  } // Next bone
}

void skin_bind_data::build_vertex_table(std::uint32_t vertex_count,
                    const std::vector<std::uint32_t>& vertex_remap,
                    vertex_data_array_t& table)
{
  std::uint32_t vertex;

  // Initialize the vertex table with the required number of vertices.
  table.reserve(vertex_count);
  for(vertex = 0; vertex < vertex_count; ++vertex)
  {
    vertex_data data;
    data.palette = -1;
    data.original_vertex = vertex;
    table.push_back(data);

  } // Next Vertex

  // Iterate through all bone information and populate the above array.
  for(size_t i = 0; i < bones_.size(); ++i)
  {
    vertex_influence_array_t& influences = bones_[i].influences;
    for(size_t j = 0; j < influences.size(); ++j)
    {

      // Vertex data has been remapped?
      if(vertex_remap.size() > 0)
      {
        vertex = vertex_remap[influences[j].vertex_index];
        if(vertex == 0xFFFFFFFF)
          continue;
        auto& data = table[vertex];
        // Push influence data.
        data.influences.push_back(static_cast<std::int32_t>(i));
        data.weights.push_back(influences[j].weight);
      } // End if remap
      else
      {
        auto& data = table[influences[j].vertex_index];
        // Push influence data.
        data.influences.push_back(static_cast<std::int32_t>(i));
        data.weights.push_back(influences[j].weight);
      }

    } // Next Influence

  } // Next Bone
}

const std::vector<skin_bind_data::bone_influence>& skin_bind_data::get_bones() const
{
  return bones_;
}

std::vector<skin_bind_data::bone_influence>& skin_bind_data::get_bones()
{
  return bones_;
}

bool skin_bind_data::has_bones() const
{
  return !get_bones().empty();
}

const skin_bind_data::bone_influence* skin_bind_data::find_bone_by_id(const std::string& name) const
{
  auto it = std::find_if(std::begin(bones_), std::end(bones_),
               [name](const auto& bone) { return name == bone.bone_id; });
  if(it != std::end(bones_))
  {
    return &(*it);
  }

  return nullptr;
}
