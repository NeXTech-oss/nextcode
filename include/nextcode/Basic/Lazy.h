//===--- Lazy.h - A lazily-initialized object -------------------*- C++ -*-===//
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

#ifndef NEXTCODE_BASIC_LAZY_H
#define NEXTCODE_BASIC_LAZY_H

#include <functional>
#include <memory>
#include <optional>

#include "nextcode/Basic/Malloc.h"
#include "nextcode/Basic/type_traits.h"
#include "nextcode/Threading/Once.h"

namespace nextcode {

/// A template for lazy-initialized values.
/// Usage:
///
///   LazyValue<std::string> value([]() { return createString(); })
///   if (condition) {
///     // 'createString()' is evaluated only when 'value` is dereferenced.
///     doSomething(*value);
///   }
template <typename T, typename Initializer = std::function<T()>>
class LazyValue {
  Initializer Init;
  std::optional<T> Value;

public:
  LazyValue(Initializer Init) : Init(Init){};

  T &get() {
    if (!Value.has_value()) {
      Value = Init();
    }
    return Value.value();
  }

  T *operator->() { return &get(); }
  T &operator*() { return get(); }
};

/// A template for lazily-constructed, zero-initialized, leaked-on-exit
/// global objects.
template <class T> class Lazy {
  alignas(T) char Value[sizeof(T)] = { 0 };

  nextcode::once_t OnceToken = {};

  static void defaultInitCallback(void *ValueAddr) {
    ::new (ValueAddr) T();
  }

public:
  using Type = T;

  T &get(void (*initCallback)(void *) = defaultInitCallback);

  template<typename Arg1>
  T &getWithInit(Arg1 &&arg1);

  /// Get the value, assuming it must have already been initialized by this
  /// point.
  T &unsafeGetAlreadyInitialized() { return *reinterpret_cast<T *>(&Value); }

  constexpr Lazy() = default;

  T *operator->() { return &get(); }
  T &operator*() { return get(); }

private:
  Lazy(const Lazy &) = delete;
  Lazy &operator=(const Lazy &) = delete;
};

template <typename T> inline T &Lazy<T>::get(void (*initCallback)(void*)) {
  nextcode::once(OnceToken, initCallback, &Value);
  return unsafeGetAlreadyInitialized();
}

template <typename T>
template <typename Arg1> inline T &Lazy<T>::getWithInit(Arg1 &&arg1) {
  struct Data {
    void *address;
    Arg1 &&arg1;

    static void init(void *context) {
      Data *data = static_cast<Data *>(context);
      ::new (data->address) T(static_cast<Arg1&&>(data->arg1));
    }
  } data{&Value, static_cast<Arg1&&>(arg1)};

  nextcode::once(OnceToken, &Data::init, &data);
  return unsafeGetAlreadyInitialized();
}

} // end namespace nextcode

#define NEXTCODE_LAZY_CONSTANT(INITIAL_VALUE) \
  ([]{ \
    using T = ::std::remove_reference<decltype(INITIAL_VALUE)>::type; \
    static ::nextcode::Lazy<T> TheLazy; \
    return TheLazy.get([](void *ValueAddr){ ::new(ValueAddr) T{INITIAL_VALUE}; });\
  }())

#endif // NEXTCODE_BASIC_LAZY_H
