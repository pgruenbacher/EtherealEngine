#include <gtest/gtest.h>

#include "runtime/ecs/components/reflection_probe_component.h"
#include <sstream>
#include <cereal/archives/json.hpp>



struct timer {
    int duration;
    int elapsed{0};
};

struct relationship {
    entt::registry<>::entity_type parent;
};

struct position {
    float x{0.0};
    float y{0.0};
};

template<typename Storage>
struct output_archive {
    output_archive(Storage &storage, const ent::Registry& reg)
        : _storage{storage}, _reg(reg)
    {}

    template<typename C>
    void set(const char* s) {
        _storage.setNextName(s);
        _storage.startNode();
        _reg.snapshot().component<C>(*this);
        _storage.finishNode();
    }

    template<typename... Value>
    void operator()(const Value &... value) {
        // std::cout << "ASS " << std::endl;
        (_storage(value), ...);
    }

private:
    Storage &_storage;
    const ent::Registry &_reg;
};

template<typename Storage>
struct input_archive {
    input_archive(Storage &storage, ent::Registry& reg)
        : _storage{storage}, _loader(reg.loader())
    {}

    template<typename C>
    void get(const char* s) {
        try {
            _storage.setNextName(s);
            _storage.startNode();
            _loader.component<C>(*this);
            _storage.finishNode();
        } catch (const cereal::Exception& e) {
            return;
        }
    }

    template<typename... Value>
    void operator()(Value &... value) {
        (_storage(value), ...);
    }

private:
    Storage& _storage;
    entt::snapshot_loader<ent::EntityType> _loader;
};

template<typename Archive>
void serialize(Archive &archive, position &position) {
  archive(position.x, position.y);
}

template<typename Archive>
void serialize(Archive &archive, timer &timer) {
  archive(timer.duration);
}

template<typename Archive>
void serialize(Archive &archive, relationship &relationship) {
  archive(relationship.parent);
}

TEST(Serialize, Basic) {

    std::stringstream storage;

    entt::registry<> source;
    entt::registry<> destination;

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
        cereal::JSONOutputArchive json_output{storage};
        output_archive output(json_output, source);
        output.set<position>("position");
        output.set<relationship>("relationship");
        output.set<timer>("timer");
    }

    std::cout << "Storag ? " << storage.str() << std::endl;

    cereal::JSONInputArchive json_input{storage};
    input_archive input(json_input, destination);
    input.get<position>("position");
    input.get<timer>("timer");
    input.get<relationship>("relationship");

    ASSERT_EQ(destination.view<timer>().size(), 1);
    ASSERT_EQ(destination.view<position>().size(), 2);
    ASSERT_EQ(destination.view<relationship>().size(), 1);
}
