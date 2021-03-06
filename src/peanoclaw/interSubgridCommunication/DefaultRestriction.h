/*
 * DefaultRestriction.h
 *
 *  Created on: Mar 7, 2013
 *      Author: kristof
 */

#ifndef PEANOCLAW_INTERSUBGRIDCOMMUNICATION_DEFAULTRESTRICTION_H_
#define PEANOCLAW_INTERSUBGRIDCOMMUNICATION_DEFAULTRESTRICTION_H_

#include "peanoclaw/grid/SubgridAccessor.h"
#include "peanoclaw/interSubgridCommunication/Restriction.h"
#include "peanoclaw/tests/GhostLayerCompositorTest.h"
#include "peanoclaw/tests/GridLevelTransferTest.h"

#include "tarch/la/Vector.h"

#include <algorithm>

namespace peanoclaw {
  namespace interSubgridCommunication {
    class DefaultRestriction;

    template<int NumberOfUnknowns>
    class DefaultRestrictionTemplate;
  }
}

template<int NumberOfUnknowns>
class peanoclaw::interSubgridCommunication::DefaultRestrictionTemplate {

  private:
    /**
     * Logging device for the trace macros.
     */
    static tarch::logging::Log  _log;

    friend class peanoclaw::tests::GhostLayerCompositorTest;
    friend class peanoclaw::tests::GridLevelTransferTest;

    /**
     * Returns the region of the region where the two given patches overlap.
     */
    inline double calculateOverlappingRegion(
        const tarch::la::Vector<DIMENSIONS, double>& position1,
        const tarch::la::Vector<DIMENSIONS, double>& size1,
        const tarch::la::Vector<DIMENSIONS, double>& position2,
        const tarch::la::Vector<DIMENSIONS, double>& size2
    ) {
      double region = 1.0;

      for(int d = 0; d < DIMENSIONS; d++) {
        double overlappingInterval =
            std::min(position1(d)+size1(d), position2(d)+size2(d))
        - std::max(position1(d), position2(d));
        region *= overlappingInterval;

        region = std::max(region, 0.0);
      }

      return region;
    }

    /**
     * Restricts the data from the given source to the destination averaging
     * over the source cells that overlap with a destination cell.
     *
     * @param source The patch to get the data from
     * @param destination The patch to restrict the data to
     * @param destinationTime The time that should be used as the destination
     * time when interpolating in time
     * @param restrictToUOld Decides wether to restrict to UOld or to UNew
     */
    void restrictRegion (
      peanoclaw::Patch&                                   source,
      peanoclaw::Patch&                                   destination,
      peanoclaw::grid::SubgridIterator<NumberOfUnknowns>& sourceIterator,
      peanoclaw::grid::SubgridIterator<NumberOfUnknowns>& destinationIterator,
      const peanoclaw::geometry::Region&                  region
    );

  public:
    void restrictSolution (
      peanoclaw::Patch& source,
      peanoclaw::Patch&       destination,
      bool restrictOnlyOverlappedRegions
    );
};

/**
 * Performs a normal averaging restriction between grid levels.
 */
class peanoclaw::interSubgridCommunication::DefaultRestriction
    : public peanoclaw::interSubgridCommunication::Restriction {
  public:
    void restrictSolution (
      peanoclaw::Patch& source,
      peanoclaw::Patch&       destination,
      bool restrictOnlyOverlappedRegions
    ) {
      switch(source.getUnknownsPerSubcell()) {
        case 1:
          peanoclaw::interSubgridCommunication::DefaultRestrictionTemplate<1> transfer1;
          transfer1.restrictSolution(source, destination, restrictOnlyOverlappedRegions);
          break;
        case 2:
          peanoclaw::interSubgridCommunication::DefaultRestrictionTemplate<2> transfer2;
          transfer2.restrictSolution(source, destination, restrictOnlyOverlappedRegions);
          break;
        case 3:
          peanoclaw::interSubgridCommunication::DefaultRestrictionTemplate<3> transfer3;
          transfer3.restrictSolution(source, destination, restrictOnlyOverlappedRegions);
          break;
        case 4:
          peanoclaw::interSubgridCommunication::DefaultRestrictionTemplate<4> transfer4;
          transfer4.restrictSolution(source, destination, restrictOnlyOverlappedRegions);
          break;
        case 5:
          peanoclaw::interSubgridCommunication::DefaultRestrictionTemplate<5> transfer5;
          transfer5.restrictSolution(source, destination, restrictOnlyOverlappedRegions);
          break;
        case 6:
          peanoclaw::interSubgridCommunication::DefaultRestrictionTemplate<6> transfer6;
          transfer6.restrictSolution(source, destination, restrictOnlyOverlappedRegions);
          break;
        case 7:
          peanoclaw::interSubgridCommunication::DefaultRestrictionTemplate<7> transfer7;
          transfer7.restrictSolution(source, destination, restrictOnlyOverlappedRegions);
          break;
        case 8:
          peanoclaw::interSubgridCommunication::DefaultRestrictionTemplate<8> transfer8;
          transfer8.restrictSolution(source, destination, restrictOnlyOverlappedRegions);
          break;
        case 9:
          peanoclaw::interSubgridCommunication::DefaultRestrictionTemplate<9> transfer9;
          transfer9.restrictSolution(source, destination, restrictOnlyOverlappedRegions);
          break;
        case 10:
          peanoclaw::interSubgridCommunication::DefaultRestrictionTemplate<10> transfer10;
          transfer10.restrictSolution(source, destination, restrictOnlyOverlappedRegions);
          break;
        default:
          assertionFail("Number of unknowns " << source.getUnknownsPerSubcell() << " not supported!");
      }
    }
};

#include "peanoclaw/interSubgridCommunication/DefaultRestriction.cpph"

#endif /* PEANOCLAW_INTERSUBGRIDCOMMUNICATION_DEFAULTRESTRICTION_H_ */
