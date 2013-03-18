/*
 * pyclawBindings.c
 *
 *  Created on: Feb 7, 2012
 *      Author: Kristof Unterweger
 */

#include <Python.h>
#include "peano/utils/Globals.h"
#include "tarch/logging/Log.h"
#include "tarch/logging/CommandLineLogger.h"

#include "peano/peano.h"

#include <list>
#include <numpy/arrayobject.h>

#include "peanoclaw/Numerics.h"
#include "peanoclaw/pyclaw/NumericsFactory.h"
#include "peanoclaw/configurations/PeanoClawConfigurationForSpacetreeGrid.h"
#include "peanoclaw/runners/PeanoClawLibraryRunner.h"
#include "tarch/tests/TestCaseRegistry.h"

#include "peanoclaw/pyclaw/PyClawCallbacks.h"

#include "tarch/parallel/NodePool.h"

#if USE_VALGRIND
#include <callgrind.h>
#endif

static bool _calledFromPython;
static PyGILState_STATE _pythonState;
static peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid* _configuration;

void importArrays() {
  import_array();
}

//void loadDefaultConfiguration() {
//  _configuration = 0;
//  tarch::logging::CommandLineLogger::FilterList filterList;
//  tarch::logging::CommandLineLogger::FilterListEntry newEntry;
//  newEntry._targetName = "";
//  newEntry._isBlackEntry = true;
//  newEntry._rank=-1;
//  newEntry._namespaceName = "";
//  filterList.insert(newEntry);
//  tarch::logging::CommandLineLogger::getInstance().clearFilterList();
//  tarch::logging::CommandLineLogger::getInstance().addFilterListEntries(filterList);
//}


extern "C"
peanoclaw::runners::PeanoClawLibraryRunner* pyclaw_peano_new (
  double initialMinimalMeshWidthScalar,
  double domainOffsetX0,
  double domainOffsetX1,
  double domainOffsetX2,
  double domainSizeX0,
  double domainSizeX1,
  double domainSizeX2,
  int subdivisionFactorX0,
  int subdivisionFactorX1,
  int subdivisionFactorX2,
  int unknownsPerSubcell,
  int auxiliarFieldsPerSubcell,
  int ghostlayerWidth,
  double initialTimestepSize,
  char* configurationFile,
  bool useDimensionalSplitting,
  InitializationCallback initializationCallback,
  BoundaryConditionCallback boundaryConditionCallback,
  SolverCallback solverCallback,
  AddPatchToSolutionCallback addPatchToSolutionCallback,
  InterPatchCommunicationCallback interpolationCallback,
  InterPatchCommunicationCallback restrictionCallback,
  InterPatchCommunicationCallback fluxCorrectionCallback,
  int* rank
) {
    peano::fillLookupTables();

#if defined(Parallel)
  char argv[2][256];
  int argc = 1;
  sprintf(argv[0], "%s", "peanoclaw");
  int parallelSetup = peano::initParallelEnvironment(&argc,(char ***)&argv);
  int sharedMemorySetup = peano::initSharedMemoryEnvironment();
#endif

  //Initialize Python
  _calledFromPython = Py_IsInitialized();

  if(_calledFromPython) {
    //Needed to take over the Python context from the calling process.
    _pythonState = PyGILState_Ensure();
  } else {
    Py_Initialize();
  }


  importArrays();

  //Initialize Logger
  static tarch::logging::Log _log("::pyclawBindings");
  logInfo("pyclaw_peano_new(...)", "Initializing Peano");

  //PyClaw - this object is copied to the runner and is stored there.
  peanoclaw::pyclaw::NumericsFactory numericsFactory;
  peanoclaw::Numerics* numerics = numericsFactory.createPyClawNumerics(
    initializationCallback,
    boundaryConditionCallback,
    solverCallback,
    addPatchToSolutionCallback,
    interpolationCallback,
    restrictionCallback,
    fluxCorrectionCallback
  );

  _configuration = new peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid;
  // assertion1(_configuration->isValid(), _configuration);

  //Construct parameters
  tarch::la::Vector<DIMENSIONS, double> domainOffset;
  domainOffset(0) = domainOffsetX0;
  domainOffset(1) = domainOffsetX1;
  #ifdef Dim3
  domainOffset(2) = domainOffsetX2;
  #endif
  tarch::la::Vector<DIMENSIONS, double> domainSize;
  domainSize(0) = domainSizeX0;
  domainSize(1) = domainSizeX1;
  #ifdef Dim3
  domainSize(2) = domainSizeX2;
  #endif
  
  tarch::la::Vector<DIMENSIONS, double> initialMinimalMeshWidth(initialMinimalMeshWidthScalar);
  tarch::la::Vector<DIMENSIONS, int> subdivisionFactor;
  subdivisionFactor(0) = subdivisionFactorX0;
  subdivisionFactor(1) = subdivisionFactorX1;
  #ifdef Dim3
  subdivisionFactor(2) = subdivisionFactorX2;
  #endif

  //Check parameters
  assertion1(tarch::la::greater(domainSizeX0, 0.0) && tarch::la::greater(domainSizeX1, 0.0), domainSize);
  if(initialMinimalMeshWidthScalar > domainSizeX0 || initialMinimalMeshWidthScalar > domainSizeX1) {
    logError("pyclaw_peano_new(...)", "Domainsize or initialMinimalMeshWidth not set properly.");
  }
  if(tarch::la::oneGreater(tarch::la::Vector<DIMENSIONS, int>(1), subdivisionFactor) ) {
    logError("pyclaw_peano_new(...)", "subdivisionFactor not set properly.");
  }
 
  // Configure the output
  tarch::logging::CommandLineLogger::getInstance().clearFilterList();
  tarch::logging::CommandLineLogger::getInstance().addFilterListEntry( ::tarch::logging::CommandLineLogger::FilterListEntry( "", false ) );
  tarch::logging::CommandLineLogger::getInstance().addFilterListEntry( ::tarch::logging::CommandLineLogger::FilterListEntry( "info", false ) );
  tarch::logging::CommandLineLogger::getInstance().addFilterListEntry( ::tarch::logging::CommandLineLogger::FilterListEntry( "debug", true ) );
//  tarch::logging::CommandLineLogger::getInstance().addFilterListEntry( ::tarch::logging::CommandLineLogger::FilterListEntry( "trace", true ) );
//  tarch::logging::CommandLineLogger::getInstance().setLogFormat( ... please consult source code documentation );
 
  std::ostringstream logFileName;
  logFileName << "rank-" << tarch::parallel::Node::getInstance().getRank() << "-trace.txt";
  tarch::logging::CommandLineLogger::getInstance().setLogFormat( " ", false, false, true, false, true, logFileName.str() );

  //Create runner
  peanoclaw::runners::PeanoClawLibraryRunner* runner
    = new peanoclaw::runners::PeanoClawLibraryRunner(
    *_configuration,
    *numerics,
    domainOffset,
    domainSize,
    initialMinimalMeshWidth,
    subdivisionFactor,
    ghostlayerWidth,
    unknownsPerSubcell,
    auxiliarFieldsPerSubcell,
    initialTimestepSize,
    useDimensionalSplitting
  );


  assertion(runner != 0);
 
  if(_calledFromPython) {
    PyGILState_Release(_pythonState);
  }

  return runner;
}

extern "C"
void pyclaw_peano_destroy(peanoclaw::runners::PeanoClawLibraryRunner* runner) {
  static tarch::logging::Log _log("::pyclawBindings");
  logTraceIn("pyclaw_peano_destroy");
  assertionMsg(runner!=0, "call pyclaw_peano_new before calling pyclaw_peano_destroy.");
  delete runner;

  if(_configuration != 0) {
    delete _configuration;
  }

  if(!_calledFromPython) {
    Py_Finalize();
  }

  peano::shutdownParallelEnvironment();
  peano::shutdownSharedMemoryEnvironment();
 
  logTraceOut("pyclaw_peano_destroy");
}

extern "C"
void pyclaw_peano_evolveToTime(
  double time,
  peanoclaw::runners::PeanoClawLibraryRunner* runner
) {
  #ifdef USE_VALGRIND
  CALLGRIND_START_INSTRUMENTATION;
  CALLGRIND_ZERO_STATS;
  #endif

  static tarch::logging::Log _log("::pyclawBindings");
  logTraceInWith1Argument("pyclaw_peano_evolveToTime", time);
  assertionMsg(runner!=0, "call pyclaw_peano_new before calling pyclaw_peano_run.");

  if(_calledFromPython) {
    _pythonState = PyGILState_Ensure();
  }

  runner->evolveToTime(time);

  if(_calledFromPython) {
    PyGILState_Release(_pythonState);
  }

  #ifdef USE_VALGRIND
  CALLGRIND_STOP_INSTRUMENTATION;
  #endif
  logTraceOut("pyclaw_peano_evolveToTime");
}

extern "C"
void pyclaw_peano_gatherSolution(
  peanoclaw::runners::PeanoClawLibraryRunner* runner
) {

  static tarch::logging::Log _log("::pyclawBindings");
  logTraceIn("pyclaw_peano_gatherSolution")
  if(_calledFromPython) {
    _pythonState = PyGILState_Ensure();
  }

  runner->gatherCurrentSolution();

  if(_calledFromPython) {
    PyGILState_Release(_pythonState);
  }
  logTraceOut("pyclaw_peano_gatherSolution");
}
