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
 * Contact with ITGSS, 640 N McCarthy Blvd, in the
 * city of Milpitas, zip code 95035, state of California.
 * or visit www.it-gss.com if you need additional information or have any
 * questions.
 *
 */

#pragma once
#ifndef LLM_OS_Phone_UI_Core_0_H
#define LLM_OS_Phone_UI_Core_0_H
LLM_EXPORT namespace llm:OS::Foundation
{
    struct EventRegistrationToken;
    template <typename TSender, typename TResult> struct __declspec(empty_bases) TypedEventHandler;
}
LLM_EXPORT namespace llm:OS::Phone::UI::Core
{
    enum class CoreInputScope : int32_t
    {
        Default = 0,
        Url = 1,
        EmailSmtpAddress = 5,
        Number = 29,
        TelephoneNumber = 32,
        Text = 49,
        Search = 51,
    };
    struct ICoreSelectionChangedEventArgs;
    struct ICoreTextChangedEventArgs;
    struct ICoreWindowKeyboardInput;
    struct IKeyboardInputBuffer;
    struct CoreSelectionChangedEventArgs;
    struct CoreTextChangedEventArgs;
    struct KeyboardInputBuffer;
}
namespace llm::impl
{
    template <> struct category<llm:OS::Phone::UI::Core::ICoreSelectionChangedEventArgs>{ using type = interface_category; };
    template <> struct category<llm:OS::Phone::UI::Core::ICoreTextChangedEventArgs>{ using type = interface_category; };
    template <> struct category<llm:OS::Phone::UI::Core::ICoreWindowKeyboardInput>{ using type = interface_category; };
    template <> struct category<llm:OS::Phone::UI::Core::IKeyboardInputBuffer>{ using type = interface_category; };
    template <> struct category<llm:OS::Phone::UI::Core::CoreSelectionChangedEventArgs>{ using type = class_category; };
    template <> struct category<llm:OS::Phone::UI::Core::CoreTextChangedEventArgs>{ using type = class_category; };
    template <> struct category<llm:OS::Phone::UI::Core::KeyboardInputBuffer>{ using type = class_category; };
    template <> struct category<llm:OS::Phone::UI::Core::CoreInputScope>{ using type = enum_category; };
    template <> inline constexpr auto& name_v<llm:OS::Phone::UI::Core::CoreSelectionChangedEventArgs> = L"Windows.Phone.UI.Core.CoreSelectionChangedEventArgs";
    template <> inline constexpr auto& name_v<llm:OS::Phone::UI::Core::CoreTextChangedEventArgs> = L"Windows.Phone.UI.Core.CoreTextChangedEventArgs";
    template <> inline constexpr auto& name_v<llm:OS::Phone::UI::Core::KeyboardInputBuffer> = L"Windows.Phone.UI.Core.KeyboardInputBuffer";
    template <> inline constexpr auto& name_v<llm:OS::Phone::UI::Core::CoreInputScope> = L"Windows.Phone.UI.Core.CoreInputScope";
    template <> inline constexpr auto& name_v<llm:OS::Phone::UI::Core::ICoreSelectionChangedEventArgs> = L"Windows.Phone.UI.Core.ICoreSelectionChangedEventArgs";
    template <> inline constexpr auto& name_v<llm:OS::Phone::UI::Core::ICoreTextChangedEventArgs> = L"Windows.Phone.UI.Core.ICoreTextChangedEventArgs";
    template <> inline constexpr auto& name_v<llm:OS::Phone::UI::Core::ICoreWindowKeyboardInput> = L"Windows.Phone.UI.Core.ICoreWindowKeyboardInput";
    template <> inline constexpr auto& name_v<llm:OS::Phone::UI::Core::IKeyboardInputBuffer> = L"Windows.Phone.UI.Core.IKeyboardInputBuffer";
    template <> inline constexpr guid guid_v<llm:OS::Phone::UI::Core::ICoreSelectionChangedEventArgs>{ 0xC2941949,0x3955,0x4EC2,{ 0x8F,0x88,0x3F,0x2B,0xFC,0x04,0xD0,0xF1 } }; // C2941949-3955-4EC2-8F88-3F2BFC04D0F1
    template <> inline constexpr guid guid_v<llm:OS::Phone::UI::Core::ICoreTextChangedEventArgs>{ 0x7E3C9572,0x60E4,0x4FEB,{ 0x86,0x83,0x4C,0xF0,0xE0,0xFA,0x76,0x59 } }; // 7E3C9572-60E4-4FEB-8683-4CF0E0FA7659
    template <> inline constexpr guid guid_v<llm:OS::Phone::UI::Core::ICoreWindowKeyboardInput>{ 0x196B175E,0x1A33,0x4B17,{ 0x99,0x35,0x33,0x8E,0xF6,0x44,0x34,0x77 } }; // 196B175E-1A33-4B17-9935-338EF6443477
    template <> inline constexpr guid guid_v<llm:OS::Phone::UI::Core::IKeyboardInputBuffer>{ 0x3776EE93,0x8079,0x40CC,{ 0x8C,0x2F,0xA7,0xA9,0x97,0xCF,0x9D,0x6E } }; // 3776EE93-8079-40CC-8C2F-A7A997CF9D6E
    template <> struct default_interface<llm:OS::Phone::UI::Core::CoreSelectionChangedEventArgs>{ using type = llm:OS::Phone::UI::Core::ICoreSelectionChangedEventArgs; };
    template <> struct default_interface<llm:OS::Phone::UI::Core::CoreTextChangedEventArgs>{ using type = llm:OS::Phone::UI::Core::ICoreTextChangedEventArgs; };
    template <> struct default_interface<llm:OS::Phone::UI::Core::KeyboardInputBuffer>{ using type = llm:OS::Phone::UI::Core::IKeyboardInputBuffer; };
    template <> struct abi<llm:OS::Phone::UI::Core::ICoreSelectionChangedEventArgs>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_Start(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_Length(uint32_t*) noexcept = 0;
        };
    };
    template <> struct abi<llm:OS::Phone::UI::Core::ICoreTextChangedEventArgs>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_Start(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_OldLength(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_NewText(void**) noexcept = 0;
        };
    };
    template <> struct abi<llm:OS::Phone::UI::Core::ICoreWindowKeyboardInput>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_IsKeyboardInputEnabled(bool*) noexcept = 0;
            virtual int32_t __stdcall put_IsKeyboardInputEnabled(bool) noexcept = 0;
            virtual int32_t __stdcall get_KeyboardInputBuffer(void**) noexcept = 0;
            virtual int32_t __stdcall put_KeyboardInputBuffer(void*) noexcept = 0;
        };
    };
    template <> struct abi<llm:OS::Phone::UI::Core::IKeyboardInputBuffer>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_Text(void**) noexcept = 0;
            virtual int32_t __stdcall put_Text(void*) noexcept = 0;
            virtual int32_t __stdcall get_SelectionStart(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_SelectionLength(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall Select(uint32_t, uint32_t) noexcept = 0;
            virtual int32_t __stdcall SelectFromTap(uint32_t) noexcept = 0;
            virtual int32_t __stdcall get_InputScope(int32_t*) noexcept = 0;
            virtual int32_t __stdcall put_InputScope(int32_t) noexcept = 0;
            virtual int32_t __stdcall add_TextChanged(void*, llm::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_TextChanged(llm::event_token) noexcept = 0;
            virtual int32_t __stdcall add_SelectionChanged(void*, llm::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_SelectionChanged(llm::event_token) noexcept = 0;
        };
    };
    template <typename D>
    struct consume_Windows_Phone_UI_Core_ICoreSelectionChangedEventArgs
    {
        [[nodiscard]] LLM_IMPL_AUTO(uint32_t) Start() const;
        [[nodiscard]] LLM_IMPL_AUTO(uint32_t) Length() const;
    };
    template <> struct consume<llm:OS::Phone::UI::Core::ICoreSelectionChangedEventArgs>
    {
        template <typename D> using type = consume_Windows_Phone_UI_Core_ICoreSelectionChangedEventArgs<D>;
    };
    template <typename D>
    struct consume_Windows_Phone_UI_Core_ICoreTextChangedEventArgs
    {
        [[nodiscard]] LLM_IMPL_AUTO(uint32_t) Start() const;
        [[nodiscard]] LLM_IMPL_AUTO(uint32_t) OldLength() const;
        [[nodiscard]] LLM_IMPL_AUTO(hstring) NewText() const;
    };
    template <> struct consume<llm:OS::Phone::UI::Core::ICoreTextChangedEventArgs>
    {
        template <typename D> using type = consume_Windows_Phone_UI_Core_ICoreTextChangedEventArgs<D>;
    };
    template <typename D>
    struct consume_Windows_Phone_UI_Core_ICoreWindowKeyboardInput
    {
        [[nodiscard]] LLM_IMPL_AUTO(bool) IsKeyboardInputEnabled() const;
        LLM_IMPL_AUTO(void) IsKeyboardInputEnabled(bool value) const;
        [[nodiscard]] LLM_IMPL_AUTO(llm:OS::Phone::UI::Core::KeyboardInputBuffer) KeyboardInputBuffer() const;
        LLM_IMPL_AUTO(void) KeyboardInputBuffer(llm:OS::Phone::UI::Core::KeyboardInputBuffer const& value) const;
    };
    template <> struct consume<llm:OS::Phone::UI::Core::ICoreWindowKeyboardInput>
    {
        template <typename D> using type = consume_Windows_Phone_UI_Core_ICoreWindowKeyboardInput<D>;
    };
    template <typename D>
    struct consume_Windows_Phone_UI_Core_IKeyboardInputBuffer
    {
        [[nodiscard]] LLM_IMPL_AUTO(hstring) Text() const;
        LLM_IMPL_AUTO(void) Text(param::hstring const& value) const;
        [[nodiscard]] LLM_IMPL_AUTO(uint32_t) SelectionStart() const;
        [[nodiscard]] LLM_IMPL_AUTO(uint32_t) SelectionLength() const;
        LLM_IMPL_AUTO(void) Select(uint32_t start, uint32_t length) const;
        LLM_IMPL_AUTO(void) SelectFromTap(uint32_t characterIndex) const;
        [[nodiscard]] LLM_IMPL_AUTO(llm:OS::Phone::UI::Core::CoreInputScope) InputScope() const;
        LLM_IMPL_AUTO(void) InputScope(llm:OS::Phone::UI::Core::CoreInputScope const& value) const;
        LLM_IMPL_AUTO(llm::event_token) TextChanged(llm:OS::Foundation::TypedEventHandler<llm:OS::Phone::UI::Core::KeyboardInputBuffer, llm:OS::Phone::UI::Core::CoreTextChangedEventArgs> const& handler) const;
        using TextChanged_revoker = impl::event_revoker<llm:OS::Phone::UI::Core::IKeyboardInputBuffer, &impl::abi_t<llm:OS::Phone::UI::Core::IKeyboardInputBuffer>::remove_TextChanged>;
        [[nodiscard]] TextChanged_revoker TextChanged(auto_revoke_t, llm:OS::Foundation::TypedEventHandler<llm:OS::Phone::UI::Core::KeyboardInputBuffer, llm:OS::Phone::UI::Core::CoreTextChangedEventArgs> const& handler) const;
        LLM_IMPL_AUTO(void) TextChanged(llm::event_token const& cookie) const noexcept;
        LLM_IMPL_AUTO(llm::event_token) SelectionChanged(llm:OS::Foundation::TypedEventHandler<llm:OS::Phone::UI::Core::KeyboardInputBuffer, llm:OS::Phone::UI::Core::CoreSelectionChangedEventArgs> const& handler) const;
        using SelectionChanged_revoker = impl::event_revoker<llm:OS::Phone::UI::Core::IKeyboardInputBuffer, &impl::abi_t<llm:OS::Phone::UI::Core::IKeyboardInputBuffer>::remove_SelectionChanged>;
        [[nodiscard]] SelectionChanged_revoker SelectionChanged(auto_revoke_t, llm:OS::Foundation::TypedEventHandler<llm:OS::Phone::UI::Core::KeyboardInputBuffer, llm:OS::Phone::UI::Core::CoreSelectionChangedEventArgs> const& handler) const;
        LLM_IMPL_AUTO(void) SelectionChanged(llm::event_token const& cookie) const noexcept;
    };
    template <> struct consume<llm:OS::Phone::UI::Core::IKeyboardInputBuffer>
    {
        template <typename D> using type = consume_Windows_Phone_UI_Core_IKeyboardInputBuffer<D>;
    };
}
#endif
