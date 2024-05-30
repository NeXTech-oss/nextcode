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
#ifndef LLM_OS_UI_Xaml_Media_Imaging_0_H
#define LLM_OS_UI_Xaml_Media_Imaging_0_H
LLM_EXPORT namespace llm::OS::ApplicationModel::Background
{
    struct IBackgroundTaskInstance;
}
LLM_EXPORT namespace llm::OS::Foundation
{
    struct EventRegistrationToken;
    struct IAsyncAction;
    template <typename TResult> struct __declspec(empty_bases) IAsyncOperation;
    template <typename TSender, typename TResult> struct __declspec(empty_bases) TypedEventHandler;
    struct Uri;
}
LLM_EXPORT namespace llm::OS::Graphics::Imaging
{
    struct SoftwareBitmap;
}
LLM_EXPORT namespace llm::OS::Storage::Streams
{
    struct IBuffer;
    struct IRandomAccessStream;
}
LLM_EXPORT namespace llm::OS::UI::Xaml
{
    struct DependencyProperty;
    struct ExceptionRoutedEventHandler;
    struct RoutedEventHandler;
    struct UIElement;
}
LLM_EXPORT namespace llm::OS::UI::Xaml::Media::Imaging
{
    enum class BitmapCreateOptions : uint32_t
    {
        None = 0,
        IgnoreImageCache = 0x8,
    };
    enum class DecodePixelType : int32_t
    {
        Physical = 0,
        Logical = 1,
    };
    enum class SvgImageSourceLoadStatus : int32_t
    {
        Success = 0,
        NetworkError = 1,
        InvalidFormat = 2,
        Other = 3,
    };
    struct IBitmapImage;
    struct IBitmapImage2;
    struct IBitmapImage3;
    struct IBitmapImageFactory;
    struct IBitmapImageStatics;
    struct IBitmapImageStatics2;
    struct IBitmapImageStatics3;
    struct IBitmapSource;
    struct IBitmapSourceFactory;
    struct IBitmapSourceStatics;
    struct IDownloadProgressEventArgs;
    struct IRenderTargetBitmap;
    struct IRenderTargetBitmapStatics;
    struct ISoftwareBitmapSource;
    struct ISurfaceImageSource;
    struct ISurfaceImageSourceFactory;
    struct ISvgImageSource;
    struct ISvgImageSourceFactory;
    struct ISvgImageSourceFailedEventArgs;
    struct ISvgImageSourceOpenedEventArgs;
    struct ISvgImageSourceStatics;
    struct IVirtualSurfaceImageSource;
    struct IVirtualSurfaceImageSourceFactory;
    struct IWriteableBitmap;
    struct IWriteableBitmapFactory;
    struct IXamlRenderingBackgroundTask;
    struct IXamlRenderingBackgroundTaskFactory;
    struct IXamlRenderingBackgroundTaskOverrides;
    struct BitmapImage;
    struct BitmapSource;
    struct DownloadProgressEventArgs;
    struct RenderTargetBitmap;
    struct SoftwareBitmapSource;
    struct SurfaceImageSource;
    struct SvgImageSource;
    struct SvgImageSourceFailedEventArgs;
    struct SvgImageSourceOpenedEventArgs;
    struct VirtualSurfaceImageSource;
    struct WriteableBitmap;
    struct XamlRenderingBackgroundTask;
    struct DownloadProgressEventHandler;
}
namespace llm::impl
{
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::IBitmapImage>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::IBitmapImage2>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::IBitmapImage3>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::IBitmapImageFactory>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::IBitmapImageStatics>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::IBitmapImageStatics2>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::IBitmapImageStatics3>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::IBitmapSource>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::IBitmapSourceFactory>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::IBitmapSourceStatics>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::IDownloadProgressEventArgs>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::IRenderTargetBitmap>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::IRenderTargetBitmapStatics>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::ISoftwareBitmapSource>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::ISurfaceImageSource>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::ISurfaceImageSourceFactory>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSource>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceFactory>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceFailedEventArgs>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceOpenedEventArgs>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceStatics>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::IVirtualSurfaceImageSource>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::IVirtualSurfaceImageSourceFactory>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::IWriteableBitmap>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::IWriteableBitmapFactory>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::IXamlRenderingBackgroundTask>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::IXamlRenderingBackgroundTaskFactory>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::IXamlRenderingBackgroundTaskOverrides>{ using type = interface_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::BitmapImage>{ using type = class_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::BitmapSource>{ using type = class_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::DownloadProgressEventArgs>{ using type = class_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::RenderTargetBitmap>{ using type = class_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::SoftwareBitmapSource>{ using type = class_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::SurfaceImageSource>{ using type = class_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::SvgImageSource>{ using type = class_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::SvgImageSourceFailedEventArgs>{ using type = class_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::SvgImageSourceOpenedEventArgs>{ using type = class_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource>{ using type = class_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::WriteableBitmap>{ using type = class_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::XamlRenderingBackgroundTask>{ using type = class_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::BitmapCreateOptions>{ using type = enum_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::DecodePixelType>{ using type = enum_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::SvgImageSourceLoadStatus>{ using type = enum_category; };
    template <> struct category<llm::OS::UI::Xaml::Media::Imaging::DownloadProgressEventHandler>{ using type = delegate_category; };
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::BitmapImage> = L"Windows.UI.Xaml.Media.Imaging.BitmapImage";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::BitmapSource> = L"Windows.UI.Xaml.Media.Imaging.BitmapSource";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::DownloadProgressEventArgs> = L"Windows.UI.Xaml.Media.Imaging.DownloadProgressEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::RenderTargetBitmap> = L"Windows.UI.Xaml.Media.Imaging.RenderTargetBitmap";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::SoftwareBitmapSource> = L"Windows.UI.Xaml.Media.Imaging.SoftwareBitmapSource";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::SurfaceImageSource> = L"Windows.UI.Xaml.Media.Imaging.SurfaceImageSource";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::SvgImageSource> = L"Windows.UI.Xaml.Media.Imaging.SvgImageSource";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::SvgImageSourceFailedEventArgs> = L"Windows.UI.Xaml.Media.Imaging.SvgImageSourceFailedEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::SvgImageSourceOpenedEventArgs> = L"Windows.UI.Xaml.Media.Imaging.SvgImageSourceOpenedEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource> = L"Windows.UI.Xaml.Media.Imaging.VirtualSurfaceImageSource";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::WriteableBitmap> = L"Windows.UI.Xaml.Media.Imaging.WriteableBitmap";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::XamlRenderingBackgroundTask> = L"Windows.UI.Xaml.Media.Imaging.XamlRenderingBackgroundTask";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::BitmapCreateOptions> = L"Windows.UI.Xaml.Media.Imaging.BitmapCreateOptions";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::DecodePixelType> = L"Windows.UI.Xaml.Media.Imaging.DecodePixelType";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::SvgImageSourceLoadStatus> = L"Windows.UI.Xaml.Media.Imaging.SvgImageSourceLoadStatus";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::IBitmapImage> = L"Windows.UI.Xaml.Media.Imaging.IBitmapImage";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::IBitmapImage2> = L"Windows.UI.Xaml.Media.Imaging.IBitmapImage2";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::IBitmapImage3> = L"Windows.UI.Xaml.Media.Imaging.IBitmapImage3";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::IBitmapImageFactory> = L"Windows.UI.Xaml.Media.Imaging.IBitmapImageFactory";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::IBitmapImageStatics> = L"Windows.UI.Xaml.Media.Imaging.IBitmapImageStatics";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::IBitmapImageStatics2> = L"Windows.UI.Xaml.Media.Imaging.IBitmapImageStatics2";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::IBitmapImageStatics3> = L"Windows.UI.Xaml.Media.Imaging.IBitmapImageStatics3";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::IBitmapSource> = L"Windows.UI.Xaml.Media.Imaging.IBitmapSource";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::IBitmapSourceFactory> = L"Windows.UI.Xaml.Media.Imaging.IBitmapSourceFactory";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::IBitmapSourceStatics> = L"Windows.UI.Xaml.Media.Imaging.IBitmapSourceStatics";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::IDownloadProgressEventArgs> = L"Windows.UI.Xaml.Media.Imaging.IDownloadProgressEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::IRenderTargetBitmap> = L"Windows.UI.Xaml.Media.Imaging.IRenderTargetBitmap";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::IRenderTargetBitmapStatics> = L"Windows.UI.Xaml.Media.Imaging.IRenderTargetBitmapStatics";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::ISoftwareBitmapSource> = L"Windows.UI.Xaml.Media.Imaging.ISoftwareBitmapSource";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::ISurfaceImageSource> = L"Windows.UI.Xaml.Media.Imaging.ISurfaceImageSource";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::ISurfaceImageSourceFactory> = L"Windows.UI.Xaml.Media.Imaging.ISurfaceImageSourceFactory";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSource> = L"Windows.UI.Xaml.Media.Imaging.ISvgImageSource";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceFactory> = L"Windows.UI.Xaml.Media.Imaging.ISvgImageSourceFactory";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceFailedEventArgs> = L"Windows.UI.Xaml.Media.Imaging.ISvgImageSourceFailedEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceOpenedEventArgs> = L"Windows.UI.Xaml.Media.Imaging.ISvgImageSourceOpenedEventArgs";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceStatics> = L"Windows.UI.Xaml.Media.Imaging.ISvgImageSourceStatics";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::IVirtualSurfaceImageSource> = L"Windows.UI.Xaml.Media.Imaging.IVirtualSurfaceImageSource";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::IVirtualSurfaceImageSourceFactory> = L"Windows.UI.Xaml.Media.Imaging.IVirtualSurfaceImageSourceFactory";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::IWriteableBitmap> = L"Windows.UI.Xaml.Media.Imaging.IWriteableBitmap";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::IWriteableBitmapFactory> = L"Windows.UI.Xaml.Media.Imaging.IWriteableBitmapFactory";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::IXamlRenderingBackgroundTask> = L"Windows.UI.Xaml.Media.Imaging.IXamlRenderingBackgroundTask";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::IXamlRenderingBackgroundTaskFactory> = L"Windows.UI.Xaml.Media.Imaging.IXamlRenderingBackgroundTaskFactory";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::IXamlRenderingBackgroundTaskOverrides> = L"Windows.UI.Xaml.Media.Imaging.IXamlRenderingBackgroundTaskOverrides";
    template <> inline constexpr auto& name_v<llm::OS::UI::Xaml::Media::Imaging::DownloadProgressEventHandler> = L"Windows.UI.Xaml.Media.Imaging.DownloadProgressEventHandler";
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::IBitmapImage>{ 0x31AF3271,0xE3B4,0x442D,{ 0xA3,0x41,0x4C,0x02,0x26,0xB2,0x72,0x5B } }; // 31AF3271-E3B4-442D-A341-4C0226B2725B
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::IBitmapImage2>{ 0x1069C1B6,0x8C9B,0x4762,{ 0xBE,0x3D,0x75,0x9F,0x56,0x98,0xF2,0xB3 } }; // 1069C1B6-8C9B-4762-BE3D-759F5698F2B3
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::IBitmapImage3>{ 0xF1DE6F26,0x3C73,0x453F,{ 0xA7,0xBA,0x9B,0x85,0xC1,0x8B,0x37,0x33 } }; // F1DE6F26-3C73-453F-A7BA-9B85C18B3733
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::IBitmapImageFactory>{ 0xC9132978,0x4810,0x4E5E,{ 0x80,0x87,0x03,0x67,0x1E,0xE6,0x0D,0x85 } }; // C9132978-4810-4E5E-8087-03671EE60D85
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::IBitmapImageStatics>{ 0x9E282143,0x70E8,0x437C,{ 0x9F,0xA4,0x2C,0xBF,0x29,0x5C,0xFF,0x84 } }; // 9E282143-70E8-437C-9FA4-2CBF295CFF84
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::IBitmapImageStatics2>{ 0xC5F5576A,0x75AF,0x41A4,{ 0xB8,0x93,0x8F,0xE9,0x1F,0xEE,0x28,0x82 } }; // C5F5576A-75AF-41A4-B893-8FE91FEE2882
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::IBitmapImageStatics3>{ 0x2B44E30D,0xF6D5,0x4411,{ 0xA8,0xCD,0xBF,0x76,0x03,0xC4,0xFA,0xA0 } }; // 2B44E30D-F6D5-4411-A8CD-BF7603C4FAA0
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::IBitmapSource>{ 0x23D86411,0x202F,0x41B2,{ 0x8C,0x5B,0xA8,0xA3,0xB3,0x33,0x80,0x0B } }; // 23D86411-202F-41B2-8C5B-A8A3B333800B
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::IBitmapSourceFactory>{ 0xE240420E,0xD4A7,0x49A4,{ 0xA0,0xB4,0xA5,0x9F,0xDD,0x77,0xE5,0x08 } }; // E240420E-D4A7-49A4-A0B4-A59FDD77E508
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::IBitmapSourceStatics>{ 0x9A9C9981,0x827B,0x4E51,{ 0x89,0x1B,0x8A,0x15,0xB5,0x11,0x84,0x2D } }; // 9A9C9981-827B-4E51-891B-8A15B511842D
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::IDownloadProgressEventArgs>{ 0x7311E0D4,0xFE94,0x4E70,{ 0x9B,0x90,0xCD,0xD4,0x7A,0xC2,0x3A,0xFB } }; // 7311E0D4-FE94-4E70-9B90-CDD47AC23AFB
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::IRenderTargetBitmap>{ 0x500DEE81,0x893C,0x4C0A,{ 0x8F,0xEC,0x46,0x78,0xAC,0x71,0x75,0x89 } }; // 500DEE81-893C-4C0A-8FEC-4678AC717589
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::IRenderTargetBitmapStatics>{ 0xF0A1EFEE,0xC131,0x4D40,{ 0x9C,0x47,0xF7,0xD7,0xCF,0x2B,0x07,0x7F } }; // F0A1EFEE-C131-4D40-9C47-F7D7CF2B077F
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::ISoftwareBitmapSource>{ 0xD2DD9ED0,0xD3C5,0x4056,{ 0x91,0xB5,0xB7,0xC1,0xD1,0xE8,0x13,0x0E } }; // D2DD9ED0-D3C5-4056-91B5-B7C1D1E8130E
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::ISurfaceImageSource>{ 0x62F7D416,0xC714,0x4C4C,{ 0x82,0x73,0xF8,0x39,0xBC,0x58,0x13,0x5C } }; // 62F7D416-C714-4C4C-8273-F839BC58135C
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::ISurfaceImageSourceFactory>{ 0x3AB2212A,0xEF65,0x4A5F,{ 0xBF,0xAC,0x73,0x99,0x3E,0x8C,0x12,0xC9 } }; // 3AB2212A-EF65-4A5F-BFAC-73993E8C12C9
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSource>{ 0x03E1CEC3,0x0CA8,0x4A4E,{ 0x8D,0x7C,0xC8,0x08,0xA0,0x83,0x85,0x86 } }; // 03E1CEC3-0CA8-4A4E-8D7C-C808A0838586
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceFactory>{ 0xC794E9E7,0xCF23,0x4D72,{ 0xBF,0x1A,0xDF,0xAA,0x16,0xD8,0xEA,0x52 } }; // C794E9E7-CF23-4D72-BF1A-DFAA16D8EA52
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceFailedEventArgs>{ 0x68BB3170,0x3CCC,0x4035,{ 0xAC,0x01,0x98,0x34,0x54,0x3D,0x74,0x4E } }; // 68BB3170-3CCC-4035-AC01-9834543D744E
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceOpenedEventArgs>{ 0x85EF4C16,0x748E,0x4008,{ 0x95,0xC7,0x6A,0x23,0xDD,0x73,0x16,0xDB } }; // 85EF4C16-748E-4008-95C7-6A23DD7316DB
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceStatics>{ 0x9C6638CE,0xBED1,0x4AAB,{ 0xAC,0xBB,0xD3,0xE2,0x18,0x5D,0x31,0x5A } }; // 9C6638CE-BED1-4AAB-ACBB-D3E2185D315A
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::IVirtualSurfaceImageSource>{ 0x4A711FEA,0xBFAC,0x11E0,{ 0xA0,0x6A,0x9D,0xE4,0x47,0x24,0x01,0x9B } }; // 4A711FEA-BFAC-11E0-A06A-9DE44724019B
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::IVirtualSurfaceImageSourceFactory>{ 0x3AB2212A,0xBFAC,0x11E0,{ 0x8A,0x92,0x69,0xE4,0x47,0x24,0x01,0x9B } }; // 3AB2212A-BFAC-11E0-8A92-69E44724019B
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::IWriteableBitmap>{ 0xBF0B7E6F,0xDF7C,0x4A85,{ 0x84,0x13,0xA1,0x21,0x62,0x85,0x83,0x5C } }; // BF0B7E6F-DF7C-4A85-8413-A1216285835C
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::IWriteableBitmapFactory>{ 0x5563EBB1,0x3EF2,0x42C5,{ 0x9C,0x6D,0x1C,0xF5,0xDC,0xC0,0x41,0xFF } }; // 5563EBB1-3EF2-42C5-9C6D-1CF5DCC041FF
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::IXamlRenderingBackgroundTask>{ 0x5D5FE9AA,0x533E,0x44B8,{ 0xA9,0x75,0xFC,0x5F,0x1E,0x3B,0xFF,0x52 } }; // 5D5FE9AA-533E-44B8-A975-FC5F1E3BFF52
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::IXamlRenderingBackgroundTaskFactory>{ 0xA3D1BB63,0x38F8,0x4DA3,{ 0x9F,0xCA,0xFD,0x81,0x28,0xA2,0xCB,0xF9 } }; // A3D1BB63-38F8-4DA3-9FCA-FD8128A2CBF9
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::IXamlRenderingBackgroundTaskOverrides>{ 0x9C2A6997,0xA908,0x4711,{ 0xB4,0xB2,0xA9,0x60,0xDB,0x3D,0x8E,0x5A } }; // 9C2A6997-A908-4711-B4B2-A960DB3D8E5A
    template <> inline constexpr guid guid_v<llm::OS::UI::Xaml::Media::Imaging::DownloadProgressEventHandler>{ 0x1ABAEE23,0x74EE,0x4CC7,{ 0x99,0xBA,0xB1,0x71,0xE3,0xCD,0xA6,0x1E } }; // 1ABAEE23-74EE-4CC7-99BA-B171E3CDA61E
    template <> struct default_interface<llm::OS::UI::Xaml::Media::Imaging::BitmapImage>{ using type = llm::OS::UI::Xaml::Media::Imaging::IBitmapImage; };
    template <> struct default_interface<llm::OS::UI::Xaml::Media::Imaging::BitmapSource>{ using type = llm::OS::UI::Xaml::Media::Imaging::IBitmapSource; };
    template <> struct default_interface<llm::OS::UI::Xaml::Media::Imaging::DownloadProgressEventArgs>{ using type = llm::OS::UI::Xaml::Media::Imaging::IDownloadProgressEventArgs; };
    template <> struct default_interface<llm::OS::UI::Xaml::Media::Imaging::RenderTargetBitmap>{ using type = llm::OS::UI::Xaml::Media::Imaging::IRenderTargetBitmap; };
    template <> struct default_interface<llm::OS::UI::Xaml::Media::Imaging::SoftwareBitmapSource>{ using type = llm::OS::UI::Xaml::Media::Imaging::ISoftwareBitmapSource; };
    template <> struct default_interface<llm::OS::UI::Xaml::Media::Imaging::SurfaceImageSource>{ using type = llm::OS::UI::Xaml::Media::Imaging::ISurfaceImageSource; };
    template <> struct default_interface<llm::OS::UI::Xaml::Media::Imaging::SvgImageSource>{ using type = llm::OS::UI::Xaml::Media::Imaging::ISvgImageSource; };
    template <> struct default_interface<llm::OS::UI::Xaml::Media::Imaging::SvgImageSourceFailedEventArgs>{ using type = llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceFailedEventArgs; };
    template <> struct default_interface<llm::OS::UI::Xaml::Media::Imaging::SvgImageSourceOpenedEventArgs>{ using type = llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceOpenedEventArgs; };
    template <> struct default_interface<llm::OS::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource>{ using type = llm::OS::UI::Xaml::Media::Imaging::IVirtualSurfaceImageSource; };
    template <> struct default_interface<llm::OS::UI::Xaml::Media::Imaging::WriteableBitmap>{ using type = llm::OS::UI::Xaml::Media::Imaging::IWriteableBitmap; };
    template <> struct default_interface<llm::OS::UI::Xaml::Media::Imaging::XamlRenderingBackgroundTask>{ using type = llm::OS::UI::Xaml::Media::Imaging::IXamlRenderingBackgroundTask; };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::IBitmapImage>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_CreateOptions(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall put_CreateOptions(uint32_t) noexcept = 0;
            virtual int32_t __stdcall get_UriSource(void**) noexcept = 0;
            virtual int32_t __stdcall put_UriSource(void*) noexcept = 0;
            virtual int32_t __stdcall get_DecodePixelWidth(int32_t*) noexcept = 0;
            virtual int32_t __stdcall put_DecodePixelWidth(int32_t) noexcept = 0;
            virtual int32_t __stdcall get_DecodePixelHeight(int32_t*) noexcept = 0;
            virtual int32_t __stdcall put_DecodePixelHeight(int32_t) noexcept = 0;
            virtual int32_t __stdcall add_DownloadProgress(void*, llm::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_DownloadProgress(llm::event_token) noexcept = 0;
            virtual int32_t __stdcall add_ImageOpened(void*, llm::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_ImageOpened(llm::event_token) noexcept = 0;
            virtual int32_t __stdcall add_ImageFailed(void*, llm::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_ImageFailed(llm::event_token) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::IBitmapImage2>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_DecodePixelType(int32_t*) noexcept = 0;
            virtual int32_t __stdcall put_DecodePixelType(int32_t) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::IBitmapImage3>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_IsAnimatedBitmap(bool*) noexcept = 0;
            virtual int32_t __stdcall get_IsPlaying(bool*) noexcept = 0;
            virtual int32_t __stdcall get_AutoPlay(bool*) noexcept = 0;
            virtual int32_t __stdcall put_AutoPlay(bool) noexcept = 0;
            virtual int32_t __stdcall Play() noexcept = 0;
            virtual int32_t __stdcall Stop() noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::IBitmapImageFactory>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall CreateInstanceWithUriSource(void*, void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::IBitmapImageStatics>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_CreateOptionsProperty(void**) noexcept = 0;
            virtual int32_t __stdcall get_UriSourceProperty(void**) noexcept = 0;
            virtual int32_t __stdcall get_DecodePixelWidthProperty(void**) noexcept = 0;
            virtual int32_t __stdcall get_DecodePixelHeightProperty(void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::IBitmapImageStatics2>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_DecodePixelTypeProperty(void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::IBitmapImageStatics3>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_IsAnimatedBitmapProperty(void**) noexcept = 0;
            virtual int32_t __stdcall get_IsPlayingProperty(void**) noexcept = 0;
            virtual int32_t __stdcall get_AutoPlayProperty(void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::IBitmapSource>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_PixelWidth(int32_t*) noexcept = 0;
            virtual int32_t __stdcall get_PixelHeight(int32_t*) noexcept = 0;
            virtual int32_t __stdcall SetSource(void*) noexcept = 0;
            virtual int32_t __stdcall SetSourceAsync(void*, void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::IBitmapSourceFactory>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall CreateInstance(void*, void**, void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::IBitmapSourceStatics>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_PixelWidthProperty(void**) noexcept = 0;
            virtual int32_t __stdcall get_PixelHeightProperty(void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::IDownloadProgressEventArgs>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_Progress(int32_t*) noexcept = 0;
            virtual int32_t __stdcall put_Progress(int32_t) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::IRenderTargetBitmap>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_PixelWidth(int32_t*) noexcept = 0;
            virtual int32_t __stdcall get_PixelHeight(int32_t*) noexcept = 0;
            virtual int32_t __stdcall RenderAsync(void*, void**) noexcept = 0;
            virtual int32_t __stdcall RenderToSizeAsync(void*, int32_t, int32_t, void**) noexcept = 0;
            virtual int32_t __stdcall GetPixelsAsync(void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::IRenderTargetBitmapStatics>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_PixelWidthProperty(void**) noexcept = 0;
            virtual int32_t __stdcall get_PixelHeightProperty(void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::ISoftwareBitmapSource>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall SetBitmapAsync(void*, void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::ISurfaceImageSource>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::ISurfaceImageSourceFactory>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall CreateInstanceWithDimensions(int32_t, int32_t, void*, void**, void**) noexcept = 0;
            virtual int32_t __stdcall CreateInstanceWithDimensionsAndOpacity(int32_t, int32_t, bool, void*, void**, void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSource>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_UriSource(void**) noexcept = 0;
            virtual int32_t __stdcall put_UriSource(void*) noexcept = 0;
            virtual int32_t __stdcall get_RasterizePixelWidth(double*) noexcept = 0;
            virtual int32_t __stdcall put_RasterizePixelWidth(double) noexcept = 0;
            virtual int32_t __stdcall get_RasterizePixelHeight(double*) noexcept = 0;
            virtual int32_t __stdcall put_RasterizePixelHeight(double) noexcept = 0;
            virtual int32_t __stdcall add_Opened(void*, llm::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_Opened(llm::event_token) noexcept = 0;
            virtual int32_t __stdcall add_OpenFailed(void*, llm::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_OpenFailed(llm::event_token) noexcept = 0;
            virtual int32_t __stdcall SetSourceAsync(void*, void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceFactory>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall CreateInstance(void*, void**, void**) noexcept = 0;
            virtual int32_t __stdcall CreateInstanceWithUriSource(void*, void*, void**, void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceFailedEventArgs>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_Status(int32_t*) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceOpenedEventArgs>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceStatics>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_UriSourceProperty(void**) noexcept = 0;
            virtual int32_t __stdcall get_RasterizePixelWidthProperty(void**) noexcept = 0;
            virtual int32_t __stdcall get_RasterizePixelHeightProperty(void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::IVirtualSurfaceImageSource>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::IVirtualSurfaceImageSourceFactory>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall CreateInstanceWithDimensions(int32_t, int32_t, void**) noexcept = 0;
            virtual int32_t __stdcall CreateInstanceWithDimensionsAndOpacity(int32_t, int32_t, bool, void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::IWriteableBitmap>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_PixelBuffer(void**) noexcept = 0;
            virtual int32_t __stdcall Invalidate() noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::IWriteableBitmapFactory>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall CreateInstanceWithDimensions(int32_t, int32_t, void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::IXamlRenderingBackgroundTask>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::IXamlRenderingBackgroundTaskFactory>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall CreateInstance(void*, void**, void**) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::IXamlRenderingBackgroundTaskOverrides>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall OnRun(void*) noexcept = 0;
        };
    };
    template <> struct abi<llm::OS::UI::Xaml::Media::Imaging::DownloadProgressEventHandler>
    {
        struct __declspec(novtable) type : unknown_abi
        {
            virtual int32_t __stdcall Invoke(void*, void*) noexcept = 0;
        };
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_IBitmapImage
    {
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Media::Imaging::BitmapCreateOptions) CreateOptions() const;
        WINRT_IMPL_AUTO(void) CreateOptions(llm::OS::UI::Xaml::Media::Imaging::BitmapCreateOptions const& value) const;
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::Foundation::Uri) UriSource() const;
        WINRT_IMPL_AUTO(void) UriSource(llm::OS::Foundation::Uri const& value) const;
        [[nodiscard]] WINRT_IMPL_AUTO(int32_t) DecodePixelWidth() const;
        WINRT_IMPL_AUTO(void) DecodePixelWidth(int32_t value) const;
        [[nodiscard]] WINRT_IMPL_AUTO(int32_t) DecodePixelHeight() const;
        WINRT_IMPL_AUTO(void) DecodePixelHeight(int32_t value) const;
        WINRT_IMPL_AUTO(llm::event_token) DownloadProgress(llm::OS::UI::Xaml::Media::Imaging::DownloadProgressEventHandler const& handler) const;
        using DownloadProgress_revoker = impl::event_revoker<llm::OS::UI::Xaml::Media::Imaging::IBitmapImage, &impl::abi_t<llm::OS::UI::Xaml::Media::Imaging::IBitmapImage>::remove_DownloadProgress>;
        [[nodiscard]] DownloadProgress_revoker DownloadProgress(auto_revoke_t, llm::OS::UI::Xaml::Media::Imaging::DownloadProgressEventHandler const& handler) const;
        WINRT_IMPL_AUTO(void) DownloadProgress(llm::event_token const& token) const noexcept;
        WINRT_IMPL_AUTO(llm::event_token) ImageOpened(llm::OS::UI::Xaml::RoutedEventHandler const& handler) const;
        using ImageOpened_revoker = impl::event_revoker<llm::OS::UI::Xaml::Media::Imaging::IBitmapImage, &impl::abi_t<llm::OS::UI::Xaml::Media::Imaging::IBitmapImage>::remove_ImageOpened>;
        [[nodiscard]] ImageOpened_revoker ImageOpened(auto_revoke_t, llm::OS::UI::Xaml::RoutedEventHandler const& handler) const;
        WINRT_IMPL_AUTO(void) ImageOpened(llm::event_token const& token) const noexcept;
        WINRT_IMPL_AUTO(llm::event_token) ImageFailed(llm::OS::UI::Xaml::ExceptionRoutedEventHandler const& handler) const;
        using ImageFailed_revoker = impl::event_revoker<llm::OS::UI::Xaml::Media::Imaging::IBitmapImage, &impl::abi_t<llm::OS::UI::Xaml::Media::Imaging::IBitmapImage>::remove_ImageFailed>;
        [[nodiscard]] ImageFailed_revoker ImageFailed(auto_revoke_t, llm::OS::UI::Xaml::ExceptionRoutedEventHandler const& handler) const;
        WINRT_IMPL_AUTO(void) ImageFailed(llm::event_token const& token) const noexcept;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::IBitmapImage>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_IBitmapImage<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_IBitmapImage2
    {
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Media::Imaging::DecodePixelType) DecodePixelType() const;
        WINRT_IMPL_AUTO(void) DecodePixelType(llm::OS::UI::Xaml::Media::Imaging::DecodePixelType const& value) const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::IBitmapImage2>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_IBitmapImage2<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_IBitmapImage3
    {
        [[nodiscard]] WINRT_IMPL_AUTO(bool) IsAnimatedBitmap() const;
        [[nodiscard]] WINRT_IMPL_AUTO(bool) IsPlaying() const;
        [[nodiscard]] WINRT_IMPL_AUTO(bool) AutoPlay() const;
        WINRT_IMPL_AUTO(void) AutoPlay(bool value) const;
        WINRT_IMPL_AUTO(void) Play() const;
        WINRT_IMPL_AUTO(void) Stop() const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::IBitmapImage3>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_IBitmapImage3<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_IBitmapImageFactory
    {
        WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Media::Imaging::BitmapImage) CreateInstanceWithUriSource(llm::OS::Foundation::Uri const& uriSource) const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::IBitmapImageFactory>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_IBitmapImageFactory<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_IBitmapImageStatics
    {
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::DependencyProperty) CreateOptionsProperty() const;
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::DependencyProperty) UriSourceProperty() const;
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::DependencyProperty) DecodePixelWidthProperty() const;
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::DependencyProperty) DecodePixelHeightProperty() const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::IBitmapImageStatics>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_IBitmapImageStatics<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_IBitmapImageStatics2
    {
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::DependencyProperty) DecodePixelTypeProperty() const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::IBitmapImageStatics2>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_IBitmapImageStatics2<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_IBitmapImageStatics3
    {
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::DependencyProperty) IsAnimatedBitmapProperty() const;
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::DependencyProperty) IsPlayingProperty() const;
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::DependencyProperty) AutoPlayProperty() const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::IBitmapImageStatics3>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_IBitmapImageStatics3<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_IBitmapSource
    {
        [[nodiscard]] WINRT_IMPL_AUTO(int32_t) PixelWidth() const;
        [[nodiscard]] WINRT_IMPL_AUTO(int32_t) PixelHeight() const;
        WINRT_IMPL_AUTO(void) SetSource(llm::OS::Storage::Streams::IRandomAccessStream const& streamSource) const;
        WINRT_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) SetSourceAsync(llm::OS::Storage::Streams::IRandomAccessStream const& streamSource) const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::IBitmapSource>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_IBitmapSource<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_IBitmapSourceFactory
    {
        WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Media::Imaging::BitmapSource) CreateInstance(llm::OS::Foundation::IInspectable const& baseInterface, llm::OS::Foundation::IInspectable& innerInterface) const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::IBitmapSourceFactory>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_IBitmapSourceFactory<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_IBitmapSourceStatics
    {
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::DependencyProperty) PixelWidthProperty() const;
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::DependencyProperty) PixelHeightProperty() const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::IBitmapSourceStatics>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_IBitmapSourceStatics<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_IDownloadProgressEventArgs
    {
        [[nodiscard]] WINRT_IMPL_AUTO(int32_t) Progress() const;
        WINRT_IMPL_AUTO(void) Progress(int32_t value) const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::IDownloadProgressEventArgs>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_IDownloadProgressEventArgs<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_IRenderTargetBitmap
    {
        [[nodiscard]] WINRT_IMPL_AUTO(int32_t) PixelWidth() const;
        [[nodiscard]] WINRT_IMPL_AUTO(int32_t) PixelHeight() const;
        WINRT_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) RenderAsync(llm::OS::UI::Xaml::UIElement const& element) const;
        WINRT_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) RenderAsync(llm::OS::UI::Xaml::UIElement const& element, int32_t scaledWidth, int32_t scaledHeight) const;
        WINRT_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<llm::OS::Storage::Streams::IBuffer>) GetPixelsAsync() const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::IRenderTargetBitmap>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_IRenderTargetBitmap<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_IRenderTargetBitmapStatics
    {
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::DependencyProperty) PixelWidthProperty() const;
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::DependencyProperty) PixelHeightProperty() const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::IRenderTargetBitmapStatics>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_IRenderTargetBitmapStatics<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_ISoftwareBitmapSource
    {
        WINRT_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) SetBitmapAsync(llm::OS::Graphics::Imaging::SoftwareBitmap const& softwareBitmap) const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::ISoftwareBitmapSource>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_ISoftwareBitmapSource<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_ISurfaceImageSource
    {
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::ISurfaceImageSource>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_ISurfaceImageSource<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_ISurfaceImageSourceFactory
    {
        WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Media::Imaging::SurfaceImageSource) CreateInstanceWithDimensions(int32_t pixelWidth, int32_t pixelHeight, llm::OS::Foundation::IInspectable const& baseInterface, llm::OS::Foundation::IInspectable& innerInterface) const;
        WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Media::Imaging::SurfaceImageSource) CreateInstanceWithDimensionsAndOpacity(int32_t pixelWidth, int32_t pixelHeight, bool isOpaque, llm::OS::Foundation::IInspectable const& baseInterface, llm::OS::Foundation::IInspectable& innerInterface) const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::ISurfaceImageSourceFactory>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_ISurfaceImageSourceFactory<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_ISvgImageSource
    {
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::Foundation::Uri) UriSource() const;
        WINRT_IMPL_AUTO(void) UriSource(llm::OS::Foundation::Uri const& value) const;
        [[nodiscard]] WINRT_IMPL_AUTO(double) RasterizePixelWidth() const;
        WINRT_IMPL_AUTO(void) RasterizePixelWidth(double value) const;
        [[nodiscard]] WINRT_IMPL_AUTO(double) RasterizePixelHeight() const;
        WINRT_IMPL_AUTO(void) RasterizePixelHeight(double value) const;
        WINRT_IMPL_AUTO(llm::event_token) Opened(llm::OS::Foundation::TypedEventHandler<llm::OS::UI::Xaml::Media::Imaging::SvgImageSource, llm::OS::UI::Xaml::Media::Imaging::SvgImageSourceOpenedEventArgs> const& handler) const;
        using Opened_revoker = impl::event_revoker<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSource, &impl::abi_t<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSource>::remove_Opened>;
        [[nodiscard]] Opened_revoker Opened(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::UI::Xaml::Media::Imaging::SvgImageSource, llm::OS::UI::Xaml::Media::Imaging::SvgImageSourceOpenedEventArgs> const& handler) const;
        WINRT_IMPL_AUTO(void) Opened(llm::event_token const& token) const noexcept;
        WINRT_IMPL_AUTO(llm::event_token) OpenFailed(llm::OS::Foundation::TypedEventHandler<llm::OS::UI::Xaml::Media::Imaging::SvgImageSource, llm::OS::UI::Xaml::Media::Imaging::SvgImageSourceFailedEventArgs> const& handler) const;
        using OpenFailed_revoker = impl::event_revoker<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSource, &impl::abi_t<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSource>::remove_OpenFailed>;
        [[nodiscard]] OpenFailed_revoker OpenFailed(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::UI::Xaml::Media::Imaging::SvgImageSource, llm::OS::UI::Xaml::Media::Imaging::SvgImageSourceFailedEventArgs> const& handler) const;
        WINRT_IMPL_AUTO(void) OpenFailed(llm::event_token const& token) const noexcept;
        WINRT_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<llm::OS::UI::Xaml::Media::Imaging::SvgImageSourceLoadStatus>) SetSourceAsync(llm::OS::Storage::Streams::IRandomAccessStream const& streamSource) const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSource>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_ISvgImageSource<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_ISvgImageSourceFactory
    {
        WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Media::Imaging::SvgImageSource) CreateInstance(llm::OS::Foundation::IInspectable const& baseInterface, llm::OS::Foundation::IInspectable& innerInterface) const;
        WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Media::Imaging::SvgImageSource) CreateInstanceWithUriSource(llm::OS::Foundation::Uri const& uriSource, llm::OS::Foundation::IInspectable const& baseInterface, llm::OS::Foundation::IInspectable& innerInterface) const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceFactory>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_ISvgImageSourceFactory<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_ISvgImageSourceFailedEventArgs
    {
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Media::Imaging::SvgImageSourceLoadStatus) Status() const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceFailedEventArgs>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_ISvgImageSourceFailedEventArgs<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_ISvgImageSourceOpenedEventArgs
    {
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceOpenedEventArgs>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_ISvgImageSourceOpenedEventArgs<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_ISvgImageSourceStatics
    {
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::DependencyProperty) UriSourceProperty() const;
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::DependencyProperty) RasterizePixelWidthProperty() const;
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::UI::Xaml::DependencyProperty) RasterizePixelHeightProperty() const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::ISvgImageSourceStatics>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_ISvgImageSourceStatics<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_IVirtualSurfaceImageSource
    {
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::IVirtualSurfaceImageSource>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_IVirtualSurfaceImageSource<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_IVirtualSurfaceImageSourceFactory
    {
        WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource) CreateInstanceWithDimensions(int32_t pixelWidth, int32_t pixelHeight) const;
        WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource) CreateInstanceWithDimensionsAndOpacity(int32_t pixelWidth, int32_t pixelHeight, bool isOpaque) const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::IVirtualSurfaceImageSourceFactory>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_IVirtualSurfaceImageSourceFactory<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_IWriteableBitmap
    {
        [[nodiscard]] WINRT_IMPL_AUTO(llm::OS::Storage::Streams::IBuffer) PixelBuffer() const;
        WINRT_IMPL_AUTO(void) Invalidate() const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::IWriteableBitmap>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_IWriteableBitmap<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_IWriteableBitmapFactory
    {
        WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Media::Imaging::WriteableBitmap) CreateInstanceWithDimensions(int32_t pixelWidth, int32_t pixelHeight) const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::IWriteableBitmapFactory>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_IWriteableBitmapFactory<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_IXamlRenderingBackgroundTask
    {
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::IXamlRenderingBackgroundTask>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_IXamlRenderingBackgroundTask<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_IXamlRenderingBackgroundTaskFactory
    {
        WINRT_IMPL_AUTO(llm::OS::UI::Xaml::Media::Imaging::XamlRenderingBackgroundTask) CreateInstance(llm::OS::Foundation::IInspectable const& baseInterface, llm::OS::Foundation::IInspectable& innerInterface) const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::IXamlRenderingBackgroundTaskFactory>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_IXamlRenderingBackgroundTaskFactory<D>;
    };
    template <typename D>
    struct consume_Windows_UI_Xaml_Media_Imaging_IXamlRenderingBackgroundTaskOverrides
    {
        WINRT_IMPL_AUTO(void) OnRun(llm::OS::ApplicationModel::Background::IBackgroundTaskInstance const& taskInstance) const;
    };
    template <> struct consume<llm::OS::UI::Xaml::Media::Imaging::IXamlRenderingBackgroundTaskOverrides>
    {
        template <typename D> using type = consume_Windows_UI_Xaml_Media_Imaging_IXamlRenderingBackgroundTaskOverrides<D>;
    };
}
#endif
