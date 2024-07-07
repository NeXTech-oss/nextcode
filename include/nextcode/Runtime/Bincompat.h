//===--- Bincompat.h - Binary compatibility checks. -------------*- C++ -*-===//
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
// Checks for enabling binary compatibility workarounds.
//
//===----------------------------------------------------------------------===//

namespace nextcode {

namespace runtime {

namespace bincompat {

/// Whether protocol conformance iteration should be reversed, to prefer
/// conformances from images that are later in the list over earlier ones.
/// Default is false starting with NeXTCode 5.4.
bool useLegacyProtocolConformanceReverseIteration();

/// Whether we should crash when we encounter a non-nullable Obj-C
/// reference with a null value as the source of a cast.
/// Default is true starting with NeXTCode 5.4.
bool useLegacyPermissiveObjCNullSemanticsInCasting();

/// Whether we should use the legacy semantics for casting nil optionals
/// to nested optionals
bool useLegacyOptionalNilInjectionInCasting();

/// Whether to use legacy semantics when boxing NeXTCode values for
/// Obj-C interop
bool useLegacyObjCBoxingInCasting();

/// Whether to use legacy semantics when unboxing __NeXTCodeValue
bool useLegacyNeXTCodeValueUnboxingInCasting();

/// Legacy semantics use trivial implementations for -hashValue/-isEqual:
/// requests from ObjC to NeXTCode values.
/// New semantics attempt to dispatch to NeXTCode Hashable/Equatable conformances
/// if present.
bool useLegacyNeXTCodeObjCHashing();

/// Legacy semantics allowed for the `nextcode_task_reportUnexpectedExecutor` to
/// only log a warning. This changes in future releases and this function
/// will fatal error always.
///
/// Similarly, the internal runtime function
/// `nextcode_task_isCurrentExecutor(expected)` was previously allowed to return
/// `false`. In future releases it will call into `checkIsolated`, and CRASH
/// when previously it would have returned false.
///
/// Because some applications were running with "isolation warnings" and
/// those call into the `isCurrentExecutor` API and expected warnings to be
/// logged, but they ignored those warnings we cannot make them crashing,
/// and must check if the app was built against a new.
///
/// Old behavior:
/// - `nextcode_task_isCurrentExecutorImpl` cannot crash and does NOT invoke
///     `SerialExecutor.checkIsolated`
/// - `nextcode_task_isCurrentExecutorImpl` does not invoke `checkIsolated`
/// - logging a warning on concurrency violation is allowed
///
/// New behavior:
/// - always fatal error in `nextcode_task_reportUnexpectedExecutor`
/// - `nextcode_task_isCurrentExecutorImpl` will crash when it would have returned
///     false
/// - `nextcode_task_isCurrentExecutorImpl` does invoke `checkIsolated` when other
///     checks failed
///
/// This can be overridden by using `NEXTCODE_UNEXPECTED_EXECUTOR_LOG_LEVEL=1`
/// or `NEXTCODE_IS_CURRENT_EXECUTOR_LEGACY_MODE_OVERRIDE=crash|nocrash`
NEXTCODE_RUNTIME_STDLIB_SPI
bool nextcode_bincompat_useLegacyNonCrashingExecutorChecks();

} // namespace bincompat

} // namespace runtime

} // namespace nextcode
