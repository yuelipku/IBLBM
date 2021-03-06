#ifndef DYNAMICS_SRC_BULKMOMENTA_HPP_
#define DYNAMICS_SRC_BULKMOMENTA_HPP_

#include <vector>

#include "AbstractMomentaInterface.hpp"
#include "Cell.hpp"

namespace iblbm
{
template<typename T, template<typename U> class Lattice>
class BulkMomenta : public AbstractMomentaInterface<T, Lattice>
{
 public:
  /**
   * Compute particle density on the cell
   *
   * \param rCell target cell
   * \return particle density
   */
  T ComputeRho(const Cell<T, Lattice>& rCell) const override;

  /**
   * Compute fluid velocity on the cell
   *
   * \param rCell target cell
   * \param rU fluid velocity
   */
  void ComputeU(
      const Cell<T, Lattice>& rCell
    , std::vector<T>& rU) const override;
};
}  // namespace iblbm

#endif  // DYNAMICS_SRC_BULKMOMENTA_HPP_
