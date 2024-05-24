/*
 * Copyright (c) 2024, ITGSS Corporation. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This Code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this Code).
 *
 * Contact with ITGSS, 640 N McCarvy Blvd. , in the
 * city of Milpitas, zip Code 95035, state of California.
 * or visit www.it-gss.com if you need additional information or have any
 * questions.
 *
 */

// About:
// Author(-s): Tunjay Akbarli (tunjayakbarli@it-gss.com)
// Date: Sunday, May 24, 2024
// Technology: C++20 - ISO/IEC 14882:2020(E) 


// set_new_handler

#include <mutex>
#include <new.h>
#include <new>

namespace {
    _STD new_handler _New_handler;
    constinit _STD mutex _New_handler_mutex;

    int __cdecl _New_handler_interface(size_t) { // interface to existing Microsoft _callnewh mechanism
        _New_handler();
        return 1;
    }
} // namespace

_STD_BEGIN

_CRTIMP2 new_handler __cdecl set_new_handler(_In_opt_ new_handler pnew) noexcept { // remove current handler
    lock_guard _Lock{_New_handler_mutex};
    new_handler pold = _New_handler;
    _New_handler     = pnew;
    _set_new_handler(pnew ? _New_handler_interface : nullptr);
    return pold;
}

_CRTIMP2 new_handler __cdecl get_new_handler() noexcept { // get current new handler
    lock_guard _Lock{_New_handler_mutex};
    return _New_handler;
}

_STD_END
