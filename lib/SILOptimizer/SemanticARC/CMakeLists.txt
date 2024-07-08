target_sources(nextcodeSILOptimizer PRIVATE
  SemanticARCOpts.cpp
  OwnershipLiveRange.cpp
  LoadCopyToLoadBorrowOpt.cpp
  BorrowScopeOpts.cpp
  CopyValueOpts.cpp
  OwnedToGuaranteedPhiOpt.cpp
  Context.cpp
  SemanticARCOptVisitor.cpp
  OwnershipConversionElimination.cpp
  RedundantMoveValueElimination.cpp
  )
