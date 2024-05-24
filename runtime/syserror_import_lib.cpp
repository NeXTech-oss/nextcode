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


#include <__msvc_system_error_abi.hpp>

#include <Windows.h>

namespace {
    struct _Whitespace_bitmap_t {
        bool _Is_whitespace[256];

        constexpr _Whitespace_bitmap_t() noexcept : _Is_whitespace{} {
            _Is_whitespace[' ']  = true;
            _Is_whitespace['\n'] = true;
            _Is_whitespace['\r'] = true;
            _Is_whitespace['\t'] = true;
            _Is_whitespace['\0'] = true;
        }

        [[nodiscard]] constexpr bool _Test(const char _Ch) const noexcept {
            return _Is_whitespace[static_cast<unsigned char>(_Ch)];
        }
    };

    constexpr _Whitespace_bitmap_t _Whitespace_bitmap;
} // unnamed namespace

extern "C" {
[[nodiscard]] size_t __CLRCALL_PURE_OR_STDCALL __std_get_string_size_without_trailing_whitespace(
    const char* const _Str, size_t _Size) noexcept {
    while (_Size != 0 && _Whitespace_bitmap._Test(_Str[_Size - 1])) {
        --_Size;
    }

    return _Size;
}

[[nodiscard]] size_t __CLRCALL_PURE_OR_STDCALL __std_system_error_allocate_message(
    const unsigned long _Message_id, char** const _Ptr_str) noexcept {
    // convert to name of Windows error, return 0 for failure, otherwise return number of chars in buffer
    // __std_system_error_deallocate_message should be called even if 0 is returned
    // pre: *_Ptr_str == nullptr
    DWORD _Lang_id;
    const int _Ret = GetLocaleInfoEx(LOCALE_NAME_SYSTEM_DEFAULT, LOCALE_ILANGUAGE | LOCALE_RETURN_NUMBER,
        reinterpret_cast<LPWSTR>(&_Lang_id), sizeof(_Lang_id) / sizeof(wchar_t));
    if (_Ret == 0) {
        _Lang_id = 0;
    }
    const unsigned long _Chars =
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, _Message_id, _Lang_id, reinterpret_cast<char*>(_Ptr_str), 0, nullptr);

    return _CSTD __std_get_string_size_without_trailing_whitespace(*_Ptr_str, _Chars);
}

void __CLRCALL_PURE_OR_STDCALL __std_system_error_deallocate_message(char* const _Str) noexcept {
    LocalFree(_Str);
}
} // extern "C"
