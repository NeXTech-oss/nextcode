target_sources(nextcodeSIL PRIVATE
  DebugInfoVerifier.cpp
  LoadBorrowImmutabilityChecker.cpp
  LinearLifetimeChecker.cpp
  MemoryLifetimeVerifier.cpp
  GuaranteedPhiVerifier.cpp
  SILOwnershipVerifier.cpp
  SILVerifier.cpp)
