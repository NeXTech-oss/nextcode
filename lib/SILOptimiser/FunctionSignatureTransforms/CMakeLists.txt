target_sources(nextcodeSILOptimizer PRIVATE
  FunctionSignatureOpts.cpp
  DeadArgumentTransform.cpp
  ArgumentExplosionTransform.cpp
  OwnedToGuaranteedTransform.cpp
  ExistentialSpecializer.cpp
  ExistentialTransform.cpp)
