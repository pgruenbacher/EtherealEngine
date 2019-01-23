#include <gtest/gtest.h>

#include <core/system/subsystem.h>
#include <runtime/ecs/constructs/utils.h>
#include <core/serialization/associative_archive.h>
#include <core/serialization/binary_archive.h>


#include "runtime/ecs/components/audio_listener_component.h"
#include "runtime/ecs/components/audio_source_component.h"
#include "runtime/ecs/components/camera_component.h"
#include "runtime/ecs/components/light_component.h"
#include "runtime/ecs/components/model_component.h"
#include "runtime/ecs/components/reflection_probe_component.h"
#include "runtime/ecs/components/relation.h"
#include "runtime/ecs/components/transform_component.h"


TEST(Serialize, Stream) {

  std::vector<EntityType> ents{0,1,2,3};
  auto copied = ents;

  {
    std::ofstream os("/tmp/test2.json", std::fstream::binary | std::fstream::trunc);

    cereal::oarchive_associative_t archive(os);
    archive.setNextName("entities");
    archive.startNode();
    archive(ents);
    archive.finishNode();
  }
  ents.clear();


  std::ifstream is("/tmp/test2.json", std::fstream::binary);
  cereal::iarchive_associative_t output(is);
  output.setNextName("entities");
  output.startNode();
  EntityType ent;
  output(ents);
  output.finishNode();
  ASSERT_EQ(ents, copied);
}

TEST(Serialize, Stream2) {

  Registry r;

  for (int i = 0; i < 10; ++i) {
    r.create();
  }

  {
    std::ofstream os("/tmp/test3.json", std::fstream::binary | std::fstream::trunc);

    cereal::oarchive_associative_t archive(os);
    archive.setNextName("entities");
    archive.startNode();
    r.snapshot().entities(archive);
    archive.finishNode();
  }

  entt::continuous_loader loader{r};

  std::ifstream is("/tmp/test3.json", std::fstream::binary);
  cereal::iarchive_associative_t output(is);
  output.setNextName("entities");
  output.startNode();
  EntityType ent;
  loader.entities(output);
  output.finishNode();

  r.each([](EntityType ent) {

  });
}


TEST(Serialize, Utils) {
  core::details::initialize();
  auto& ecs = core::add_subsystem<SpatialSystem>();

  auto factory = ecs::utils::get_default_ent_factory(ecs);
  {
    auto ent = factory.create();
    ecs.assign<camera_component>(ent);
  }

  {
    auto ent = factory.create();
    ecs.assign<transform_component>(ent);
  }

  {
    auto ent = factory.create();
    ecs.assign<reflection_probe_component>(ent);
  }

  {
    auto ent = factory.create();
    ecs.assign<light_component>(ent);
  }

  {
    auto ent = factory.create();
    ecs.assign<light_component>(ent);
  }

  std::vector<EntityType> ents;
  ecs.each([&ents](EntityType e) { ents.push_back(e); });
  ecs::utils::save_entities_to_file("/tmp/test.json", ents);
  ecs::utils::load_entities_from_file("/tmp/test.json", ents);
}
