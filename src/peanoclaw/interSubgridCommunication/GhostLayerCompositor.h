/*
 * GhostLayerCompositor.h
 *
 *  Created on: Feb 16, 2012
 *      Author: Kristof Unterweger
 */

#ifndef PEANO_APPLICATIONS_PEANOCLAW_GHOSTLAYERCOMPOSITOR_H_
#define PEANO_APPLICATIONS_PEANOCLAW_GHOSTLAYERCOMPOSITOR_H_

#include "peanoclaw/interSubgridCommunication/Interpolation.h"
#include "peanoclaw/interSubgridCommunication/Restriction.h"
#include "peanoclaw/pyclaw/PyClaw.h"
#include "peanoclaw/records/CellDescription.h"

#include "peano/utils/Globals.h"

#include "tarch/logging/Log.h"

namespace peanoclaw {

  class Patch;

  namespace interSubgridCommunication {
    class GhostLayerCompositor;
  }

  namespace tests {
    class GhostLayerCompositorTest;
  }
} /* namespace peanoclaw */

/**
 * In this class the functionality for setting the ghostlayer
 * data is implemented as it is seen from a vertex. The numbering
 * of the patches array is done like the setting of the adjacent
 * indices on the arrays, thus a cell writes its index in the
 * i-th adjacent vertex in position i. This leads to the
 * following numbering:
 *
 * \code
 * patch1   patch0
 *     vertex
 * patch3   patch2
 * \endcode
 *
 * Besides the mere filling of the ghostlayers, the computation
 * of the "maximum neighbor time interval" and of the
 * "minimal neighbor time" is done in this class.
 *
 * @see peanoclaw::Patch for further details.
 *
 *
 */
class peanoclaw::interSubgridCommunication::GhostLayerCompositor
{

private:
  /**
   * Logging device.
   */
  static tarch::logging::Log _log;

  friend class peanoclaw::tests::GhostLayerCompositorTest;

  Patch* _patches;

  int _level;

  peanoclaw::pyclaw::PyClaw& _pyClaw;

  peanoclaw::interSubgridCommunication::Interpolation _interpolation;

  peanoclaw::interSubgridCommunication::Restriction _restriction;

  bool _useDimensionalSplitting;

  /**
   * Performs a copy of a block from the uNew of one cell to the ghostlayer of uOld
   * in a neighbouring cell.
   */
  void copyGhostLayerDataBlock(
    const tarch::la::Vector<DIMENSIONS, int>&    size,
    const tarch::la::Vector<DIMENSIONS, int>&    sourceOffset,
    const tarch::la::Vector<DIMENSIONS, int>&    destinationOffset,
    const peanoclaw::Patch& source,
    peanoclaw::Patch&       destination);

  /**
   * Updates the maximum allowed timestep size for a patch dependent on the
   * time interval spanned by a neighboring patch.
   *
   */
  void updateNeighborTime(int updatedPatchIndex, int neighborPatchIndex);

  /**
   * Proxy, can be removed when the interpolation is fixed.
   */
//  void interpolate(
//    const tarch::la::Vector<DIMENSIONS, int>&    destinationSize,
//    const tarch::la::Vector<DIMENSIONS, int>&    destinationOffset,
//    const peanoclaw::Patch& source,
//    peanoclaw::Patch&       destination,
//    bool interpolateToUOld,
//    bool interpolateToCurrentTime
//  );

  //Ghost layers, i.e. copy from one patch to another.
  void fillLeftGhostLayer();

  void fillLowerLeftGhostLayer();

  void fillLowerGhostLayer();

  void fillUpperLeftGhostLayer();

  void fillUpperGhostLayer();

  void fillUpperRightGhostLayer();

  void fillRightGhostLayer();

  void fillLowerRightGhostLayer();

  /**
   * Returns the area of the region where the two given
   * patches overlap.
   * This overload projects the patches along the given projection axis
   * and just calculates the overlap in this projection.
   *
   * In 2d this refers to a projection to one-dimensional intervals and
   * the intersection between these intervals.
   */
  double calculateOverlappingArea(
    tarch::la::Vector<DIMENSIONS, double> position1,
    tarch::la::Vector<DIMENSIONS, double> size1,
    tarch::la::Vector<DIMENSIONS, double> position2,
    tarch::la::Vector<DIMENSIONS, double> size2,
    int projectionAxis
  ) const;

  /**
   * Updates the lower ghostlayer bound for the given pair of patches.
   */
  void updateLowerGhostlayerBound(int updatedPatchIndex, int neighborPatchIndex, int dimension);

  /**
   * Updates the upper ghostlayer bound for the given pair of patches.
   */
  void updateUpperGhostlayerBound(int updatedPatchIndex, int neighborPatchIndex, int dimension);

  /**
   * Applies the correction depending on the fine grid fluxes on the
   * coarse grid.
   *
   * @param finePatch
   * @param coarsePatch
   * @param dimension Dimensions that is perpendicular to the interface between
   *                  the patches.
   * @param direction Either 1 or -1, depending wether the coarse patch is
   *                  located "higher" or "lower" with respect to the dimension.
   */
  void applyCoarseGridCorrection(
    const Patch& finePatch,
    Patch& coarsePatch,
    int dimension,
    int direction
  ) const;

  /**
   * Estimates, whether ghostlayer data should be transfered
   * from the source to the destination patch.
   */
  bool shouldTransferGhostlayerData(Patch& sourcePatch, Patch& destinationPatch);

public:

  /**
   * Constructor for a GhostLayerCompositor. GhostLayerCompositors are vertex-centered.
   * Therefore, the given patches are the $2^d$ patches adjacent to the current vertex
   * and the given level is the level on which the vertex is residing.
   */
  GhostLayerCompositor(
    peanoclaw::Patch patches[TWO_POWER_D],
    int level,
    peanoclaw::pyclaw::PyClaw& pyClaw,
    bool useDimensionalSplitting
  );

  ~GhostLayerCompositor();

  /**
   * Fills the ghostlayers of the given patches as far as possible. Also the
   * maximum timestep size for the patches is adjusted.
   */
  void fillGhostLayers(int destinationPatchIndex = -1);

  /**
   * Updates the maximum timesteps for the given patches depending on the time
   * interval spanned by the other patches and the maximum neighbor time interval.
   */
  void updateNeighborTimes();

  /**
   * Updates the ghostlayer bounds, i.e. how far a patch is overlapped by
   * neighboring ghostlayers.
   */
  void updateGhostlayerBounds();

  /**
   * Applies the coarse grid correction on all adjacent coarse patches.
   */
  void applyCoarseGridCorrection();
};
#endif /* PEANO_APPLICATIONS_PEANOCLAW_GHOSTLAYERCOMPOSITOR_H_ */
