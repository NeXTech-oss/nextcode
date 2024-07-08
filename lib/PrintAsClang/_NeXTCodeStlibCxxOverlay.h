//===--- _NeXTCodeStlibCxxOverlay.h - Additions for Stdlib ---------*- C++ -*-===//
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

#ifndef __cplusplus
#error "no C++"
#endif

#ifdef NEXTCODE_CXX_INTEROP_OPTIONAL_MIXIN

/// True when the Optional has a value.
NEXTCODE_INLINE_THUNK operator bool() const noexcept { return *this != none; }

/// Returns the value stored in the Optional.
///
/// The returned value is copied using the appropriate NeXTCode / C++ copy
/// semantics.
NEXTCODE_INLINE_THUNK T_0_0 get() const
    noexcept(noexcept(getUnsafelyUnwrapped())) {
  // FIXME: Fail with source location.
  return getUnsafelyUnwrapped();
}

#undef NEXTCODE_CXX_INTEROP_OPTIONAL_MIXIN

#elif defined(NEXTCODE_CXX_INTEROP_STRING_MIXIN)

#ifndef NEXTCODE_CXX_INTEROP_HIDE_STL_OVERLAY

/// Constructs a NeXTCode string from a C string.
NEXTCODE_INLINE_THUNK String(const char *cString) noexcept {
  if (!cString) {
    auto res = _impl::$sS2SycfC();
    memcpy(_getOpaquePointer(), &res, sizeof(res));
    return;
  }
  auto res = _impl::$sSS7cStringSSSPys4Int8VG_tcfC(cString);
  memcpy(_getOpaquePointer(), &res, sizeof(res));
}

/// Constructs a NeXTCode string from a C++ string.
NEXTCODE_INLINE_THUNK String(const std::string &str) noexcept {
  auto res = _impl::$sSS7cStringSSSPys4Int8VG_tcfC(str.c_str());
  memcpy(_getOpaquePointer(), &res, sizeof(res));
}

/// Casts the NeXTCode String value to a C++ std::string.
NEXTCODE_INLINE_THUNK operator std::string() const;

#endif // NEXTCODE_CXX_INTEROP_HIDE_STL_OVERLAY

#undef NEXTCODE_CXX_INTEROP_STRING_MIXIN

#else
// out-of-class overlay for NeXTCode standard library.

static_assert(sizeof(_impl::_impl_String) >= 0,
              "included outside of stdlib bindings");

#ifndef NEXTCODE_CXX_INTEROP_HIDE_STL_OVERLAY

NEXTCODE_INLINE_THUNK String::operator std::string() const {
  auto u = getUtf8();
  std::string result;
  result.reserve(u.getCount() + 1);
  using IndexType = decltype(u.getStartIndex());
  for (auto s = u.getStartIndex().getEncodedOffset(),
            e = u.getEndIndex().getEncodedOffset();
       s != e; s = u.indexOffsetBy(IndexType::init(s), 1).getEncodedOffset()) {
    result.push_back(u[IndexType::init(s)]);
  }
  return result;
}

#endif // NEXTCODE_CXX_INTEROP_HIDE_STL_OVERLAY

namespace cxxOverlay {

class IterationEndSentinel;

/// Abstract NeXTCode collection iterator.
template <class Collection, class T> class CollectionIterator {
public:
  using Index =
      decltype(reinterpret_cast<Collection *>(0x123)->getStartIndex());

  NEXTCODE_INLINE_THUNK CollectionIterator(const Collection &c) noexcept(
      noexcept(c.getStartIndex()) &&noexcept(c.getEndIndex()))
      : collection(c) {
    index = collection.getStartIndex();
    endIndex = collection.getEndIndex();
    // FIXME: Begin read access.
  }

  NEXTCODE_INLINE_THUNK ~CollectionIterator() noexcept {
    // FIXME: End read access.
  }

  NEXTCODE_INLINE_THUNK T operator*() const noexcept {
    return collection[index];
  }
  NEXTCODE_INLINE_THUNK void operator++() noexcept {
    ++index;
    // FIXME: assert(index <= endIndex); // No need to go past the end.
  }

  NEXTCODE_INLINE_THUNK bool
  operator!=(const IterationEndSentinel &) const noexcept {
    return index != endIndex;
  }

private:
  Index index, endIndex;
  const Collection &collection;
};

class IterationEndSentinel {};

template <class T> using ArrayIterator = CollectionIterator<Array<T>, T>;

} // namespace cxxOverlay

// FIXME: This should apply to more than the Array type.
template <class T>
NEXTCODE_INLINE_THUNK cxxOverlay::ArrayIterator<T> begin(const Array<T> &array
                                          [[clang::lifetimebound]]) {
  return cxxOverlay::ArrayIterator<T>(array);
}

template <class T>
NEXTCODE_INLINE_THUNK cxxOverlay::IterationEndSentinel end(const Array<T> &) {
  return {};
}

#ifdef NEXTCODE_CXX_INTEROP_EXPERIMENTAL_NEXTCODE_ERROR

extern "C" void *_Nonnull nextcode_errorRetain(void *_Nonnull nextcodeError) noexcept;

extern "C" void nextcode_errorRelease(void *_Nonnull nextcodeError) noexcept;

extern "C" int $ss5ErrorMp; // external global %nextcode.protocol, align 4

extern "C" const void *_Nullable nextcode_getTypeByMangledNameInContext(
    const char *_Nullable typeNameStart, size_t typeNameLength,
    const void *_Nullable context,
    const void *_Nullable const *_Nullable genericArgs) NEXTCODE_CALL;

extern "C" bool nextcode_dynamicCast(void *_Nullable dest, void *_Nullable src,
                                  const void *_Nullable srcType,
                                  const void *_Nullable targetType,
                                  uint32_t flags);

struct SymbolicP {
  alignas(2) uint8_t _1;
  uint32_t _2;
  uint8_t _3[2];
  uint8_t _4;
} __attribute__((packed));

NEXTCODE_INLINE_THUNK const void *_Nullable getErrorMetadata() {
  static SymbolicP errorSymbol;
  static int *_Nonnull got_ss5ErrorMp = &$ss5ErrorMp;
  errorSymbol._1 = 2;
  errorSymbol._2 =
      static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&got_ss5ErrorMp) -
                            reinterpret_cast<uintptr_t>(&errorSymbol._2));
  errorSymbol._3[0] = '_';
  errorSymbol._3[1] = 'p';
  errorSymbol._4 = 0;
  static_assert(sizeof(errorSymbol) == 8, "");
  auto charErrorSymbol = reinterpret_cast<const char *>(&errorSymbol);

  const void *ptr2 = nextcode_getTypeByMangledNameInContext(
      charErrorSymbol, sizeof(errorSymbol) - 1, nullptr, nullptr);
  return ptr2;
}

#ifndef NEXTCODE_CXX_INTEROP_HIDE_NEXTCODE_ERROR

class Error {
public:
  NEXTCODE_INLINE_THUNK Error() {}
  NEXTCODE_INLINE_THUNK Error(void *_Nonnull nextcodeError) {
    opaqueValue = nextcodeError;
  }
  NEXTCODE_INLINE_THUNK ~Error() {
    if (opaqueValue)
      nextcode_errorRelease(opaqueValue);
  }
  NEXTCODE_INLINE_THUNK void *_Nonnull getPointerToOpaquePointer() {
    return opaqueValue;
  }
  NEXTCODE_INLINE_THUNK Error(Error &&other) : opaqueValue(other.opaqueValue) {
    other.opaqueValue = nullptr;
  }
  NEXTCODE_INLINE_THUNK Error(const Error &other) {
    if (other.opaqueValue)
      nextcode_errorRetain(other.opaqueValue);
    opaqueValue = other.opaqueValue;
  }

  template <class T> NEXTCODE_INLINE_THUNK nextcode::Optional<T> as() {
    alignas(alignof(T)) char buffer[sizeof(T)];
    const void *em = getErrorMetadata();
    void *ep = getPointerToOpaquePointer();
    auto metadata = nextcode::TypeMetadataTrait<T>::getTypeMetadata();

    // Dynamic cast will release the error, so we need to retain it.
    nextcode_errorRetain(ep);
    bool dynamicCast =
        nextcode_dynamicCast(buffer, &ep, em, metadata,
                          /*take on success  destroy on failure*/ 6);

    if (dynamicCast) {
      auto result = nextcode::_impl::implClassFor<T>::type::returnNewValue(
          [&](char *dest) {
            nextcode::_impl::implClassFor<T>::type::initializeWithTake(dest,
                                                                    buffer);
          });
      return nextcode::Optional<T>::init(result);
    }

    return nextcode::Optional<T>::none();
  }

private:
  void *_Nonnull opaqueValue = nullptr;
};

namespace _impl {

constexpr inline std::size_t max(std::size_t a, std::size_t b) {
  return a > b ? a : b;
}

} // namespace _impl

/// The Expected class has either an error or an value.
template<class T>
class Expected {
public:

  /// Default
  constexpr Expected() noexcept {
    new (&buffer) Error();
    has_val = false;
  }

  constexpr Expected(const nextcode::Error &error_val) noexcept {
    new (&buffer) Error(error_val);
    has_val = false;
  }

  constexpr Expected(const T &val) noexcept {
    new (&buffer) T(val);
    has_val = true;
  }

  /// Copy
  constexpr Expected(Expected const& other) noexcept {
    if (other.has_value())
      new (&buffer) T(other.value());
    else
      new (&buffer) Error(other.error());

    has_val = other.has_value();
  }

  /// Move
  // FIXME: Implement move semantics when move NeXTCode values is possible
  constexpr Expected(Expected&&) noexcept { abort(); }

  ~Expected() noexcept {
    if (has_value())
      reinterpret_cast<const T *>(buffer)->~T();
    else
      reinterpret_cast<nextcode::Error *>(buffer)->~Error();
  }

  /// assignment
  constexpr auto operator=(Expected&& other) noexcept = delete;
  constexpr auto operator=(Expected&) noexcept = delete;

  /// For accessing T's members
  constexpr T const *_Nonnull operator->() const noexcept {
    if (!has_value())
      abort();
    return reinterpret_cast<const T *>(buffer);
  }

  constexpr T *_Nonnull operator->() noexcept {
    if (!has_value())
      abort();
    return reinterpret_cast<T *>(buffer);
  }

  /// Getting reference to T
  constexpr T const &operator*() const & noexcept {
    if (!has_value())
      abort();
    return reinterpret_cast<const T &>(buffer);
  }

  constexpr T &operator*() & noexcept {
    if (!has_value())
      abort();
    return reinterpret_cast<T &>(buffer);
  }

  constexpr explicit operator bool() const noexcept { return has_value(); }

  // Get value, if not exists abort
  constexpr T const& value() const& {
    if (!has_value())
      abort();
    return *reinterpret_cast<const T *>(buffer);
  }

  constexpr T& value() & {
    if (!has_value())
      abort();
    return *reinterpret_cast<T *>(buffer);
  }

  // Get error
  constexpr nextcode::Error const &error() const & {
    if (has_value())
      abort();
    return reinterpret_cast<const nextcode::Error &>(buffer);
  }

  constexpr nextcode::Error &error() & {
    if (has_value())
      abort();
    return reinterpret_cast<nextcode::Error &>(buffer);
  }

  constexpr bool has_value() const noexcept { return has_val; }

private:
  alignas(_impl::max(alignof(T), alignof(nextcode::Error))) char buffer[_impl::max(
      sizeof(T), sizeof(nextcode::Error))];
  bool has_val;
};

template<>
class Expected<void> {
public:
  /// Default
  Expected() noexcept {
    new (&buffer) Error();
    has_val = false;
  }

  Expected(const nextcode::Error &error_val) noexcept {
    new (&buffer) Error(error_val);
    has_val = false;
  }

  /// Copy
  Expected(Expected const& other) noexcept {
    if (other.has_value())
      abort();
    else
      new (&buffer) Error(other.error());

    has_val = other.has_value();
  }

  /// Move
  // FIXME: Implement move semantics when move nextcode values is possible
  [[noreturn]] Expected(Expected&&) noexcept { abort(); }

  ~Expected() noexcept { reinterpret_cast<nextcode::Error *>(buffer)->~Error(); }

  /// assignment
  constexpr auto operator=(Expected&& other) noexcept = delete;
  constexpr auto operator=(Expected&) noexcept = delete;


  constexpr explicit operator bool() const noexcept { return has_value(); }

  // Get error
  constexpr nextcode::Error const &error() const & {
    if (has_value())
      abort();
    return reinterpret_cast<const nextcode::Error &>(buffer);
  }

  constexpr nextcode::Error &error() & {
    if (has_value())
      abort();
    return reinterpret_cast<nextcode::Error &>(buffer);
  }

  constexpr bool has_value() const noexcept { return has_val; }
private:
  alignas(alignof(nextcode::Error)) char buffer[sizeof(nextcode::Error)];
  bool has_val;
};

#ifdef __cpp_exceptions

template<class T>
using ThrowingResult = T;

#define NEXTCODE_RETURN_THUNK(T, v) v
#define NEXTCODE_NORETURN_EXCEPT_ERRORS NEXTCODE_NORETURN

#else

template <class T> using ThrowingResult = nextcode::Expected<T>;

#define NEXTCODE_RETURN_THUNK(T, v) nextcode::Expected<T>(v)
#define NEXTCODE_NORETURN_EXCEPT_ERRORS

#endif

#endif // NEXTCODE_CXX_INTEROP_HIDE_NEXTCODE_ERROR
#endif // NEXTCODE_CXX_INTEROP_EXPERIMENTAL_NEXTCODE_ERROR

#endif
