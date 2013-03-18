// This file is part of the Peano project. For conditions of distribution and 
// use, please see the copyright notice at www.peano-framework.org
#ifndef PEANOCLAW_ADAPTERS_Cleanup_H_
#define PEANOCLAW_ADAPTERS_Cleanup_H_


#include "tarch/logging/Log.h"
#include "tarch/la/Vector.h"

#include "peano/grid/VertexEnumerator.h"
#include "peano/MappingSpecification.h"

#include "tarch/multicore/MulticoreDefinitions.h"

#include "peanoclaw/Vertex.h"
#include "peanoclaw/Cell.h"
#include "peanoclaw/State.h"

#include "peanoclaw/mappings/Cleanup.h"




namespace peanoclaw {
      namespace adapters {
        class Cleanup;
      } 
}


/**
 * This is a mapping from the spacetree traversal events to your user-defined activities.
 * The latter are realised within the mappings. 
 * 
 * @author Peano Development Toolkit (PDT) by  Tobias Weinzierl
 * @version $Revision: 1.10 $
 */
class peanoclaw::adapters::Cleanup {
  private:
    peanoclaw::mappings::Cleanup _map2Cleanup;


  public:
    static peano::MappingSpecification   touchVertexLastTimeSpecification();
    static peano::MappingSpecification   touchVertexFirstTimeSpecification();
    static peano::MappingSpecification   enterCellSpecification();
    static peano::MappingSpecification   leaveCellSpecification();
    static peano::MappingSpecification   ascendSpecification();
    static peano::MappingSpecification   descendSpecification();

    Cleanup();

    #if defined(SharedMemoryParallelisation)
    Cleanup(const Cleanup& masterThread);
    #endif

    virtual ~Cleanup();
  
    #if defined(SharedMemoryParallelisation)
    void mergeWithWorkerThread(const Cleanup& workerThread);
    #endif

    void createInnerVertex(
      peanoclaw::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      peanoclaw::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      peanoclaw::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
    );


    void createBoundaryVertex(
      peanoclaw::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      peanoclaw::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      peanoclaw::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
    );


    void createHangingVertex(
      peanoclaw::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      peanoclaw::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      peanoclaw::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
    );


    void destroyHangingVertex(
      const peanoclaw::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      peanoclaw::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      peanoclaw::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
    );


    void destroyVertex(
      const peanoclaw::Vertex&   fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      peanoclaw::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      peanoclaw::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
    );


    void createCell(
      peanoclaw::Cell&                 fineGridCell,
      peanoclaw::Vertex * const         fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      peanoclaw::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      peanoclaw::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
    );


    void destroyCell(
      const peanoclaw::Cell&           fineGridCell,
      peanoclaw::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      peanoclaw::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      peanoclaw::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
    );
        
    #ifdef Parallel
    void mergeWithNeighbour(
      peanoclaw::Vertex&  vertex,
      const peanoclaw::Vertex&  neighbour,
      int                                           fromRank,
      const tarch::la::Vector<DIMENSIONS,double>&   fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&   fineGridH,
      int                                           level
    );

    void prepareSendToNeighbour(
      peanoclaw::Vertex&  vertex,
      int  toRank,
      int  level
    );

    void prepareCopyToRemoteNode(
      peanoclaw::Vertex&  localVertex,
      int  toRank
    );

    void prepareCopyToRemoteNode(
      peanoclaw::Cell&  localCell,
      int  toRank
    );

    void mergeWithRemoteDataDueToForkOrJoin(
      peanoclaw::Vertex&  localVertex,
      const peanoclaw::Vertex&  masterOrWorkerVertex,
      int                                       fromRank,
      const tarch::la::Vector<DIMENSIONS,double>&  x,
      const tarch::la::Vector<DIMENSIONS,double>&  h,
      int                                       level
    );

    void mergeWithRemoteDataDueToForkOrJoin(
      peanoclaw::Cell&  localCell,
      const peanoclaw::Cell&  masterOrWorkerCell,
      int                                       fromRank,
      const tarch::la::Vector<DIMENSIONS,double>&  cellCentre,
      const tarch::la::Vector<DIMENSIONS,double>&  cellSize,
      int                                       level
    );

    void prepareSendToWorker(
      peanoclaw::Cell&                 fineGridCell,
      peanoclaw::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      peanoclaw::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      peanoclaw::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell,
      int                                                                  worker
    );

    void prepareSendToMaster(
      peanoclaw::Cell&     localCell,
      peanoclaw::Vertex *  vertices,
      const peano::grid::VertexEnumerator&  verticesEnumerator
    );

    void mergeWithMaster(
      const peanoclaw::Cell&           workerGridCell,
      peanoclaw::Vertex * const        workerGridVertices,
      const peano::grid::VertexEnumerator& workerEnumerator,
      peanoclaw::Cell&                 fineGridCell,
      peanoclaw::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      peanoclaw::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      peanoclaw::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell,
      int                                                                  worker,
      const peanoclaw::State&           workerState,
      peanoclaw::State&                 masterState
    );


    void receiveDataFromMaster(
      peanoclaw::Cell&                    receivedCell, 
      peanoclaw::Vertex *                 receivedVertices,
      const peano::grid::VertexEnumerator&    verticesEnumerator
    );


    void mergeWithWorker(
      peanoclaw::Cell&           localCell, 
      const peanoclaw::Cell&     receivedMasterCell
    );


    void mergeWithWorker(
      peanoclaw::Vertex&        localVertex,
      const peanoclaw::Vertex&  receivedMasterVertex
    );
    #endif


    void touchVertexFirstTime(
      peanoclaw::Vertex&               fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
      peanoclaw::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      peanoclaw::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
    );


    void touchVertexLastTime(
      peanoclaw::Vertex&         fineGridVertex,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
      const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
      peanoclaw::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      peanoclaw::Cell&           coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
    );
    

    void enterCell(
      peanoclaw::Cell&                 fineGridCell,
      peanoclaw::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      peanoclaw::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      peanoclaw::Cell&                 coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
    );


    void leaveCell(
      peanoclaw::Cell&                          fineGridCell,
      peanoclaw::Vertex * const                 fineGridVertices,
      const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
      peanoclaw::Vertex * const                 coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      peanoclaw::Cell&                          coarseGridCell,
      const tarch::la::Vector<DIMENSIONS,int>&      fineGridPositionOfCell
    );


    void beginIteration(
      peanoclaw::State&  solverState
    );


    void endIteration(
      peanoclaw::State&  solverState
    );

    void descend(
      peanoclaw::Cell * const          fineGridCells,
      peanoclaw::Vertex * const        fineGridVertices,
      const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
      peanoclaw::Vertex * const        coarseGridVertices,
      const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
      peanoclaw::Cell&                 coarseGridCell
    );


    void ascend(
      peanoclaw::Cell * const    fineGridCells,
      peanoclaw::Vertex * const  fineGridVertices,
      const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
      peanoclaw::Vertex * const  coarseGridVertices,
      const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
      peanoclaw::Cell&           coarseGridCell
    );    
};


#endif