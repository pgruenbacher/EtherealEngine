#include <gtest/gtest.h>

#include <memory>
#include "runtime/ecs/components/reflection_probe_component.h"


reflection_probe_component factory() {
  return reflection_probe_component();
}

TEST(Reflection, Probe) {
  entt::reflect<reflection_probe_component>(
    "reflection_probe_component",
    std::make_pair("category"_hs, "LIGHTING"),
    std::make_pair("pretty_name"_hs, "Reflection Probe Component")
  ).ctor<>();

  // entt::reflect<reflection_probe_component>(
  //   "reflection_probe_component",
  //   std::make_pair("pretty_name", "Reflection Probe Component")
  // );

  // entt::reflect<reflection_probe_component>(
  //   "reflection_probe_component"
  // ).ctor<>();


  entt::reflect<std::shared_ptr<reflection_probe_component>>(
    "reflection_probe_component_ptr"
  ).ctor<std::make_shared<reflection_probe_component>>();


  auto resolved = entt::resolve("reflection_probe_component_ptr");

  auto any = resolved.ctor().invoke();
  ASSERT_TRUE(any);
  ASSERT_TRUE(any.can_cast<std::shared_ptr<reflection_probe_component>>());

  any = entt::resolve("reflection_probe_component").ctor().invoke();
  ASSERT_TRUE(any);
  ASSERT_TRUE(any.can_cast<reflection_probe_component>());

  auto pr = entt::resolve<reflection_probe_component>().prop("category"_hs);
  ASSERT_TRUE(pr);
  pr = entt::resolve<reflection_probe_component>().prop("pretty_name"_hs);
  ASSERT_TRUE(pr);
  // ASSERT_EQ(pr.key(), "category");
  // ASSERT_EQ(pr.value(), "LIGHTING");

}

