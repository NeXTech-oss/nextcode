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
#ifndef LLM_OS_Devices_Input_0_H
#define LLM_OS_Devices_Input_0_H
LLM_EXPORT namespace llm::OS::Devices::Haptics
{
    struct SimpleHapticsController;
}
LLM_EXPORT namespace llm::OS::Foundation
{
    struct EventRegistrationToken;
    struct Rect;
    template <typename TSender, typename TResult> struct __declspec(empty_bases) TypedEventHandler;
}
LLM_EXPORT namespace llm::OS::Foundation::Collections
{
    template <typename T> struct __declspec(empty_bases) IVectorView;
}
LLM_EXPORT namespace llm::OS::Devices::Input
{
    enum class PointerDeviceType : int32_t
    {
        Touch = 0,
        Pen = 1,
        Mouse = 2,
    };
    struct IKeyboardCapabilities;
    struct IMouseCapabilities;
    struct IMouseDevice;
    struct IMouseDeviceStatics;
    struct IMouseEventArgs;
    struct IPenButtonListener;
    struct IPenButtonListenerStatics;
    struct IPenDevice;
    struct IPenDevice2;
    struct IPenDeviceStatics;
    struct IPenDockListener;
    struct IPenDockListenerStatics;
    struct IPenDockedEventArgs;
    struct IPenTailButtonClickedEventArgs;
    struct IPenTailButtonDoubleClickedEventArgs;
    struct IPenTailButtonLongPressedEventArgs;
    struct IPenUndockedEventArgs;
    struct IPointerDevice;
    struct IPointerDevice2;
    struct IPointerDeviceStatics;
    struct ITouchCapabilities;
    struct KeyboardCapabilities;
    struct MouseCapabilities;
    struct MouseDevice;
    struct MouseEventArgs;
    struct PenButtonListener;
    struct PenDevice;
    struct PenDockListener;
    struct PenDockedEventArgs;
    struct PenTailButtonClickedEventArgs;
    struct PenTailButtonDoubleClickedEventArgs;
    struct PenTailButtonLongPressedEventArgs;
    struct PenUndockedEventArgs;
    struct PointerDevice;
    struct TouchCapabilities;
    struct MouseDelta;
    struct PointerDeviceUsage;
}
namespace llm::impl
{
    template <> struct category<llm::OS::Devices::Input::IKeyboardCapabilities>{ using type = interface_category; };
    template <> struct category<llm::OS::Devices::Input::IMouseCapabilities>{ using type = interface_category; };
    template <> struct category<llm::OS::Devices::Input::IMouseDevice>{ using type = interface_category; };
    template <> struct category<llm::OS::Devices::Input::IMouseDeviceStatics>{ using type = interface_category; };
    template <> struct category<llm::OS::Devices::Input::IMouseEventArgs>{ using type = interface_category; };
    template <> struct category<llm::OS::Devices::Input::IPenButtonListener>{ using type = interface_category; };
    template <> struct category<llm::OS::Devices::Input::IPenButtonListenerStatics>{ using type = interface_category; };
    template <> struct category<llm::OS::Devices::Input::IPenDevice>{ using type = interface_category; };
    template <> struct category<llm::OS::Devices::Input::IPenDevice2>{ using type = interface_category; };
    template <> struct category<llm::OS::Devices::Input::IPenDeviceStatics>{ using type = interface_category; };
    template <> struct category<llm::OS::Devices::Input::IPenDockListener>{ using type = interface_category; };
    template <> struct category<llm::OS::Devices::Input::IPenDockListenerStatics>{ using type = interface_category; };
    template <> struct category<llm::OS::Devices::Input::IPenDockedEventArgs>{ using type = interface_category; };
    template <> struct category<llm::OS::Devices::Input::IPenTailButtonClickedEventArgs>{ using type = interface_category; };
    template <> struct category<llm::OS::Devices::Input::IPenTailButtonDoubleClickedEventArgs>{ using type = interface_category; };
    template <> struct category<llm::OS::Devices::Input::IPenTailButtonLongPressedEventArgs>{ using type = interface_category; };
    template <> struct category<llm::OS::Devices::Input::IPenUndockedEventArgs>{ using type = interface_category; };
    template <> struct category<llm::OS::Devices::Input::IPointerDevice>{ using type = interface_category; };
    template <> struct category<llm::OS::Devices::Input::IPointerDevice2>{ using type = interface_category; };
    template <> struct category<llm::OS::Devices::Input::IPointerDeviceStatics>{ using type = interface_category; };
    template <> struct category<llm::OS::Devices::Input::ITouchCapabilities>{ using type = interface_category; };
    template <> struct category<llm::OS::Devices::Input::KeyboardCapabilities>{ using type = class_category; };
    template <> struct category<llm::OS::Devices::Input::MouseCapabilities>{ using type = class_category; };
    template <> struct category<llm::OS::Devices::Input::MouseDevice>{ using type = class_category; };
    template <> struct category<llm::OS::Devices::Input::MouseEventArgs>{ using type = class_category; };
    template <> struct category<llm::OS::Devices::Input::PenButtonListener>{ using type = class_category; };
    template <> struct category<llm::OS::Devices::Input::PenDevice>{ using type = class_category; };
    template <> struct category<llm::OS::Devices::Input::PenDockListener>{ using type = class_category; };
    template <> struct category<llm::OS::Devices::Input::PenDockedEventArgs>{ using type = class_category; };
    template <> struct category<llm::OS::Devices::Input::PenTailButtonClickedEventArgs>{ using type = class_category; };
    template <> struct category<llm::OS::Devices::Input::PenTailButtonDoubleClickedEventArgs>{ using type = class_category; };
    template <> struct category<llm::OS::Devices::Input::PenTailButtonLongPressedEventArgs>{ using type = class_category; };
    template <> struct category<llm::OS::Devices::Input::PenUndockedEventArgs>{ using type = class_category; };
    template <> struct category<llm::OS::Devices::Input::PointerDevice>{ using type = class_category; };
    template <> struct category<llm::OS::Devices::Input::TouchCapabilities>{ using type = class_category; };
    template <> struct category<llm::OS::Devices::Input::PointerDeviceType>{ using type = enum_category; };
    template <> struct category<llm::OS::Devices::Input::MouseDelta>{ using type = struct_category<int32_t, int32_t>; };
    template <> struct category<llm::OS::Devices::Input::PointerDeviceUsage>{ using type = struct_category<uint32_t, uint32_t, int32_t, int32_t, int32_t, int32_t, uint32_t, float>; };
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::KeyboardCapabilities> = L"Windows.Devices.Input.KeyboardCapabilities";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::MouseCapabilities> = L"Windows.Devices.Input.MouseCapabilities";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::MouseDevice> = L"Windows.Devices.Input.MouseDevice";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::MouseEventArgs> = L"Windows.Devices.Input.MouseEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::PenButtonListener> = L"Windows.Devices.Input.PenButtonListener";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::PenDevice> = L"Windows.Devices.Input.PenDevice";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::PenDockListener> = L"Windows.Devices.Input.PenDockListener";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::PenDockedEventArgs> = L"Windows.Devices.Input.PenDockedEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::PenTailButtonClickedEventArgs> = L"Windows.Devices.Input.PenTailButtonClickedEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::PenTailButtonDoubleClickedEventArgs> = L"Windows.Devices.Input.PenTailButtonDoubleClickedEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::PenTailButtonLongPressedEventArgs> = L"Windows.Devices.Input.PenTailButtonLongPressedEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::PenUndockedEventArgs> = L"Windows.Devices.Input.PenUndockedEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::PointerDevice> = L"Windows.Devices.Input.PointerDevice";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::TouchCapabilities> = L"Windows.Devices.Input.TouchCapabilities";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::PointerDeviceType> = L"Windows.Devices.Input.PointerDeviceType";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::MouseDelta> = L"Windows.Devices.Input.MouseDelta";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::PointerDeviceUsage> = L"Windows.Devices.Input.PointerDeviceUsage";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::IKeyboardCapabilities> = L"Windows.Devices.Input.IKeyboardCapabilities";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::IMouseCapabilities> = L"Windows.Devices.Input.IMouseCapabilities";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::IMouseDevice> = L"Windows.Devices.Input.IMouseDevice";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::IMouseDeviceStatics> = L"Windows.Devices.Input.IMouseDeviceStatics";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::IMouseEventArgs> = L"Windows.Devices.Input.IMouseEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::IPenButtonListener> = L"Windows.Devices.Input.IPenButtonListener";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::IPenButtonListenerStatics> = L"Windows.Devices.Input.IPenButtonListenerStatics";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::IPenDevice> = L"Windows.Devices.Input.IPenDevice";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::IPenDevice2> = L"Windows.Devices.Input.IPenDevice2";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::IPenDeviceStatics> = L"Windows.Devices.Input.IPenDeviceStatics";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::IPenDockListener> = L"Windows.Devices.Input.IPenDockListener";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::IPenDockListenerStatics> = L"Windows.Devices.Input.IPenDockListenerStatics";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::IPenDockedEventArgs> = L"Windows.Devices.Input.IPenDockedEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::IPenTailButtonClickedEventArgs> = L"Windows.Devices.Input.IPenTailButtonClickedEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::IPenTailButtonDoubleClickedEventArgs> = L"Windows.Devices.Input.IPenTailButtonDoubleClickedEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::IPenTailButtonLongPressedEventArgs> = L"Windows.Devices.Input.IPenTailButtonLongPressedEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::IPenUndockedEventArgs> = L"Windows.Devices.Input.IPenUndockedEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::IPointerDevice> = L"Windows.Devices.Input.IPointerDevice";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::IPointerDevice2> = L"Windows.Devices.Input.IPointerDevice2";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::IPointerDeviceStatics> = L"Windows.Devices.Input.IPointerDeviceStatics";
    template <> inline constexpr auto& name_v<llm::OS::Devices::Input::ITouchCapabilities> = L"Windows.Devices.Input.ITouchCapabilities";
    template <> inline constexpr guid guid_v<llm::OS::Devices::Input::IKeyboardCapabilities>{ 0x3A3F9B56,0x6798,0x4BBC,{ 0x83,0x3E,0x0F,0x34,0xB1,0x7C,0x65,0xFF } }; // 3A3F9B56-6798-4BBC-833E-0F34B17C65FF
    template <> inline constexpr guid guid_v<llm::OS::Devices::Input::IMouseCapabilities>{ 0xBCA5E023,0x7DD9,0x4B6B,{ 0x9A,0x92,0x55,0xD4,0x3C,0xB3,0x8F,0x73 } }; // BCA5E023-7DD9-4B6B-9A92-55D43CB38F73
    template <> inline constexpr guid guid_v<llm::OS::Devices::Input::IMouseDevice>{ 0x88EDF458,0xF2C8,0x49F4,{ 0xBE,0x1F,0xC2,0x56,0xB3,0x88,0xBC,0x11 } }; // 88EDF458-F2C8-49F4-BE1F-C256B388BC11
    template <> inline constexpr guid guid_v<llm::OS::Devices::Input::IMouseDeviceStatics>{ 0x484A9045,0x6D70,0x49DB,{ 0x8E,0x68,0x46,0xFF,0xBD,0x17,0xD3,0x8D } }; // 484A9045-6D70-49DB-8E68-46FFBD17D38D
    template <> inline constexpr guid guid_v<llm::OS::Devices::Input::IMouseEventArgs>{ 0xF625AA5D,0x2354,0x4CC7,{ 0x92,0x30,0x96,0x94,0x1C,0x96,0x9F,0xDE } }; // F625AA5D-2354-4CC7-9230-96941C969FDE
    template <> inline constexpr guid guid_v<llm::OS::Devices::Input::IPenButtonListener>{ 0x8245C376,0x1EE3,0x53F7,{ 0xB1,0xF7,0x83,0x34,0xA1,0x6F,0x28,0x15 } }; // 8245C376-1EE3-53F7-B1F7-8334A16F2815
    template <> inline constexpr guid guid_v<llm::OS::Devices::Input::IPenButtonListenerStatics>{ 0x19A8A584,0x862F,0x5F69,{ 0xBF,0xEA,0x05,0xF6,0x58,0x4F,0x13,0x3F } }; // 19A8A584-862F-5F69-BFEA-05F6584F133F
    template <> inline constexpr guid guid_v<llm::OS::Devices::Input::IPenDevice>{ 0x31856EBA,0xA738,0x5A8C,{ 0xB8,0xF6,0xF9,0x7E,0xF6,0x8D,0x18,0xEF } }; // 31856EBA-A738-5A8C-B8F6-F97EF68D18EF
    template <> inline constexpr guid guid_v<llm::OS::Devices::Input::IPenDevice2>{ 0x0207D327,0x7FB8,0x5566,{ 0x8C,0x34,0xF8,0x34,0x20,0x37,0xB7,0xF9 } }; // 0207D327-7FB8-5566-8C34-F8342037B7F9
    template <> inline constexpr guid guid_v<llm::OS::Devices::Input::IPenDeviceStatics>{ 0x9DFBBE01,0x0966,0x5180,{ 0xBC,0xB4,0xB8,0x50,0x60,0xE3,0x94,0x79 } }; // 9DFBBE01-0966-5180-BCB4-B85060E39479
    template <> inline constexpr guid guid_v<llm::OS::Devices::Input::IPenDockListener>{ 0x759F4D90,0x1DC0,0x55CB,{ 0xAD,0x18,0xB9,0x10,0x14,0x56,0xF5,0x92 } }; // 759F4D90-1DC0-55CB-AD18-B9101456F592
    template <> inline constexpr guid guid_v<llm::OS::Devices::Input::IPenDockListenerStatics>{ 0xCAB75E9A,0x0016,0x5C72,{ 0x96,0x9E,0xA9,0x7E,0x11,0x99,0x2A,0x93 } }; // CAB75E9A-0016-5C72-969E-A97E11992A93
    template <> inline constexpr guid guid_v<llm::OS::Devices::Input::IPenDockedEventArgs>{ 0xFD4277C6,0xCA63,0x5D4E,{ 0x9E,0xD3,0xA2,0x8A,0x54,0x52,0x1C,0x8C } }; // FD4277C6-CA63-5D4E-9ED3-A28A54521C8C
    template <> inline constexpr guid guid_v<llm::OS::Devices::Input::IPenTailButtonClickedEventArgs>{ 0x5D2FB7B6,0x6AD3,0x5D3E,{ 0xAB,0x29,0x05,0xEA,0x24,0x10,0xE3,0x90 } }; // 5D2FB7B6-6AD3-5D3E-AB29-05EA2410E390
    template <> inline constexpr guid guid_v<llm::OS::Devices::Input::IPenTailButtonDoubleClickedEventArgs>{ 0x846321A2,0x618A,0x5478,{ 0xB0,0x4C,0xB3,0x58,0x23,0x1D,0xA4,0xA7 } }; // 846321A2-618A-5478-B04C-B358231DA4A7
    template <> inline constexpr guid guid_v<llm::OS::Devices::Input::IPenTailButtonLongPressedEventArgs>{ 0xF37C606E,0xC60A,0x5F42,{ 0xB8,0x18,0xA5,0x31,0x12,0x40,0x6C,0x13 } }; // F37C606E-C60A-5F42-B818-A53112406C13
    template <> inline constexpr guid guid_v<llm::OS::Devices::Input::IPenUndockedEventArgs>{ 0xCCD09150,0x261B,0x59E6,{ 0xA5,0xD4,0xC1,0x96,0x4C,0xD0,0x3F,0xEB } }; // CCD09150-261B-59E6-A5D4-C1964CD03FEB
    template <> inline constexpr guid guid_v<llm::OS::Devices::Input::IPointerDevice>{ 0x93C9BAFC,0xEBCB,0x467E,{ 0x82,0xC6,0x27,0x6F,0xEA,0xE3,0x6B,0x5A } }; // 93C9BAFC-EBCB-467E-82C6-276FEAE36B5A
    template <> inline constexpr guid guid_v<llm::OS::Devices::Input::IPointerDevice2>{ 0xF8A6D2A0,0xC484,0x489F,{ 0xAE,0x3E,0x30,0xD2,0xEE,0x1F,0xFD,0x3E } }; // F8A6D2A0-C484-489F-AE3E-30D2EE1FFD3E
    template <> inline constexpr guid guid_v<llm::OS::Devices::Input::IPointerDeviceStatics>{ 0xD8B89AA1,0xD1C6,0x416E,{ 0xBD,0x8D,0x57,0x90,0x91,0x4D,0xC5,0x63 } }; // D8B89AA1-D1C6-416E-BD8D-5790914DC563
    template <> inline constexpr guid guid_v<llm::OS::Devices::Input::ITouchCapabilities>{ 0x20DD55F9,0x13F1,0x46C8,{ 0x92,0x85,0x2C,0x05,0xFA,0x3E,0xDA,0x6F } }; // 20DD55F9-13F1-46C8-9285-2C05FA3EDA6F
    template <> struct default_interface<llm::OS::Devices::Input::KeyboardCapabilities>{ using type = llm::OS::Devices::Input::IKeyboardCapabilities; };
    template <> struct default_interface<llm::OS::Devices::Input::MouseCapabilities>{ using type = llm::OS::Devices::Input::IMouseCapabilities; };
    template <> struct default_interface<llm::OS::Devices::Input::MouseDevice>{ using type = llm::OS::Devices::Input::IMouseDevice; };
    template <> struct default_interface<llm::OS::Devices::Input::MouseEventArgs>{ using type = llm::OS::Devices::Input::IMouseEventArgs; };
    template <> struct default_interface<llm::OS::Devices::Input::PenButtonListener>{ using type = llm::OS::Devices::Input::IPenButtonListener; };
    template <> struct default_interface<llm::OS::Devices::Input::PenDevice>{ using type = llm::OS::Devices::Input::IPenDevice; };
    template <> struct default_interface<llm::OS::Devices::Input::PenDockListener>{ using type = llm::OS::Devices::Input::IPenDockListener; };
    template <> struct default_interface<llm::OS::Devices::Input::PenDockedEventArgs>{ using type = llm::OS::Devices::Input::IPenDockedEventArgs; };
    template <> struct default_interface<llm::OS::Devices::Input::PenTailButtonClickedEventArgs>{ using type = llm::OS::Devices::Input::IPenTailButtonClickedEventArgs; };
    template <> struct default_interface<llm::OS::Devices::Input::PenTailButtonDoubleClickedEventArgs>{ using type = llm::OS::Devices::Input::IPenTailButtonDoubleClickedEventArgs; };
    template <> struct default_interface<llm::OS::Devices::Input::PenTailButtonLongPressedEventArgs>{ using type = llm::OS::Devices::Input::IPenTailButtonLongPressedEventArgs; };
    template <> struct default_interface<llm::OS::Devices::Input::PenUndockedEventArgs>{ using type = llm::OS::Devices::Input::IPenUndockedEventArgs; };
    template <> struct default_interface<llm::OS::Devices::Input::PointerDevice>{ using type = llm::OS::Devices::Input::IPointerDevice; };
    template <> struct default_interface<llm::OS::Devices::Input::TouchCapabilities>{ using type = llm::OS::Devices::Input::ITouchCapabilities; };
    template <> struct abi<llm::OS::Devices::Input::IKeyboardCapabilities>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_KeyboardPresent(int32_t*) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::Devices::Input::IMouseCapabilities>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_MousePresent(int32_t*) noexcept = 0;
            virtual int32_t __stdcall get_VerticalWheelPresent(int32_t*) noexcept = 0;
            virtual int32_t __stdcall get_HorizontalWheelPresent(int32_t*) noexcept = 0;
            virtual int32_t __stdcall get_SwapButtons(int32_t*) noexcept = 0;
            virtual int32_t __stdcall get_NumberOfButtons(uint32_t*) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::Devices::Input::IMouseDevice>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall add_MouseMoved(void*, llm::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_MouseMoved(llm::event_token) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::Devices::Input::IMouseDeviceStatics>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall GetForCurrentView(void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::Devices::Input::IMouseEventArgs>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_MouseDelta(struct struct_Windows_Devices_Input_MouseDelta*) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::Devices::Input::IPenButtonListener>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall IsSupported(bool*) noexcept = 0;
            virtual int32_t __stdcall add_IsSupportedChanged(void*, llm::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_IsSupportedChanged(llm::event_token) noexcept = 0;
            virtual int32_t __stdcall add_TailButtonClicked(void*, llm::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_TailButtonClicked(llm::event_token) noexcept = 0;
            virtual int32_t __stdcall add_TailButtonDoubleClicked(void*, llm::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_TailButtonDoubleClicked(llm::event_token) noexcept = 0;
            virtual int32_t __stdcall add_TailButtonLongPressed(void*, llm::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_TailButtonLongPressed(llm::event_token) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::Devices::Input::IPenButtonListenerStatics>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall GetDefault(void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::Devices::Input::IPenDevice>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_PenId(llm::guid*) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::Devices::Input::IPenDevice2>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_SimpleHapticsController(void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::Devices::Input::IPenDeviceStatics>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall GetFromPointerId(uint32_t, void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::Devices::Input::IPenDockListener>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall IsSupported(bool*) noexcept = 0;
            virtual int32_t __stdcall add_IsSupportedChanged(void*, llm::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_IsSupportedChanged(llm::event_token) noexcept = 0;
            virtual int32_t __stdcall add_Docked(void*, llm::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_Docked(llm::event_token) noexcept = 0;
            virtual int32_t __stdcall add_Undocked(void*, llm::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_Undocked(llm::event_token) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::Devices::Input::IPenDockListenerStatics>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall GetDefault(void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::Devices::Input::IPenDockedEventArgs>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
        };
    };
    template <> struct abi<llm::OS::Devices::Input::IPenTailButtonClickedEventArgs>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
        };
    };
    template <> struct abi<llm::OS::Devices::Input::IPenTailButtonDoubleClickedEventArgs>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
        };
    };
    template <> struct abi<llm::OS::Devices::Input::IPenTailButtonLongPressedEventArgs>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
        };
    };
    template <> struct abi<llm::OS::Devices::Input::IPenUndockedEventArgs>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
        };
    };
    template <> struct abi<llm::OS::Devices::Input::IPointerDevice>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_PointerDeviceType(int32_t*) noexcept = 0;
            virtual int32_t __stdcall get_IsIntegrated(bool*) noexcept = 0;
            virtual int32_t __stdcall get_MaxContacts(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_PhysicalDeviceRect(llm::OS::Foundation::Rect*) noexcept = 0;
            virtual int32_t __stdcall get_ScreenRect(llm::OS::Foundation::Rect*) noexcept = 0;
            virtual int32_t __stdcall get_SupportedUsages(void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::Devices::Input::IPointerDevice2>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_MaxPointersWithZDistance(uint32_t*) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::Devices::Input::IPointerDeviceStatics>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall GetPointerDevice(uint32_t, void**) noexcept = 0;
            virtual int32_t __stdcall GetPointerDevices(void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::Devices::Input::ITouchCapabilities>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_TouchPresent(int32_t*) noexcept = 0;
            virtual int32_t __stdcall get_Contacts(uint32_t*) noexcept = 0;
        };
    };
    template <typename D>
    struct consume_Windows_Devices_Input_IKeyboardCapabilities
    {
        [[nodiscard]] LLM_IMPL_AUTO(int32_t) KeyboardPresent() const;
    };
    template <> struct consume<llm::OS::Devices::Input::IKeyboardCapabilities>
    {
        template <typename D> using type = consume_Windows_Devices_Input_IKeyboardCapabilities<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_Input_IMouseCapabilities
    {
        [[nodiscard]] LLM_IMPL_AUTO(int32_t) MousePresent() const;
        [[nodiscard]] LLM_IMPL_AUTO(int32_t) VerticalWheelPresent() const;
        [[nodiscard]] LLM_IMPL_AUTO(int32_t) HorizontalWheelPresent() const;
        [[nodiscard]] LLM_IMPL_AUTO(int32_t) SwapButtons() const;
        [[nodiscard]] LLM_IMPL_AUTO(uint32_t) NumberOfButtons() const;
    };
    template <> struct consume<llm::OS::Devices::Input::IMouseCapabilities>
    {
        template <typename D> using type = consume_Windows_Devices_Input_IMouseCapabilities<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_Input_IMouseDevice
    {
        LLM_IMPL_AUTO(llm::event_token) MouseMoved(llm::OS::Foundation::TypedEventHandler<llm::OS::Devices::Input::MouseDevice, llm::OS::Devices::Input::MouseEventArgs> const& handler) const;
        using MouseMoved_revoker = impl::event_revoker<llm::OS::Devices::Input::IMouseDevice, &impl::abi_t<llm::OS::Devices::Input::IMouseDevice>::remove_MouseMoved>;
        [[nodiscard]] MouseMoved_revoker MouseMoved(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::Devices::Input::MouseDevice, llm::OS::Devices::Input::MouseEventArgs> const& handler) const;
        LLM_IMPL_AUTO(void) MouseMoved(llm::event_token const& cookie) const noexcept;
    };
    template <> struct consume<llm::OS::Devices::Input::IMouseDevice>
    {
        template <typename D> using type = consume_Windows_Devices_Input_IMouseDevice<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_Input_IMouseDeviceStatics
    {
        LLM_IMPL_AUTO(llm::OS::Devices::Input::MouseDevice) GetForCurrentView() const;
    };
    template <> struct consume<llm::OS::Devices::Input::IMouseDeviceStatics>
    {
        template <typename D> using type = consume_Windows_Devices_Input_IMouseDeviceStatics<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_Input_IMouseEventArgs
    {
        [[nodiscard]] LLM_IMPL_AUTO(llm::OS::Devices::Input::MouseDelta) MouseDelta() const;
    };
    template <> struct consume<llm::OS::Devices::Input::IMouseEventArgs>
    {
        template <typename D> using type = consume_Windows_Devices_Input_IMouseEventArgs<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_Input_IPenButtonListener
    {
        LLM_IMPL_AUTO(bool) IsSupported() const;
        LLM_IMPL_AUTO(llm::event_token) IsSupportedChanged(llm::OS::Foundation::TypedEventHandler<llm::OS::Devices::Input::PenButtonListener, llm::OS::Foundation::IInspectable> const& handler) const;
        using IsSupportedChanged_revoker = impl::event_revoker<llm::OS::Devices::Input::IPenButtonListener, &impl::abi_t<llm::OS::Devices::Input::IPenButtonListener>::remove_IsSupportedChanged>;
        [[nodiscard]] IsSupportedChanged_revoker IsSupportedChanged(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::Devices::Input::PenButtonListener, llm::OS::Foundation::IInspectable> const& handler) const;
        LLM_IMPL_AUTO(void) IsSupportedChanged(llm::event_token const& token) const noexcept;
        LLM_IMPL_AUTO(llm::event_token) TailButtonClicked(llm::OS::Foundation::TypedEventHandler<llm::OS::Devices::Input::PenButtonListener, llm::OS::Devices::Input::PenTailButtonClickedEventArgs> const& handler) const;
        using TailButtonClicked_revoker = impl::event_revoker<llm::OS::Devices::Input::IPenButtonListener, &impl::abi_t<llm::OS::Devices::Input::IPenButtonListener>::remove_TailButtonClicked>;
        [[nodiscard]] TailButtonClicked_revoker TailButtonClicked(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::Devices::Input::PenButtonListener, llm::OS::Devices::Input::PenTailButtonClickedEventArgs> const& handler) const;
        LLM_IMPL_AUTO(void) TailButtonClicked(llm::event_token const& token) const noexcept;
        LLM_IMPL_AUTO(llm::event_token) TailButtonDoubleClicked(llm::OS::Foundation::TypedEventHandler<llm::OS::Devices::Input::PenButtonListener, llm::OS::Devices::Input::PenTailButtonDoubleClickedEventArgs> const& handler) const;
        using TailButtonDoubleClicked_revoker = impl::event_revoker<llm::OS::Devices::Input::IPenButtonListener, &impl::abi_t<llm::OS::Devices::Input::IPenButtonListener>::remove_TailButtonDoubleClicked>;
        [[nodiscard]] TailButtonDoubleClicked_revoker TailButtonDoubleClicked(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::Devices::Input::PenButtonListener, llm::OS::Devices::Input::PenTailButtonDoubleClickedEventArgs> const& handler) const;
        LLM_IMPL_AUTO(void) TailButtonDoubleClicked(llm::event_token const& token) const noexcept;
        LLM_IMPL_AUTO(llm::event_token) TailButtonLongPressed(llm::OS::Foundation::TypedEventHandler<llm::OS::Devices::Input::PenButtonListener, llm::OS::Devices::Input::PenTailButtonLongPressedEventArgs> const& handler) const;
        using TailButtonLongPressed_revoker = impl::event_revoker<llm::OS::Devices::Input::IPenButtonListener, &impl::abi_t<llm::OS::Devices::Input::IPenButtonListener>::remove_TailButtonLongPressed>;
        [[nodiscard]] TailButtonLongPressed_revoker TailButtonLongPressed(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::Devices::Input::PenButtonListener, llm::OS::Devices::Input::PenTailButtonLongPressedEventArgs> const& handler) const;
        LLM_IMPL_AUTO(void) TailButtonLongPressed(llm::event_token const& token) const noexcept;
    };
    template <> struct consume<llm::OS::Devices::Input::IPenButtonListener>
    {
        template <typename D> using type = consume_Windows_Devices_Input_IPenButtonListener<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_Input_IPenButtonListenerStatics
    {
        LLM_IMPL_AUTO(llm::OS::Devices::Input::PenButtonListener) GetDefault() const;
    };
    template <> struct consume<llm::OS::Devices::Input::IPenButtonListenerStatics>
    {
        template <typename D> using type = consume_Windows_Devices_Input_IPenButtonListenerStatics<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_Input_IPenDevice
    {
        [[nodiscard]] LLM_IMPL_AUTO(llm::guid) PenId() const;
    };
    template <> struct consume<llm::OS::Devices::Input::IPenDevice>
    {
        template <typename D> using type = consume_Windows_Devices_Input_IPenDevice<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_Input_IPenDevice2
    {
        [[nodiscard]] LLM_IMPL_AUTO(llm::OS::Devices::Haptics::SimpleHapticsController) SimpleHapticsController() const;
    };
    template <> struct consume<llm::OS::Devices::Input::IPenDevice2>
    {
        template <typename D> using type = consume_Windows_Devices_Input_IPenDevice2<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_Input_IPenDeviceStatics
    {
        LLM_IMPL_AUTO(llm::OS::Devices::Input::PenDevice) GetFromPointerId(uint32_t pointerId) const;
    };
    template <> struct consume<llm::OS::Devices::Input::IPenDeviceStatics>
    {
        template <typename D> using type = consume_Windows_Devices_Input_IPenDeviceStatics<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_Input_IPenDockListener
    {
        LLM_IMPL_AUTO(bool) IsSupported() const;
        LLM_IMPL_AUTO(llm::event_token) IsSupportedChanged(llm::OS::Foundation::TypedEventHandler<llm::OS::Devices::Input::PenDockListener, llm::OS::Foundation::IInspectable> const& handler) const;
        using IsSupportedChanged_revoker = impl::event_revoker<llm::OS::Devices::Input::IPenDockListener, &impl::abi_t<llm::OS::Devices::Input::IPenDockListener>::remove_IsSupportedChanged>;
        [[nodiscard]] IsSupportedChanged_revoker IsSupportedChanged(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::Devices::Input::PenDockListener, llm::OS::Foundation::IInspectable> const& handler) const;
        LLM_IMPL_AUTO(void) IsSupportedChanged(llm::event_token const& token) const noexcept;
        LLM_IMPL_AUTO(llm::event_token) Docked(llm::OS::Foundation::TypedEventHandler<llm::OS::Devices::Input::PenDockListener, llm::OS::Devices::Input::PenDockedEventArgs> const& handler) const;
        using Docked_revoker = impl::event_revoker<llm::OS::Devices::Input::IPenDockListener, &impl::abi_t<llm::OS::Devices::Input::IPenDockListener>::remove_Docked>;
        [[nodiscard]] Docked_revoker Docked(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::Devices::Input::PenDockListener, llm::OS::Devices::Input::PenDockedEventArgs> const& handler) const;
        LLM_IMPL_AUTO(void) Docked(llm::event_token const& token) const noexcept;
        LLM_IMPL_AUTO(llm::event_token) Undocked(llm::OS::Foundation::TypedEventHandler<llm::OS::Devices::Input::PenDockListener, llm::OS::Devices::Input::PenUndockedEventArgs> const& handler) const;
        using Undocked_revoker = impl::event_revoker<llm::OS::Devices::Input::IPenDockListener, &impl::abi_t<llm::OS::Devices::Input::IPenDockListener>::remove_Undocked>;
        [[nodiscard]] Undocked_revoker Undocked(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::Devices::Input::PenDockListener, llm::OS::Devices::Input::PenUndockedEventArgs> const& handler) const;
        LLM_IMPL_AUTO(void) Undocked(llm::event_token const& token) const noexcept;
    };
    template <> struct consume<llm::OS::Devices::Input::IPenDockListener>
    {
        template <typename D> using type = consume_Windows_Devices_Input_IPenDockListener<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_Input_IPenDockListenerStatics
    {
        LLM_IMPL_AUTO(llm::OS::Devices::Input::PenDockListener) GetDefault() const;
    };
    template <> struct consume<llm::OS::Devices::Input::IPenDockListenerStatics>
    {
        template <typename D> using type = consume_Windows_Devices_Input_IPenDockListenerStatics<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_Input_IPenDockedEventArgs
    {
    };
    template <> struct consume<llm::OS::Devices::Input::IPenDockedEventArgs>
    {
        template <typename D> using type = consume_Windows_Devices_Input_IPenDockedEventArgs<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_Input_IPenTailButtonClickedEventArgs
    {
    };
    template <> struct consume<llm::OS::Devices::Input::IPenTailButtonClickedEventArgs>
    {
        template <typename D> using type = consume_Windows_Devices_Input_IPenTailButtonClickedEventArgs<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_Input_IPenTailButtonDoubleClickedEventArgs
    {
    };
    template <> struct consume<llm::OS::Devices::Input::IPenTailButtonDoubleClickedEventArgs>
    {
        template <typename D> using type = consume_Windows_Devices_Input_IPenTailButtonDoubleClickedEventArgs<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_Input_IPenTailButtonLongPressedEventArgs
    {
    };
    template <> struct consume<llm::OS::Devices::Input::IPenTailButtonLongPressedEventArgs>
    {
        template <typename D> using type = consume_Windows_Devices_Input_IPenTailButtonLongPressedEventArgs<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_Input_IPenUndockedEventArgs
    {
    };
    template <> struct consume<llm::OS::Devices::Input::IPenUndockedEventArgs>
    {
        template <typename D> using type = consume_Windows_Devices_Input_IPenUndockedEventArgs<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_Input_IPointerDevice
    {
        [[nodiscard]] LLM_IMPL_AUTO(llm::OS::Devices::Input::PointerDeviceType) PointerDeviceType() const;
        [[nodiscard]] LLM_IMPL_AUTO(bool) IsIntegrated() const;
        [[nodiscard]] LLM_IMPL_AUTO(uint32_t) MaxContacts() const;
        [[nodiscard]] LLM_IMPL_AUTO(llm::OS::Foundation::Rect) PhysicalDeviceRect() const;
        [[nodiscard]] LLM_IMPL_AUTO(llm::OS::Foundation::Rect) ScreenRect() const;
        [[nodiscard]] LLM_IMPL_AUTO(llm::OS::Foundation::Collections::IVectorView<llm::OS::Devices::Input::PointerDeviceUsage>) SupportedUsages() const;
    };
    template <> struct consume<llm::OS::Devices::Input::IPointerDevice>
    {
        template <typename D> using type = consume_Windows_Devices_Input_IPointerDevice<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_Input_IPointerDevice2
    {
        [[nodiscard]] LLM_IMPL_AUTO(uint32_t) MaxPointersWithZDistance() const;
    };
    template <> struct consume<llm::OS::Devices::Input::IPointerDevice2>
    {
        template <typename D> using type = consume_Windows_Devices_Input_IPointerDevice2<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_Input_IPointerDeviceStatics
    {
        LLM_IMPL_AUTO(llm::OS::Devices::Input::PointerDevice) GetPointerDevice(uint32_t pointerId) const;
        LLM_IMPL_AUTO(llm::OS::Foundation::Collections::IVectorView<llm::OS::Devices::Input::PointerDevice>) GetPointerDevices() const;
    };
    template <> struct consume<llm::OS::Devices::Input::IPointerDeviceStatics>
    {
        template <typename D> using type = consume_Windows_Devices_Input_IPointerDeviceStatics<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_Input_ITouchCapabilities
    {
        [[nodiscard]] LLM_IMPL_AUTO(int32_t) TouchPresent() const;
        [[nodiscard]] LLM_IMPL_AUTO(uint32_t) Contacts() const;
    };
    template <> struct consume<llm::OS::Devices::Input::ITouchCapabilities>
    {
        template <typename D> using type = consume_Windows_Devices_Input_ITouchCapabilities<D>;
    };
    struct struct_Windows_Devices_Input_MouseDelta
    {
        int32_t X;
        int32_t Y;
    };
    template <> struct abi<Windows::Devices::Input::MouseDelta>
    {
        using type = struct_Windows_Devices_Input_MouseDelta;
    };
    struct struct_Windows_Devices_Input_PointerDeviceUsage
    {
        uint32_t UsagePage;
        uint32_t Usage;
        int32_t MinLogical;
        int32_t MaxLogical;
        int32_t MinPhysical;
        int32_t MaxPhysical;
        uint32_t Unit;
        float PhysicalMultiplier;
    };
    template <> struct abi<Windows::Devices::Input::PointerDeviceUsage>
    {
        using type = struct_Windows_Devices_Input_PointerDeviceUsage;
    };
}
#endif
