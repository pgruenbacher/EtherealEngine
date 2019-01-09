#include <gtest/gtest.h>

#include <sstream>
#include <cereal/archives/json.hpp>
#include "runtime/ecs/components/reflection_probe_component.h"


struct position {
    float x{0.0};
    float y{0.0};
};

template<typename Storage>
struct output_archive {
    output_archive(Storage &storage)
        : storage{storage}
    {}

    template<typename... Value>
    void operator()(const ent::EntityType ent, const Value &... value) {
        std::cout << "ent operator " << ent << std::endl;
        // (std::get<std::queue<Value>>(storage).push(ent));
        // (std::get<std::queue<Value>>(storage).push(value), ...);
    }

    template<typename... Value>
    void operator()(const Value &... value) {
        std::cout << "basic operator " << std::endl;
        // (std::get<std::queue<Value>>(storage).push(value), ...);
    }

private:
    Storage &storage;
};

template<typename Storage>
struct input_archive {
    input_archive(Storage &storage)
        : storage{storage}
    {}

    template<typename... Value>
    void operator()(Value &... value) {
        auto assign = [this](auto &value) {
            std::cout << "assign? " << std::endl;
            // auto &queue = std::get<std::queue<std::decay_t<decltype(value)>>>(storage);
            // value = queue.front();
            // queue.pop();
        };

        (assign(value), ...);
    }

private:
    Storage &storage;
};

template<typename Archive>
void serialize(Archive &archive, position &position) {
  archive(position.x, position.y);
}

TEST(Serialize, ReflectionProbeComponent) {

    std::stringstream storage;

    entt::registry<> source;
    entt::registry<> destination;

    auto e0 = source.create();
    source.assign<position>(e0);
    auto e1 = source.create();
    source.assign<position>(e1);

    std::cout << "E " << e0 << " " << e1 << std::endl;


    ASSERT_EQ(source.size(), 2);
    {
        // output finishes flushing its contents when it goes out of scope
        output_archive<std::stringstream> output{storage};
        source.snapshot()
          .component<position>(output)
          .component<position>(output);
    }

    // std::cout << "Storag ? " << storage.str() << std::endl;

    input_archive<std::stringstream> input{storage};
    destination.loader()
        .entities(input)
        // .destroyed(input)
        .component<position>(input);
}