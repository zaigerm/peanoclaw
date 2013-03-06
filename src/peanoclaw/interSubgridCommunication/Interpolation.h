/*
 * Interpolation.h
 *
 *  Created on: Mar 5, 2013
 *      Author: kristof
 */

#ifndef PEANOCLAW_INTERSUBGRIDCOMMUNICATION_INTERPOLATION_H_
#define PEANOCLAW_INTERSUBGRIDCOMMUNICATION_INTERPOLATION_H_

#include "tarch/la/Vector.h"
#include "peano/utils/Dimensions.h"

namespace peanoclaw {
  namespace interSubgridCommunication {
    class Interpolation;
  }

  class Patch;

  namespace pyclaw {
    class PyClaw;
  }
}

class peanoclaw::interSubgridCommunication::Interpolation {

  private:

    const peanoclaw::pyclaw::PyClaw& _pyClaw;

    /**
     * Copies data from the source patch to the destination patch. The difference
     * to the method copyGhostLayerDataBlock is that in this method a discrete
     * part (i.e. a rectangular block of cells) is specified in the destination
     * patch while the appropriate part from the source block is determined by
     * the positions, sizes and subdivision factors of the patches. Therefore,
     * this method also works on patches which have different subcell sizes, due
     * to level or subdivision factor.
     * This method performs a mapping from continuous points in one patch to the
     * discrete cells in the other patch. We use a d-linear interpolation. Since
     * for fine cells we might need information outside the source patch, we use
     * d-linear extrapolation for this.
     *
     * The two parameters interpolateToUOld and interpolateToCurrentTime are
     * helpers while the UNew array does not hold a ghostlayer. There are three
     * situations when an interpolation is performed:
     *
     *  - Filling of ghostlayer: Interpolating to UOld array but using the time
     *    of UNew
     *  - Interpolating to UNew of new patch: Interpolating to UNew array and
     *    using the time of UNew
     *  - Interpolating to UOld of new patch: Interpolating to UOld array and
     *    using the time of UOld
     *
     */
    void interpolateDLinear (
        const tarch::la::Vector<DIMENSIONS, int>&    destinationSize,
        const tarch::la::Vector<DIMENSIONS, int>&    destinationOffset,
        const peanoclaw::Patch& source,
        peanoclaw::Patch&        destination,
        bool interpolateToUOld,
        bool interpolateToCurrentTime
    );

    void interpolateDLinearVersion2 (
        const tarch::la::Vector<DIMENSIONS, int>&    destinationSize,
        const tarch::la::Vector<DIMENSIONS, int>&    destinationOffset,
        const peanoclaw::Patch& source,
        peanoclaw::Patch&        destination,
        bool interpolateToUOld,
        bool interpolateToCurrentTime
    );

  public:

    /**
     * Constructs an Interpolation object based on a PyClaw object
     * that might hold a customized interpolation method.
     */
    Interpolation(const peanoclaw::pyclaw::PyClaw& pyclaw);

    /**
     * Copies data from the source patch to the destination patch. The difference
     * to the method copyGhostLayerDataBlock is that in this method a discrete
     * part (i.e. a rectangular block of cells) is specified in the destination
     * patch while the appropriate part from the source block is determined by
     * the positions, sizes and subdivision factors of the patches. Therefore,
     * this method also works on patches which have different subcell sizes, due
     * to level or subdivision factor.
     * This method performs a mapping from continuous points in one patch to the
     * discrete cells in the other patch. We use a d-linear interpolation. Since
     * for fine cells we might need information outside the source patch, we use
     * d-linear extrapolation for this.
     *
     * The two parameters interpolateToUOld and interpolateToCurrentTime are
     * helpers while the UNew array does not hold a ghostlayer. There are three
     * situations when an interpolation is performed:
     *
     *  - Filling of ghostlayer: Interpolating to UOld array but using the time
     *    of UNew
     *  - Interpolating to UNew of new patch: Interpolating to UNew array and
     *    using the time of UNew
     *  - Interpolating to UOld of new patch: Interpolating to UOld array and
     *    using the time of UOld
     *
     */
    void interpolate (
        const tarch::la::Vector<DIMENSIONS, int>&    destinationSize,
        const tarch::la::Vector<DIMENSIONS, int>&    destinationOffset,
        const peanoclaw::Patch& source,
        peanoclaw::Patch&        destination,
        bool interpolateToUOld = true,
        bool interpolateToCurrentTime = true
    );

};


#endif /* PEANOCLAW_INTERSUBGRIDCOMMUNICATION_INTERPOLATION_H_ */
