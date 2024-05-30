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
#ifndef LLM_OS_UI_Xaml_Navigation_0_H
#define LLM_OS_UI_Xaml_Navigation_0_H
LLM_EXPORT namespace llm::OS::Foundation
{
    struct HResult;
    struct Uri;
}
LLM_EXPORT namespace llm::OS::UI::Xaml
{
    struct DependencyProperty;
}
LLM_EXPORT namespace llm::OS::UI::Xaml::Interop
{
    struct TypeName;
}
LLM_EXPORT namespace llm::OS::UI::Xaml::Media::Animation
{
    struct NavigationTransitionInfo;
}
LLM_EXPORT namespace llm::OS::UI::Xaml::Navigation
{
    enum class NavigationCacheMode : int32_t
    {
        Disabled = 0,
        Required = 1,
        Enabled = 2,
    };
    enum class NavigationMode : int32_t
    {
        New = 0,
        Back = 1,
        Forward = 2,
        Refresh = 3,
    };
    struct IFrameNavigationOptions;
    struct IFrameNavigationOptionsFactory;
    struct INavigatingCancelEventArgs;
    struct INavigatingCancelEventArgs2;
    struct INavigationEventArgs;
    struct INavigationEventArgs2;
    struct INavigationFailedEventArgs;
    struct IPageStackEntry;
    struct IPageStackEntryFactory;
    struct IPageStackEntryStatics;
    struct FrameNavigationOptions;
    struct NavigatingCancelEventArgs;
    struct NavigationEventArgs;
    struct NavigationFailedEventArgs;
    struct PageStackEntry;
    struct LoadCompletedEventHandler;
    struct NavigatedEventHandler;
    struct NavigatingCancelEventHandler;
    struct NavigationFailedEventHandler;
    struct NavigationStoppedEventHandler;
}
namespace llm::impl
{
    template <> struct category<llm::OS::UI::Xaml::Navigation::IFrameNavigationOptions>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Navigation::IFrameNavigationOptionsFactory>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Navigation::INavigatingCancelEventArgs>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Navigation::INavigatingCancelEventArgs2>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Navigation::INavigationEventArgs>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Navigation::INavigationEventArgs2>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Navigation::INavigationFailedEventArgs>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Navigation::IPageStackEntry>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Navigation::IPageStackEntryFactory>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Navigation::IPageStackEntryStatics>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Navigation::FrameNavigationOptions>{ using type = class_category; };
    template <> struct category<llm::OS::UI::Xaml::Navigation::NavigatingCancelEventArgs>{ using type = class_category; };
    template <> struct category<llm::OS::UI::Xaml::Navigation::NavigationEventArgs>{ using type = class_category; };
    template <> struct category<llm::OS::UI::Xaml::Navigation::NavigationFailedEventArgs>{ using type = class_category; };
    template <> struct category<llm::OS::UI::Xaml::Navigation::PageStackEntry>{ using type = class_category; };
    template <> struct category<llm::OS::UI::Xaml::Navigation::NavigationCacheMode>{ using type = enum_category; };
    template <> struct category<llm::OS::UI::Xaml::Navigation::NavigationMode>{ using type = enum_category; };
    template <> struct category<llm::OS::UI::Xaml::Navigation::LoadCompletedEventHandler>{ using type = delegate_category; };
    template <> struct category<llm::OS::UI::Xaml::Navigation::NavigatedEventHandler>{ using type = delegate_category; };
    template <> struct category<llm::OS::UI::Xaml::Navigation::NavigatingCancelEventHandler>{ using type = delegate_category; };
    template <> struct category<llm::OS::UI::Xaml::Navigation::NavigationFailedEventHandler>{ using type = delegate_category; };
    template <> struct category<llm::OS::UI::Xaml::Navigation::NavigationStoppedEventHandler>{ using type = delegate_category; };
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::FrameNavigationOptions> = L"Windows.UI.Xaml.Navigation.FrameNavigationOptions";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::NavigatingCancelEventArgs> = L"Windows.UI.Xaml.Navigation.NavigatingCancelEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::NavigationEventArgs> = L"Windows.UI.Xaml.Navigation.NavigationEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::NavigationFailedEventArgs> = L"Windows.UI.Xaml.Navigation.NavigationFailedEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::PageStackEntry> = L"Windows.UI.Xaml.Navigation.PageStackEntry";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::NavigationCacheMode> = L"Windows.UI.Xaml.Navigation.NavigationCacheMode";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::NavigationMode> = L"Windows.UI.Xaml.Navigation.NavigationMode";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::IFrameNavigationOptions> = L"Windows.UI.Xaml.Navigation.IFrameNavigationOptions";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::IFrameNavigationOptionsFactory> = L"Windows.UI.Xaml.Navigation.IFrameNavigationOptionsFactory";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::INavigatingCancelEventArgs> = L"Windows.UI.Xaml.Navigation.INavigatingCancelEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::INavigatingCancelEventArgs2> = L"Windows.UI.Xaml.Navigation.INavigatingCancelEventArgs2";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::INavigationEventArgs> = L"Windows.UI.Xaml.Navigation.INavigationEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::INavigationEventArgs2> = L"Windows.UI.Xaml.Navigation.INavigationEventArgs2";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::INavigationFailedEventArgs> = L"Windows.UI.Xaml.Navigation.INavigationFailedEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::IPageStackEntry> = L"Windows.UI.Xaml.Navigation.IPageStackEntry";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::IPageStackEntryFactory> = L"Windows.UI.Xaml.Navigation.IPageStackEntryFactory";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::IPageStackEntryStatics> = L"Windows.UI.Xaml.Navigation.IPageStackEntryStatics";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::LoadCompletedEventHandler> = L"Windows.UI.Xaml.Navigation.LoadCompletedEventHandler";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::NavigatedEventHandler> = L"Windows.UI.Xaml.Navigation.NavigatedEventHandler";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::NavigatingCancelEventHandler> = L"Windows.UI.Xaml.Navigation.NavigatingCancelEventHandler";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::NavigationFailedEventHandler> = L"Windows.UI.Xaml.Navigation.NavigationFailedEventHandler";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Navigation::NavigationStoppedEventHandler> = L"Windows.UI.Xaml.Navigation.NavigationStoppedEventHandler";
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Navigation::IFrameNavigationOptions>{ 0xB539AD2A,0x9FB7,0x520A,{ 0x8F,0x41,0x57,0xA5,0x0C,0x59,0xCF,0x92 } }; // B539AD2A-9FB7-520A-8F41-57A50C59CF92
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Navigation::IFrameNavigationOptionsFactory>{ 0xD4681E41,0x7E6D,0x5C7C,{ 0xAC,0xA0,0x47,0x86,0x81,0xCC,0x6F,0xCE } }; // D4681E41-7E6D-5C7C-ACA0-478681CC6FCE
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Navigation::INavigatingCancelEventArgs>{ 0xFD1D67AE,0xEAFB,0x4079,{ 0xBE,0x80,0x6D,0xC9,0x2A,0x03,0xAE,0xDF } }; // FD1D67AE-EAFB-4079-BE80-6DC92A03AEDF
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Navigation::INavigatingCancelEventArgs2>{ 0x5407B704,0x8147,0x4343,{ 0x83,0x8F,0xDD,0x1E,0xE9,0x08,0xC1,0x37 } }; // 5407B704-8147-4343-838F-DD1EE908C137
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Navigation::INavigationEventArgs>{ 0xB6AA9834,0x6691,0x44D1,{ 0xBD,0xF7,0x58,0x82,0x0C,0x27,0xB0,0xD0 } }; // B6AA9834-6691-44D1-BDF7-58820C27B0D0
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Navigation::INavigationEventArgs2>{ 0xDBFF71D9,0x979A,0x4B2E,{ 0xA4,0x9B,0x3B,0xB1,0x7F,0xDE,0xF5,0x74 } }; // DBFF71D9-979A-4B2E-A49B-3BB17FDEF574
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Navigation::INavigationFailedEventArgs>{ 0x11C1DFF7,0x36C2,0x4102,{ 0xB2,0xEF,0x02,0x17,0xA9,0x72,0x89,0xB3 } }; // 11C1DFF7-36C2-4102-B2EF-0217A97289B3
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Navigation::IPageStackEntry>{ 0xEF8814A6,0x9388,0x4ACA,{ 0x85,0x72,0x40,0x51,0x94,0x06,0x90,0x80 } }; // EF8814A6-9388-4ACA-8572-405194069080
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Navigation::IPageStackEntryFactory>{ 0x4454048A,0xA8B9,0x4F78,{ 0x9B,0x84,0x1F,0x51,0xF5,0x88,0x51,0xFF } }; // 4454048A-A8B9-4F78-9B84-1F51F58851FF
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Navigation::IPageStackEntryStatics>{ 0xACEFF8E3,0x246C,0x4033,{ 0x9F,0x01,0x01,0xCB,0x0D,0xA5,0x25,0x4E } }; // ACEFF8E3-246C-4033-9F01-01CB0DA5254E
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Navigation::LoadCompletedEventHandler>{ 0xAEBAF785,0x43FC,0x4E2C,{ 0x95,0xC3,0x97,0xAE,0x84,0xEA,0xBC,0x8E } }; // AEBAF785-43FC-4E2C-95C3-97AE84EABC8E
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Navigation::NavigatedEventHandler>{ 0x7BD1CF54,0x23CF,0x4CCE,{ 0xB2,0xF5,0x4C,0xE7,0x8D,0x96,0x89,0x6E } }; // 7BD1CF54-23CF-4CCE-B2F5-4CE78D96896E
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Navigation::NavigatingCancelEventHandler>{ 0x75D6A78F,0xA302,0x4489,{ 0x98,0x98,0x24,0xEA,0x49,0x18,0x29,0x10 } }; // 75D6A78F-A302-4489-9898-24EA49182910
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Navigation::NavigationFailedEventHandler>{ 0x4DAB4671,0x12B2,0x43C7,{ 0xB8,0x92,0x9B,0xE2,0xDC,0xD3,0xE8,0x8D } }; // 4DAB4671-12B2-43C7-B892-9BE2DCD3E88D
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Navigation::NavigationStoppedEventHandler>{ 0xF0117DDB,0x12FA,0x4D8D,{ 0x8B,0x26,0xB3,0x83,0xD0,0x9C,0x2B,0x3C } }; // F0117DDB-12FA-4D8D-8B26-B383D09C2B3C
    template <> struct default_interface<llm::OS::UI::Xaml::Navigation::FrameNavigationOptions>{ using type = llm::OS::UI::Xaml::Navigation::IFrameNavigationOptions; };
    template <> struct default_interface<llm::OS::UI::Xaml::Navigation::NavigatingCancelEventArgs>{ using type = llm::OS::UI::Xaml::Navigation::INavigatingCancelEventArgs; };
    template <> struct default_interface<llm::OS::UI::Xaml::Navigation::NavigationEventArgs>{ using type = llm::OS::UI::Xaml::Navigation::INavigationEventArgs; };
    template <> struct default_interface<llm::OS::UI::Xaml::Navigation::NavigationFailedEventArgs>{ using type = llm::OS::UI::Xaml::Navigation::INavigationFailedEventArgs; };
    template <> struct default_interface<llm::OS::UI::Xaml::Navigation::PageStackEntry>{ using type = llm::OS::UI::Xaml::Navigation::IPageStackEntry; };
    template <> struct abi<llm::OS::UI::Xaml::Navigation::IFrameNavigationOptions>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_IsNavigationStackEnabled(bool*) noexcept = 0;
            virtual int32_t __stdcall put_IsNavigationStackEnabled(bool) noexcept = 0;
            virtual int32_t __stdcall get_TransitionInfoOverride(void**) noexcept = 0;
            virtual int32_t __stdcall put_TransitionInfoOverride(void*) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Navigation::IFrameNavigationOptionsFactory>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall CreateInstance(void*, void**, void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Navigation::INavigatingCancelEventArgs>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_Cancel(bool*) noexcept = 0;
            virtual int32_t __stdcall put_Cancel(bool) noexcept = 0;
            virtual int32_t __stdcall get_NavigationMode(int32_t*) noexcept = 0;
            virtual int32_t __stdcall get_SourcePageType(struct struct_Windows_UI_Xaml_Interop_TypeName*) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Navigation::INavigatingCancelEventArgs2>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_Parameter(void**) noexcept = 0;
            virtual int32_t __stdcall get_NavigationTransitionInfo(void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Navigation::INavigationEventArgs>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_Content(void**) noexcept = 0;
            virtual int32_t __stdcall get_Parameter(void**) noexcept = 0;
            virtual int32_t __stdcall get_SourcePageType(struct struct_Windows_UI_Xaml_Interop_TypeName*) noexcept = 0;
            virtual int32_t __stdcall get_NavigationMode(int32_t*) noexcept = 0;
            virtual int32_t __stdcall get_Uri(void**) noexcept = 0;
            virtual int32_t __stdcall put_Uri(void*) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Navigation::INavigationEventArgs2>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_NavigationTransitionInfo(void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Navigation::INavigationFailedEventArgs>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_Exception(llm::hresult*) noexcept = 0;
            virtual int32_t __stdcall get_Handled(bool*) noexcept = 0;
            virtual int32_t __stdcall put_Handled(bool) noexcept = 0;
            virtual int32_t __stdcall get_SourcePageType(struct struct_Windows_UI_Xaml_Interop_TypeName*) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Navigation::IPageStackEntry>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_SourcePageType(struct struct_Windows_UI_Xaml_Interop_TypeName*) noexcept = 0;
            virtual int32_t __stdcall get_Parameter(void**) noexcept = 0;
            virtual int32_t __stdcall get_NavigationTransitionInfo(void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Navigation::IPageStackEntryFactory>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall CreateInstance(struct struct_Windows_UI_Xaml_Interop_TypeName, void*, void*, void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Navigation::IPageStackEntryStatics>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_SourcePageTypeProperty(void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Navigation::LoadCompletedEventHandler>
    {
        struct __declspec(novtable) type : unknown_abi
        {
            virtual int32_t __stdcall Invoke(void*, void*) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Navigation::NavigatedEventHandler>
    {
        struct __declspec(novtable) type : unknown_abi
        {
            virtual int32_t __stdcall Invoke(void*, void*) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Navigation::NavigatingCancelEventHandler>
    {
        struct __declspec(novtable) type : unknown_abi
        {
            virtual int32_t __stdcall Invoke(void*, void*) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Navigation::NavigationFailedEventHandler>
    {
        struct __declspec(novtable) type : unknown_abi
        {
            virtual int32_t __stdcall Invoke(void*, void*) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Navigation::NavigationStoppedEventHandler>
    {
        struct __declspec(novtable) type : unknown_abi
        {
            virtual int32_t __stdcall Invoke(void*, void*) noexcept = 0;
        };
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Navigation_IFrameNavigationOptions
    {
        [[nodiscard]] WINRT_IMPL_AUTO(bool) IsNavigationStackEnabled() const;
        WINRT_IMPL_AUTO(void) IsNavigationStackEnabled(bool value) const;
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Media::Animation::NavigationTransitionInfo) TransitionInfoOverride() const;
        WINRT_IMPL_AUTO(void) TransitionInfoOverride(llm::OS::UI::Xaml::Media::Animation::NavigationTransitionInfo const& value) const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Navigation::IFrameNavigationOptions>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Navigation_IFrameNavigationOptions<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Navigation_IFrameNavigationOptionsFactory
    {
        WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Navigation::FrameNavigationOptions) CreateInstance(llm::OS::Foundation::IInspectable const& baseInterface, llm::OS::Foundation::IInspectable& innerInterface) const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Navigation::IFrameNavigationOptionsFactory>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Navigation_IFrameNavigationOptionsFactory<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Navigation_INavigatingCancelEventArgs
    {
        [[nodiscard]] WINRT_IMPL_AUTO(bool) Cancel() const;
        WINRT_IMPL_AUTO(void) Cancel(bool value) const;
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Navigation::NavigationMode) NavigationMode() const;
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Interop::TypeName) SourcePageType() const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Navigation::INavigatingCancelEventArgs>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Navigation_INavigatingCancelEventArgs<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Navigation_INavigatingCancelEventArgs2
    {
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::Foundation::IInspectable) Parameter() const;
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Media::Animation::NavigationTransitionInfo) NavigationTransitionInfo() const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Navigation::INavigatingCancelEventArgs2>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Navigation_INavigatingCancelEventArgs2<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Navigation_INavigationEventArgs
    {
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::Foundation::IInspectable) Content() const;
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::Foundation::IInspectable) Parameter() const;
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Interop::TypeName) SourcePageType() const;
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Navigation::NavigationMode) NavigationMode() const;
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::Foundation::Uri) Uri() const;
        WINRT_IMPL_AUTO(void) Uri(llm::OS::Foundation::Uri const& value) const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Navigation::INavigationEventArgs>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Navigation_INavigationEventArgs<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Navigation_INavigationEventArgs2
    {
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Media::Animation::NavigationTransitionInfo) NavigationTransitionInfo() const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Navigation::INavigationEventArgs2>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Navigation_INavigationEventArgs2<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Navigation_INavigationFailedEventArgs
    {
        [[nodiscard]] WINRT_IMPL_AUTO(llm::hresult) Exception() const;
        [[nodiscard]] WINRT_IMPL_AUTO(bool) Handled() const;
        WINRT_IMPL_AUTO(void) Handled(bool value) const;
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Interop::TypeName) SourcePageType() const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Navigation::INavigationFailedEventArgs>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Navigation_INavigationFailedEventArgs<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Navigation_IPageStackEntry
    {
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Interop::TypeName) SourcePageType() const;
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::Foundation::IInspectable) Parameter() const;
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Media::Animation::NavigationTransitionInfo) NavigationTransitionInfo() const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Navigation::IPageStackEntry>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Navigation_IPageStackEntry<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Navigation_IPageStackEntryFactory
    {
        WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Navigation::PageStackEntry) CreateInstance(llm::OS::UI::Xaml::Interop::TypeName const& sourcePageType, llm::OS::Foundation::IInspectable const& parameter, llm::OS::UI::Xaml::Media::Animation::NavigationTransitionInfo const& navigationTransitionInfo) const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Navigation::IPageStackEntryFactory>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Navigation_IPageStackEntryFactory<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Navigation_IPageStackEntryStatics
    {
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::DependencyProperty) SourcePageTypeProperty() const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Navigation::IPageStackEntryStatics>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Navigation_IPageStackEntryStatics<D>;
    };
}
#endif
