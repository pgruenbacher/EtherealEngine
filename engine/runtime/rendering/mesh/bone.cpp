#include "./bone.h"

///////////////////////////////////////////////////////////////////////////////
// bone_palette Member Definitions
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
//  Name : bone_palette() (Constructor)
/// <summary>
/// Class constructor.
/// </summary>
//-----------------------------------------------------------------------------
bone_palette::bone_palette(std::uint32_t palette_size)
  : data_group_id_(0)
  , maximum_size_(palette_size)
  , maximum_blend_index_(-1)
{
}

//-----------------------------------------------------------------------------
//  Name : bone_palette() (Copy Constructor)
/// <summary>
/// Class copy constructor.
/// </summary>
//-----------------------------------------------------------------------------
bone_palette::bone_palette(const bone_palette& rhs)
  : bones_lut_(rhs.bones_lut_)
  , bones_(rhs.bones_)
  , faces_(rhs.faces_)
  , data_group_id_(rhs.data_group_id_)
  , maximum_size_(rhs.maximum_size_)
  , maximum_blend_index_(rhs.maximum_blend_index_)
{
}

//-----------------------------------------------------------------------------
//  Name : ~bone_palette() (Destructor)
/// <summary>
/// Clean up any resources being used.
/// </summary>
//-----------------------------------------------------------------------------
bone_palette::~bone_palette()
{
}

std::vector<math::transform>
bone_palette::get_skinning_matrices(const std::vector<math::transform>& node_transforms,
                  const skin_bind_data& bind_data, bool compute_inverse_transpose) const
{
  // Retrieve the main list of bones from the skin bind data that will
  // be referenced by the palette's bone index list.
  const auto& bind_list = bind_data.get_bones();
  if(node_transforms.empty())
    return node_transforms;

  const std::uint32_t max_blend_transforms = gfx::get_max_blend_transforms();
  std::vector<math::transform> transforms;
  transforms.resize(max_blend_transforms);

  // Compute transformation matrix for each bone in the palette
  for(size_t i = 0; i < bones_.size(); ++i)
  {
    auto bone = bones_[i];
    const auto& bone_transform = node_transforms[bone];
    const auto& bone_data = bind_list[bone];
    auto& transform = transforms[i];
    transform = bone_transform * bone_data.bind_pose_transform;
    if(compute_inverse_transpose)
    {
      transform = math::transpose(math::inverse(transform));
    }

  } // Next Bone

  return transforms;
}

void bone_palette::assign_bones(bone_index_map_t& bones, std::vector<std::uint32_t>& faces)
{
  bone_index_map_t::iterator it_bone, it_bone2;

  // Iterate through newly specified input bones and add any unique ones to the
  // palette.
  for(it_bone = bones.begin(); it_bone != bones.end(); ++it_bone)
  {
    it_bone2 = bones_lut_.find(it_bone->first);
    if(it_bone2 == bones_lut_.end())
    {
      bones_lut_[it_bone->first] = static_cast<std::uint32_t>(bones_.size());
      bones_.push_back(it_bone->first);

    } // End if not already added

  } // Next Bone

  // Merge the new face list with ours.
  faces_.insert(faces_.end(), faces.begin(), faces.end());
}

void bone_palette::assign_bones(const std::vector<std::uint32_t>& bones)
{
  bone_index_map_t::iterator it_bone;

  // Clear out prior data.
  bones_.clear();
  bones_lut_.clear();

  // Iterate through newly specified input bones and add any unique ones to the
  // palette.
  for(size_t i = 0; i < bones.size(); ++i)
  {
    it_bone = bones_lut_.find(bones[i]);
    if(it_bone == bones_lut_.end())
    {
      bones_lut_[bones[i]] = static_cast<std::uint32_t>(bones_.size());
      bones_.push_back(bones[i]);

    } // End if not already added

  } // Next Bone
}

void bone_palette::compute_palette_fit(bone_index_map_t& input, std::int32_t& current_space,
                     std::int32_t& common_bones, std::int32_t& additional_bones)
{
  // Reset values
  current_space = static_cast<std::int32_t>(maximum_size_ - static_cast<std::uint32_t>(bones_.size()));
  common_bones = 0;
  additional_bones = 0;

  // Early out if possible
  if(bones_.size() == 0)
  {
    additional_bones = static_cast<std::int32_t>(input.size());
    return;

  } // End if no bones stored
  else if(input.size() == 0)
  {
    return;

  } // End if no bones input

  // Iterate through newly specified input bones and see how many
  // indices it has in common with our existing set.
  bone_index_map_t::iterator it_bone, it_bone2;
  for(it_bone = input.begin(); it_bone != input.end(); ++it_bone)
  {
    it_bone2 = bones_lut_.find(it_bone->first);
    if(it_bone2 != bones_lut_.end())
      common_bones++;
    else
      additional_bones++;

  } // Next Bone
}

std::uint32_t bone_palette::translate_bone_to_palette(std::uint32_t bone_index) const
{
  auto it_bone = bones_lut_.find(bone_index);
  if(it_bone == bones_lut_.end())
    return 0xFFFFFFFF;
  return it_bone->second;
}

std::uint32_t bone_palette::get_data_group() const
{
  return data_group_id_;
}

void bone_palette::set_data_group(std::uint32_t group)
{
  data_group_id_ = group;
}

std::int32_t bone_palette::get_maximum_blend_index() const
{
  return maximum_blend_index_;
}

void bone_palette::set_maximum_blend_index(int nIndex)
{
  maximum_blend_index_ = nIndex;
}

std::uint32_t bone_palette::get_maximum_size() const
{
  return maximum_size_;
}

std::vector<std::uint32_t>& bone_palette::get_influenced_faces()
{
  return faces_;
}

void bone_palette::clear_influenced_faces()
{
  faces_.clear();
}

const std::vector<std::uint32_t>& bone_palette::get_bones() const
{
  return bones_;
}