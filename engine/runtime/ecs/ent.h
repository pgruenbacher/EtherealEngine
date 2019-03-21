#pragma once

#include <entt/entt.hpp>
#include <core/serialization/serialization.h>
#include <core/reflection/registration.h>
#include <core/common/nonstd/type_index.hpp>

using EntityType = entt::registry::entity_type;
using ActorType = entt::actor;
using Registry = entt::registry;
using SpatialSystem = Registry;

namespace ecs {
  using frame_getter_t = std::function<std::uint64_t()>;
  void set_frame_getter(frame_getter_t frame_getter);
  std::uint64_t get_frame();
}

namespace ent {


  template <typename C>
  using chandle = std::weak_ptr<C>;

  class component : public std::enable_shared_from_this<component> {
    REFLECTABLEV(component)
    SERIALIZABLE(component)
  protected:
    virtual rtti::type_index_sequential_t::index_t runtime_id() const = 0;
  };


  template <typename T>
  class component_impl : public component
  {
  private:
    std::uint32_t last_touched_;

  public:
    rtti::type_index_sequential_t::index_t runtime_id() const override
    {
      return static_id();
    }
    // component_impl() = default;
    // component_impl(component_impl& rhs) = delete;
    // component_impl& operator=(component_impl& rhs) = delete;

    static rtti::type_index_sequential_t::index_t static_id()
    {
      return rtti::type_index_sequential_t::id<component, T>();
    }

    // chandle<T> handle()
    // {
    //   return std::static_pointer_cast<T>(shared_from_this());
    // }

    void touch()
    {
      last_touched_ = static_cast<std::uint32_t>(ecs::get_frame());
    }
    bool is_touched() const
    {
      return last_touched_ == static_cast<std::uint32_t>(ecs::get_frame()) - 1;
    }
  };

}
