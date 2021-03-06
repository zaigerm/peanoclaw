#include "peanoclaw/configurations/PeanoClawConfigurationForSpacetreeGrid.h"
#include "peanoclaw/runners/PeanoClawLibraryRunner.h"

#include "tarch/logging/CommandLineLogger.h"

#include <fstream>

tarch::logging::Log peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid::_log("peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid");

bool peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid::getBoolValue(std::stringstream& s) {
  std::string value;
  s >> value;
  assert(value == "yes" || value == "no");
  return value == "yes";
}

int peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid::getIntegerValue(std::stringstream& s) {
  int value;
  s >> value;
  return value;
}

void peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid::addProbe(std::stringstream& s) {
  std::string name;
  s >> name;

  int unknown;
  s >> unknown;

  tarch::la::Vector<DIMENSIONS,double> position;
  for(int d = 0; d < DIMENSIONS; d++) {
    s >> position[d];
  }
  _probes.push_back(peanoclaw::statistics::Probe(name, position, unknown));
}

void peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid::processEntry(
  const std::string& name,
  std::stringstream& values
) {

  if(name == "plot" || name == "plotAtOutputTimes") {
    _plotAtOutputTimes = getBoolValue(values);
  } else if(name == "plotAtEnd") {
    _plotAtEndTime = getBoolValue(values);
  } else if(name == "plotAtSubsteps") {
    _plotSubsteps = getBoolValue(values);
  } else if(name == "restrictStatistics") {
    _restrictStatistics = getBoolValue(values);
  } else if(name == "fluxCorrection") {
    _fluxCorrection = getBoolValue(values);
  } else if(name == "reduceReductions") {
    _reduceReductions = getBoolValue(values);
  } else if(name == "probe") {
    addProbe(values);
  } else if(name == "numberOfThreads") {
    _numberOfThreads = getIntegerValue(values);
  } else if(name == "neighborInducedMaximumTimesteps") {
    _estimateNeighborInducedMaximumTimestep = getBoolValue(values);
  } else if(name == "useDimensionalSplittingOptimization") {
    _useDimensionalSplittingOptimization = getBoolValue(values);
  } else {
    _isValid = false;
    logError("processEntry(string,string)", "Invalid entry: '" << name << "' '" << values << "'");
  }
}

void peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid::parseLine(const std::string& line) {

  //Delimiter
  std::string modified = line;
  if(modified.find("=") != std::string::npos) {
    modified.replace(modified.find("="), 1, " ");
  }

  std::stringstream s(modified);
  std::string name;
  s >> name;

  processEntry(name, s);
}

peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid::PeanoClawConfigurationForSpacetreeGrid():
  _isValid(true),
  _plotAtOutputTimes(false),
  _plotSubsteps(false),
  _plotSubstepsAfterOutputTime(-1),
  _additionalLevelsForPredefinedRefinement(1),
  _useDimensionalSplittingOptimization(false),
  _restrictStatistics(true),
  _fluxCorrection(false),
  _reduceReductions(false),
  _numberOfThreads(1),
  _estimateNeighborInducedMaximumTimestep(false)
  {
  std::string configFileName = "peanoclaw.config";
  std::ifstream configFile(configFileName.c_str());
  if(configFile) {
    if (!configFile.good()) {
      logError( "PeanoClawConfigurationForSpacetreeGrid", "was not able to open input file " << configFileName );
    }

    int linenumber = 1;
    std::string line;
    while (!configFile.eof()) {

      std::getline(configFile, line);
      if(line.length() > 0 && line[0]!='#') {
        parseLine(line);
      }

      linenumber++;
    }
  }
}


peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid::~PeanoClawConfigurationForSpacetreeGrid() {
}


bool peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid::isValid() const { 
  return true;
}

bool peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid::plotAtOutputTimes() const {
  return _plotAtOutputTimes;
}
 
bool peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid::plotSubsteps() const {
  return _plotSubsteps;
}

bool peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid::plotAtEndTime() const {
  return _plotAtEndTime;
}

int peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid::plotSubstepsAfterOutputTime() const {
  return _plotSubstepsAfterOutputTime;
}

bool peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid::useDimensionalSplittingOptimization() const {
  return _useDimensionalSplittingOptimization;
}

bool peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid::restrictStatistics() const {
  return _restrictStatistics;
}

bool peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid::enableFluxCorrection() const {
  return _fluxCorrection;
}

bool peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid::shouldReduceReductions() const  {
  return _reduceReductions;
}

std::vector<peanoclaw::statistics::Probe> peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid::getProbeList() const {
  return _probes;
}

int peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid::getNumberOfThreads() const {
  return _numberOfThreads;
}

bool peanoclaw::configurations::PeanoClawConfigurationForSpacetreeGrid::estimateNeighborInducedMaximumTimestep() const {
  return _estimateNeighborInducedMaximumTimestep;
}
