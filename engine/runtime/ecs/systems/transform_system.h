#pragma once

#include <core/common/basetypes.hpp>

namespace runtime
{
class transform_system
{
public:
  transform_system();
  ~transform_system();
  //-----------------------------------------------------------------------------
  //  Name : frame_update (virtual )
  /// <summary>
  ///
  ///
  ///
  /// </summary>
  //-----------------------------------------------------------------------------
  void frame_update(delta_t dt);
};
}
