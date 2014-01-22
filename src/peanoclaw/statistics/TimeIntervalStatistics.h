#ifndef _PEANOCLAW_STATISTICS_TIMEINTERVALSTATISTICS_H
#define _PEANOCLAW_STATISTICS_TIMEINTERVALSTATISTICS_H

#include "peano/utils/Globals.h"
#include "tarch/compiler/CompilerSpecificSettings.h"
#include "peano/utils/PeanoOptimisations.h"
#ifdef Parallel
	#include "tarch/parallel/Node.h"
#endif
#ifdef Parallel
	#include <mpi.h>
#endif
#include "tarch/logging/Log.h"
#include "tarch/la/Vector.h"
#include <bitset>
#include <complex>
#include <string>
#include <iostream>

namespace peanoclaw {
   namespace statistics {
      class TimeIntervalStatistics;
      class TimeIntervalStatisticsPacked;
   }
}

/**
 * @author This class is generated by DaStGen
 * 		   DataStructureGenerator (DaStGen)
 * 		   2007-2009 Wolfgang Eckhardt
 * 		   2012      Tobias Weinzierl
 *
 * 		   build date: 22-10-2013 20:59
 *
 * @date   22/01/2014 19:02
 */
class peanoclaw::statistics::TimeIntervalStatistics { 
   
   public:
      
      typedef peanoclaw::statistics::TimeIntervalStatisticsPacked Packed;
      
      struct PersistentRecords {
         int _minimalPatchIndex;
         int _minimalPatchParentIndex;
         double _minimalPatchTime;
         double _startMaximumLocalTimeInterval;
         double _endMaximumLocalTimeInterval;
         double _startMinimumLocalTimeInterval;
         double _endMinimumLocalTimeInterval;
         double _minimalTimestep;
         bool _allPatchesEvolvedToGlobalTimestep;
         double _averageGlobalTimeInterval;
         double _globalTimestepEndTime;
         bool _minimalPatchBlockedDueToCoarsening;
         bool _minimalPatchBlockedDueToGlobalTimestep;
         /**
          * Generated
          */
         PersistentRecords();
         
         /**
          * Generated
          */
         PersistentRecords(const int& minimalPatchIndex, const int& minimalPatchParentIndex, const double& minimalPatchTime, const double& startMaximumLocalTimeInterval, const double& endMaximumLocalTimeInterval, const double& startMinimumLocalTimeInterval, const double& endMinimumLocalTimeInterval, const double& minimalTimestep, const bool& allPatchesEvolvedToGlobalTimestep, const double& averageGlobalTimeInterval, const double& globalTimestepEndTime, const bool& minimalPatchBlockedDueToCoarsening, const bool& minimalPatchBlockedDueToGlobalTimestep);
         
         /**
          * Generated
          */
          int getMinimalPatchIndex() const ;
         
         /**
          * Generated
          */
          void setMinimalPatchIndex(const int& minimalPatchIndex) ;
         
         /**
          * Generated
          */
          int getMinimalPatchParentIndex() const ;
         
         /**
          * Generated
          */
          void setMinimalPatchParentIndex(const int& minimalPatchParentIndex) ;
         
         /**
          * Generated
          */
          double getMinimalPatchTime() const ;
         
         /**
          * Generated
          */
          void setMinimalPatchTime(const double& minimalPatchTime) ;
         
         /**
          * Generated
          */
          double getStartMaximumLocalTimeInterval() const ;
         
         /**
          * Generated
          */
          void setStartMaximumLocalTimeInterval(const double& startMaximumLocalTimeInterval) ;
         
         /**
          * Generated
          */
          double getEndMaximumLocalTimeInterval() const ;
         
         /**
          * Generated
          */
          void setEndMaximumLocalTimeInterval(const double& endMaximumLocalTimeInterval) ;
         
         /**
          * Generated
          */
          double getStartMinimumLocalTimeInterval() const ;
         
         /**
          * Generated
          */
          void setStartMinimumLocalTimeInterval(const double& startMinimumLocalTimeInterval) ;
         
         /**
          * Generated
          */
          double getEndMinimumLocalTimeInterval() const ;
         
         /**
          * Generated
          */
          void setEndMinimumLocalTimeInterval(const double& endMinimumLocalTimeInterval) ;
         
         /**
          * Generated
          */
          double getMinimalTimestep() const ;
         
         /**
          * Generated
          */
          void setMinimalTimestep(const double& minimalTimestep) ;
         
         /**
          * Generated
          */
          bool getAllPatchesEvolvedToGlobalTimestep() const ;
         
         /**
          * Generated
          */
          void setAllPatchesEvolvedToGlobalTimestep(const bool& allPatchesEvolvedToGlobalTimestep) ;
         
         /**
          * Generated
          */
          double getAverageGlobalTimeInterval() const ;
         
         /**
          * Generated
          */
          void setAverageGlobalTimeInterval(const double& averageGlobalTimeInterval) ;
         
         /**
          * Generated
          */
          double getGlobalTimestepEndTime() const ;
         
         /**
          * Generated
          */
          void setGlobalTimestepEndTime(const double& globalTimestepEndTime) ;
         
         /**
          * Generated
          */
          bool getMinimalPatchBlockedDueToCoarsening() const ;
         
         /**
          * Generated
          */
          void setMinimalPatchBlockedDueToCoarsening(const bool& minimalPatchBlockedDueToCoarsening) ;
         
         /**
          * Generated
          */
          bool getMinimalPatchBlockedDueToGlobalTimestep() const ;
         
         /**
          * Generated
          */
          void setMinimalPatchBlockedDueToGlobalTimestep(const bool& minimalPatchBlockedDueToGlobalTimestep) ;
         
         
      };
      
   private: 
      PersistentRecords _persistentRecords;
      
   public:
      /**
       * Generated
       */
      TimeIntervalStatistics();
      
      /**
       * Generated
       */
      TimeIntervalStatistics(const PersistentRecords& persistentRecords);
      
      /**
       * Generated
       */
      TimeIntervalStatistics(const int& minimalPatchIndex, const int& minimalPatchParentIndex, const double& minimalPatchTime, const double& startMaximumLocalTimeInterval, const double& endMaximumLocalTimeInterval, const double& startMinimumLocalTimeInterval, const double& endMinimumLocalTimeInterval, const double& minimalTimestep, const bool& allPatchesEvolvedToGlobalTimestep, const double& averageGlobalTimeInterval, const double& globalTimestepEndTime, const bool& minimalPatchBlockedDueToCoarsening, const bool& minimalPatchBlockedDueToGlobalTimestep);
      
      /**
       * Generated
       */
      ~TimeIntervalStatistics();
      
      /**
       * Generated
       */
       int getMinimalPatchIndex() const ;
      
      /**
       * Generated
       */
       void setMinimalPatchIndex(const int& minimalPatchIndex) ;
      
      /**
       * Generated
       */
       int getMinimalPatchParentIndex() const ;
      
      /**
       * Generated
       */
       void setMinimalPatchParentIndex(const int& minimalPatchParentIndex) ;
      
      /**
       * Generated
       */
       double getMinimalPatchTime() const ;
      
      /**
       * Generated
       */
       void setMinimalPatchTime(const double& minimalPatchTime) ;
      
      /**
       * Generated
       */
       double getStartMaximumLocalTimeInterval() const ;
      
      /**
       * Generated
       */
       void setStartMaximumLocalTimeInterval(const double& startMaximumLocalTimeInterval) ;
      
      /**
       * Generated
       */
       double getEndMaximumLocalTimeInterval() const ;
      
      /**
       * Generated
       */
       void setEndMaximumLocalTimeInterval(const double& endMaximumLocalTimeInterval) ;
      
      /**
       * Generated
       */
       double getStartMinimumLocalTimeInterval() const ;
      
      /**
       * Generated
       */
       void setStartMinimumLocalTimeInterval(const double& startMinimumLocalTimeInterval) ;
      
      /**
       * Generated
       */
       double getEndMinimumLocalTimeInterval() const ;
      
      /**
       * Generated
       */
       void setEndMinimumLocalTimeInterval(const double& endMinimumLocalTimeInterval) ;
      
      /**
       * Generated
       */
       double getMinimalTimestep() const ;
      
      /**
       * Generated
       */
       void setMinimalTimestep(const double& minimalTimestep) ;
      
      /**
       * Generated
       */
       bool getAllPatchesEvolvedToGlobalTimestep() const ;
      
      /**
       * Generated
       */
       void setAllPatchesEvolvedToGlobalTimestep(const bool& allPatchesEvolvedToGlobalTimestep) ;
      
      /**
       * Generated
       */
       double getAverageGlobalTimeInterval() const ;
      
      /**
       * Generated
       */
       void setAverageGlobalTimeInterval(const double& averageGlobalTimeInterval) ;
      
      /**
       * Generated
       */
       double getGlobalTimestepEndTime() const ;
      
      /**
       * Generated
       */
       void setGlobalTimestepEndTime(const double& globalTimestepEndTime) ;
      
      /**
       * Generated
       */
       bool getMinimalPatchBlockedDueToCoarsening() const ;
      
      /**
       * Generated
       */
       void setMinimalPatchBlockedDueToCoarsening(const bool& minimalPatchBlockedDueToCoarsening) ;
      
      /**
       * Generated
       */
       bool getMinimalPatchBlockedDueToGlobalTimestep() const ;
      
      /**
       * Generated
       */
       void setMinimalPatchBlockedDueToGlobalTimestep(const bool& minimalPatchBlockedDueToGlobalTimestep) ;
      
      /**
       * Generated
       */
      std::string toString() const;
      
      /**
       * Generated
       */
      void toString(std::ostream& out) const;
      
      
      PersistentRecords getPersistentRecords() const;
      /**
       * Generated
       */
      TimeIntervalStatisticsPacked convert() const;
      
      
   #ifdef Parallel
      protected:
         static tarch::logging::Log _log;
         
      public:
         
         /**
          * Global that represents the mpi datatype.
          * There are two variants: Datatype identifies only those attributes marked with
          * parallelise. FullDatatype instead identifies the whole record with all fields.
          */
         static MPI_Datatype Datatype;
         static MPI_Datatype FullDatatype;
         
         /**
          * Initializes the data type for the mpi operations. Has to be called
          * before the very first send or receive operation is called.
          */
         static void initDatatype();
         
         static void shutdownDatatype();
         
         void send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise);
         
         void receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise);
         
         static bool isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise);
         
         #endif
            
         };
         
         /**
          * @author This class is generated by DaStGen
          * 		   DataStructureGenerator (DaStGen)
          * 		   2007-2009 Wolfgang Eckhardt
          * 		   2012      Tobias Weinzierl
          *
          * 		   build date: 22-10-2013 20:59
          *
          * @date   22/01/2014 19:02
          */
         class peanoclaw::statistics::TimeIntervalStatisticsPacked { 
            
            public:
               
               struct PersistentRecords {
                  int _minimalPatchIndex;
                  int _minimalPatchParentIndex;
                  double _minimalPatchTime;
                  double _startMaximumLocalTimeInterval;
                  double _endMaximumLocalTimeInterval;
                  double _startMinimumLocalTimeInterval;
                  double _endMinimumLocalTimeInterval;
                  double _minimalTimestep;
                  double _averageGlobalTimeInterval;
                  double _globalTimestepEndTime;
                  
                  /** mapping of records:
                  || Member 	|| startbit 	|| length
                   |  allPatchesEvolvedToGlobalTimestep	| startbit 0	| #bits 1
                   |  minimalPatchBlockedDueToCoarsening	| startbit 1	| #bits 1
                   |  minimalPatchBlockedDueToGlobalTimestep	| startbit 2	| #bits 1
                   */
                  short int _packedRecords0;
                  
                  /**
                   * Generated
                   */
                  PersistentRecords();
                  
                  /**
                   * Generated
                   */
                  PersistentRecords(const int& minimalPatchIndex, const int& minimalPatchParentIndex, const double& minimalPatchTime, const double& startMaximumLocalTimeInterval, const double& endMaximumLocalTimeInterval, const double& startMinimumLocalTimeInterval, const double& endMinimumLocalTimeInterval, const double& minimalTimestep, const bool& allPatchesEvolvedToGlobalTimestep, const double& averageGlobalTimeInterval, const double& globalTimestepEndTime, const bool& minimalPatchBlockedDueToCoarsening, const bool& minimalPatchBlockedDueToGlobalTimestep);
                  
                  /**
                   * Generated
                   */
                   int getMinimalPatchIndex() const ;
                  
                  /**
                   * Generated
                   */
                   void setMinimalPatchIndex(const int& minimalPatchIndex) ;
                  
                  /**
                   * Generated
                   */
                   int getMinimalPatchParentIndex() const ;
                  
                  /**
                   * Generated
                   */
                   void setMinimalPatchParentIndex(const int& minimalPatchParentIndex) ;
                  
                  /**
                   * Generated
                   */
                   double getMinimalPatchTime() const ;
                  
                  /**
                   * Generated
                   */
                   void setMinimalPatchTime(const double& minimalPatchTime) ;
                  
                  /**
                   * Generated
                   */
                   double getStartMaximumLocalTimeInterval() const ;
                  
                  /**
                   * Generated
                   */
                   void setStartMaximumLocalTimeInterval(const double& startMaximumLocalTimeInterval) ;
                  
                  /**
                   * Generated
                   */
                   double getEndMaximumLocalTimeInterval() const ;
                  
                  /**
                   * Generated
                   */
                   void setEndMaximumLocalTimeInterval(const double& endMaximumLocalTimeInterval) ;
                  
                  /**
                   * Generated
                   */
                   double getStartMinimumLocalTimeInterval() const ;
                  
                  /**
                   * Generated
                   */
                   void setStartMinimumLocalTimeInterval(const double& startMinimumLocalTimeInterval) ;
                  
                  /**
                   * Generated
                   */
                   double getEndMinimumLocalTimeInterval() const ;
                  
                  /**
                   * Generated
                   */
                   void setEndMinimumLocalTimeInterval(const double& endMinimumLocalTimeInterval) ;
                  
                  /**
                   * Generated
                   */
                   double getMinimalTimestep() const ;
                  
                  /**
                   * Generated
                   */
                   void setMinimalTimestep(const double& minimalTimestep) ;
                  
                  /**
                   * Generated
                   */
                   bool getAllPatchesEvolvedToGlobalTimestep() const ;
                  
                  /**
                   * Generated
                   */
                   void setAllPatchesEvolvedToGlobalTimestep(const bool& allPatchesEvolvedToGlobalTimestep) ;
                  
                  /**
                   * Generated
                   */
                   double getAverageGlobalTimeInterval() const ;
                  
                  /**
                   * Generated
                   */
                   void setAverageGlobalTimeInterval(const double& averageGlobalTimeInterval) ;
                  
                  /**
                   * Generated
                   */
                   double getGlobalTimestepEndTime() const ;
                  
                  /**
                   * Generated
                   */
                   void setGlobalTimestepEndTime(const double& globalTimestepEndTime) ;
                  
                  /**
                   * Generated
                   */
                   bool getMinimalPatchBlockedDueToCoarsening() const ;
                  
                  /**
                   * Generated
                   */
                   void setMinimalPatchBlockedDueToCoarsening(const bool& minimalPatchBlockedDueToCoarsening) ;
                  
                  /**
                   * Generated
                   */
                   bool getMinimalPatchBlockedDueToGlobalTimestep() const ;
                  
                  /**
                   * Generated
                   */
                   void setMinimalPatchBlockedDueToGlobalTimestep(const bool& minimalPatchBlockedDueToGlobalTimestep) ;
                  
                  
               };
               
            private: 
               PersistentRecords _persistentRecords;
               
            public:
               /**
                * Generated
                */
               TimeIntervalStatisticsPacked();
               
               /**
                * Generated
                */
               TimeIntervalStatisticsPacked(const PersistentRecords& persistentRecords);
               
               /**
                * Generated
                */
               TimeIntervalStatisticsPacked(const int& minimalPatchIndex, const int& minimalPatchParentIndex, const double& minimalPatchTime, const double& startMaximumLocalTimeInterval, const double& endMaximumLocalTimeInterval, const double& startMinimumLocalTimeInterval, const double& endMinimumLocalTimeInterval, const double& minimalTimestep, const bool& allPatchesEvolvedToGlobalTimestep, const double& averageGlobalTimeInterval, const double& globalTimestepEndTime, const bool& minimalPatchBlockedDueToCoarsening, const bool& minimalPatchBlockedDueToGlobalTimestep);
               
               /**
                * Generated
                */
               ~TimeIntervalStatisticsPacked();
               
               /**
                * Generated
                */
                int getMinimalPatchIndex() const ;
               
               /**
                * Generated
                */
                void setMinimalPatchIndex(const int& minimalPatchIndex) ;
               
               /**
                * Generated
                */
                int getMinimalPatchParentIndex() const ;
               
               /**
                * Generated
                */
                void setMinimalPatchParentIndex(const int& minimalPatchParentIndex) ;
               
               /**
                * Generated
                */
                double getMinimalPatchTime() const ;
               
               /**
                * Generated
                */
                void setMinimalPatchTime(const double& minimalPatchTime) ;
               
               /**
                * Generated
                */
                double getStartMaximumLocalTimeInterval() const ;
               
               /**
                * Generated
                */
                void setStartMaximumLocalTimeInterval(const double& startMaximumLocalTimeInterval) ;
               
               /**
                * Generated
                */
                double getEndMaximumLocalTimeInterval() const ;
               
               /**
                * Generated
                */
                void setEndMaximumLocalTimeInterval(const double& endMaximumLocalTimeInterval) ;
               
               /**
                * Generated
                */
                double getStartMinimumLocalTimeInterval() const ;
               
               /**
                * Generated
                */
                void setStartMinimumLocalTimeInterval(const double& startMinimumLocalTimeInterval) ;
               
               /**
                * Generated
                */
                double getEndMinimumLocalTimeInterval() const ;
               
               /**
                * Generated
                */
                void setEndMinimumLocalTimeInterval(const double& endMinimumLocalTimeInterval) ;
               
               /**
                * Generated
                */
                double getMinimalTimestep() const ;
               
               /**
                * Generated
                */
                void setMinimalTimestep(const double& minimalTimestep) ;
               
               /**
                * Generated
                */
                bool getAllPatchesEvolvedToGlobalTimestep() const ;
               
               /**
                * Generated
                */
                void setAllPatchesEvolvedToGlobalTimestep(const bool& allPatchesEvolvedToGlobalTimestep) ;
               
               /**
                * Generated
                */
                double getAverageGlobalTimeInterval() const ;
               
               /**
                * Generated
                */
                void setAverageGlobalTimeInterval(const double& averageGlobalTimeInterval) ;
               
               /**
                * Generated
                */
                double getGlobalTimestepEndTime() const ;
               
               /**
                * Generated
                */
                void setGlobalTimestepEndTime(const double& globalTimestepEndTime) ;
               
               /**
                * Generated
                */
                bool getMinimalPatchBlockedDueToCoarsening() const ;
               
               /**
                * Generated
                */
                void setMinimalPatchBlockedDueToCoarsening(const bool& minimalPatchBlockedDueToCoarsening) ;
               
               /**
                * Generated
                */
                bool getMinimalPatchBlockedDueToGlobalTimestep() const ;
               
               /**
                * Generated
                */
                void setMinimalPatchBlockedDueToGlobalTimestep(const bool& minimalPatchBlockedDueToGlobalTimestep) ;
               
               /**
                * Generated
                */
               std::string toString() const;
               
               /**
                * Generated
                */
               void toString(std::ostream& out) const;
               
               
               PersistentRecords getPersistentRecords() const;
               /**
                * Generated
                */
               TimeIntervalStatistics convert() const;
               
               
            #ifdef Parallel
               protected:
                  static tarch::logging::Log _log;
                  
               public:
                  
                  /**
                   * Global that represents the mpi datatype.
                   * There are two variants: Datatype identifies only those attributes marked with
                   * parallelise. FullDatatype instead identifies the whole record with all fields.
                   */
                  static MPI_Datatype Datatype;
                  static MPI_Datatype FullDatatype;
                  
                  /**
                   * Initializes the data type for the mpi operations. Has to be called
                   * before the very first send or receive operation is called.
                   */
                  static void initDatatype();
                  
                  static void shutdownDatatype();
                  
                  void send(int destination, int tag, bool exchangeOnlyAttributesMarkedWithParallelise);
                  
                  void receive(int source, int tag, bool exchangeOnlyAttributesMarkedWithParallelise);
                  
                  static bool isMessageInQueue(int tag, bool exchangeOnlyAttributesMarkedWithParallelise);
                  
                  #endif
                     
                  };
                  
                  #endif
                  
