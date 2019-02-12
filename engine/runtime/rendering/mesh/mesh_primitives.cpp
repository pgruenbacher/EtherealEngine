
#include "./mesh.h"
#include "runtime/rendering/generator/generator.hpp"

static void create_mesh(const gfx::vertex_layout& format, const generator::any_mesh& mesh,
            mesh::preparation_data& data, math::bbox& bbox)
{

  // Determine the correct offset to any relevant elements in the vertex
  bool has_position = format.has(gfx::attribute::Position);
  bool has_texcoord0 = format.has(gfx::attribute::TexCoord0);
  bool has_normals = format.has(gfx::attribute::Normal);
  bool has_tangents = format.has(gfx::attribute::Tangent);
  bool has_bitangents = format.has(gfx::attribute::Bitangent);
  std::uint16_t vertex_stride = format.getStride();

  auto triangle_count = generator::count(mesh.triangles());
  auto vertex_count = generator::count(mesh.vertices());
  data.triangle_count = std::uint32_t(triangle_count);
  data.vertex_count = std::uint32_t(vertex_count);

  // Allocate enough space for the new vertex and triangle data
  data.vertex_data.resize(data.vertex_count * vertex_stride);
  data.vertex_flags.resize(data.vertex_count);
  data.triangle_data.resize(data.triangle_count);

  std::uint8_t* current_vertex_ptr = &data.vertex_data[0];
  size_t i = 0;
  for(const auto& v : mesh.vertices())
  {

    math::vec3 position = v.position;
    math::vec4 normal = math::vec4(v.normal, 0.0f);
    math::vec2 texcoords0 = v.tex_coord;
    // Store vertex components
    if(has_position)
      gfx::vertex_pack(&position.x, false, gfx::attribute::Position, format, current_vertex_ptr,
               std::uint32_t(i));
    if(has_normals)
      gfx::vertex_pack(&normal.x, true, gfx::attribute::Normal, format, current_vertex_ptr,
               std::uint32_t(i));
    if(has_texcoord0)
      gfx::vertex_pack(&texcoords0.x, true, gfx::attribute::TexCoord0, format, current_vertex_ptr,
               std::uint32_t(i));

    bbox.add_point(position);
    i++;
  }

  size_t tri_idx = 0;
  for(const auto& triangle : mesh.triangles())
  {
    const auto& indices = triangle.vertices;
    auto& tri = data.triangle_data[tri_idx];
    tri.indices[0] = std::uint32_t(indices[0]);
    tri.indices[1] = std::uint32_t(indices[1]);
    tri.indices[2] = std::uint32_t(indices[2]);

    tri_idx++;
  }

  // We need to generate binormals / tangents?
  data.compute_binormals = has_bitangents;
  data.compute_tangents = has_tangents;
}

bool mesh::create_cylinder(const gfx::vertex_layout& format, float radius, float height, std::uint32_t stacks,
               std::uint32_t slices, mesh_create_origin origin, bool hardware_copy /* = true */)
{
  // Clear out old data.
  dispose();

  // We are in the process of preparing.
  prepare_status_ = mesh_status::preparing;
  vertex_format_ = format;

  using namespace generator;
  capped_cylinder_mesh_t cylinder(radius, height * 0.5, slices, stacks);
  math::quat rot(math::vec3(math::radians(-90.0f), 0.f, 0.0f));
  auto mesh = rotate_mesh(cylinder, rot);

  create_mesh(vertex_format_, mesh, preparation_data_, bbox_);
  // Finish up
  return end_prepare(hardware_copy, false, false);
}

bool mesh::create_capsule(const gfx::vertex_layout& format, float radius, float height, std::uint32_t stacks,
              std::uint32_t slices, mesh_create_origin origin, bool hardware_copy /* = true */)
{
  // Clear out old data.
  dispose();

  // We are in the process of preparing.
  prepare_status_ = mesh_status::preparing;
  vertex_format_ = format;

  using namespace generator;
  capsule_mesh_t capsule(radius, height * 0.5, slices, stacks);
  math::quat rot(math::vec3(math::radians(-90.0f), 0.f, 0.0f));
  auto mesh = rotate_mesh(capsule, rot);

  create_mesh(vertex_format_, mesh, preparation_data_, bbox_);
  // Finish up
  return end_prepare(hardware_copy, false, false);
}

bool mesh::create_sphere(const gfx::vertex_layout& format, float radius, std::uint32_t stacks,
             std::uint32_t slices, mesh_create_origin origin, bool hardware_copy /* = true */)
{
  // Clear out old data.
  dispose();

  // We are in the process of preparing.
  prepare_status_ = mesh_status::preparing;
  vertex_format_ = format;

  using namespace generator;
  sphere_mesh_t sphere(radius, slices, stacks);
  math::quat rot(math::vec3(math::radians(-90.0f), 0.f, 0.0f));
  auto mesh = rotate_mesh(sphere, rot);

  create_mesh(vertex_format_, mesh, preparation_data_, bbox_);
  // Finish up
  return end_prepare(hardware_copy, false, false);
}

bool mesh::create_torus(const gfx::vertex_layout& format, float outer_radius, float inner_radius,
            std::uint32_t bands, std::uint32_t sides, mesh_create_origin origin,
            bool hardware_copy /* = true */)
{
  // Clear out old data.
  dispose();

  // We are in the process of preparing.
  prepare_status_ = mesh_status::preparing;
  vertex_format_ = format;

  using namespace generator;
  torus_mesh_t torus(inner_radius, outer_radius, sides, bands);
  math::quat rot(math::vec3(math::radians(-90.0f), 0.f, 0.0f));
  auto mesh = rotate_mesh(torus, rot);

  create_mesh(vertex_format_, mesh, preparation_data_, bbox_);
  // Finish up
  return end_prepare(hardware_copy, false, false);
}

bool mesh::create_teapot(const gfx::vertex_layout& format, bool hardware_copy /*= true*/)
{
  // Clear out old data.
  dispose();

  // We are in the process of preparing.
  prepare_status_ = mesh_status::preparing;
  vertex_format_ = format;

  using namespace generator;
  teapot_mesh_t teapot;
  math::quat rot(math::vec3(math::radians(-90.0f), 0.f, 0.0f));
  auto mesh = rotate_mesh(teapot, rot);

  create_mesh(vertex_format_, mesh, preparation_data_, bbox_);
  // Finish up
  return end_prepare(hardware_copy, false, false);
}

bool mesh::create_icosahedron(const gfx::vertex_layout& format, bool hardware_copy /*= true*/)
{
  // Clear out old data.
  dispose();

  // We are in the process of preparing.
  prepare_status_ = mesh_status::preparing;
  vertex_format_ = format;

  using namespace generator;
  icosahedron_mesh_t icosahedron;
  math::quat rot(math::vec3(math::radians(-90.0f), 0.f, 0.0f));
  auto mesh = rotate_mesh(icosahedron, rot);

  create_mesh(vertex_format_, mesh, preparation_data_, bbox_);
  // Finish up
  return end_prepare(hardware_copy, false, false);
}

bool mesh::create_dodecahedron(const gfx::vertex_layout& format, bool hardware_copy /*= true*/)
{
  // Clear out old data.
  dispose();

  // We are in the process of preparing.
  prepare_status_ = mesh_status::preparing;
  vertex_format_ = format;

  using namespace generator;
  dodecahedron_mesh_t dodecahedron;
  math::quat rot(math::vec3(math::radians(-90.0f), 0.f, 0.0f));
  auto mesh = rotate_mesh(dodecahedron, rot);

  create_mesh(vertex_format_, mesh, preparation_data_, bbox_);
  // Finish up
  return end_prepare(hardware_copy, false, false);
}

bool mesh::create_icosphere(const gfx::vertex_layout& format, int tesselation_level,
              bool hardware_copy /*= true*/)
{
  // Clear out old data.
  dispose();

  // We are in the process of preparing.
  prepare_status_ = mesh_status::preparing;
  vertex_format_ = format;

  using namespace generator;
  ico_sphere_mesh_t icosphere(1, tesselation_level + 1);
  math::quat rot(math::vec3(math::radians(-90.0f), 0.f, 0.0f));
  auto mesh = rotate_mesh(icosphere, rot);

  create_mesh(vertex_format_, mesh, preparation_data_, bbox_);
  // Finish up
  return end_prepare(hardware_copy, false, false);
}

bool mesh::create_cone(const gfx::vertex_layout& format, float radius, float radius_tip, float height,
             std::uint32_t stacks, std::uint32_t slices, mesh_create_origin origin,
             bool hardware_copy /* = true */)
{
  // Clear out old data.
  dispose();

  // We are in the process of preparing.
  prepare_status_ = mesh_status::preparing;
  vertex_format_ = format;

  using namespace generator;
  capped_cone_mesh_t cone(radius, 1.0, stacks, slices);
  math::quat rot(math::vec3(math::radians(-90.0f), 0.f, 0.0f));
  auto mesh = rotate_mesh(cone, rot);

  create_mesh(vertex_format_, mesh, preparation_data_, bbox_);
  // Finish up
  return end_prepare(hardware_copy, false, false);
}

bool mesh::create_plane(const gfx::vertex_layout& format, float width, float height,
            std::uint32_t width_segments, std::uint32_t height_segments,
            mesh_create_origin origin, bool hardware_copy /* = true */)
{
  // Clear out old data.
  dispose();

  // We are in the process of preparing.
  prepare_status_ = mesh_status::preparing;
  vertex_format_ = format;

  using namespace generator;
  plane_mesh_t plane({width * 0.5f, height * 0.5f}, {width_segments, height_segments});
  math::quat rot(math::vec3(math::radians(-90.0f), 0.f, 0.0f));
  auto mesh = rotate_mesh(plane, rot);

  create_mesh(vertex_format_, mesh, preparation_data_, bbox_);
  // Finish up
  return end_prepare(hardware_copy, false, false);
}

bool mesh::create_cube(const gfx::vertex_layout& format, float width, float height, float depth,
             std::uint32_t width_segments, std::uint32_t height_segments,
             std::uint32_t depth_segments, mesh_create_origin origin,
             bool hardware_copy /* = true */)
{
  // Clear out old data.
  dispose();

  // We are in the process of preparing.
  prepare_status_ = mesh_status::preparing;
  vertex_format_ = format;

  using namespace generator;
  box_mesh_t box({width * 0.5f, height * 0.5f, depth * 0.5f},
           {width_segments, height_segments, depth_segments});
  math::quat rot(math::vec3(math::radians(-90.0f), 0.f, 0.0f));
  auto mesh = rotate_mesh(box, rot);

  create_mesh(vertex_format_, mesh, preparation_data_, bbox_);
  // Finish up
  return end_prepare(hardware_copy, false, false);
}