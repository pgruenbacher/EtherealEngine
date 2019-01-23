#include <gtest/gtest.h>

#include "editor_runtime/system/app.h"
#include <core/filesystem/filesystem.h>
#include <runtime/ecs/ent.h>

void initialize_protocols() {

  fs::path engine_path = fs::absolute(fs::path(ENGINE_DIRECTORY));
  fs::path shader_include_path = fs::absolute(fs::path(SHADER_INCLUDE_DIRECTORY));

  fs::path engine = engine_path / "engine_data";
  fs::path editor = engine_path / "editor_data";
  // fs::path binary_path = fs::executable_path(argv[0]).parent_path();
  fs::add_path_protocol("engine:", engine);
  fs::add_path_protocol("editor:", editor);
  // fs::add_path_protocol("binary:", binary_path);
  fs::add_path_protocol("shader_include:", shader_include_path);

}

TEST(App, DefaultScene) {
  // return;
  initialize_protocols();

  editor::app edit;

  edit.setup_testing();
  edit.create_new_scene();
  // auto& ecs = core::get_subsystem<SpatialSystem>();
  // ecs.reset();
  // edit.run_one_frame();

  edit.stop();
  core::details::dispose();
}
