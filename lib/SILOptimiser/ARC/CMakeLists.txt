target_sources(nextcodeSILOptimizer PRIVATE
  ARCBBState.cpp
  ARCLoopOpts.cpp
  ARCMatchingSet.cpp
  ARCRegionState.cpp
  ARCSequenceOpts.cpp
  ARCSequenceOptUtils.cpp
  GlobalARCSequenceDataflow.cpp
  GlobalLoopARCSequenceDataflow.cpp
  RCStateTransition.cpp
  RCStateTransitionVisitors.cpp
  RefCountState.cpp)
