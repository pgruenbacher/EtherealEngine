#include <gtest/gtest.h>
#include "runtime/rendering/mesh.h"
#include <core/graphics/index_buffer.h>
#include <core/graphics/vertex_buffer.h>


TEST(Mesh, Memory) {
  {
    auto ptr = std::make_shared<gfx::vertex_buffer>();
    auto ptr2 = std::make_shared<gfx::index_buffer>();
  }

  {
    mesh m;
  }
}
