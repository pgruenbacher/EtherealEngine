#pragma once

#include <core/common/basetypes.hpp>
#include <core/graphics/graphics.h>
#include <core/math/math_includes.h>
#include <core/reflection/registration.h>
#include <core/serialization/serialization.h>

#include <map>
#include <memory>
#include <vector>

//-----------------------------------------------------------------------------
// Main Class Declarations
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Name : skin_bind_data (Class)
/// <summary>
/// Structure describing how a skinned mesh should be bound to any bones
/// that influence its vertices.
/// </summary>
//-----------------------------------------------------------------------------
class skin_bind_data
{
  REFLECTABLE(skin_bind_data)
  SERIALIZABLE(skin_bind_data)
public:
  //-------------------------------------------------------------------------
  // Public Typedefs, Structures & Enumerations
  //-------------------------------------------------------------------------
  // Describes how a bone influences a specific vertex.
  struct vertex_influence
  {
    std::uint32_t vertex_index = 0;
    float weight = 0.0f;

    // Constructors
    vertex_influence() = default;
    vertex_influence(std::uint32_t _index, float _weight)
      : vertex_index(_index)
      , weight(_weight)
    {
    }
  };
  using vertex_influence_array_t = std::vector<vertex_influence>;
  // Describes the vertices that are connected to the referenced bone, and how
  // much influence it has on
  // them.
  struct bone_influence
  {
    /// Unique identifier of the bone from which transformation information
    /// should be taken.
    std::string bone_id;
    /// The "bind pose" or "offset" transform that describes how the bone was
    /// positioned in relation to
    /// the original vertex data.
    math::transform bind_pose_transform;
    /// List of vertices influenced by the referenced bone.
    vertex_influence_array_t influences;
  };
  using bone_influence_array_t = std::vector<bone_influence>;
  // Contains per-vertex influence and weight information.
  struct vertex_data
  {
    /// List of bones that influence this vertex.
    std::vector<std::int32_t> influences;
    /// List of weights that describe how this vertex is influenced.
    std::vector<float> weights;
    /// Index of the palette to which this vertex has been assigned.
    std::int32_t palette;
    /// The index of the original vertex stored in the mesh.
    std::uint32_t original_vertex;
  };
  using vertex_data_array_t = std::vector<vertex_data>;
  //-------------------------------------------------------------------------
  // Public Methods
  //-------------------------------------------------------------------------
  //-----------------------------------------------------------------------------
  //  Name : add_bone()
  /// <summary>
  /// Add influence information for a specific bone.
  /// </summary>
  //-----------------------------------------------------------------------------
  void add_bone(const bone_influence& bone);

  //-----------------------------------------------------------------------------
  //  Name : remove_empty_bones()
  /// <summary>
  /// Strip out any bones that did not contain any influences.
  /// </summary>
  //-----------------------------------------------------------------------------
  void remove_empty_bones();

  //-----------------------------------------------------------------------------
  //  Name : build_vertex_table()
  /// <summary>
  /// Construct a list of bone influences and weights for each vertex based
  /// on the binding data provided.
  /// </summary>
  //-----------------------------------------------------------------------------
  void build_vertex_table(std::uint32_t vertex_count, const std::vector<std::uint32_t>& vertex_remap,
              vertex_data_array_t& table);

  //-----------------------------------------------------------------------------
  //  Name : remap_vertices()
  /// <summary>
  /// remap the vertex references stored in the binding based on the supplied
  /// remap array (Array[old vertex index] = new vertex index). Store an index
  /// of 0xFFFFFFFF to indicate that the vertex was removed, or use an index
  /// greater than the remap array size to indicate the new location of a split
  /// vertex.
  /// </summary>
  //-----------------------------------------------------------------------------
  void remap_vertices(const std::vector<std::uint32_t>& remap);

  //-----------------------------------------------------------------------------
  //  Name : get_bones ()
  /// <summary>
  /// Retrieve a list of all bones that influence the skin in some way.
  /// </summary>
  //-----------------------------------------------------------------------------
  const bone_influence_array_t& get_bones() const;

  //-----------------------------------------------------------------------------
  //  Name : get_bones ()
  /// <summary>
  /// Retrieve a list of all bones that influence the skin in some way.
  /// </summary>
  //-----------------------------------------------------------------------------
  bone_influence_array_t& get_bones();

  //-----------------------------------------------------------------------------
  //  Name : getBones ()
  /// <summary>
  /// Retrieve whether or not his is a valid skin data
  /// </summary>
  //-----------------------------------------------------------------------------
  bool has_bones() const;

  //-----------------------------------------------------------------------------
  //  Name : find_bone_by_id ()
  /// <summary>
  /// Finds a bone by id.
  /// </summary>
  //-----------------------------------------------------------------------------
  const bone_influence* find_bone_by_id(const std::string& id) const;

  //-----------------------------------------------------------------------------
  //  Name : clear_vertex_influences()
  /// <summary>
  /// Release memory allocated for vertex influences in each stored bone.
  /// </summary>
  //-----------------------------------------------------------------------------
  void clear_vertex_influences();

  //-----------------------------------------------------------------------------
  //  Name : clear()
  /// <summary>
  /// Clears out the bone information stored in this object.
  /// </summary>
  //-----------------------------------------------------------------------------
  void clear();

private:
  //-------------------------------------------------------------------------
  // Private Variables
  //-------------------------------------------------------------------------
  /// List of bones that influence the skin mesh vertices.
  bone_influence_array_t bones_;

}; // End Class skin_bind_data
