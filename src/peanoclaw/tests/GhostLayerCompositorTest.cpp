/*
 * GhostLayerCompositorTest.cpp
 *
 *  Created on: Mar 6, 2012
 *      Author: Kristof Unterweger
 */
#include "peano/applications/peanoclaw/tests/GhostLayerCompositorTest.h"

#include "peano/applications/peanoclaw/tests/PyClawTestStump.h"
#include "peano/applications/peanoclaw/tests/Helper.h"

#include "peano/applications/peanoclaw/Area.h"
#include "peano/applications/peanoclaw/Patch.h"
#include "peano/applications/peanoclaw/PyClaw.h"
#include "peano/applications/peanoclaw/PatchOperations.h"
#include "peano/applications/peanoclaw/GhostLayerCompositor.h"
#include "peano/applications/peanoclaw/records/CellDescription.h"

#include "peano/kernel/heap/Heap.h"
#include "peano/utils/Loop.h"

#include "tarch/tests/TestCaseFactory.h"
registerTest(peano::applications::peanoclaw::tests::GhostLayerCompositorTest)


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",off)
#endif

tarch::logging::Log peano::applications::peanoclaw::tests::GhostLayerCompositorTest::_log("peano::applications::peanoclaw::tests::GhostLayerCompositorTest");

peano::applications::peanoclaw::tests::GhostLayerCompositorTest::GhostLayerCompositorTest() {
}

peano::applications::peanoclaw::tests::GhostLayerCompositorTest::~GhostLayerCompositorTest() {
}

void peano::applications::peanoclaw::tests::GhostLayerCompositorTest::setUp() {
}

void peano::applications::peanoclaw::tests::GhostLayerCompositorTest::run() {
  //TODO unterweg debug
//  testMethod( testTimesteppingVeto2D );
//  testMethod( testInterpolationFromCoarseToFinePatchLeftGhostLayer2D );
//  testMethod( testInterpolationFromCoarseToFinePatchRightGhostLayer2D );
//  testMethod( testProjectionFromCoarseToFinePatchRightGhostLayer2D );
//  testMethod( testCoarseGridCorrection );
  testMethod( testRestrictionWithOverlappingBounds );
  testMethod( testPartialRestrictionAreas );
  testMethod( testPartialRestrictionAreasWithInfiniteLowerBounds );
}

void peano::applications::peanoclaw::tests::GhostLayerCompositorTest::testTimesteppingVeto2D() {
  #ifdef Dim2

  for(int vetoIndex = 0; vetoIndex < TWO_POWER_D; vetoIndex++) {

    logDebug("testTimesteppingVeto2D", "testing vetoIndex=" << vetoIndex);

    peano::applications::peanoclaw::Patch patches[TWO_POWER_D];

    for(int i = 0; i < TWO_POWER_D; i++) {
      patches[i] = createPatch(
        3,       //Unknowns per subcell
        0,       //Aux fields per subcell
        16,      //Subdivision factor
        2,       //Ghostlayer width
        0.0,     //Position
        1.0/3.0, //Size
        0,       //Level
        0.0,     //Current time
        1.0,     //Timestep size
        1.0      //Minimal neighbor time
      );
    }
    patches[vetoIndex] =
        createPatch(
            3,       //Unknowns per subcell
            0,       //Aux fields per subcell
            16,      //Subdivision factor
            2,       //Ghostlayer width
            0.0,     //Position
            1.0/3.0, //Size
            0,       //Level
            0.5,     //Current time
            1.0,     //Timestep size
            1.0      //Minimal neighbor time
           );

    for(int i = 0; i < TWO_POWER_D; i++) {
      patches[i].resetMinimalNeighborTimeConstraint();
      validate(patches[i].isValid());
    }

    peano::applications::peanoclaw::PyClaw pyClaw(0, 0, 0, 0);
    peano::applications::peanoclaw::GhostLayerCompositor ghostLayerCompositor
      = peano::applications::peanoclaw::GhostLayerCompositor(
          patches,
          0,
          pyClaw,
          false
          );
    ghostLayerCompositor.fillGhostLayers(-1);
    ghostLayerCompositor.updateNeighborTimes();

    for(int cellIndex = 0; cellIndex < 4; cellIndex++) {
      if(cellIndex == vetoIndex) {
        validateNumericalEqualsWithParams2(patches[cellIndex].getMinimalNeighborTimeConstraint(), 1.0, vetoIndex, cellIndex);
        validateNumericalEqualsWithParams2(patches[cellIndex].isAllowedToAdvanceInTime(), false, vetoIndex, cellIndex);
      } else {
        validateNumericalEqualsWithParams2(patches[cellIndex].getMinimalNeighborTimeConstraint(), 1.0, vetoIndex, cellIndex);
        validateNumericalEqualsWithParams2(patches[cellIndex].isAllowedToAdvanceInTime(), true, vetoIndex, cellIndex);
      }
    }
  }
  #endif
}

void peano::applications::peanoclaw::tests::GhostLayerCompositorTest::testInterpolationFromCoarseToFinePatchLeftGhostLayer2D() {
  #ifdef Dim2
  //Patch-array for lower-left vertex in fine patch
  peano::applications::peanoclaw::Patch patches[TWO_POWER_D];

  //Settings
  int coarseSubdivisionFactor = 2;
  int coarseGhostlayerWidth = 1;
  int fineSubdivisionFactor = 2;
  int fineGhostlayerWidth = 2;
  int unknownsPerSubcell = 1;

  //Setup coarse patch
  tarch::la::Vector<DIMENSIONS, double> coarsePosition;
  assignList(coarsePosition) = 2.0, 1.0;
  patches[1] = createPatch(
    unknownsPerSubcell,
    0,   //Aux fields per subcell
    coarseSubdivisionFactor,
    coarseGhostlayerWidth,
    coarsePosition,
    3.0, //Coarse size
    0,   //Level
    0.0, //Time
    1.0, //Timestep size
    1.0, //Minimal neighbor time
    false   //Overlapped by coarse ghost layer
  );
  patches[3] = patches[1];

  //Setup fine patch
  tarch::la::Vector<DIMENSIONS, double> finePosition;
  assignList(finePosition) = 5.0, 2.0;
  patches[0] = createPatch(
    unknownsPerSubcell,
    0,   //Aux fields per subcell
    fineSubdivisionFactor,
    fineGhostlayerWidth,
    finePosition,
    1.0, //Size
    1,   //Level
    0.0, //Time
    1.0/3.0, //Timestep size
    1.0  //Minimal neighbor time
  );

  //Fill coarse patch
  dfor(index, coarseSubdivisionFactor) {
    tarch::la::Vector<DIMENSIONS, int> subcellIndex;
    subcellIndex(0) = index(1);
    subcellIndex(1) = index(0);
    patches[1].setValueUNew(subcellIndex, 0, 1.0);
  }
  dfor(index, coarseSubdivisionFactor + 2*coarseGhostlayerWidth) {
    tarch::la::Vector<DIMENSIONS, int> subcellIndex;
    subcellIndex(0) = index(1) - coarseGhostlayerWidth;
    subcellIndex(1) = index(0) - coarseGhostlayerWidth;
    patches[1].setValueUOld(subcellIndex, 0, -1.0);
  }
  tarch::la::Vector<DIMENSIONS, int> subcellIndex;
  assignList(subcellIndex) = 0, 0;
  patches[1].setValueUOld(subcellIndex, 0, 6.0);
  assignList(subcellIndex) = 0, 1;
  patches[1].setValueUOld(subcellIndex, 0, 7.0);
  assignList(subcellIndex) = 1, 0;
  patches[1].setValueUOld(subcellIndex, 0, 10.0);
  assignList(subcellIndex) = 1, 1;
  patches[1].setValueUOld(subcellIndex, 0, 11.0);

  //Fill left ghostlayer
  peano::applications::peanoclaw::tests::PyClawTestStump pyClaw;
  peano::applications::peanoclaw::GhostLayerCompositor ghostLayerCompositor(
    patches,
    1,
    pyClaw,
    false
  );
  ghostLayerCompositor.fillGhostLayers(-1);

  assignList(subcellIndex) = -2, 0;
  validateNumericalEquals(patches[0].getValueUOld(subcellIndex, 0), 65.0/9.0);
  assignList(subcellIndex) = -2, 1;
  validateNumericalEquals(patches[0].getValueUOld(subcellIndex, 0), 67.0/9.0);
  assignList(subcellIndex) = -1, 0;
  validateNumericalEquals(patches[0].getValueUOld(subcellIndex, 0), 73.0/9.0);
  assignList(subcellIndex) = -1, 1;
  validateNumericalEquals(patches[0].getValueUOld(subcellIndex, 0), 25.0/3.0);
  #endif
}

void peano::applications::peanoclaw::tests::GhostLayerCompositorTest::testInterpolationFromCoarseToFinePatchRightGhostLayer2D() {
  #ifdef Dim2
  //Patch-array for lower-left vertex in fine patch
  peano::applications::peanoclaw::Patch patches[TWO_POWER_D];

  //Settings
  int coarseSubdivisionFactor = 2;
  int coarseGhostlayerWidth = 1;
  int fineSubdivisionFactor = 2;
  int fineGhostlayerWidth = 2;
  int unknownsPerSubcell = 1;

  //Setup coarse patch
  tarch::la::Vector<DIMENSIONS, double> coarsePosition;
  assignList(coarsePosition) = 2.0, 1.0;
  patches[2] = createPatch(
    unknownsPerSubcell,
    0,   //Aux fields per subcell
    coarseSubdivisionFactor,
    coarseGhostlayerWidth,
    coarsePosition,
    3.0, //Coarse size
    0,   //Level
    0.0, //Time
    1.0, //Timestep size
    1.0  //Minimal neighbor time
  );
  patches[0] = patches[2];

  //Setup fine patch
  tarch::la::Vector<DIMENSIONS, double> finePosition;
  assignList(finePosition) = 1.0, 1.0;
  patches[3] = createPatch(
    unknownsPerSubcell,
    0,   //Aux fields per subcell
    fineSubdivisionFactor,
    fineGhostlayerWidth,
    finePosition,
    1.0, //Size
    1,   //Level
    0.0, //Time
    1.0/3.0, //Timestep size
    1.0  //Minimal neighbor time
  );

  //Fill coarse patch
  dfor(index, coarseSubdivisionFactor) {
    tarch::la::Vector<DIMENSIONS, int> subcellIndex;
    subcellIndex(0) = index(1);
    subcellIndex(1) = index(0);
    patches[2].setValueUNew(subcellIndex, 0, 1.0);
  }
  dfor(index, coarseSubdivisionFactor + 2*coarseGhostlayerWidth) {
    tarch::la::Vector<DIMENSIONS, int> subcellIndex;
    subcellIndex(0) = index(1) - coarseGhostlayerWidth;
    subcellIndex(1) = index(0) - coarseGhostlayerWidth;
    patches[2].setValueUOld(subcellIndex, 0, -1.0);
  }
  tarch::la::Vector<DIMENSIONS, int> subcellIndex;
  assignList(subcellIndex) = 0, 0;
  patches[2].setValueUOld(subcellIndex, 0, 6.0);
  assignList(subcellIndex) = 0, 1;
  patches[2].setValueUOld(subcellIndex, 0, 7.0);
  assignList(subcellIndex) = 1, 0;
  patches[2].setValueUOld(subcellIndex, 0, 10.0);
  assignList(subcellIndex) = 1, 1;
  patches[2].setValueUOld(subcellIndex, 0, 11.0);

  //Fill left ghostlayer
  peano::applications::peanoclaw::tests::PyClawTestStump pyClaw;
  peano::applications::peanoclaw::GhostLayerCompositor ghostLayerCompositor(
    patches,
    1,
    pyClaw,
    false
  );
  ghostLayerCompositor.fillGhostLayers(-1);

  assignList(subcellIndex) = 2, 0;
  validateNumericalEquals(patches[3].getValueUOld(subcellIndex, 0), 29.0/9.0);
  assignList(subcellIndex) = 2, 1;
  validateNumericalEquals(patches[3].getValueUOld(subcellIndex, 0), 31.0/9.0);
  assignList(subcellIndex) = 3, 0;
  validateNumericalEquals(patches[3].getValueUOld(subcellIndex, 0), 37.0/9.0);
  assignList(subcellIndex) = 3, 1;
  validateNumericalEquals(patches[3].getValueUOld(subcellIndex, 0), 13.0/3.0);
  #endif
}

void peano::applications::peanoclaw::tests::GhostLayerCompositorTest::testProjectionFromCoarseToFinePatchRightGhostLayer2D() {
  #ifdef Dim2

//  //Patch-array for upper-right vertex in fine patch
//  peano::applications::peanoclaw::Patch patches[TWO_POWER_D];
//
//  //Setup coarse patch
//  tarch::la::Vector<DIMENSIONS, double> coarsePosition;
//  assignList(coarsePosition) = 1.0, 2.0;
//  std::vector<peano::applications::peanoclaw::records::Data> uNewCoarse;
//  for(int i = 0; i < 4*4*3; i++) {
//    uNewCoarse.push_back(peano::applications::peanoclaw::records::Data());
//    uNewCoarse[i].setU(i);
//  }
//  std::vector<peano::applications::peanoclaw::records::Data> uOldCoarse;
//  for(int i = 0; i < (4+4) * (4+4) * 3; i++) {
//    uOldCoarse.push_back(peano::applications::peanoclaw::records::Data());
//    uOldCoarse[i] = -1.0;
//  }
//  patches[0] = peano::applications::peanoclaw::Patch(
//    coarsePosition, //Position
//    0.3,            //Size
//    3,              //Unknows per subcell
//    4,              //Subdivision factor
//    2,              //Ghostlayer width
//    0.0,            //Current time
//    1.0,            //Timestep size
//    0.0,            //CFL
//    1.0,            //Maximum timestep size
//    0,              //Level
//    &uNewCoarse,    //uNew
//    &uOldCoarse     //uOld
//  );
//  patches[2] = patches[0];
//
//  //Setup fine patch
//  tarch::la::Vector<DIMENSIONS, double> finePosition;
//  assignList(finePosition) = 0.9, 2.0;
//  std::vector<peano::applications::peanoclaw::records::Data> uNewFine;
//  for(int i = 0; i < 4*4*3; i++) {
//    uNewFine.push_back(peano::applications::peanoclaw::records::Data());
//    uNewFine[i].setU(-2.0);
//  }
//  std::vector<peano::applications::peanoclaw::records::Data> uOldFine;
//  for(int i = 0; i < (4+4) * (4+4) * 3; i++) {
//    uOldFine.push_back(peano::applications::peanoclaw::records::Data());
//    uOldFine[i].setU(-1.0);
//  }
//
//  patches[3] = peano::applications::peanoclaw::Patch(
//    finePosition,   //Position
//    0.1,            //Size
//    3,              //Unknows per subcell
//    4,              //Subdivision factor
//    2,              //Ghostlayer width
//    0.0,            //Current time
//    1.0,            //Timestep size
//    0.0,            //CFL
//    1.0,            //Maximum timestep size
//    1,              //Level
//    &uNewFine,      //uNew
//    &uOldFine       //uOld
//  );
//
//  //Fill right ghostlayer
//  peano::applications::peanoclaw::PyClaw pyClaw(0, 0, 0, 0);
//  peano::applications::peanoclaw::GhostLayerCompositor ghostLayerCompositor(
//    patches,
//    1,
//    pyClaw
//  );
//  ghostLayerCompositor.fillGhostLayers();
//
//  validateNumericalEquals(patches[3].getValueUOld(4, -2, 0), -1.0);
//  validateNumericalEquals(patches[3].getValueUOld(4, -1, 0), -1.0);
//  validateNumericalEquals(patches[3].getValueUOld(4, 0, 0), 0.0);
//  validateNumericalEquals(patches[3].getValueUOld(4, 1, 0), 0.0);
//  validateNumericalEquals(patches[3].getValueUOld(4, 2, 0), 0.0);
//  validateNumericalEquals(patches[3].getValueUOld(4, 3, 0), 1.0);
//  validateNumericalEquals(patches[3].getValueUOld(4, 4, 0), 1.0);
//  validateNumericalEquals(patches[3].getValueUOld(4, 5, 0), 1.0);
//
//  validateNumericalEquals(patches[3].getValueUOld(5, -2, 0), -1.0);
//  validateNumericalEquals(patches[3].getValueUOld(5, -1, 0), -1.0);
//  validateNumericalEquals(patches[3].getValueUOld(5, 0, 0), 0.0);
//  validateNumericalEquals(patches[3].getValueUOld(5, 1, 0), 0.0);
//  validateNumericalEquals(patches[3].getValueUOld(5, 2, 0), 0.0);
//  validateNumericalEquals(patches[3].getValueUOld(5, 3, 0), 1.0);
//  validateNumericalEquals(patches[3].getValueUOld(5, 4, 0), 1.0);
//  validateNumericalEquals(patches[3].getValueUOld(5, 5, 0), 1.0);
  #endif
}

void peano::applications::peanoclaw::tests::GhostLayerCompositorTest::testCoarseGridCorrection() {
  int unknownsPerSubcell = 3;
  int coarseSubdivisionFactor = 2;
  int fineSubdivisionFactor = 3;
  int ghostlayerWidth = 2;

  tarch::la::Vector<DIMENSIONS, double> coarsePosition(0.0);
  coarsePosition(0) = 1.0;

  Patch coarsePatch = createPatch(
    unknownsPerSubcell,
    0,   //Aux per subcell
    coarseSubdivisionFactor,
    ghostlayerWidth,
    coarsePosition,
    1.0, //Size
    0, //Level
    0.0, //Current time
    1.0, //Timestep size
    1.0  //Minimal neighbor time
  );

  //Fill coarse patch
  std::vector<peano::applications::peanoclaw::records::Data>& uNewCoarse
    = peano::kernel::heap::Heap<peano::applications::peanoclaw::records::Data>::getInstance()
    .getData(coarsePatch.getUNewIndex());
  for(int i = 0; i < coarseSubdivisionFactor*coarseSubdivisionFactor*unknownsPerSubcell; i++) {
    uNewCoarse.push_back(peano::applications::peanoclaw::records::Data());
    if(i < coarseSubdivisionFactor*coarseSubdivisionFactor) {
      uNewCoarse.at(i).setU(1.0);
    } else {
      uNewCoarse.at(i).setU(0.5);
    }
  }
  std::vector<peano::applications::peanoclaw::records::Data>& uOldCoarse
    = peano::kernel::heap::Heap<peano::applications::peanoclaw::records::Data>::getInstance()
    .getData(coarsePatch.getUOldIndex());
  for(int i = 0; i < (coarseSubdivisionFactor+2*ghostlayerWidth) * (coarseSubdivisionFactor+2*ghostlayerWidth) * unknownsPerSubcell; i++) {
    uOldCoarse.push_back(peano::applications::peanoclaw::records::Data());
    if(i < (coarseSubdivisionFactor+2*ghostlayerWidth) * (coarseSubdivisionFactor+2*ghostlayerWidth)) {
      uOldCoarse.at(i).setU(1.0);
    } else {
      uOldCoarse.at(i).setU(0.5);
    }
  }

  tarch::la::Vector<DIMENSIONS, double> finePosition(2.0/3.0);
  finePosition(1) = 1.0/3.0;

  Patch finePatch = createPatch(
    unknownsPerSubcell,
    0,         //Aux per subcell
    fineSubdivisionFactor,
    ghostlayerWidth,
    finePosition,
    1.0 / 3.0, //Size
    1,         //Level
    0.0,       //Current time
    1.0 / 3.0, //Timestep size
    1.0       //Minimal neighbor time
  );

  //Fine patch
  std::vector<peano::applications::peanoclaw::records::Data>& uNewFine
    = peano::kernel::heap::Heap<peano::applications::peanoclaw::records::Data>::getInstance()
    .getData(finePatch.getUNewIndex());
  for(int i = 0; i < fineSubdivisionFactor*fineSubdivisionFactor*unknownsPerSubcell; i++) {
    uNewFine.push_back(peano::applications::peanoclaw::records::Data());
    if(i < fineSubdivisionFactor*fineSubdivisionFactor) {
      uNewFine.at(i).setU(1.0);
    } else {
      uNewFine.at(i).setU(0.5);
    }
  }
  std::vector<peano::applications::peanoclaw::records::Data>& uOldFine
    = peano::kernel::heap::Heap<peano::applications::peanoclaw::records::Data>::getInstance()
    .getData(finePatch.getUOldIndex());
  for(int i = 0; i < (fineSubdivisionFactor+2*ghostlayerWidth) * (fineSubdivisionFactor+2*ghostlayerWidth) * unknownsPerSubcell; i++) {
    uOldFine.push_back(peano::applications::peanoclaw::records::Data());
    if(i < (fineSubdivisionFactor+2*ghostlayerWidth) * (fineSubdivisionFactor+2*ghostlayerWidth)) {
      uOldFine.at(i).setU(1.0);
    } else {
      uOldFine.at(i).setU(0.5);
    }
  }
  uNewFine[6] = 18.0;
  uNewFine[7] = 36.0;
  uNewFine[8] = 54.0;

  //GhostLayerCompositor
  PyClawTestStump pyClaw;
  peano::applications::peanoclaw::Patch patches[TWO_POWER_D];
  GhostLayerCompositor ghostLayerCompositor(patches, 0, pyClaw, false);

//  tarch::la::Vector<DIMENSIONS, double> normal;
//  assignList(normal) = 1.0, 0.0;

  ghostLayerCompositor.applyCoarseGridCorrection(finePatch, coarsePatch, 0, 1);

  tarch::la::Vector<DIMENSIONS, int> subcellIndex;
  assignList(subcellIndex) = 0, 0;
  validateNumericalEquals(coarsePatch.getValueUNew(subcellIndex, 0), 2558.0 / 729.0);
  assignList(subcellIndex) = 0, 1;
  validateNumericalEquals(coarsePatch.getValueUNew(subcellIndex, 0), 4466.0/729.0);
  assignList(subcellIndex) = 1, 0;
  validateNumericalEquals(coarsePatch.getValueUNew(subcellIndex, 0), 1.0);
  assignList(subcellIndex) = 1, 1;
  validateNumericalEquals(coarsePatch.getValueUNew(subcellIndex, 0), 1.0);
}

void peano::applications::peanoclaw::tests::GhostLayerCompositorTest::testInterpolationInTime() {
  Patch sourcePatch
    = createPatch(
      1,       //Unknowns per subcell
      0,       //Aux fields per subcell
      2,       //Subdivision factor
      1,       //Ghostlayer width
      0.0,     //Position
      1.0,     //Size
      0,       //Level
      0.0,     //Current time
      1.0,     //Timestep size
      1.0      //Minimal neighbor time
    );

  tarch::la::Vector<DIMENSIONS, double> sourcePosition;
  assignList(sourcePosition) = 1.0, 0.0;
  Patch destinationPatch
    = createPatch(
      1,              //Unknowns per subcell
      0,              //Aux fields per subcell
      2,              //Subdivision factor
      1,              //Ghostlayer width
      sourcePosition,
      1.0,            //Size
      0,              //Level
      0.0,            //Current time
      1.0/3.0,        //Timestep size
      1.0             //Minimal neighbor time
    );

  //Fill source
  tarch::la::Vector<DIMENSIONS, int> subcellIndex;
  assignList(subcellIndex) = 0, 0;
  sourcePatch.setValueUOld(subcellIndex, 0, 1.0);
  assignList(subcellIndex) = 1, 0;
  sourcePatch.setValueUOld(subcellIndex, 0, 1.0);
  assignList(subcellIndex) = 0, 1;
  sourcePatch.setValueUOld(subcellIndex, 0, 1.0);
  assignList(subcellIndex) = 1, 1;
  sourcePatch.setValueUOld(subcellIndex, 0, 1.0);

  assignList(subcellIndex) = 0, 0;
  sourcePatch.setValueUNew(subcellIndex, 0, 10.0);
  assignList(subcellIndex) = 1, 0;
  sourcePatch.setValueUNew(subcellIndex, 0, 20.0);
  assignList(subcellIndex) = 0, 1;
  sourcePatch.setValueUNew(subcellIndex, 0, 30.0);
  assignList(subcellIndex) = 1, 1;
  sourcePatch.setValueUNew(subcellIndex, 0, 40.0);

  peano::applications::peanoclaw::Patch patches[TWO_POWER_D];
  patches[3] = sourcePatch;
  patches[2] = destinationPatch;

  peano::applications::peanoclaw::PyClaw pyClaw(0, 0, 0, 0);
  peano::applications::peanoclaw::GhostLayerCompositor ghostLayerCompositor
    = peano::applications::peanoclaw::GhostLayerCompositor(
        patches,
        0,
        pyClaw,
        true
      );
  //TODO Add a test for filling only one adjacent patch? (i.e. parameter != -1)
  ghostLayerCompositor.fillGhostLayers(-1);

  assignList(subcellIndex) = -1, 0;
  validateNumericalEquals(destinationPatch.getValueUOld(subcellIndex, 0), 320.0/3.0);
  assignList(subcellIndex) = -1, 1;
  validateNumericalEquals(destinationPatch.getValueUOld(subcellIndex, 0), 80.0/3.0);
}

void peano::applications::peanoclaw::tests::GhostLayerCompositorTest::testRestrictionWithOverlappingBounds() {
  tarch::la::Vector<DIMENSIONS, double> lowerNeighboringGhostlayerBounds(1.0);
  tarch::la::Vector<DIMENSIONS, double> upperNeighboringGhostlayerBounds(1.1);
  tarch::la::Vector<DIMENSIONS, double> sourceSubcellSize(0.11);
  tarch::la::Vector<DIMENSIONS, double> sourcePosition(0.0);
  tarch::la::Vector<DIMENSIONS, double> sourceSize(2.2);
  tarch::la::Vector<DIMENSIONS, int> sourceSubdivisionFactor(20);

  peano::applications::peanoclaw::Area areas[TWO_POWER_D];
  peano::applications::peanoclaw::getAreasForRestriction(
    lowerNeighboringGhostlayerBounds,
    upperNeighboringGhostlayerBounds,
    sourcePosition,
    sourceSize,
    sourceSubcellSize,
    sourceSubdivisionFactor,
    areas
  );

  tarch::la::Vector<DIMENSIONS,int> zero(0);

  for(int d = 0; d < DIMENSIONS; d++) {
    validateEqualsWithParams1(areas[2*d]._offset, zero, d);
    validateEqualsWithParams1(areas[2*d + 1]._offset, zero, d);

    if(d == 0) {
      validateEqualsWithParams1(areas[2*d]._size, sourceSubdivisionFactor, d);
      validateEqualsWithParams1(areas[2*d + 1]._size, zero, d);
    } else {
      validateEqualsWithParams1(areas[2*d]._size, zero, d);
      validateEqualsWithParams1(areas[2*d + 1]._size, zero, d);
    }
  }
}

void peano::applications::peanoclaw::tests::GhostLayerCompositorTest::testPartialRestrictionAreas() {
  #ifdef Dim2
  tarch::la::Vector<DIMENSIONS, double> lowerNeighboringGhostlayerBounds(1.8 + 1e-13);
  tarch::la::Vector<DIMENSIONS, double> upperNeighboringGhostlayerBounds(1.2);
  tarch::la::Vector<DIMENSIONS, double> sourcePosition(1.0);
  tarch::la::Vector<DIMENSIONS, double> sourceSize(1.0);
  tarch::la::Vector<DIMENSIONS, double> sourceSubcellSize(0.1);
  tarch::la::Vector<DIMENSIONS, int> sourceSubdivisionFactor(10);

  peano::applications::peanoclaw::Area areas[TWO_POWER_D];
  peano::applications::peanoclaw::getAreasForRestriction(
    lowerNeighboringGhostlayerBounds,
    upperNeighboringGhostlayerBounds,
    sourcePosition,
    sourceSize,
    sourceSubcellSize,
    sourceSubdivisionFactor,
    areas
  );

  tarch::la::Vector<DIMENSIONS,int> offset;
  tarch::la::Vector<DIMENSIONS,int> size;

  //Left and Right
  assignList(offset) = 0, 0;
  assignList(size) = 2, 10;
  validateEquals(areas[0]._offset, offset);
  validateEquals(areas[0]._size, size);
  assignList(offset) = 8, 0;
  assignList(size) = 2, 10;
  validateEquals(areas[1]._offset, offset);
  validateEquals(areas[1]._size, size);
  assignList(offset) = 2, 0;
  assignList(size) = 6, 2;
  validateEquals(areas[2]._offset, offset);
  validateEquals(areas[2]._size, size);
  assignList(offset) = 2, 8;
  assignList(size) = 6, 2;
  validateEquals(areas[3]._offset, offset);
  validateEquals(areas[3]._size, size);
  #endif
}

void peano::applications::peanoclaw::tests::GhostLayerCompositorTest::testPartialRestrictionAreasWithInfiniteLowerBounds() {
  #ifdef Dim2
  tarch::la::Vector<DIMENSIONS, double> lowerNeighboringGhostlayerBounds(std::numeric_limits<double>::max());
  tarch::la::Vector<DIMENSIONS, double> upperNeighboringGhostlayerBounds;
  assignList(upperNeighboringGhostlayerBounds) = 4.0/27.0, 6.0/27.0 + 1.0/27.0/6.0;
  tarch::la::Vector<DIMENSIONS, double> sourcePosition;
  assignList(sourcePosition) = 4.0/27.0, 2.0/9.0;
  tarch::la::Vector<DIMENSIONS, double> sourceSize(1.0/27.0);
  tarch::la::Vector<DIMENSIONS, double> sourceSubcellSize(1.0/27.0/6.0);
  tarch::la::Vector<DIMENSIONS, int> sourceSubdivisionFactor(6);

  peano::applications::peanoclaw::Area areas[TWO_POWER_D];
  peano::applications::peanoclaw::getAreasForRestriction(
      lowerNeighboringGhostlayerBounds,
      upperNeighboringGhostlayerBounds,
      sourcePosition,
      sourceSize,
      sourceSubcellSize,
      sourceSubdivisionFactor,
      areas
  );

  tarch::la::Vector<DIMENSIONS,int> offset;
  tarch::la::Vector<DIMENSIONS,int> size;

  //Left and Right
  assignList(offset) = 0, 0;
  assignList(size) = 0, 6;
  validateEquals(areas[0]._offset, offset);
  validateEquals(areas[0]._size, size);
  assignList(offset) = 6, 0;
  assignList(size) = 0, 6;
  validateEquals(areas[1]._offset, offset);
  validateEquals(areas[1]._size, size);
  assignList(offset) = 0, 0;
  assignList(size) = 6, 1;
  validateEquals(areas[2]._offset, offset);
  validateEquals(areas[2]._size, size);
  assignList(offset) = 0, 6;
  assignList(size) = 6, 0;
  validateEquals(areas[3]._offset, offset);
  validateEquals(areas[3]._size, size);
  #endif
}

#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",on)
#endif