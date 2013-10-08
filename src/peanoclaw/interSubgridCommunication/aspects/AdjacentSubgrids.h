/*
 * AdjacentSubgrids.h
 *
 *  Created on: Jul 24, 2013
 *      Author: kristof
 */

#ifndef PEANOCLAW_INTERSUBGRIDCOMMUNICATION_ASPECTS_ADJACENTSUBGRIDS_H_
#define PEANOCLAW_INTERSUBGRIDCOMMUNICATION_ASPECTS_ADJACENTSUBGRIDS_H_

#include "peanoclaw/Vertex.h"
#include "peanoclaw/interSubgridCommunication/GridLevelTransfer.h"
#include "peanoclaw/records/VertexDescription.h"

#include "tarch/la/Vector.h"
#include "tarch/la/VectorCompare.h"
#include "tarch/logging/Log.h"

#include <map>


#define DIMENSIONS_PLUS_ONE (DIMENSIONS+1)

namespace peanoclaw {
  namespace interSubgridCommunication {
    namespace aspects {
      class AdjacentSubgrids;
      class CheckIntersectingParallelAndAdaptiveBoundaryFunctor;
    }
  }
}

/**
 * Encapsulation of the adjacent subgrids/peano cells stored within
 * a vertex. This class provides the functionality to keep this
 * adjacency information consistent.
 */
class peanoclaw::interSubgridCommunication::aspects::AdjacentSubgrids {

  public:
    typedef peanoclaw::records::CellDescription CellDescription;
    typedef peanoclaw::records::VertexDescription VertexDescription;
    typedef std::map<tarch::la::Vector<DIMENSIONS_PLUS_ONE,double>, VertexDescription, tarch::la::VectorCompare<DIMENSIONS_PLUS_ONE> > VertexMap;

  private:
    /**
     * Logging device.
     */
    static tarch::logging::Log            _log;
    peanoclaw::Vertex&                    _vertex;
    VertexMap&                            _vertexMap;
    tarch::la::Vector<DIMENSIONS, double> _position;
    int                                   _level;

    tarch::la::Vector<DIMENSIONS_PLUS_ONE, double> createVertexKey() const;

  public:
    AdjacentSubgrids(
      peanoclaw::Vertex& vertex,
      VertexMap& vertexMap,
      tarch::la::Vector<DIMENSIONS, double> position,
      int level
    );

    /**
     * Called when a subgrid adjacent to the corresponding vertex of
     * this AdjacentSubgrids object has been created.
     *
     * @param cellDescriptionIndex The global index referencing the cell
     * description of the newly created subgrid.
     * @param subgridIndex The local index that describes the position of the
     * created subgrid with respect to the current vertex.
     */
    void createdAdjacentSubgrid(int cellDescriptionIndex, int subgridIndex);

    /**
     * Called when a persistent vertex is destroyed, since it may be turned
     * to a hanging vertex.
     */
    void convertPersistentToHangingVertex();

    /**
     * Called when a persistent vertex is created, since it may be constructed
     * from a hanging vertex.
     */
    void convertHangingVertexToPersistentVertex();

    /**
     * Called when creating a hanging vertex.
     */
    void createHangingVertex(
      peanoclaw::Vertex * const                                coarseGridVertices,
      const peano::grid::VertexEnumerator&                     coarseGridVerticesEnumerator,
      const tarch::la::Vector<DIMENSIONS,int>&                 fineGridPositionOfVertex,
      tarch::la::Vector<DIMENSIONS, double>                    domainOffset,
      tarch::la::Vector<DIMENSIONS, double>                    domainSize,
      peanoclaw::interSubgridCommunication::GridLevelTransfer& gridLevelTransfer
    );

    /**
     * Called when destroying a hanging vertex.
     */
    void destroyHangingVertex(
      tarch::la::Vector<DIMENSIONS, double>    domainOffset,
      tarch::la::Vector<DIMENSIONS, double>    domainSize
    );

    /**
     * Stores the adjacent ranks of the current vertex for the
     * next grid iteration.
     */
    void storeAdjacencyInformation();

    /**
     * Triggers the refinement of vertices in such a way that the
     * grid is always at least 2-irregular.
     */
    void regainTwoIrregularity(
      peanoclaw::Vertex * const            coarseGridVertices,
      const peano::grid::VertexEnumerator& coarseGridVerticesEnumerator
    );

    /**
     * TODO unterweg dissertation
     * Triggers a refine when a vertex resides on both, a parallel
     * and an adaptive boundary and two subgrids have to communicate
     * over their vertices (or edges in 3D).
     *
     * Loops through dimensions and check whether the adaptive boundary
     * is completely perpendicular to this dimension. If for one
     * dimension this does not hold, the vertex needs to be refined.
     */
    void refineOnParallelAndAdaptiveBoundary();
};

/**
 * Used to determine whether a parallel boundary coincides with a corner
 * of an adaptive boundary, such that a refinement is required.
 */
class peanoclaw::interSubgridCommunication::aspects::CheckIntersectingParallelAndAdaptiveBoundaryFunctor {

  private:
    const tarch::la::Vector<DIMENSIONS_TIMES_TWO, int>& _adjacentRanks;
    bool                                                _parallelBoundaryCoincidesWithAdaptiveBoundaryCorner;

  public:
    CheckIntersectingParallelAndAdaptiveBoundaryFunctor(
      const tarch::la::Vector<DIMENSIONS_TIMES_TWO, int>& adjacentRanks
    );

    void operator() (
      peanoclaw::Patch&                         patch1,
      int                                       index1,
      peanoclaw::Patch&                         patch2,
      int                                       index2,
      const tarch::la::Vector<DIMENSIONS, int>& direction
    );

    bool doesParallelBoundaryCoincideWithAdaptiveBoundaryCorner() const;
};

#endif /* PEANOCLAW_INTERSUBGRIDCOMMUNICATION_ADJACENTSUBGRIDS_H_ */
