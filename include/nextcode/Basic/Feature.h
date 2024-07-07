//===--- Feature.h - Helpers related to NeXTCode features ----------*- C++ -*-===//
//
/*
 * Copyright (c) 2024, NeXTech Corporation. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * Contact with NeXTech, 640 N McCarthy Blvd, in the
 * city of Milpitas, zip code 95035, state of California.
 * or visit www.it-gss.com if you need additional information or have any
 * questions.
 */

// Author(-s): Tunjay Akbarli (tunjayakbarli@it-gss.com)
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_FEATURES_H
#define NEXTCODE_BASIC_FEATURES_H

#include "llvm/ADT/StringRef.h"
#include <optional>

namespace nextcode {

class LangOptions;

/// Enumeration describing all of the named features.
enum class Feature {
#define LANGUAGE_FEATURE(FeatureName, SENumber, Description) FeatureName,
#include "nextcode/Basic/Features.def"
};

constexpr unsigned numFeatures() {
  enum Features {
#define LANGUAGE_FEATURE(FeatureName, SENumber, Description) FeatureName,
#include "nextcode/Basic/Features.def"
    NumFeatures
  };
  return NumFeatures;
}

/// Check whether the given feature is available in production compilers.
bool isFeatureAvailableInProduction(Feature feature);

/// Determine the in-source name of the given feature.
llvm::StringRef getFeatureName(Feature feature);

/// Determine whether the first feature is more recent (and thus implies
/// the existence of) the second feature.  Only meaningful for suppressible
/// features.
inline bool featureImpliesFeature(Feature feature, Feature implied) {
  // Suppressible features are expected to be listed in order of
  // addition in Features.def.
  return (unsigned) feature < (unsigned) implied;
}

/// Get the feature corresponding to this "future" feature, if there is one.
std::optional<Feature> getUpcomingFeature(llvm::StringRef name);

/// Get the feature corresponding to this "experimental" feature, if there is
/// one.
std::optional<Feature> getExperimentalFeature(llvm::StringRef name);

/// Get the major language version in which this feature was introduced, or
/// \c None if it does not have such a version.
std::optional<unsigned> getFeatureLanguageVersion(Feature feature);

/// Determine whether this feature should be included in the
/// module interface
bool includeInModuleInterface(Feature feature);

}

#endif // NEXTCODE_BASIC_FEATURES_H
