#include <gtest/gtest.h>

#include "runtime/ecs/components/reflection_probe_component.h"
#include <sstream>
#include <cereal/archives/json.hpp>
#include "runtime/ecs/constructs/snapshots.h"


struct timer {
    int duration;
    int elapsed{0};
};

struct relationship {
    entt::registry::entity_type parent;
    entt::registry::entity_type global_id;
};

struct position {
    float x{0.0};
    float y{0.0};
};

struct notused {
    float x{0.0};
};

using ecs::output_archive;
using ecs::input_archive;
using ecs::continuous_archive;

template<typename Archive>
void serialize(Archive &archive, position &position) {
    archive(cereal::make_nvp("x", position.x));
    archive(cereal::make_nvp("y", position.y));
}

template<typename Archive>
void serialize(Archive &archive, timer &timer) {
  archive(timer.duration);
}

template<typename Archive>
void serialize(Archive &archive, relationship &relationship) {
    archive(cereal::make_nvp("parent", relationship.parent));
    archive(cereal::make_nvp("global", relationship.global_id));
}

template<typename Archive>
void serialize(Archive &archive, notused &unused) {
    archive(unused.x);
}

TEST(Serialize, Basic) {

    std::stringstream storage;

    entt::registry source;
    entt::registry destination;

    auto e0 = source.create();
    source.assign<position>(e0);
    auto e1 = source.create();
    source.assign<position>(e1);

    auto e3 = source.create();
    source.assign<timer>(e3);
    auto e4 = source.create();
    source.assign<relationship>(e4, e3);

    ASSERT_EQ(source.size(), 4);
    {
        // output finishes flushing its contents when it goes out of scope
        cereal::oarchive_associative_t json_output{storage};
        output_archive output(json_output, source);
        output.set<position>("position");
        output.set<relationship>("relationship");
        output.set<timer>("timer");
        // components that don't exist should be skipped.
        output.set<notused>("notused");
    }

    // std::cout << "Storag ? " << storage.str() << std::endl;

    cereal::iarchive_associative_t json_input{storage};
    input_archive input(json_input, destination);
    input.get<position>("position");
    input.get<timer>("timer");
    input.get<relationship>("relationship");
    input.get<notused>("notused");

    ASSERT_EQ(destination.view<timer>().size(), 1);
    ASSERT_EQ(destination.view<position>().size(), 2);
    ASSERT_EQ(destination.view<relationship>().size(), 1);
}

TEST(Serialize, Continuous) {

    std::stringstream storage;

    entt::registry source;
    entt::registry destination;


    auto e0 = source.create();
    source.assign<position>(e0);
    auto e1 = source.create();
    source.assign<position>(e1);

    auto globalEnt = source.create();

    auto e3 = source.create();
    source.assign<timer>(e3);
    auto e4 = source.create();
    auto& rel = source.assign<relationship>(e4, relationship{e3, globalEnt});
    std::cout << "global?" << rel.global_id << std::endl;

    ASSERT_EQ(source.size(), 5);
    {
        // output finishes flushing its contents when it goes out of scope
        cereal::oarchive_associative_t json_output{storage};
        output_archive output(json_output, source);
        output.set<position>("position");
        output.set<relationship>("relationship");
        output.set<timer>("timer");
    }

    std::cout << "Storag ? " << storage.str() << std::endl;

    cereal::iarchive_associative_t json_input{storage};

    {
        continuous_archive input(json_input, destination);
        input.get<position>("position");
        input.get<timer>("timer");
        input.get<relationship>("relationship", &relationship::parent);
    }
    {
        continuous_archive input(json_input, destination);
        input.get<position>("position");
        input.get<timer>("timer");
        input.get<relationship>("relationship", &relationship::parent);
    }

    ASSERT_EQ(destination.view<timer>().size(), 2);
    ASSERT_EQ(destination.view<position>().size(), 4);
    ASSERT_EQ(destination.view<relationship>().size(), 2);

    auto view = destination.view<relationship>();
    EntityType prevParent = entt::null;
    for (auto ent : view) {
        // the parent entity should be unique for each new copy.
        auto rel = view.get(ent);
        ASSERT_NE(prevParent, rel.parent);
        prevParent = rel.parent;
        // non-local ents stay as they were. so be careful.
        ASSERT_EQ(rel.global_id, globalEnt);
    }
}
