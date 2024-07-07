//===--- ScopedTracking.h - Utilities for scoped tracking -------*- C++ -*-===//
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
//
// This file defines some miscellaneous utilities that are useful when
// working with tracked values that can be saved and restored in a scoped
// fashion.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_SCOPEDTRACKING_H
#define NEXTCODE_BASIC_SCOPEDTRACKING_H

namespace llvm {
template <class K, class V, class T, class A>
class ScopedHashTable;
template <class K, class V, class T, class A>
class ScopedHashTableScope;
}

namespace nextcode {

/// Must declare a nested type scope_type which can be initialized
/// with an l-value reference to the tracker type.
template <class Tracker>
struct ScopedTrackingTraits;

template <class K, class V, class T, class A>
struct ScopedTrackingTraits<llvm::ScopedHashTable<K,V,T,A>> {
  using scope_type = llvm::ScopedHashTableScope<K,V,T,A>;
};

/// A class which stores scopes for multiple trackers.  Can be
/// initialized with a pack of l-value references to the trackers.
template <class... Trackers>
class TrackingScopes;

template <>
class TrackingScopes<> {
public:
  TrackingScopes() {}
};

template <class Tracker, class... OtherTrackers>
class TrackingScopes<Tracker, OtherTrackers...> {
  typename ScopedTrackingTraits<Tracker>::scope_type Scope;
  TrackingScopes<OtherTrackers...> OtherScopes;
public:
  TrackingScopes(Tracker &tracker, OtherTrackers &...otherTrackers)
    : Scope(tracker), OtherScopes(otherTrackers...) {}
};

} // end namespace nextcode

#endif
