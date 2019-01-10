#include "./ent.h"

namespace ecs {

  namespace detail
  {
  frame_getter_t& get_frame_getter()
  {
   static frame_getter_t f;
   return f;
  }
  }

  // frame_getter_t& get_frame_getter()
  // {
  //   static frame_getter_t f;
  //   return f;
  // }


  void set_frame_getter(frame_getter_t frame_getter)
  {
    detail::get_frame_getter() = frame_getter;
  }

  uint64_t get_frame()
  {
    const auto& f = detail::get_frame_getter();
    if(f)
    {
      return f();
    }
    return 0;
  }

} // namespace ent
