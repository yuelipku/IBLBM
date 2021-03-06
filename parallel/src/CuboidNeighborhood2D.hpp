#ifndef PARALLEL_SRC_CUBOIDNEIGHTBORHOOD2D_HPP_
#define PARALLEL_SRC_CUBOIDNEIGHTBORHOOD2D_HPP_

#include "gsl/gsl"
#include "MpiManager.hpp"
#include "Vector.hpp"

namespace iblbm
{
template<typename T> class SuperStructure2D;

template<typename T>
struct Cell2D
{
  Cell2D()
    : mGlobalCuboidIndex{-1}
  {}

  Cell2D(
      gsl::index globalCuboidIndex
    , gsl::index xIndex
    , gsl::index yIndex
    , const Vector2D<T>& rPhysR)
    : mGlobalCuboidIndex{globalCuboidIndex},
      mLatticeR{Vector2D<gsl::index>{xIndex, yIndex}},
      mPhysR{rPhysR}
  {}

  /** Copy constructor */
  Cell2D(const Cell2D& rRhs) = default;

  /** Copy assignment */
  Cell2D& operator=(const Cell2D& rRhs) = default;

  /**
   * is equal to operator. check that both local position and cuboid number
   * are equal
   */
  bool operator==(const Cell2D& rRhs) const
  {
    return mLatticeR == rRhs.mLatticeR &&
        mGlobalCuboidIndex == rRhs.mGlobalCuboidIndex;
  }

  /** cuboid number */
  gsl::index mGlobalCuboidIndex;
  /** local position */
  Vector2D<gsl::index> mLatticeR;
  /** global position */
  Vector2D<T> mPhysR;
};

template<typename T>
class CuboidNeighborhood2D
{
  friend class TestCuboidNeighborhood2D;
 public:
  /**
   * Constructor
   *
   * \param rSuperStructure reference to superstructure
   * \param cuboidIndex cuboid number
   */
  CuboidNeighborhood2D(
      SuperStructure2D<T>& rSuperStructure
    , gsl::index cuboidIndex);

  /**
   * Destructor
   */
  ~CuboidNeighborhood2D();

  /**
   * Copy constructor
   */
  CuboidNeighborhood2D(const CuboidNeighborhood2D<T>& rRhs);

  /** \return read-only access to mInCells */
  const Cell2D<T>& rGetInCell(gsl::index i) const;

  /** \return number of cells in mInCells */
  std::size_t GetInCellsSize() const;

  /** \return read-only access to mInNbrCuboid */
  const gsl::index& rGetInNeighborCuboid(gsl::index i) const;

  /** \return number of cuboids in mInNbrCuboids */
  std::size_t GetInNeighborCuboidsSize() const;

  /** \return read and write access to mpInData */
  bool** pGetInData();

  /** \return read and write access to mpExData */
  bool** pGetExData();

  /** Add a cell to the vector mInCells */
  void AddInCell(const Cell2D<T>& rCell);

  /** Add a cell to the vector mOutCells */
  void AddExCell(const Cell2D<T>& rCell);

  /**
   * Add a cell to the vector mInCells if the cell is not already there
   * and if there is a cuboid which can deliver the information
   *
   * \param xIndex x index
   * \param yIndex y index
   */
  void AddInCell(
      gsl::index xIndex
    , gsl::index yIndex);

  /**
   * Add all cells with the distance overlap * mDeltaR to the vector
   * mInCells
   *
   * \param overlap size of overlap margin
   */
  void AddInCells(std::size_t overlap);

  /**
   * Initializes mInNbrCuboids and mInNbrNumCells
   */
  void InitializeInNeighbor();

  /**
   * Initializes mOutNbrCuboids and mOutNbrNumCells
   */
  void InitializeExNeighbor();

  /**
   * Sends data coordinates (pure MPI method)
   */
  void SendInDataCoordinates();

  /**
   * Receive data coordinates (pure MPI method)
   */
  void ReceiveExDataCoordinates();

  /**
   * Finishes a communication step
   */
  void FinishComm();

  /**
   * Buffers data to be sent. Interpolation is required if globalXPosition and
   * globalYPosition are not integers
   */
  void BufferExData();

  /**
   * Sends data (pure MPI method)
   */
  void SendExData();

  /**
   * Receives data (pure MPI method)
   */
  void ReceiveInData();

  /**
   * Write all data to the corresponding lattice cells
   */
  void WriteInData();

  /**
   * Resets neighborhood after initialization (InitializeInternal and
   * InitializeExternal)
   */
  void Reset();

 private:
  /** Reference to the super structure */
  SuperStructure2D<T>& mrSuperStructure;
  /** global cuboid index */
  gsl::index mGlobalCuboidIndex;
  /** Number of cuboids in this structure */
  std::size_t mNumCuboid;
  /** node spacing of the cuboid */
  T mDeltaR;
  /** Number of data to be transferred */
  std::size_t mNumData;
  /** Size of the underlying data type */
  std::size_t mSizeofDataType;
  /** Internal needed cells */
  std::vector<Cell2D<T>> mInCells;
  /** External needed cells */
  std::vector<Cell2D<T>> mExCells;
  /** Internal needed neighbor cuboid */
  std::vector<gsl::index> mInNbrCuboids;
  /** Internal needed neighbor number of cells */
  std::vector<std::size_t> mInNbrNumCells;
  /** External needed neighbor cuboid */
  std::vector<gsl::index> mExNbrCuboids;
  /** External needed neighbor number of cells */
  std::vector<std::size_t> mExNbrNumCells;
  /** Buffer for internal needed data */
  bool** mpInData;
  /** Buffer for external needed data */
  bool** mpExData;
  /** Buffer for internal needed data coordinates */
  T** mpInDataCoords;
  /** Buffer for external needed data coordinates */
  T** mpExDataCoords;
  /** Temporary buffer to compute neighbor cuboid index and num cells */
  std::size_t* mpTmpInNbrNumCells;
  /**
   * Flag to indicate if internal neighbor cuboid and number of cells is
   * initialized
   */
  bool mHasInitializedInNeighbor;
  /**
   * Flag to indicate if external neighbor cuboid and number of cells is
   * initialized
   */
  bool mHasInitializedExNeighbor;
#ifdef IBLBM_PARALLEL_MPI
  MpiNonblockingHelper mNonblockingHelper;
#endif  // IBLBM_PARALLEL_MPI
};
}  // namespace iblbm

#endif  // PARALLEL_SRC_CUBOIDNEIGHTBORHOOD2D_HPP_
