#pragma once

#include <core/common/basetypes.hpp>
#include <core/graphics/graphics.h>
#include <core/math/math_includes.h>
#include <core/reflection/registration.h>
#include <core/serialization/serialization.h>

#include <map>
#include <memory>
#include <vector>

#include "./skin.h"

//-----------------------------------------------------------------------------
//  Name : bone_palette (Class)
/// <summary>
/// Outlines a collection of bones that influence a given set of faces /
/// vertices in the mesh.
/// </summary>
//-----------------------------------------------------------------------------
class bone_palette
{
public:
  //-------------------------------------------------------------------------
  // Public Typedefs, Structures & Enumerations
  //-------------------------------------------------------------------------
  using bone_index_map_t = std::map<std::uint32_t, std::uint32_t>;
  //-------------------------------------------------------------------------
  // Constructors & Destructors
  //-------------------------------------------------------------------------
  bone_palette(std::uint32_t paletteSize);
  bone_palette(const bone_palette& init);
  ~bone_palette();

  //-------------------------------------------------------------------------
  // Public Methods
  //-------------------------------------------------------------------------
  //-----------------------------------------------------------------------------
  //  Name : get_skinning_matrices()
  /// <summary>
  /// Gather the bone / palette information and matrices ready for
  /// drawing the skinned mesh.
  /// </summary>
  //-----------------------------------------------------------------------------
  std::vector<math::transform> get_skinning_matrices(const std::vector<math::transform>& node_transforms,
                             const skin_bind_data& bind_data,
                             bool compute_inverse_transpose) const;

  //-----------------------------------------------------------------------------
  //  Name : compute_palette_fit()
  /// <summary>
  /// Determine the relevant "fit" information that can be used to
  /// discover if and how the specified combination of bones will fit into
  /// this palette.
  /// </summary>
  //-----------------------------------------------------------------------------
  void compute_palette_fit(bone_index_map_t& input, std::int32_t& current_space, std::int32_t& common_base,
               std::int32_t& additional_bones);

  //-----------------------------------------------------------------------------
  //  Name : assign_bones()
  /// <summary>
  /// Assign the specified bones (and faces) to this bone palette.
  /// </summary>
  //-----------------------------------------------------------------------------
  void assign_bones(bone_index_map_t& bones, std::vector<std::uint32_t>& faces);

  //-----------------------------------------------------------------------------
  //  Name : assign_bones()
  /// <summary>
  /// Assign the specified bones to this bone palette.
  /// </summary>
  //-----------------------------------------------------------------------------
  void assign_bones(const std::vector<std::uint32_t>& bones);

  //-----------------------------------------------------------------------------
  //  Name : translate_bone_to_palette ()
  /// <summary>
  /// Translate the specified bone index into its associated position in
  /// the palette. If it does not exist, a value of -1 will be returned.
  /// </summary>
  //-----------------------------------------------------------------------------
  std::uint32_t translate_bone_to_palette(std::uint32_t bone_index) const;

  //-----------------------------------------------------------------------------
  //  Name : get_maximum_blend_index ()
  /// <summary>
  /// Retrieve the maximum vertex blend index for this palette (i.e. if
  /// every vertex was only influenced by one bone, this variable would
  /// contain a value of 0).
  /// </summary>
  //-----------------------------------------------------------------------------
  std::int32_t get_maximum_blend_index() const;

  //-----------------------------------------------------------------------------
  //  Name : get_maximum_size ()
  /// <summary>
  /// Retrieve the maximum size of the palette -- the maximum number of bones
  /// capable of being referenced.
  /// </summary>
  //-----------------------------------------------------------------------------
  std::uint32_t get_maximum_size() const;

  //-----------------------------------------------------------------------------
  //  Name : get_data_group ()
  /// <summary>
  /// Retrieve the identifier of the data group assigned to the subset of the
  /// mesh reserved for this bone palette.
  /// </summary>
  //-----------------------------------------------------------------------------
  std::uint32_t get_data_group() const;

  //-----------------------------------------------------------------------------
  //  Name : get_influenced_faces ()
  /// <summary>
  /// Retrieve the list of faces assigned to this palette.
  /// </summary>
  //-----------------------------------------------------------------------------
  std::vector<std::uint32_t>& get_influenced_faces();

  //-----------------------------------------------------------------------------
  //  Name : get_bones ()
  /// <summary>
  /// Retrieve the indices of the bones referenced by this palette.
  /// </summary>
  //-----------------------------------------------------------------------------
  const std::vector<std::uint32_t>& get_bones() const;

  //-----------------------------------------------------------------------------
  //  Name : set_maximum_blend_index ()
  /// <summary>
  /// Set the maximum vertex blend index for this palette (i.e. if every
  /// vertex was only influenced by one bone, this variable would contain a
  /// value of 0).
  /// </summary>
  //-----------------------------------------------------------------------------
  void set_maximum_blend_index(int index);

  //-----------------------------------------------------------------------------
  //  Name : set_data_group ()
  /// <summary>
  /// Set the identifier of the data group assigned to the subset of the mesh
  /// reserved for this bone palette.
  /// </summary>
  //-----------------------------------------------------------------------------
  void set_data_group(std::uint32_t group);

  //-----------------------------------------------------------------------------
  //  Name : clear_influenced_faces ()
  /// <summary>
  /// Clear out the temporary face influences array.
  /// </summary>
  //-----------------------------------------------------------------------------
  void clear_influenced_faces();

protected:
  //-------------------------------------------------------------------------
  // Protected Variables
  //-------------------------------------------------------------------------
  /// Sorted list of bones in this palette. References the elements in the
  /// standard list.
  bone_index_map_t bones_lut_;
  /// Main palette of indices that reference the bones outlined in the main skin
  /// binding data.
  std::vector<std::uint32_t> bones_;
  /// List of faces assigned to this palette.
  std::vector<std::uint32_t> faces_;
  /// The data group identifier used to separate the mesh data into subsets
  /// relevant to this bone palette.
  std::uint32_t data_group_id_;
  /// The maximum size of this palette.
  std::uint32_t maximum_size_;
  /// The maximum vertex blend index for this palette (i.e. if every vertex was
  /// only influenced by one bone,
  /// this variable would contain a value of 0).
  std::int32_t maximum_blend_index_;

}; // End Class bone_palette