/*
 * PatchTest.cpp
 *
 *  Created on: Feb 15, 2012
 *      Author: Kristof Unterweger
 */
#include "peanoclaw/tests/PatchTest.h"

#include "peanoclaw/ParallelSubgrid.h"
#include "peanoclaw/Patch.h"
#include "peanoclaw/Vertex.h"
#include "peanoclaw/tests/Helper.h"
#include "peanoclaw/tests/TestVertexEnumerator.h"

#include "peano/utils/Globals.h"

#include "tarch/tests/TestCaseFactory.h"
registerTest(peanoclaw::tests::PatchTest)


#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",off)
#endif


peanoclaw::tests::PatchTest::PatchTest():
  tarch::tests::TestCase( "peanoclaw::tests::PeanoClawTest" ) {
}


peanoclaw::tests::PatchTest::~PatchTest() {
}


void peanoclaw::tests::PatchTest::setUp() {
}


void peanoclaw::tests::PatchTest::run() {
  testMethod( testFillingOfUNewArray );
  testMethod( testFillingOfUOldArray );
  testMethod( testInvalidPatch );
  testMethod( testCoarsePatchTimeInterval );
  testMethod( testCountingOfAdjacentParallelSubgrids );
  testMethod( testCountingOfAdjacentParallelSubgridsFourNeighboringRanks );
}

void peanoclaw::tests::PatchTest::testFillingOfUNewArray() {

  Patch patch = createPatch(
    2,   //Unknowns per subcell
    0,   //Aux fields per subcell
    3,   //Subdivision factor
    1,   //Ghostlayer width
    0.0, //Position
    1.0, //Size
    0,   //Level
    0.0, //Current time
    0.0, //Timestep size
    0.0  //Minimal neighbor time
  );

  #ifdef Dim2
  for(int unknown = 0; unknown < 2; unknown++) {
    for(int x = 0; x < 3; x++) {
      for(int y = 0; y < 3; y++) {
        tarch::la::Vector<DIMENSIONS, int> subcellIndex;
        assignList(subcellIndex) = x, y;
        patch.setValueUNew(subcellIndex, unknown, unknown * 9 + x * 3 + y);
      }
    }
  }
  #endif

  //TODO unterweg fixen!
//  double uNewArray[3*3*2];
//  patch.fillUNewArray(uNewArray);
//
//  for(int i = 0; i < 18; i++) {
//    validateNumericalEquals(uNewArray[i], i);
//  }
}

void peanoclaw::tests::PatchTest::testFillingOfUOldArray() {
//  Patch patch(
//    0.0,    //Position
//    1.0,    //Size
//    2,      //Number of unknowns
//    3,      //Subdivision factor
//    1,      //Ghostlayer width
//    0.0,    //Current time
//    0.0,    //Timestep size
//    0.0,    //CFL
//    0.0,    //Maximum timestep size
//    0,      //Level
//    &uNew,
//    &uOld
//  );
  Patch patch = createPatch(
    2,       //Unknowns per subcell
    0,       //Aux fields per subcell
    3,       //Subdivision factor
    1,       //Ghostlayer width
    0.0,     //Position
    1.0,     //Size
    0,       //Level
    0.0,     //Current time
    0.0,     //Timestep size
    0.0      //Minimal neighbor time
  );

//  std::vector<Data> uNew;
//  std::vector<Data> uOld;
//  for(int i = 0; i < 50; i++) {
//    Data data;
//    data.setU(i);
//    uOld.push_back(data);
//  }

  #ifdef Dim2
  int counter = 0;
  for(int unknown = 0; unknown < 2; unknown++) {
    for(int x = -1; x < 3 + 1; x++) {
      for(int y = -1; y < 3 + 1; y++) {
        tarch::la::Vector<DIMENSIONS, int> subcellIndex;
        assignList(subcellIndex) = x, y;
        patch.setValueUOld(subcellIndex, unknown, counter++);
      }
    }
  }
  #endif

  //TODO unterweg fixen!
//  double uOldArray[5*5*2];
//  patch.fillUOldWithGhostLayerArray(uOldArray);
//
//  for(int i = 0; i < 50; i++) {
//    validateNumericalEquals(uOldArray[i], i);
//  }
}

void peanoclaw::tests::PatchTest::testInvalidPatch() {
  Patch patch;
  validate(!patch.isValid());
}

void peanoclaw::tests::PatchTest::testCoarsePatchTimeInterval() {
  peanoclaw::Patch coarsePatch = createPatch(
    2,       //Unknowns per subcell
    0,       //Aux fields per subcell
    3,       //Subdivision factor
    1,       //Ghostlayer width
    0.0,     //Position
    1.0,     //Size
    0,       //Level
    0.0,     //Current time
    0.0,     //Timestep size
    0.0      //Minimal neighbor time
  );
  peanoclaw::Patch finePatches[THREE_POWER_D];

  for(int i = 0; i < THREE_POWER_D; i++) {
    finePatches[i] = createPatch(
      2,       //Unknowns per subcell
      0,       //Aux fields per subcell
      3,       //Subdivision factor
      1,       //Ghostlayer width
      0.0,     //Position
      1.0/3.0, //Size
      1,       //Level
      0.0,     //Current time
      0.0,     //Timestep size
      0.0      //Minimal neighbor time
    );
  }

  //Prepare fine patches
  finePatches[0].getTimeIntervals().setCurrentTime(1.0);
  finePatches[0].getTimeIntervals().setTimestepSize(2.0);

  finePatches[1].getTimeIntervals().setCurrentTime(1.0);
  finePatches[1].getTimeIntervals().setTimestepSize(2.0);

  finePatches[2].getTimeIntervals().setCurrentTime(1.0);
  finePatches[2].getTimeIntervals().setTimestepSize(3.0);

  finePatches[3].getTimeIntervals().setCurrentTime(0.0);
  finePatches[3].getTimeIntervals().setTimestepSize(3.0);

  finePatches[4].getTimeIntervals().setCurrentTime(1.0);
  finePatches[4].getTimeIntervals().setTimestepSize(1.7);

  finePatches[5].getTimeIntervals().setCurrentTime(0.0);
  finePatches[5].getTimeIntervals().setTimestepSize(4.0);

  finePatches[6].getTimeIntervals().setCurrentTime(0.0);
  finePatches[6].getTimeIntervals().setTimestepSize(3.0);

  finePatches[7].getTimeIntervals().setCurrentTime(1.0);
  finePatches[7].getTimeIntervals().setTimestepSize(2.0);

  finePatches[8].getTimeIntervals().setCurrentTime(2.5);
  finePatches[8].getTimeIntervals().setTimestepSize(1.5);

  for(int i = 9; i < THREE_POWER_D; i++) {
    finePatches[i].getTimeIntervals().setCurrentTime(0.0);
    finePatches[i].getTimeIntervals().setTimestepSize(3.0);
  }

  //execute methods
  coarsePatch.getTimeIntervals().resetMinimalFineGridTimeInterval();
  for(int i = 0; i < THREE_POWER_D; i++) {
    coarsePatch.getTimeIntervals().updateMinimalFineGridTimeInterval(finePatches[i].getTimeIntervals().getCurrentTime(), finePatches[i].getTimeIntervals().getTimestepSize());
  }
  coarsePatch.switchValuesAndTimeIntervalToMinimalFineGridTimeInterval();

  //Check
  validateNumericalEquals(coarsePatch.getTimeIntervals().getCurrentTime(), 2.5);
  validateNumericalEquals(coarsePatch.getTimeIntervals().getTimestepSize(), 0.2);
}

void peanoclaw::tests::PatchTest::testCountingOfAdjacentParallelSubgrids() {
  #ifdef Parallel
  Vertex vertices[TWO_POWER_D];
  TestVertexEnumerator enumerator(1.0);

  for(int i = 0; i < TWO_POWER_D; i++) {
    vertices[i].switchToNonhangingNode();
  }

  vertices[0].setAdjacentRank(0, 0);
  vertices[0].setAdjacentRank(1, 0);
  vertices[0].setAdjacentRank(2, 0);
  vertices[0].setAdjacentRank(3, 0);

  vertices[1].setAdjacentRank(0, 0);
  vertices[1].setAdjacentRank(1, 0);
  vertices[1].setAdjacentRank(2, 0);
  vertices[1].setAdjacentRank(3, 0);

  vertices[2].setAdjacentRank(0, 0);
  vertices[2].setAdjacentRank(1, 0);
  vertices[2].setAdjacentRank(2, 1);
  vertices[2].setAdjacentRank(3, 1);

  vertices[3].setAdjacentRank(0, 0);
  vertices[3].setAdjacentRank(1, 0);
  vertices[3].setAdjacentRank(2, 1);
  vertices[3].setAdjacentRank(3, 1);

  peanoclaw::records::CellDescription cellDescription;

  //Test
  peanoclaw::ParallelSubgrid parallelSubgrid(cellDescription);
  parallelSubgrid.countNumberOfAdjacentParallelSubgrids(vertices, enumerator);

  validateEqualsWithParams1(parallelSubgrid.getAdjacentRanks()(0), 1, parallelSubgrid.getAdjacentRanks());
  validateEqualsWithParams1(parallelSubgrid.getAdjacentRanks()(1), -1, parallelSubgrid.getAdjacentRanks());
  validateEqualsWithParams1(parallelSubgrid.getAdjacentRanks()(2), -1, parallelSubgrid.getAdjacentRanks());
  validateEqualsWithParams1(parallelSubgrid.getAdjacentRanks()(3), -1, parallelSubgrid.getAdjacentRanks());
  validateEquals(parallelSubgrid.getNumberOfSharedAdjacentVertices()(0), 2);
  validateEquals(parallelSubgrid.getNumberOfSharedAdjacentVertices()(1), 0);
  validateEquals(parallelSubgrid.getNumberOfSharedAdjacentVertices()(2), 0);
  validateEquals(parallelSubgrid.getNumberOfSharedAdjacentVertices()(3), 0);
  #endif
}

void peanoclaw::tests::PatchTest::testCountingOfAdjacentParallelSubgridsFourNeighboringRanks() {
  #ifdef Parallel
  Vertex vertices[TWO_POWER_D];
  TestVertexEnumerator enumerator(1.0);

  vertices[0].setAdjacentRank(0, 3);
  vertices[0].setAdjacentRank(1, 3);
  vertices[0].setAdjacentRank(2, 4);
  vertices[0].setAdjacentRank(3, 0);

  vertices[1].setAdjacentRank(0, 3);
  vertices[1].setAdjacentRank(1, 3);
  vertices[1].setAdjacentRank(2, 0);
  vertices[1].setAdjacentRank(3, 2);

  vertices[2].setAdjacentRank(0, 4);
  vertices[2].setAdjacentRank(1, 0);
  vertices[2].setAdjacentRank(2, 1);
  vertices[2].setAdjacentRank(3, 1);

  vertices[3].setAdjacentRank(0, 0);
  vertices[3].setAdjacentRank(1, 2);
  vertices[3].setAdjacentRank(2, 1);
  vertices[3].setAdjacentRank(3, 1);

  peanoclaw::records::CellDescription cellDescription;

  //Test
  peanoclaw::ParallelSubgrid parallelSubgrid(cellDescription);
  parallelSubgrid.countNumberOfAdjacentParallelSubgrids(vertices, enumerator);

  validateEquals(parallelSubgrid.getAdjacentRanks()(0), 3);
  validateEquals(parallelSubgrid.getAdjacentRanks()(1), 4);
  validateEquals(parallelSubgrid.getAdjacentRanks()(2), 2);
  validateEquals(parallelSubgrid.getAdjacentRanks()(3), 1);
  validateEqualsWithParams1(parallelSubgrid.getNumberOfSharedAdjacentVertices()(0), 2, parallelSubgrid.getNumberOfSharedAdjacentVertices());
  validateEqualsWithParams1(parallelSubgrid.getNumberOfSharedAdjacentVertices()(1), 2, parallelSubgrid.getNumberOfSharedAdjacentVertices());
  validateEqualsWithParams1(parallelSubgrid.getNumberOfSharedAdjacentVertices()(2), 2, parallelSubgrid.getNumberOfSharedAdjacentVertices());
  validateEqualsWithParams1(parallelSubgrid.getNumberOfSharedAdjacentVertices()(3), 2, parallelSubgrid.getNumberOfSharedAdjacentVertices());
  #endif
}

#ifdef UseTestSpecificCompilerSettings
#pragma optimize("",on)
#endif

