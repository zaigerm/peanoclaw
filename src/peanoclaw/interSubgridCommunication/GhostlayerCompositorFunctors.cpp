/*
 * GhostlayerCompositorFunctors.cpp
 *
 *  Created on: Sep 5, 2013
 *      Author: unterweg
 */
#include "peanoclaw/interSubgridCommunication/GhostlayerCompositorFunctors.h"

#include "peanoclaw/interSubgridCommunication/GhostLayerCompositor.h"
#include "peanoclaw/Numerics.h"
#include "peanoclaw/Patch.h"

peanoclaw::interSubgridCommunication::FillGhostlayerFaceFunctor::FillGhostlayerFaceFunctor(
  GhostLayerCompositor& ghostlayerCompositor,
  int                   destinationPatchIndex
) : _ghostlayerCompositor(ghostlayerCompositor), _destinationPatchIndex(destinationPatchIndex) {
}

void peanoclaw::interSubgridCommunication::FillGhostlayerFaceFunctor::operator()
(
  peanoclaw::Patch&                         source,
  int                                       sourceIndex,
  peanoclaw::Patch&                         destination,
  int                                       destinationIndex,
  const tarch::la::Vector<DIMENSIONS, int>& direction
) {
  if(
    (_destinationPatchIndex == -1 || _destinationPatchIndex == destinationIndex)
    && _ghostlayerCompositor.shouldTransferGhostlayerData(source, destination)
  ) {
    int dimension = -1;
    int offset = 0;
    for(int d = 0; d < DIMENSIONS; d++) {
      if(abs(direction(d)) == 1) {
        dimension = d;
        offset = direction(d);
      }
    }
    assertion3(dimension!=-1 && offset != 0, dimension, offset, direction);

    assertionEquals2(destination.getSubdivisionFactor(), source.getSubdivisionFactor(), source, destination);
    tarch::la::Vector<DIMENSIONS, int> subdivisionFactor = source.getSubdivisionFactor();
    tarch::la::Vector<DIMENSIONS, int> faceSize(subdivisionFactor);
    faceSize(dimension) = destination.getGhostLayerWidth();
    tarch::la::Vector<DIMENSIONS, int> destinationOffset(0);
    destinationOffset(dimension) = (offset==1) ? -destination.getGhostLayerWidth() : subdivisionFactor(dimension);

    if(source.getLevel() == destination.getLevel() && source.getLevel() == _ghostlayerCompositor._level) {
      tarch::la::Vector<DIMENSIONS, int> sourceOffset(0);
      sourceOffset(dimension)
        = (offset==1) ? (source.getSubdivisionFactor()(dimension) - source.getGhostLayerWidth()) : 0;

      _ghostlayerCompositor.copyGhostLayerDataBlock(faceSize, sourceOffset, destinationOffset, source, destination);
    } else if(source.getLevel() < destination.getLevel() && destination.getLevel() == _ghostlayerCompositor._level && source.isLeaf()) {
      _ghostlayerCompositor._numerics.interpolate(
        faceSize,
        destinationOffset,
        source,
        destination,
        true,
        false
      );
    }
  }
}

peanoclaw::interSubgridCommunication::FillGhostlayerEdgeFunctor::FillGhostlayerEdgeFunctor(
  GhostLayerCompositor& ghostlayerCompositor,
  int                   destinationPatchIndex
) : _ghostlayerCompositor(ghostlayerCompositor), _destinationPatchIndex(destinationPatchIndex) {
}

void peanoclaw::interSubgridCommunication::FillGhostlayerEdgeFunctor::operator() (
  peanoclaw::Patch&                         source,
  int                                       sourceIndex,
  peanoclaw::Patch&                         destination,
  int                                       destinationIndex,
  const tarch::la::Vector<DIMENSIONS, int>& direction
) {
  if(
    (_destinationPatchIndex == -1 || _destinationPatchIndex == destinationIndex)
    && _ghostlayerCompositor.shouldTransferGhostlayerData(source, destination)
  ) {
    assertionEquals2(source.getSubdivisionFactor(), destination.getSubdivisionFactor(), source, destination);
    int ghostlayerWidth = destination.getGhostLayerWidth();
    tarch::la::Vector<DIMENSIONS, int> subdivisionFactor = source.getSubdivisionFactor();
    tarch::la::Vector<DIMENSIONS, int> edgeSize(ghostlayerWidth);
    tarch::la::Vector<DIMENSIONS, int> sourceOffset(0);
    tarch::la::Vector<DIMENSIONS, int> destinationOffset(0);
    for(int d = 0; d < DIMENSIONS; d++) {
      if(direction(d) == 0) {
        edgeSize(d) = subdivisionFactor(d);
      } else if(direction(d) == 1) {
        sourceOffset(d) = subdivisionFactor(d) - ghostlayerWidth;
        destinationOffset(d) = -ghostlayerWidth;
      } else if(direction(d) == -1) {
        destinationOffset(d) = subdivisionFactor(d);
      } else {
        assertionFail("Direction " << direction << " is invalid!");
      }
    }
    if(source.getLevel() == destination.getLevel() && source.getLevel() == _ghostlayerCompositor._level) {
      _ghostlayerCompositor.copyGhostLayerDataBlock(
        edgeSize,
        sourceOffset,
        destinationOffset,
        source,
        destination
      );
    } else if(source.getLevel() < destination.getLevel() && destination.getLevel() == _ghostlayerCompositor._level && source.isLeaf()) {
      _ghostlayerCompositor._numerics.interpolate(
        edgeSize,
        destinationOffset,
        source,
        destination,
        true,
        false
      );
    }
  }
}

peanoclaw::interSubgridCommunication::FillGhostlayerCornerFunctor::FillGhostlayerCornerFunctor(
  GhostLayerCompositor& ghostlayerCompositor,
  int                   destinationPatchIndex
) : _ghostlayerCompositor(ghostlayerCompositor), _destinationPatchIndex(destinationPatchIndex) {
}

void peanoclaw::interSubgridCommunication::FillGhostlayerCornerFunctor::operator() (
  peanoclaw::Patch&                         source,
  int                                       sourceIndex,
  peanoclaw::Patch&                         destination,
  int                                       destinationIndex,
  const tarch::la::Vector<DIMENSIONS, int>& direction
) {
  if(
    (_destinationPatchIndex == -1 || _destinationPatchIndex == destinationIndex)
    && _ghostlayerCompositor.shouldTransferGhostlayerData(source, destination)
  ) {
    assertionEquals2(source.getSubdivisionFactor(), destination.getSubdivisionFactor(), source, destination);
    int ghostlayerWidth = destination.getGhostLayerWidth();
    tarch::la::Vector<DIMENSIONS, int> subdivisionFactor = source.getSubdivisionFactor();
    tarch::la::Vector<DIMENSIONS, int> cornerSize(ghostlayerWidth);
    tarch::la::Vector<DIMENSIONS, int> sourceOffset(0);
    tarch::la::Vector<DIMENSIONS, int> destinationOffset(0);
    for(int d = 0; d < DIMENSIONS; d++) {
      if(direction(d) == 1) {
        sourceOffset(d) = subdivisionFactor(d) - ghostlayerWidth;
        destinationOffset(d) = -ghostlayerWidth;
      } else if(direction(d) == -1) {
        destinationOffset(d) = subdivisionFactor(d);
      } else {
        assertionFail("Direction " << direction << " is invalid!");
      }
    }
    if(source.getLevel() == destination.getLevel() && source.getLevel() == _ghostlayerCompositor._level) {
      _ghostlayerCompositor.copyGhostLayerDataBlock(
        cornerSize,
        sourceOffset,
        destinationOffset,
        source,
        destination
      );
    } else if(source.getLevel() < destination.getLevel() && destination.getLevel() == _ghostlayerCompositor._level && source.isLeaf()) {
      _ghostlayerCompositor._numerics.interpolate(
        cornerSize,
        destinationOffset,
        source,
        destination,
        true,
        false
      );
    }
  }
}

peanoclaw::interSubgridCommunication::UpdateNeighborTimeFunctor::UpdateNeighborTimeFunctor(
  GhostLayerCompositor& ghostlayerCompositor
) : _ghostlayerCompositor(ghostlayerCompositor) {
}

void peanoclaw::interSubgridCommunication::UpdateNeighborTimeFunctor::operator() (
  peanoclaw::Patch&                         neighborPatch,
  int                                       neighborPatchIndex,
  peanoclaw::Patch&                         updatedPatch,
  int                                       updatedPatchIndexIndex,
  const tarch::la::Vector<DIMENSIONS, int>& direction
) {
  if(updatedPatch.isValid() && neighborPatch.isValid()) {
    updatedPatch.updateMinimalNeighborTimeConstraint(
      neighborPatch.getTimeConstraint(),
      neighborPatch.getCellDescriptionIndex()
    );
    updatedPatch.updateMaximalNeighborTimeInterval(
      neighborPatch.getCurrentTime(),
      neighborPatch.getTimestepSize()
    );

    if(neighborPatch.isLeaf()) {
      updatedPatch.updateMinimalLeafNeighborTimeConstraint(neighborPatch.getTimeConstraint());
    }
  }
}

peanoclaw::interSubgridCommunication::FluxCorrectionFunctor::FluxCorrectionFunctor(
  Numerics& numerics
) : _numerics(numerics) {
}

void peanoclaw::interSubgridCommunication::FluxCorrectionFunctor::operator() (
  peanoclaw::Patch&                         patch1,
  int                                       index1,
  peanoclaw::Patch&                         patch2,
  int                                       index2,
  const tarch::la::Vector<DIMENSIONS, int>& direction
) {
  if(patch1.isLeaf()
      && patch2.isLeaf()) {
    int dimension = -1;
    int offset = 0;
    for(int d = 0; d < DIMENSIONS; d++) {
      if(abs(direction(d)) == 1) {
        dimension = d;
        offset = direction(d);
      }
    }
    assertion3(dimension!=-1 && offset != 0, dimension, offset, direction);

    //Correct from patch1 to patch2
    if(patch1.getLevel() > patch2.getLevel()) {
      _numerics.applyFluxCorrection(patch1, patch2, dimension, offset);
    }

    //Correct from right to left
    if(patch2.getLevel() > patch1.getLevel()) {
      _numerics.applyFluxCorrection(patch2, patch1, dimension, offset);
    }
  }
}

peanoclaw::interSubgridCommunication::UpdateGhostlayerBoundsFaceFunctor::UpdateGhostlayerBoundsFaceFunctor (
  GhostLayerCompositor& ghostlayerCompositor
) : _ghostlayerCompositor(ghostlayerCompositor) {
}

void peanoclaw::interSubgridCommunication::UpdateGhostlayerBoundsFaceFunctor::operator() (
  peanoclaw::Patch&                         patch1,
  int                                       index1,
  peanoclaw::Patch&                         patch2,
  int                                       index2,
  const tarch::la::Vector<DIMENSIONS, int>& direction
) {
  int dimension = -1;
  for(int d = 0; d < DIMENSIONS; d++) {
    if(abs(direction(d)) == 1) {
      dimension = d;
    }
  }

  if(patch1.isLeaf() && patch2.isValid()
      && patch1.getLevel() == patch2.getLevel()) {
    if(index1 < index2) {
      _ghostlayerCompositor.updateUpperGhostlayerBound(index2, index1, dimension);
    } else {
      _ghostlayerCompositor.updateLowerGhostlayerBound(index2, index1, dimension);
    }
  }
}

peanoclaw::interSubgridCommunication::UpdateGhostlayerBoundsEdgeFunctor::UpdateGhostlayerBoundsEdgeFunctor (
  GhostLayerCompositor& ghostlayerCompositor
) : _ghostlayerCompositor(ghostlayerCompositor) {
}

void peanoclaw::interSubgridCommunication::UpdateGhostlayerBoundsEdgeFunctor::operator() (
  peanoclaw::Patch&                         patch1,
  int                                       index1,
  peanoclaw::Patch&                         patch2,
  int                                       index2,
  peanoclaw::Patch&                         faceNeighbor1,
  int                                       indexFaceNeighbor1,
  peanoclaw::Patch&                         faceNeighbor2,
  int                                       indexFaceNeighbor2,
  const tarch::la::Vector<DIMENSIONS, int>& direction
) {
  if((!faceNeighbor1.isValid() || !faceNeighbor1.isLeaf()) && (!faceNeighbor2.isValid() || !faceNeighbor2.isLeaf())) {
    if(patch1.isValid() && patch2.isValid() && (patch1.getLevel() == patch2.getLevel())) {
      for(int d = 0; d < DIMENSIONS; d++) {
        if(direction(d) != 0) {
          if(index1 < index2) {
            _ghostlayerCompositor.updateUpperGhostlayerBound(index2, index1, d);
          } else {
            _ghostlayerCompositor.updateLowerGhostlayerBound(index2, index1, d);
          }
        }
      }
    }
  }
}

peanoclaw::interSubgridCommunication::UpdateGhostlayerBoundsCornerFunctor::UpdateGhostlayerBoundsCornerFunctor (
  GhostLayerCompositor& ghostlayerCompositor
) : _ghostlayerCompositor(ghostlayerCompositor) {
}

void peanoclaw::interSubgridCommunication::UpdateGhostlayerBoundsCornerFunctor::operator() (
  peanoclaw::Patch&                         patch1,
  int                                       index1,
  peanoclaw::Patch&                         patch2,
  int                                       index2,
  peanoclaw::Patch&                         faceNeighbor1,
  int                                       indexFaceNeighbor1,
  peanoclaw::Patch&                         faceNeighbor2,
  int                                       indexFaceNeighbor2,
  const tarch::la::Vector<DIMENSIONS, int>& direction
) {
  assertion("Not implemented, yet!");
//  if((!faceNeighbor1.isValid() || !faceNeighbor1.isLeaf()) && (!faceNeighbor2.isValid() || !faceNeighbor2.isLeaf())) {
//    if(patch1.isValid() && patch2.isValid() && (patch1.getLevel() == patch2.getLevel())) {
//      for(int d = 0; d < DIMENSIONS; d++) {
//        if(direction(d) != 0) {
//          if(index1 < index2) {
//            _ghostlayerCompositor.updateUpperGhostlayerBound(index2, index1, d);
//          } else {
//            _ghostlayerCompositor.updateLowerGhostlayerBound(index2, index1, d);
//          }
//        }
//      }
//    }
//  }
}