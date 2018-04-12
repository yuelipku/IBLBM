#ifndef SRC_GEOMETRY_ABSTRACTBLOCKGEOMETRYINTERFACE2D_HPP_
#define SRC_GEOMETRY_ABSTRACTBLOCKGEOMETRYINTERFACE2D_HPP_

#include "gsl/gsl"
#include "OstreamManager.hpp"

namespace iblbm
{
template<typename T>
class AbstractBlockGeometryInterface2D
{
 public:
  /**
   * Constructor
   */
  AbstractBlockGeometryInterface2D(gsl::index globalCuboidIndex = -1);

 protected:
  /** global cuboid number, default = -1 */
  gsl::index mGlobalCuboidIndex;
  /** Class speciic output stream */
  mutable OstreamManager mOstream;
};
}  // namespace iblbm

#endif  // SRC_GEOMETRY_ABSTRACTBLOCKGEOMETRYINTERFACE2D_HPP_
