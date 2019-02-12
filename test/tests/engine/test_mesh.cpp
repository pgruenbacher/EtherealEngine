#include <gtest/gtest.h>
#include "runtime/rendering/mesh/mesh.h"
#include <core/graphics/index_buffer.h>
#include <core/graphics/vertex_buffer.h>
#include <runtime/animation/animation.h>
#include <editor_runtime/assets/mesh_importer.h>


TEST(Mesh, Memory) {
  {
    auto ptr = std::make_shared<gfx::vertex_buffer>();
    auto ptr2 = std::make_shared<gfx::index_buffer>();
  }

  {
    mesh m;
  }
}

void validate(std::string path) {
  mesh::load_data data;
  std::vector<runtime::animation> animations;
  bool valid = importer::load_mesh_data_from_file(path, data, animations);
  ASSERT_TRUE(valid);
  ASSERT_GT(data.vertex_count, 0);
  ASSERT_EQ(animations.size(), 0);
}

TEST(Mesh, Obj) {
  validate("../3rdparty/assimp/assimp/test/models/OBJ/regr01.obj");
}

TEST(Mesh, HumanBlend) {
  validate("../3rdparty/assimp/assimp/test/models/BLEND/HUMAN.blend");
}

TEST(Mesh, DAE) {
  validate("../3rdparty/assimp/assimp/test/models/Collada/duck.dae");
}

TEST(Mesh, FBX) {
  validate("../3rdparty/assimp/assimp/test/models/FBX/spider.fbx");
}


TEST(Mesh, Primitives) {

  // no gpu use during test.
  bool hardwareCopy = false;

  mesh m;

  // m.create_cylinder();
  // m.create_capsule();
  m.create_sphere(gfx::mesh_vertex::get_layout(), 0.5f, 20, 20, mesh_create_origin::center, hardwareCopy);
  // m.create_torus();
  // m.create_teapot();
  // m.create_icosahedron();
  // m.create_dodecahedron();
  // m.create_icosphere();
  // m.create_cone();
  // m.create_plane();
  // m.create_cube();
}