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
#ifndef LLM_OS_UI_Xaml_Controls_2_H
#define LLM_OS_UI_Xaml_Controls_2_H
#include "llm/impl/Windows.Foundation.1.h"
#include "llm/impl/Windows.Foundation.Collections.1.h"
#include "llm/impl/Windows.UI.Composition.1.h"
#include "llm/impl/Windows.UI.Input.Inking.1.h"
#include "llm/impl/Windows.UI.Xaml.1.h"
#include "llm/impl/Windows.UI.Xaml.Automation.Peers.1.h"
#include "llm/impl/Windows.UI.Xaml.Automation.Provider.1.h"
#include "llm/impl/Windows.UI.Xaml.Controls.Primitives.1.h"
#include "llm/impl/Windows.UI.Xaml.Data.1.h"
#include "llm/impl/Windows.UI.Xaml.Input.1.h"
#include "llm/impl/Windows.UI.Xaml.Media.1.h"
#include "llm/impl/Windows.UI.Xaml.Navigation.1.h"
#include "llm/impl/Windows.UI.Xaml.Controls.1.h"
LLM_EXPORT namespace llm::OS::UI::Xaml::Controls
{
    struct BackClickEventHandler : llm::OS::Foundation::IUnknown
    {
        BackClickEventHandler(std::nullptr_t = nullptr) noexcept {}
        BackClickEventHandler(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::Foundation::IUnknown(ptr, take_ownership_from_abi) {}
        template <typename L> BackClickEventHandler(L lambda);
        template <typename F> BackClickEventHandler(F* function);
        template <typename O, typename M> BackClickEventHandler(O* object, M method);
        template <typename O, typename M> BackClickEventHandler(com_ptr<O>&& object, M method);
        template <typename O, typename M> BackClickEventHandler(weak_ref<O>&& object, M method);
        auto operator()(llm::OS::Foundation::IInspectable const& sender, llm::OS::UI::Xaml::Controls::BackClickEventArgs const& e) const;
    };
    struct CalendarViewDayItemChangingEventHandler : llm::OS::Foundation::IUnknown
    {
        CalendarViewDayItemChangingEventHandler(std::nullptr_t = nullptr) noexcept {}
        CalendarViewDayItemChangingEventHandler(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::Foundation::IUnknown(ptr, take_ownership_from_abi) {}
        template <typename L> CalendarViewDayItemChangingEventHandler(L lambda);
        template <typename F> CalendarViewDayItemChangingEventHandler(F* function);
        template <typename O, typename M> CalendarViewDayItemChangingEventHandler(O* object, M method);
        template <typename O, typename M> CalendarViewDayItemChangingEventHandler(com_ptr<O>&& object, M method);
        template <typename O, typename M> CalendarViewDayItemChangingEventHandler(weak_ref<O>&& object, M method);
        auto operator()(llm::OS::UI::Xaml::Controls::CalendarView const& sender, llm::OS::UI::Xaml::Controls::CalendarViewDayItemChangingEventArgs const& e) const;
    };
    struct CleanUpVirtualizedItemEventHandler : llm::OS::Foundation::IUnknown
    {
        CleanUpVirtualizedItemEventHandler(std::nullptr_t = nullptr) noexcept {}
        CleanUpVirtualizedItemEventHandler(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::Foundation::IUnknown(ptr, take_ownership_from_abi) {}
        template <typename L> CleanUpVirtualizedItemEventHandler(L lambda);
        template <typename F> CleanUpVirtualizedItemEventHandler(F* function);
        template <typename O, typename M> CleanUpVirtualizedItemEventHandler(O* object, M method);
        template <typename O, typename M> CleanUpVirtualizedItemEventHandler(com_ptr<O>&& object, M method);
        template <typename O, typename M> CleanUpVirtualizedItemEventHandler(weak_ref<O>&& object, M method);
        auto operator()(llm::OS::Foundation::IInspectable const& sender, llm::OS::UI::Xaml::Controls::CleanUpVirtualizedItemEventArgs const& e) const;
    };
    struct ContextMenuOpeningEventHandler : llm::OS::Foundation::IUnknown
    {
        ContextMenuOpeningEventHandler(std::nullptr_t = nullptr) noexcept {}
        ContextMenuOpeningEventHandler(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::Foundation::IUnknown(ptr, take_ownership_from_abi) {}
        template <typename L> ContextMenuOpeningEventHandler(L lambda);
        template <typename F> ContextMenuOpeningEventHandler(F* function);
        template <typename O, typename M> ContextMenuOpeningEventHandler(O* object, M method);
        template <typename O, typename M> ContextMenuOpeningEventHandler(com_ptr<O>&& object, M method);
        template <typename O, typename M> ContextMenuOpeningEventHandler(weak_ref<O>&& object, M method);
        auto operator()(llm::OS::Foundation::IInspectable const& sender, llm::OS::UI::Xaml::Controls::ContextMenuEventArgs const& e) const;
    };
    struct DragItemsStartingEventHandler : llm::OS::Foundation::IUnknown
    {
        DragItemsStartingEventHandler(std::nullptr_t = nullptr) noexcept {}
        DragItemsStartingEventHandler(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::Foundation::IUnknown(ptr, take_ownership_from_abi) {}
        template <typename L> DragItemsStartingEventHandler(L lambda);
        template <typename F> DragItemsStartingEventHandler(F* function);
        template <typename O, typename M> DragItemsStartingEventHandler(O* object, M method);
        template <typename O, typename M> DragItemsStartingEventHandler(com_ptr<O>&& object, M method);
        template <typename O, typename M> DragItemsStartingEventHandler(weak_ref<O>&& object, M method);
        auto operator()(llm::OS::Foundation::IInspectable const& sender, llm::OS::UI::Xaml::Controls::DragItemsStartingEventArgs const& e) const;
    };
    struct HubSectionHeaderClickEventHandler : llm::OS::Foundation::IUnknown
    {
        HubSectionHeaderClickEventHandler(std::nullptr_t = nullptr) noexcept {}
        HubSectionHeaderClickEventHandler(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::Foundation::IUnknown(ptr, take_ownership_from_abi) {}
        template <typename L> HubSectionHeaderClickEventHandler(L lambda);
        template <typename F> HubSectionHeaderClickEventHandler(F* function);
        template <typename O, typename M> HubSectionHeaderClickEventHandler(O* object, M method);
        template <typename O, typename M> HubSectionHeaderClickEventHandler(com_ptr<O>&& object, M method);
        template <typename O, typename M> HubSectionHeaderClickEventHandler(weak_ref<O>&& object, M method);
        auto operator()(llm::OS::Foundation::IInspectable const& sender, llm::OS::UI::Xaml::Controls::HubSectionHeaderClickEventArgs const& e) const;
    };
    struct ItemClickEventHandler : llm::OS::Foundation::IUnknown
    {
        ItemClickEventHandler(std::nullptr_t = nullptr) noexcept {}
        ItemClickEventHandler(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::Foundation::IUnknown(ptr, take_ownership_from_abi) {}
        template <typename L> ItemClickEventHandler(L lambda);
        template <typename F> ItemClickEventHandler(F* function);
        template <typename O, typename M> ItemClickEventHandler(O* object, M method);
        template <typename O, typename M> ItemClickEventHandler(com_ptr<O>&& object, M method);
        template <typename O, typename M> ItemClickEventHandler(weak_ref<O>&& object, M method);
        auto operator()(llm::OS::Foundation::IInspectable const& sender, llm::OS::UI::Xaml::Controls::ItemClickEventArgs const& e) const;
    };
    struct ListViewItemToKeyHandler : llm::OS::Foundation::IUnknown
    {
        ListViewItemToKeyHandler(std::nullptr_t = nullptr) noexcept {}
        ListViewItemToKeyHandler(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::Foundation::IUnknown(ptr, take_ownership_from_abi) {}
        template <typename L> ListViewItemToKeyHandler(L lambda);
        template <typename F> ListViewItemToKeyHandler(F* function);
        template <typename O, typename M> ListViewItemToKeyHandler(O* object, M method);
        template <typename O, typename M> ListViewItemToKeyHandler(com_ptr<O>&& object, M method);
        template <typename O, typename M> ListViewItemToKeyHandler(weak_ref<O>&& object, M method);
        auto operator()(llm::OS::Foundation::IInspectable const& item) const;
    };
    struct ListViewKeyToItemHandler : llm::OS::Foundation::IUnknown
    {
        ListViewKeyToItemHandler(std::nullptr_t = nullptr) noexcept {}
        ListViewKeyToItemHandler(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::Foundation::IUnknown(ptr, take_ownership_from_abi) {}
        template <typename L> ListViewKeyToItemHandler(L lambda);
        template <typename F> ListViewKeyToItemHandler(F* function);
        template <typename O, typename M> ListViewKeyToItemHandler(O* object, M method);
        template <typename O, typename M> ListViewKeyToItemHandler(com_ptr<O>&& object, M method);
        template <typename O, typename M> ListViewKeyToItemHandler(weak_ref<O>&& object, M method);
        auto operator()(param::hstring const& key) const;
    };
    struct NotifyEventHandler : llm::OS::Foundation::IUnknown
    {
        NotifyEventHandler(std::nullptr_t = nullptr) noexcept {}
        NotifyEventHandler(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::Foundation::IUnknown(ptr, take_ownership_from_abi) {}
        template <typename L> NotifyEventHandler(L lambda);
        template <typename F> NotifyEventHandler(F* function);
        template <typename O, typename M> NotifyEventHandler(O* object, M method);
        template <typename O, typename M> NotifyEventHandler(com_ptr<O>&& object, M method);
        template <typename O, typename M> NotifyEventHandler(weak_ref<O>&& object, M method);
        auto operator()(llm::OS::Foundation::IInspectable const& sender, llm::OS::UI::Xaml::Controls::NotifyEventArgs const& e) const;
    };
    struct SectionsInViewChangedEventHandler : llm::OS::Foundation::IUnknown
    {
        SectionsInViewChangedEventHandler(std::nullptr_t = nullptr) noexcept {}
        SectionsInViewChangedEventHandler(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::Foundation::IUnknown(ptr, take_ownership_from_abi) {}
        template <typename L> SectionsInViewChangedEventHandler(L lambda);
        template <typename F> SectionsInViewChangedEventHandler(F* function);
        template <typename O, typename M> SectionsInViewChangedEventHandler(O* object, M method);
        template <typename O, typename M> SectionsInViewChangedEventHandler(com_ptr<O>&& object, M method);
        template <typename O, typename M> SectionsInViewChangedEventHandler(weak_ref<O>&& object, M method);
        auto operator()(llm::OS::Foundation::IInspectable const& sender, llm::OS::UI::Xaml::Controls::SectionsInViewChangedEventArgs const& e) const;
    };
    struct SelectionChangedEventHandler : llm::OS::Foundation::IUnknown
    {
        SelectionChangedEventHandler(std::nullptr_t = nullptr) noexcept {}
        SelectionChangedEventHandler(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::Foundation::IUnknown(ptr, take_ownership_from_abi) {}
        template <typename L> SelectionChangedEventHandler(L lambda);
        template <typename F> SelectionChangedEventHandler(F* function);
        template <typename O, typename M> SelectionChangedEventHandler(O* object, M method);
        template <typename O, typename M> SelectionChangedEventHandler(com_ptr<O>&& object, M method);
        template <typename O, typename M> SelectionChangedEventHandler(weak_ref<O>&& object, M method);
        auto operator()(llm::OS::Foundation::IInspectable const& sender, llm::OS::UI::Xaml::Controls::SelectionChangedEventArgs const& e) const;
    };
    struct SemanticZoomViewChangedEventHandler : llm::OS::Foundation::IUnknown
    {
        SemanticZoomViewChangedEventHandler(std::nullptr_t = nullptr) noexcept {}
        SemanticZoomViewChangedEventHandler(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::Foundation::IUnknown(ptr, take_ownership_from_abi) {}
        template <typename L> SemanticZoomViewChangedEventHandler(L lambda);
        template <typename F> SemanticZoomViewChangedEventHandler(F* function);
        template <typename O, typename M> SemanticZoomViewChangedEventHandler(O* object, M method);
        template <typename O, typename M> SemanticZoomViewChangedEventHandler(com_ptr<O>&& object, M method);
        template <typename O, typename M> SemanticZoomViewChangedEventHandler(weak_ref<O>&& object, M method);
        auto operator()(llm::OS::Foundation::IInspectable const& sender, llm::OS::UI::Xaml::Controls::SemanticZoomViewChangedEventArgs const& e) const;
    };
    struct TextChangedEventHandler : llm::OS::Foundation::IUnknown
    {
        TextChangedEventHandler(std::nullptr_t = nullptr) noexcept {}
        TextChangedEventHandler(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::Foundation::IUnknown(ptr, take_ownership_from_abi) {}
        template <typename L> TextChangedEventHandler(L lambda);
        template <typename F> TextChangedEventHandler(F* function);
        template <typename O, typename M> TextChangedEventHandler(O* object, M method);
        template <typename O, typename M> TextChangedEventHandler(com_ptr<O>&& object, M method);
        template <typename O, typename M> TextChangedEventHandler(weak_ref<O>&& object, M method);
        auto operator()(llm::OS::Foundation::IInspectable const& sender, llm::OS::UI::Xaml::Controls::TextChangedEventArgs const& e) const;
    };
    struct TextControlPasteEventHandler : llm::OS::Foundation::IUnknown
    {
        TextControlPasteEventHandler(std::nullptr_t = nullptr) noexcept {}
        TextControlPasteEventHandler(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::Foundation::IUnknown(ptr, take_ownership_from_abi) {}
        template <typename L> TextControlPasteEventHandler(L lambda);
        template <typename F> TextControlPasteEventHandler(F* function);
        template <typename O, typename M> TextControlPasteEventHandler(O* object, M method);
        template <typename O, typename M> TextControlPasteEventHandler(com_ptr<O>&& object, M method);
        template <typename O, typename M> TextControlPasteEventHandler(weak_ref<O>&& object, M method);
        auto operator()(llm::OS::Foundation::IInspectable const& sender, llm::OS::UI::Xaml::Controls::TextControlPasteEventArgs const& e) const;
    };
    struct WebViewNavigationFailedEventHandler : llm::OS::Foundation::IUnknown
    {
        WebViewNavigationFailedEventHandler(std::nullptr_t = nullptr) noexcept {}
        WebViewNavigationFailedEventHandler(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::Foundation::IUnknown(ptr, take_ownership_from_abi) {}
        template <typename L> WebViewNavigationFailedEventHandler(L lambda);
        template <typename F> WebViewNavigationFailedEventHandler(F* function);
        template <typename O, typename M> WebViewNavigationFailedEventHandler(O* object, M method);
        template <typename O, typename M> WebViewNavigationFailedEventHandler(com_ptr<O>&& object, M method);
        template <typename O, typename M> WebViewNavigationFailedEventHandler(weak_ref<O>&& object, M method);
        auto operator()(llm::OS::Foundation::IInspectable const& sender, llm::OS::UI::Xaml::Controls::WebViewNavigationFailedEventArgs const& e) const;
    };
    struct __declspec(empty_bases) AnchorRequestedEventArgs : llm::OS::UI::Xaml::Controls::IAnchorRequestedEventArgs
    {
        AnchorRequestedEventArgs(std::nullptr_t) noexcept {}
        AnchorRequestedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IAnchorRequestedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) AppBar : llm::OS::UI::Xaml::Controls::IAppBar,
        impl::base<AppBar, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<AppBar, llm::OS::UI::Xaml::Controls::IAppBar2, llm::OS::UI::Xaml::Controls::IAppBar3, llm::OS::UI::Xaml::Controls::IAppBar4, llm::OS::UI::Xaml::Controls::IAppBarOverrides, llm::OS::UI::Xaml::Controls::IAppBarOverrides3, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        AppBar(std::nullptr_t) noexcept {}
        AppBar(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IAppBar(ptr, take_ownership_from_abi) {}
        AppBar();
        [[nodiscard]] static auto IsOpenProperty();
        [[nodiscard]] static auto IsStickyProperty();
        [[nodiscard]] static auto ClosedDisplayModeProperty();
        [[nodiscard]] static auto LightDismissOverlayModeProperty();
    };
    struct __declspec(empty_bases) AppBarButton : llm::OS::UI::Xaml::Controls::IAppBarButton,
        impl::base<AppBarButton, llm::OS::UI::Xaml::Controls::Button, llm::OS::UI::Xaml::Controls::Primitives::ButtonBase, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<AppBarButton, llm::OS::UI::Xaml::Controls::IAppBarButton3, llm::OS::UI::Xaml::Controls::IAppBarButton4, llm::OS::UI::Xaml::Controls::IAppBarButton5, llm::OS::UI::Xaml::Controls::ICommandBarElement, llm::OS::UI::Xaml::Controls::ICommandBarElement2, llm::OS::UI::Xaml::Controls::IButton, llm::OS::UI::Xaml::Controls::IButtonWithFlyout, llm::OS::UI::Xaml::Controls::Primitives::IButtonBase, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        AppBarButton(std::nullptr_t) noexcept {}
        AppBarButton(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IAppBarButton(ptr, take_ownership_from_abi) {}
        AppBarButton();
        [[nodiscard]] static auto LabelProperty();
        [[nodiscard]] static auto IconProperty();
        [[nodiscard]] static auto IsCompactProperty();
        [[nodiscard]] static auto LabelPositionProperty();
        [[nodiscard]] static auto IsInOverflowProperty();
        [[nodiscard]] static auto DynamicOverflowOrderProperty();
        [[nodiscard]] static auto KeyboardAcceleratorTextOverrideProperty();
    };
    struct __declspec(empty_bases) AppBarElementContainer : llm::OS::UI::Xaml::Controls::IAppBarElementContainer,
        impl::base<AppBarElementContainer, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<AppBarElementContainer, llm::OS::UI::Xaml::Controls::ICommandBarElement, llm::OS::UI::Xaml::Controls::ICommandBarElement2, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        AppBarElementContainer(std::nullptr_t) noexcept {}
        AppBarElementContainer(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IAppBarElementContainer(ptr, take_ownership_from_abi) {}
        AppBarElementContainer();
        [[nodiscard]] static auto IsCompactProperty();
        [[nodiscard]] static auto IsInOverflowProperty();
        [[nodiscard]] static auto DynamicOverflowOrderProperty();
    };
    struct __declspec(empty_bases) AppBarSeparator : llm::OS::UI::Xaml::Controls::IAppBarSeparator,
        impl::base<AppBarSeparator, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<AppBarSeparator, llm::OS::UI::Xaml::Controls::ICommandBarElement, llm::OS::UI::Xaml::Controls::ICommandBarElement2, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        AppBarSeparator(std::nullptr_t) noexcept {}
        AppBarSeparator(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IAppBarSeparator(ptr, take_ownership_from_abi) {}
        AppBarSeparator();
        [[nodiscard]] static auto IsCompactProperty();
        [[nodiscard]] static auto IsInOverflowProperty();
        [[nodiscard]] static auto DynamicOverflowOrderProperty();
    };
    struct __declspec(empty_bases) AppBarToggleButton : llm::OS::UI::Xaml::Controls::IAppBarToggleButton,
        impl::base<AppBarToggleButton, llm::OS::UI::Xaml::Controls::Primitives::ToggleButton, llm::OS::UI::Xaml::Controls::Primitives::ButtonBase, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<AppBarToggleButton, llm::OS::UI::Xaml::Controls::IAppBarToggleButton3, llm::OS::UI::Xaml::Controls::IAppBarToggleButton4, llm::OS::UI::Xaml::Controls::IAppBarToggleButton5, llm::OS::UI::Xaml::Controls::ICommandBarElement, llm::OS::UI::Xaml::Controls::ICommandBarElement2, llm::OS::UI::Xaml::Controls::Primitives::IToggleButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButtonOverrides, llm::OS::UI::Xaml::Controls::Primitives::IButtonBase, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        AppBarToggleButton(std::nullptr_t) noexcept {}
        AppBarToggleButton(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IAppBarToggleButton(ptr, take_ownership_from_abi) {}
        AppBarToggleButton();
        [[nodiscard]] static auto LabelProperty();
        [[nodiscard]] static auto IconProperty();
        [[nodiscard]] static auto IsCompactProperty();
        [[nodiscard]] static auto LabelPositionProperty();
        [[nodiscard]] static auto IsInOverflowProperty();
        [[nodiscard]] static auto DynamicOverflowOrderProperty();
        [[nodiscard]] static auto KeyboardAcceleratorTextOverrideProperty();
    };
    struct __declspec(empty_bases) AutoSuggestBox : llm::OS::UI::Xaml::Controls::IAutoSuggestBox,
        impl::base<AutoSuggestBox, llm::OS::UI::Xaml::Controls::ItemsControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<AutoSuggestBox, llm::OS::UI::Xaml::Controls::IAutoSuggestBox2, llm::OS::UI::Xaml::Controls::IAutoSuggestBox3, llm::OS::UI::Xaml::Controls::IAutoSuggestBox4, llm::OS::UI::Xaml::Controls::IItemsControl, llm::OS::UI::Xaml::Controls::IItemsControl2, llm::OS::UI::Xaml::Controls::IItemsControl3, llm::OS::UI::Xaml::Controls::IItemsControlOverrides, llm::OS::UI::Xaml::Controls::IItemContainerMapping, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        AutoSuggestBox(std::nullptr_t) noexcept {}
        AutoSuggestBox(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IAutoSuggestBox(ptr, take_ownership_from_abi) {}
        AutoSuggestBox();
        [[nodiscard]] static auto MaxSuggestionListHeightProperty();
        [[nodiscard]] static auto IsSuggestionListOpenProperty();
        [[nodiscard]] static auto TextMemberPathProperty();
        [[nodiscard]] static auto TextProperty();
        [[nodiscard]] static auto UpdateTextOnSelectProperty();
        [[nodiscard]] static auto PlaceholderTextProperty();
        [[nodiscard]] static auto HeaderProperty();
        [[nodiscard]] static auto AutoMaximizeSuggestionAreaProperty();
        [[nodiscard]] static auto TextBoxStyleProperty();
        [[nodiscard]] static auto QueryIconProperty();
        [[nodiscard]] static auto LightDismissOverlayModeProperty();
        [[nodiscard]] static auto DescriptionProperty();
    };
    struct __declspec(empty_bases) AutoSuggestBoxQuerySubmittedEventArgs : llm::OS::UI::Xaml::Controls::IAutoSuggestBoxQuerySubmittedEventArgs,
        impl::base<AutoSuggestBoxQuerySubmittedEventArgs, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<AutoSuggestBoxQuerySubmittedEventArgs, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        AutoSuggestBoxQuerySubmittedEventArgs(std::nullptr_t) noexcept {}
        AutoSuggestBoxQuerySubmittedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IAutoSuggestBoxQuerySubmittedEventArgs(ptr, take_ownership_from_abi) {}
        AutoSuggestBoxQuerySubmittedEventArgs();
    };
    struct __declspec(empty_bases) AutoSuggestBoxSuggestionChosenEventArgs : llm::OS::UI::Xaml::Controls::IAutoSuggestBoxSuggestionChosenEventArgs,
        impl::base<AutoSuggestBoxSuggestionChosenEventArgs, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<AutoSuggestBoxSuggestionChosenEventArgs, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        AutoSuggestBoxSuggestionChosenEventArgs(std::nullptr_t) noexcept {}
        AutoSuggestBoxSuggestionChosenEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IAutoSuggestBoxSuggestionChosenEventArgs(ptr, take_ownership_from_abi) {}
        AutoSuggestBoxSuggestionChosenEventArgs();
    };
    struct __declspec(empty_bases) AutoSuggestBoxTextChangedEventArgs : llm::OS::UI::Xaml::Controls::IAutoSuggestBoxTextChangedEventArgs,
        impl::base<AutoSuggestBoxTextChangedEventArgs, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<AutoSuggestBoxTextChangedEventArgs, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        AutoSuggestBoxTextChangedEventArgs(std::nullptr_t) noexcept {}
        AutoSuggestBoxTextChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IAutoSuggestBoxTextChangedEventArgs(ptr, take_ownership_from_abi) {}
        AutoSuggestBoxTextChangedEventArgs();
        [[nodiscard]] static auto ReasonProperty();
    };
    struct __declspec(empty_bases) BackClickEventArgs : llm::OS::UI::Xaml::Controls::IBackClickEventArgs
    {
        BackClickEventArgs(std::nullptr_t) noexcept {}
        BackClickEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IBackClickEventArgs(ptr, take_ownership_from_abi) {}
        BackClickEventArgs();
    };
    struct __declspec(empty_bases) BitmapIcon : llm::OS::UI::Xaml::Controls::IBitmapIcon,
        impl::base<BitmapIcon, llm::OS::UI::Xaml::Controls::IconElement, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<BitmapIcon, llm::OS::UI::Xaml::Controls::IBitmapIcon2, llm::OS::UI::Xaml::Controls::IIconElement, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        BitmapIcon(std::nullptr_t) noexcept {}
        BitmapIcon(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IBitmapIcon(ptr, take_ownership_from_abi) {}
        BitmapIcon();
        [[nodiscard]] static auto UriSourceProperty();
        [[nodiscard]] static auto ShowAsMonochromeProperty();
    };
    struct __declspec(empty_bases) BitmapIconSource : llm::OS::UI::Xaml::Controls::IBitmapIconSource,
        impl::base<BitmapIconSource, llm::OS::UI::Xaml::Controls::IconSource, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<BitmapIconSource, llm::OS::UI::Xaml::Controls::IIconSource, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        BitmapIconSource(std::nullptr_t) noexcept {}
        BitmapIconSource(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IBitmapIconSource(ptr, take_ownership_from_abi) {}
        BitmapIconSource();
        [[nodiscard]] static auto UriSourceProperty();
        [[nodiscard]] static auto ShowAsMonochromeProperty();
    };
    struct __declspec(empty_bases) Border : llm::OS::UI::Xaml::Controls::IBorder,
        impl::base<Border, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<Border, llm::OS::UI::Xaml::Controls::IBorder2, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        Border(std::nullptr_t) noexcept {}
        Border(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IBorder(ptr, take_ownership_from_abi) {}
        Border();
        [[nodiscard]] static auto BorderBrushProperty();
        [[nodiscard]] static auto BorderThicknessProperty();
        [[nodiscard]] static auto BackgroundProperty();
        [[nodiscard]] static auto CornerRadiusProperty();
        [[nodiscard]] static auto PaddingProperty();
        [[nodiscard]] static auto ChildTransitionsProperty();
        [[nodiscard]] static auto BackgroundSizingProperty();
    };
    struct __declspec(empty_bases) Button : llm::OS::UI::Xaml::Controls::IButton,
        impl::base<Button, llm::OS::UI::Xaml::Controls::Primitives::ButtonBase, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<Button, llm::OS::UI::Xaml::Controls::IButtonWithFlyout, llm::OS::UI::Xaml::Controls::Primitives::IButtonBase, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        Button(std::nullptr_t) noexcept {}
        Button(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IButton(ptr, take_ownership_from_abi) {}
        Button();
        [[nodiscard]] static auto FlyoutProperty();
    };
    struct __declspec(empty_bases) CalendarDatePicker : llm::OS::UI::Xaml::Controls::ICalendarDatePicker,
        impl::base<CalendarDatePicker, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<CalendarDatePicker, llm::OS::UI::Xaml::Controls::ICalendarDatePicker2, llm::OS::UI::Xaml::Controls::ICalendarDatePicker3, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        CalendarDatePicker(std::nullptr_t) noexcept {}
        CalendarDatePicker(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ICalendarDatePicker(ptr, take_ownership_from_abi) {}
        CalendarDatePicker();
        [[nodiscard]] static auto DateProperty();
        [[nodiscard]] static auto IsCalendarOpenProperty();
        [[nodiscard]] static auto DateFormatProperty();
        [[nodiscard]] static auto PlaceholderTextProperty();
        [[nodiscard]] static auto HeaderProperty();
        [[nodiscard]] static auto HeaderTemplateProperty();
        [[nodiscard]] static auto CalendarViewStyleProperty();
        [[nodiscard]] static auto MinDateProperty();
        [[nodiscard]] static auto MaxDateProperty();
        [[nodiscard]] static auto IsTodayHighlightedProperty();
        [[nodiscard]] static auto DisplayModeProperty();
        [[nodiscard]] static auto FirstDayOfWeekProperty();
        [[nodiscard]] static auto DayOfWeekFormatProperty();
        [[nodiscard]] static auto CalendarIdentifierProperty();
        [[nodiscard]] static auto IsOutOfScopeEnabledProperty();
        [[nodiscard]] static auto IsGroupLabelVisibleProperty();
        [[nodiscard]] static auto LightDismissOverlayModeProperty();
        [[nodiscard]] static auto DescriptionProperty();
    };
    struct __declspec(empty_bases) CalendarDatePickerDateChangedEventArgs : llm::OS::UI::Xaml::Controls::ICalendarDatePickerDateChangedEventArgs
    {
        CalendarDatePickerDateChangedEventArgs(std::nullptr_t) noexcept {}
        CalendarDatePickerDateChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ICalendarDatePickerDateChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) CalendarView : llm::OS::UI::Xaml::Controls::ICalendarView,
        impl::base<CalendarView, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<CalendarView, llm::OS::UI::Xaml::Controls::ICalendarView2, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        CalendarView(std::nullptr_t) noexcept {}
        CalendarView(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ICalendarView(ptr, take_ownership_from_abi) {}
        CalendarView();
        [[nodiscard]] static auto CalendarIdentifierProperty();
        [[nodiscard]] static auto DayOfWeekFormatProperty();
        [[nodiscard]] static auto IsGroupLabelVisibleProperty();
        [[nodiscard]] static auto DisplayModeProperty();
        [[nodiscard]] static auto FirstDayOfWeekProperty();
        [[nodiscard]] static auto IsOutOfScopeEnabledProperty();
        [[nodiscard]] static auto IsTodayHighlightedProperty();
        [[nodiscard]] static auto MaxDateProperty();
        [[nodiscard]] static auto MinDateProperty();
        [[nodiscard]] static auto NumberOfWeeksInViewProperty();
        [[nodiscard]] static auto SelectedDatesProperty();
        [[nodiscard]] static auto SelectionModeProperty();
        [[nodiscard]] static auto TemplateSettingsProperty();
        [[nodiscard]] static auto FocusBorderBrushProperty();
        [[nodiscard]] static auto SelectedHoverBorderBrushProperty();
        [[nodiscard]] static auto SelectedPressedBorderBrushProperty();
        [[nodiscard]] static auto SelectedBorderBrushProperty();
        [[nodiscard]] static auto HoverBorderBrushProperty();
        [[nodiscard]] static auto PressedBorderBrushProperty();
        [[nodiscard]] static auto CalendarItemBorderBrushProperty();
        [[nodiscard]] static auto OutOfScopeBackgroundProperty();
        [[nodiscard]] static auto CalendarItemBackgroundProperty();
        [[nodiscard]] static auto PressedForegroundProperty();
        [[nodiscard]] static auto TodayForegroundProperty();
        [[nodiscard]] static auto BlackoutForegroundProperty();
        [[nodiscard]] static auto SelectedForegroundProperty();
        [[nodiscard]] static auto OutOfScopeForegroundProperty();
        [[nodiscard]] static auto CalendarItemForegroundProperty();
        [[nodiscard]] static auto DayItemFontFamilyProperty();
        [[nodiscard]] static auto DayItemFontSizeProperty();
        [[nodiscard]] static auto DayItemFontStyleProperty();
        [[nodiscard]] static auto DayItemFontWeightProperty();
        [[nodiscard]] static auto TodayFontWeightProperty();
        [[nodiscard]] static auto FirstOfMonthLabelFontFamilyProperty();
        [[nodiscard]] static auto FirstOfMonthLabelFontSizeProperty();
        [[nodiscard]] static auto FirstOfMonthLabelFontStyleProperty();
        [[nodiscard]] static auto FirstOfMonthLabelFontWeightProperty();
        [[nodiscard]] static auto MonthYearItemFontFamilyProperty();
        [[nodiscard]] static auto MonthYearItemFontSizeProperty();
        [[nodiscard]] static auto MonthYearItemFontStyleProperty();
        [[nodiscard]] static auto MonthYearItemFontWeightProperty();
        [[nodiscard]] static auto FirstOfYearDecadeLabelFontFamilyProperty();
        [[nodiscard]] static auto FirstOfYearDecadeLabelFontSizeProperty();
        [[nodiscard]] static auto FirstOfYearDecadeLabelFontStyleProperty();
        [[nodiscard]] static auto FirstOfYearDecadeLabelFontWeightProperty();
        [[nodiscard]] static auto HorizontalDayItemAlignmentProperty();
        [[nodiscard]] static auto VerticalDayItemAlignmentProperty();
        [[nodiscard]] static auto HorizontalFirstOfMonthLabelAlignmentProperty();
        [[nodiscard]] static auto VerticalFirstOfMonthLabelAlignmentProperty();
        [[nodiscard]] static auto CalendarItemBorderThicknessProperty();
        [[nodiscard]] static auto CalendarViewDayItemStyleProperty();
        [[nodiscard]] static auto SelectedDisabledBorderBrushProperty();
        [[nodiscard]] static auto TodaySelectedInnerBorderBrushProperty();
        [[nodiscard]] static auto BlackoutStrikethroughBrushProperty();
        [[nodiscard]] static auto BlackoutBackgroundProperty();
        [[nodiscard]] static auto CalendarItemHoverBackgroundProperty();
        [[nodiscard]] static auto CalendarItemPressedBackgroundProperty();
        [[nodiscard]] static auto CalendarItemDisabledBackgroundProperty();
        [[nodiscard]] static auto TodayBackgroundProperty();
        [[nodiscard]] static auto TodayBlackoutBackgroundProperty();
        [[nodiscard]] static auto TodayHoverBackgroundProperty();
        [[nodiscard]] static auto TodayPressedBackgroundProperty();
        [[nodiscard]] static auto TodayDisabledBackgroundProperty();
        [[nodiscard]] static auto TodayBlackoutForegroundProperty();
        [[nodiscard]] static auto SelectedHoverForegroundProperty();
        [[nodiscard]] static auto SelectedPressedForegroundProperty();
        [[nodiscard]] static auto SelectedDisabledForegroundProperty();
        [[nodiscard]] static auto OutOfScopeHoverForegroundProperty();
        [[nodiscard]] static auto OutOfScopePressedForegroundProperty();
        [[nodiscard]] static auto DisabledForegroundProperty();
        [[nodiscard]] static auto DayItemMarginProperty();
        [[nodiscard]] static auto MonthYearItemMarginProperty();
        [[nodiscard]] static auto FirstOfMonthLabelMarginProperty();
        [[nodiscard]] static auto FirstOfYearDecadeLabelMarginProperty();
        [[nodiscard]] static auto CalendarItemCornerRadiusProperty();
    };
    struct __declspec(empty_bases) CalendarViewDayItem : llm::OS::UI::Xaml::Controls::ICalendarViewDayItem,
        impl::base<CalendarViewDayItem, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<CalendarViewDayItem, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        CalendarViewDayItem(std::nullptr_t) noexcept {}
        CalendarViewDayItem(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ICalendarViewDayItem(ptr, take_ownership_from_abi) {}
        CalendarViewDayItem();
        [[nodiscard]] static auto IsBlackoutProperty();
        [[nodiscard]] static auto DateProperty();
    };
    struct __declspec(empty_bases) CalendarViewDayItemChangingEventArgs : llm::OS::UI::Xaml::Controls::ICalendarViewDayItemChangingEventArgs
    {
        CalendarViewDayItemChangingEventArgs(std::nullptr_t) noexcept {}
        CalendarViewDayItemChangingEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ICalendarViewDayItemChangingEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) CalendarViewSelectedDatesChangedEventArgs : llm::OS::UI::Xaml::Controls::ICalendarViewSelectedDatesChangedEventArgs
    {
        CalendarViewSelectedDatesChangedEventArgs(std::nullptr_t) noexcept {}
        CalendarViewSelectedDatesChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ICalendarViewSelectedDatesChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) CandidateWindowBoundsChangedEventArgs : llm::OS::UI::Xaml::Controls::ICandidateWindowBoundsChangedEventArgs
    {
        CandidateWindowBoundsChangedEventArgs(std::nullptr_t) noexcept {}
        CandidateWindowBoundsChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ICandidateWindowBoundsChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) Canvas : llm::OS::UI::Xaml::Controls::ICanvas,
        impl::base<Canvas, llm::OS::UI::Xaml::Controls::Panel, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<Canvas, llm::OS::UI::Xaml::Controls::IPanel, llm::OS::UI::Xaml::Controls::IPanel2, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        Canvas(std::nullptr_t) noexcept {}
        Canvas(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ICanvas(ptr, take_ownership_from_abi) {}
        Canvas();
        [[nodiscard]] static auto LeftProperty();
        static auto GetLeft(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetLeft(llm::OS::UI::Xaml::UIElement const& element, double length);
        [[nodiscard]] static auto TopProperty();
        static auto GetTop(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetTop(llm::OS::UI::Xaml::UIElement const& element, double length);
        [[nodiscard]] static auto ZIndexProperty();
        static auto GetZIndex(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetZIndex(llm::OS::UI::Xaml::UIElement const& element, int32_t value);
    };
    struct __declspec(empty_bases) CaptureElement : llm::OS::UI::Xaml::Controls::ICaptureElement,
        impl::base<CaptureElement, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<CaptureElement, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        CaptureElement(std::nullptr_t) noexcept {}
        CaptureElement(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ICaptureElement(ptr, take_ownership_from_abi) {}
        CaptureElement();
        [[nodiscard]] static auto SourceProperty();
        [[nodiscard]] static auto StretchProperty();
    };
    struct __declspec(empty_bases) CheckBox : llm::OS::UI::Xaml::Controls::ICheckBox,
        impl::base<CheckBox, llm::OS::UI::Xaml::Controls::Primitives::ToggleButton, llm::OS::UI::Xaml::Controls::Primitives::ButtonBase, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<CheckBox, llm::OS::UI::Xaml::Controls::Primitives::IToggleButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButtonOverrides, llm::OS::UI::Xaml::Controls::Primitives::IButtonBase, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        CheckBox(std::nullptr_t) noexcept {}
        CheckBox(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ICheckBox(ptr, take_ownership_from_abi) {}
        CheckBox();
    };
    struct __declspec(empty_bases) ChoosingGroupHeaderContainerEventArgs : llm::OS::UI::Xaml::Controls::IChoosingGroupHeaderContainerEventArgs
    {
        ChoosingGroupHeaderContainerEventArgs(std::nullptr_t) noexcept {}
        ChoosingGroupHeaderContainerEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IChoosingGroupHeaderContainerEventArgs(ptr, take_ownership_from_abi) {}
        ChoosingGroupHeaderContainerEventArgs();
    };
    struct __declspec(empty_bases) ChoosingItemContainerEventArgs : llm::OS::UI::Xaml::Controls::IChoosingItemContainerEventArgs
    {
        ChoosingItemContainerEventArgs(std::nullptr_t) noexcept {}
        ChoosingItemContainerEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IChoosingItemContainerEventArgs(ptr, take_ownership_from_abi) {}
        ChoosingItemContainerEventArgs();
    };
    struct __declspec(empty_bases) CleanUpVirtualizedItemEventArgs : llm::OS::UI::Xaml::Controls::ICleanUpVirtualizedItemEventArgs,
        impl::base<CleanUpVirtualizedItemEventArgs, llm::OS::UI::Xaml::RoutedEventArgs>,
        impl::require<CleanUpVirtualizedItemEventArgs, llm::OS::UI::Xaml::IRoutedEventArgs>
    {
        CleanUpVirtualizedItemEventArgs(std::nullptr_t) noexcept {}
        CleanUpVirtualizedItemEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ICleanUpVirtualizedItemEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ColorChangedEventArgs : llm::OS::UI::Xaml::Controls::IColorChangedEventArgs
    {
        ColorChangedEventArgs(std::nullptr_t) noexcept {}
        ColorChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IColorChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ColorPicker : llm::OS::UI::Xaml::Controls::IColorPicker,
        impl::base<ColorPicker, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ColorPicker, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ColorPicker(std::nullptr_t) noexcept {}
        ColorPicker(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IColorPicker(ptr, take_ownership_from_abi) {}
        ColorPicker();
        [[nodiscard]] static auto ColorProperty();
        [[nodiscard]] static auto PreviousColorProperty();
        [[nodiscard]] static auto IsAlphaEnabledProperty();
        [[nodiscard]] static auto IsColorSpectrumVisibleProperty();
        [[nodiscard]] static auto IsColorPreviewVisibleProperty();
        [[nodiscard]] static auto IsColorSliderVisibleProperty();
        [[nodiscard]] static auto IsAlphaSliderVisibleProperty();
        [[nodiscard]] static auto IsMoreButtonVisibleProperty();
        [[nodiscard]] static auto IsColorChannelTextInputVisibleProperty();
        [[nodiscard]] static auto IsAlphaTextInputVisibleProperty();
        [[nodiscard]] static auto IsHexInputVisibleProperty();
        [[nodiscard]] static auto MinHueProperty();
        [[nodiscard]] static auto MaxHueProperty();
        [[nodiscard]] static auto MinSaturationProperty();
        [[nodiscard]] static auto MaxSaturationProperty();
        [[nodiscard]] static auto MinValueProperty();
        [[nodiscard]] static auto MaxValueProperty();
        [[nodiscard]] static auto ColorSpectrumShapeProperty();
        [[nodiscard]] static auto ColorSpectrumComponentsProperty();
    };
    struct __declspec(empty_bases) ColumnDefinition : llm::OS::UI::Xaml::Controls::IColumnDefinition,
        impl::base<ColumnDefinition, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ColumnDefinition, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ColumnDefinition(std::nullptr_t) noexcept {}
        ColumnDefinition(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IColumnDefinition(ptr, take_ownership_from_abi) {}
        ColumnDefinition();
        [[nodiscard]] static auto WidthProperty();
        [[nodiscard]] static auto MaxWidthProperty();
        [[nodiscard]] static auto MinWidthProperty();
    };
    struct __declspec(empty_bases) ColumnDefinitionCollection : llm::OS::Foundation::Collections::IVector<llm::OS::UI::Xaml::Controls::ColumnDefinition>
    {
        ColumnDefinitionCollection(std::nullptr_t) noexcept {}
        ColumnDefinitionCollection(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::Foundation::Collections::IVector<llm::OS::UI::Xaml::Controls::ColumnDefinition>(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ComboBox : llm::OS::UI::Xaml::Controls::IComboBox,
        impl::base<ComboBox, llm::OS::UI::Xaml::Controls::Primitives::Selector, llm::OS::UI::Xaml::Controls::ItemsControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ComboBox, llm::OS::UI::Xaml::Controls::IComboBox2, llm::OS::UI::Xaml::Controls::IComboBox3, llm::OS::UI::Xaml::Controls::IComboBox4, llm::OS::UI::Xaml::Controls::IComboBox5, llm::OS::UI::Xaml::Controls::IComboBox6, llm::OS::UI::Xaml::Controls::IComboBoxOverrides, llm::OS::UI::Xaml::Controls::Primitives::ISelector, llm::OS::UI::Xaml::Controls::IItemsControl, llm::OS::UI::Xaml::Controls::IItemsControl2, llm::OS::UI::Xaml::Controls::IItemsControl3, llm::OS::UI::Xaml::Controls::IItemsControlOverrides, llm::OS::UI::Xaml::Controls::IItemContainerMapping, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ComboBox(std::nullptr_t) noexcept {}
        ComboBox(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IComboBox(ptr, take_ownership_from_abi) {}
        ComboBox();
        using llm::OS::UI::Xaml::Controls::IComboBox::IsEditable;
        using impl::consume_t<ComboBox, llm::OS::UI::Xaml::Controls::IComboBox6>::IsEditable;
        [[nodiscard]] static auto IsDropDownOpenProperty();
        [[nodiscard]] static auto MaxDropDownHeightProperty();
        [[nodiscard]] static auto HeaderProperty();
        [[nodiscard]] static auto HeaderTemplateProperty();
        [[nodiscard]] static auto PlaceholderTextProperty();
        [[nodiscard]] static auto LightDismissOverlayModeProperty();
        [[nodiscard]] static auto IsTextSearchEnabledProperty();
        [[nodiscard]] static auto SelectionChangedTriggerProperty();
        [[nodiscard]] static auto PlaceholderForegroundProperty();
        [[nodiscard]] static auto IsEditableProperty();
        [[nodiscard]] static auto TextProperty();
        [[nodiscard]] static auto TextBoxStyleProperty();
        [[nodiscard]] static auto DescriptionProperty();
    };
    struct __declspec(empty_bases) ComboBoxItem : llm::OS::UI::Xaml::Controls::IComboBoxItem,
        impl::base<ComboBoxItem, llm::OS::UI::Xaml::Controls::Primitives::SelectorItem, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ComboBoxItem, llm::OS::UI::Xaml::Controls::Primitives::ISelectorItem, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ComboBoxItem(std::nullptr_t) noexcept {}
        ComboBoxItem(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IComboBoxItem(ptr, take_ownership_from_abi) {}
        ComboBoxItem();
    };
    struct __declspec(empty_bases) ComboBoxTextSubmittedEventArgs : llm::OS::UI::Xaml::Controls::IComboBoxTextSubmittedEventArgs
    {
        ComboBoxTextSubmittedEventArgs(std::nullptr_t) noexcept {}
        ComboBoxTextSubmittedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IComboBoxTextSubmittedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) CommandBar : llm::OS::UI::Xaml::Controls::ICommandBar,
        impl::base<CommandBar, llm::OS::UI::Xaml::Controls::AppBar, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<CommandBar, llm::OS::UI::Xaml::Controls::ICommandBar2, llm::OS::UI::Xaml::Controls::ICommandBar3, llm::OS::UI::Xaml::Controls::IAppBar, llm::OS::UI::Xaml::Controls::IAppBar2, llm::OS::UI::Xaml::Controls::IAppBar3, llm::OS::UI::Xaml::Controls::IAppBar4, llm::OS::UI::Xaml::Controls::IAppBarOverrides, llm::OS::UI::Xaml::Controls::IAppBarOverrides3, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        CommandBar(std::nullptr_t) noexcept {}
        CommandBar(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ICommandBar(ptr, take_ownership_from_abi) {}
        CommandBar();
        [[nodiscard]] static auto PrimaryCommandsProperty();
        [[nodiscard]] static auto SecondaryCommandsProperty();
        [[nodiscard]] static auto CommandBarOverflowPresenterStyleProperty();
        [[nodiscard]] static auto DefaultLabelPositionProperty();
        [[nodiscard]] static auto OverflowButtonVisibilityProperty();
        [[nodiscard]] static auto IsDynamicOverflowEnabledProperty();
    };
    struct __declspec(empty_bases) CommandBarFlyout : llm::OS::UI::Xaml::Controls::ICommandBarFlyout,
        impl::base<CommandBarFlyout, llm::OS::UI::Xaml::Controls::Primitives::FlyoutBase, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<CommandBarFlyout, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase2, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase3, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase4, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase5, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase6, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBaseOverrides, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBaseOverrides4, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        CommandBarFlyout(std::nullptr_t) noexcept {}
        CommandBarFlyout(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ICommandBarFlyout(ptr, take_ownership_from_abi) {}
        CommandBarFlyout();
        using impl::consume_t<CommandBarFlyout, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase>::ShowAt;
        using impl::consume_t<CommandBarFlyout, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase5>::ShowAt;
    };
    struct __declspec(empty_bases) CommandBarOverflowPresenter : llm::OS::UI::Xaml::Controls::ICommandBarOverflowPresenter,
        impl::base<CommandBarOverflowPresenter, llm::OS::UI::Xaml::Controls::ItemsControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<CommandBarOverflowPresenter, llm::OS::UI::Xaml::Controls::IItemsControl, llm::OS::UI::Xaml::Controls::IItemsControl2, llm::OS::UI::Xaml::Controls::IItemsControl3, llm::OS::UI::Xaml::Controls::IItemsControlOverrides, llm::OS::UI::Xaml::Controls::IItemContainerMapping, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        CommandBarOverflowPresenter(std::nullptr_t) noexcept {}
        CommandBarOverflowPresenter(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ICommandBarOverflowPresenter(ptr, take_ownership_from_abi) {}
        CommandBarOverflowPresenter();
    };
    struct __declspec(empty_bases) ContainerContentChangingEventArgs : llm::OS::UI::Xaml::Controls::IContainerContentChangingEventArgs
    {
        ContainerContentChangingEventArgs(std::nullptr_t) noexcept {}
        ContainerContentChangingEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IContainerContentChangingEventArgs(ptr, take_ownership_from_abi) {}
        ContainerContentChangingEventArgs();
    };
    struct __declspec(empty_bases) ContentControl : llm::OS::UI::Xaml::Controls::IContentControl,
        impl::base<ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ContentControl(std::nullptr_t) noexcept {}
        ContentControl(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IContentControl(ptr, take_ownership_from_abi) {}
        ContentControl();
        [[nodiscard]] static auto ContentProperty();
        [[nodiscard]] static auto ContentTemplateProperty();
        [[nodiscard]] static auto ContentTemplateSelectorProperty();
        [[nodiscard]] static auto ContentTransitionsProperty();
    };
    struct __declspec(empty_bases) ContentDialog : llm::OS::UI::Xaml::Controls::IContentDialog,
        impl::base<ContentDialog, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ContentDialog, llm::OS::UI::Xaml::Controls::IContentDialog2, llm::OS::UI::Xaml::Controls::IContentDialog3, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ContentDialog(std::nullptr_t) noexcept {}
        ContentDialog(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IContentDialog(ptr, take_ownership_from_abi) {}
        ContentDialog();
        using llm::OS::UI::Xaml::Controls::IContentDialog::ShowAsync;
        using impl::consume_t<ContentDialog, llm::OS::UI::Xaml::Controls::IContentDialog3>::ShowAsync;
        [[nodiscard]] static auto TitleProperty();
        [[nodiscard]] static auto TitleTemplateProperty();
        [[nodiscard]] static auto FullSizeDesiredProperty();
        [[nodiscard]] static auto PrimaryButtonTextProperty();
        [[nodiscard]] static auto SecondaryButtonTextProperty();
        [[nodiscard]] static auto PrimaryButtonCommandProperty();
        [[nodiscard]] static auto SecondaryButtonCommandProperty();
        [[nodiscard]] static auto PrimaryButtonCommandParameterProperty();
        [[nodiscard]] static auto SecondaryButtonCommandParameterProperty();
        [[nodiscard]] static auto IsPrimaryButtonEnabledProperty();
        [[nodiscard]] static auto IsSecondaryButtonEnabledProperty();
        [[nodiscard]] static auto CloseButtonTextProperty();
        [[nodiscard]] static auto CloseButtonCommandProperty();
        [[nodiscard]] static auto CloseButtonCommandParameterProperty();
        [[nodiscard]] static auto PrimaryButtonStyleProperty();
        [[nodiscard]] static auto SecondaryButtonStyleProperty();
        [[nodiscard]] static auto CloseButtonStyleProperty();
        [[nodiscard]] static auto DefaultButtonProperty();
    };
    struct __declspec(empty_bases) ContentDialogButtonClickDeferral : llm::OS::UI::Xaml::Controls::IContentDialogButtonClickDeferral
    {
        ContentDialogButtonClickDeferral(std::nullptr_t) noexcept {}
        ContentDialogButtonClickDeferral(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IContentDialogButtonClickDeferral(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ContentDialogButtonClickEventArgs : llm::OS::UI::Xaml::Controls::IContentDialogButtonClickEventArgs
    {
        ContentDialogButtonClickEventArgs(std::nullptr_t) noexcept {}
        ContentDialogButtonClickEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IContentDialogButtonClickEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ContentDialogClosedEventArgs : llm::OS::UI::Xaml::Controls::IContentDialogClosedEventArgs
    {
        ContentDialogClosedEventArgs(std::nullptr_t) noexcept {}
        ContentDialogClosedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IContentDialogClosedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ContentDialogClosingDeferral : llm::OS::UI::Xaml::Controls::IContentDialogClosingDeferral
    {
        ContentDialogClosingDeferral(std::nullptr_t) noexcept {}
        ContentDialogClosingDeferral(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IContentDialogClosingDeferral(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ContentDialogClosingEventArgs : llm::OS::UI::Xaml::Controls::IContentDialogClosingEventArgs
    {
        ContentDialogClosingEventArgs(std::nullptr_t) noexcept {}
        ContentDialogClosingEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IContentDialogClosingEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ContentDialogOpenedEventArgs : llm::OS::UI::Xaml::Controls::IContentDialogOpenedEventArgs
    {
        ContentDialogOpenedEventArgs(std::nullptr_t) noexcept {}
        ContentDialogOpenedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IContentDialogOpenedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ContentLinkChangedEventArgs : llm::OS::UI::Xaml::Controls::IContentLinkChangedEventArgs
    {
        ContentLinkChangedEventArgs(std::nullptr_t) noexcept {}
        ContentLinkChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IContentLinkChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ContentPresenter : llm::OS::UI::Xaml::Controls::IContentPresenter,
        impl::base<ContentPresenter, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ContentPresenter, llm::OS::UI::Xaml::Controls::IContentPresenter2, llm::OS::UI::Xaml::Controls::IContentPresenter3, llm::OS::UI::Xaml::Controls::IContentPresenter4, llm::OS::UI::Xaml::Controls::IContentPresenter5, llm::OS::UI::Xaml::Controls::IContentPresenterOverrides, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ContentPresenter(std::nullptr_t) noexcept {}
        ContentPresenter(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IContentPresenter(ptr, take_ownership_from_abi) {}
        ContentPresenter();
        [[nodiscard]] static auto ContentProperty();
        [[nodiscard]] static auto ContentTemplateProperty();
        [[nodiscard]] static auto ContentTemplateSelectorProperty();
        [[nodiscard]] static auto ContentTransitionsProperty();
        [[nodiscard]] static auto FontSizeProperty();
        [[nodiscard]] static auto FontFamilyProperty();
        [[nodiscard]] static auto FontWeightProperty();
        [[nodiscard]] static auto FontStyleProperty();
        [[nodiscard]] static auto FontStretchProperty();
        [[nodiscard]] static auto CharacterSpacingProperty();
        [[nodiscard]] static auto ForegroundProperty();
        [[nodiscard]] static auto OpticalMarginAlignmentProperty();
        [[nodiscard]] static auto TextLineBoundsProperty();
        [[nodiscard]] static auto IsTextScaleFactorEnabledProperty();
        [[nodiscard]] static auto TextWrappingProperty();
        [[nodiscard]] static auto MaxLinesProperty();
        [[nodiscard]] static auto LineStackingStrategyProperty();
        [[nodiscard]] static auto LineHeightProperty();
        [[nodiscard]] static auto BorderBrushProperty();
        [[nodiscard]] static auto BorderThicknessProperty();
        [[nodiscard]] static auto CornerRadiusProperty();
        [[nodiscard]] static auto PaddingProperty();
        [[nodiscard]] static auto BackgroundProperty();
        [[nodiscard]] static auto HorizontalContentAlignmentProperty();
        [[nodiscard]] static auto VerticalContentAlignmentProperty();
        [[nodiscard]] static auto BackgroundSizingProperty();
    };
    struct __declspec(empty_bases) ContextMenuEventArgs : llm::OS::UI::Xaml::Controls::IContextMenuEventArgs,
        impl::base<ContextMenuEventArgs, llm::OS::UI::Xaml::RoutedEventArgs>,
        impl::require<ContextMenuEventArgs, llm::OS::UI::Xaml::IRoutedEventArgs>
    {
        ContextMenuEventArgs(std::nullptr_t) noexcept {}
        ContextMenuEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IContextMenuEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) Control : llm::OS::UI::Xaml::Controls::IControl,
        impl::base<Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<Control, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        Control(std::nullptr_t) noexcept {}
        Control(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IControl(ptr, take_ownership_from_abi) {}
        [[nodiscard]] static auto FontSizeProperty();
        [[nodiscard]] static auto FontFamilyProperty();
        [[nodiscard]] static auto FontWeightProperty();
        [[nodiscard]] static auto FontStyleProperty();
        [[nodiscard]] static auto FontStretchProperty();
        [[nodiscard]] static auto CharacterSpacingProperty();
        [[nodiscard]] static auto ForegroundProperty();
        [[nodiscard]] static auto IsTabStopProperty();
        [[nodiscard]] static auto IsEnabledProperty();
        [[nodiscard]] static auto TabIndexProperty();
        [[nodiscard]] static auto TabNavigationProperty();
        [[nodiscard]] static auto TemplateProperty();
        [[nodiscard]] static auto PaddingProperty();
        [[nodiscard]] static auto HorizontalContentAlignmentProperty();
        [[nodiscard]] static auto VerticalContentAlignmentProperty();
        [[nodiscard]] static auto BackgroundProperty();
        [[nodiscard]] static auto BorderThicknessProperty();
        [[nodiscard]] static auto BorderBrushProperty();
        [[nodiscard]] static auto DefaultStyleKeyProperty();
        [[nodiscard]] static auto FocusStateProperty();
        [[nodiscard]] static auto IsTextScaleFactorEnabledProperty();
        [[nodiscard]] static auto UseSystemFocusVisualsProperty();
        [[nodiscard]] static auto IsTemplateFocusTargetProperty();
        static auto GetIsTemplateFocusTarget(llm::OS::UI::Xaml::FrameworkElement const& element);
        static auto SetIsTemplateFocusTarget(llm::OS::UI::Xaml::FrameworkElement const& element, bool value);
        [[nodiscard]] static auto IsFocusEngagementEnabledProperty();
        [[nodiscard]] static auto IsFocusEngagedProperty();
        [[nodiscard]] static auto RequiresPointerProperty();
        [[nodiscard]] static auto XYFocusLeftProperty();
        [[nodiscard]] static auto XYFocusRightProperty();
        [[nodiscard]] static auto XYFocusUpProperty();
        [[nodiscard]] static auto XYFocusDownProperty();
        [[nodiscard]] static auto ElementSoundModeProperty();
        [[nodiscard]] static auto DefaultStyleResourceUriProperty();
        [[nodiscard]] static auto IsTemplateKeyTipTargetProperty();
        static auto GetIsTemplateKeyTipTarget(llm::OS::UI::Xaml::DependencyObject const& element);
        static auto SetIsTemplateKeyTipTarget(llm::OS::UI::Xaml::DependencyObject const& element, bool value);
        [[nodiscard]] static auto BackgroundSizingProperty();
        [[nodiscard]] static auto CornerRadiusProperty();
    };
    struct __declspec(empty_bases) ControlTemplate : llm::OS::UI::Xaml::Controls::IControlTemplate,
        impl::base<ControlTemplate, llm::OS::UI::Xaml::FrameworkTemplate, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ControlTemplate, llm::OS::UI::Xaml::IFrameworkTemplate, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ControlTemplate(std::nullptr_t) noexcept {}
        ControlTemplate(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IControlTemplate(ptr, take_ownership_from_abi) {}
        ControlTemplate();
    };
    struct __declspec(empty_bases) DataTemplateSelector : llm::OS::UI::Xaml::Controls::IDataTemplateSelector,
        impl::require<DataTemplateSelector, llm::OS::UI::Xaml::Controls::IDataTemplateSelector2, llm::OS::UI::Xaml::Controls::IDataTemplateSelectorOverrides, llm::OS::UI::Xaml::Controls::IDataTemplateSelectorOverrides2, llm::OS::UI::Xaml::IElementFactory>
    {
        DataTemplateSelector(std::nullptr_t) noexcept {}
        DataTemplateSelector(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IDataTemplateSelector(ptr, take_ownership_from_abi) {}
        DataTemplateSelector();
        using llm::OS::UI::Xaml::Controls::IDataTemplateSelector::SelectTemplate;
        using impl::consume_t<DataTemplateSelector, llm::OS::UI::Xaml::Controls::IDataTemplateSelector2>::SelectTemplate;
        using impl::consume_t<DataTemplateSelector, llm::OS::UI::Xaml::Controls::IDataTemplateSelectorOverrides>::SelectTemplateCore;
        using impl::consume_t<DataTemplateSelector, llm::OS::UI::Xaml::Controls::IDataTemplateSelectorOverrides2>::SelectTemplateCore;
    };
    struct __declspec(empty_bases) DatePickedEventArgs : llm::OS::UI::Xaml::Controls::IDatePickedEventArgs,
        impl::base<DatePickedEventArgs, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<DatePickedEventArgs, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        DatePickedEventArgs(std::nullptr_t) noexcept {}
        DatePickedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IDatePickedEventArgs(ptr, take_ownership_from_abi) {}
        DatePickedEventArgs();
    };
    struct __declspec(empty_bases) DatePicker : llm::OS::UI::Xaml::Controls::IDatePicker,
        impl::base<DatePicker, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<DatePicker, llm::OS::UI::Xaml::Controls::IDatePicker2, llm::OS::UI::Xaml::Controls::IDatePicker3, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        DatePicker(std::nullptr_t) noexcept {}
        DatePicker(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IDatePicker(ptr, take_ownership_from_abi) {}
        DatePicker();
        [[nodiscard]] static auto HeaderProperty();
        [[nodiscard]] static auto HeaderTemplateProperty();
        [[nodiscard]] static auto CalendarIdentifierProperty();
        [[nodiscard]] static auto DateProperty();
        [[nodiscard]] static auto DayVisibleProperty();
        [[nodiscard]] static auto MonthVisibleProperty();
        [[nodiscard]] static auto YearVisibleProperty();
        [[nodiscard]] static auto DayFormatProperty();
        [[nodiscard]] static auto MonthFormatProperty();
        [[nodiscard]] static auto YearFormatProperty();
        [[nodiscard]] static auto MinYearProperty();
        [[nodiscard]] static auto MaxYearProperty();
        [[nodiscard]] static auto OrientationProperty();
        [[nodiscard]] static auto LightDismissOverlayModeProperty();
        [[nodiscard]] static auto SelectedDateProperty();
    };
    struct __declspec(empty_bases) DatePickerFlyout : llm::OS::UI::Xaml::Controls::IDatePickerFlyout,
        impl::base<DatePickerFlyout, llm::OS::UI::Xaml::Controls::Primitives::PickerFlyoutBase, llm::OS::UI::Xaml::Controls::Primitives::FlyoutBase, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<DatePickerFlyout, llm::OS::UI::Xaml::Controls::IDatePickerFlyout2, llm::OS::UI::Xaml::Controls::Primitives::IPickerFlyoutBase, llm::OS::UI::Xaml::Controls::Primitives::IPickerFlyoutBaseOverrides, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase2, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase3, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase4, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase5, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase6, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBaseOverrides, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBaseOverrides4, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        DatePickerFlyout(std::nullptr_t) noexcept {}
        DatePickerFlyout(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IDatePickerFlyout(ptr, take_ownership_from_abi) {}
        DatePickerFlyout();
        using impl::consume_t<DatePickerFlyout, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase>::ShowAt;
        using impl::consume_t<DatePickerFlyout, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase5>::ShowAt;
        [[nodiscard]] static auto CalendarIdentifierProperty();
        [[nodiscard]] static auto DateProperty();
        [[nodiscard]] static auto DayVisibleProperty();
        [[nodiscard]] static auto MonthVisibleProperty();
        [[nodiscard]] static auto YearVisibleProperty();
        [[nodiscard]] static auto MinYearProperty();
        [[nodiscard]] static auto MaxYearProperty();
        [[nodiscard]] static auto DayFormatProperty();
        [[nodiscard]] static auto MonthFormatProperty();
        [[nodiscard]] static auto YearFormatProperty();
    };
    struct __declspec(empty_bases) DatePickerFlyoutItem : llm::OS::UI::Xaml::Controls::IDatePickerFlyoutItem,
        impl::base<DatePickerFlyoutItem, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<DatePickerFlyoutItem, llm::OS::UI::Xaml::Data::ICustomPropertyProvider, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        DatePickerFlyoutItem(std::nullptr_t) noexcept {}
        DatePickerFlyoutItem(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IDatePickerFlyoutItem(ptr, take_ownership_from_abi) {}
        [[nodiscard]] static auto PrimaryTextProperty();
        [[nodiscard]] static auto SecondaryTextProperty();
    };
    struct __declspec(empty_bases) DatePickerFlyoutPresenter : llm::OS::UI::Xaml::Controls::IDatePickerFlyoutPresenter,
        impl::base<DatePickerFlyoutPresenter, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<DatePickerFlyoutPresenter, llm::OS::UI::Xaml::Controls::IDatePickerFlyoutPresenter2, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        DatePickerFlyoutPresenter(std::nullptr_t) noexcept {}
        DatePickerFlyoutPresenter(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IDatePickerFlyoutPresenter(ptr, take_ownership_from_abi) {}
        [[nodiscard]] static auto IsDefaultShadowEnabledProperty();
    };
    struct __declspec(empty_bases) DatePickerSelectedValueChangedEventArgs : llm::OS::UI::Xaml::Controls::IDatePickerSelectedValueChangedEventArgs
    {
        DatePickerSelectedValueChangedEventArgs(std::nullptr_t) noexcept {}
        DatePickerSelectedValueChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IDatePickerSelectedValueChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) DatePickerValueChangedEventArgs : llm::OS::UI::Xaml::Controls::IDatePickerValueChangedEventArgs
    {
        DatePickerValueChangedEventArgs(std::nullptr_t) noexcept {}
        DatePickerValueChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IDatePickerValueChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) DragItemsCompletedEventArgs : llm::OS::UI::Xaml::Controls::IDragItemsCompletedEventArgs
    {
        DragItemsCompletedEventArgs(std::nullptr_t) noexcept {}
        DragItemsCompletedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IDragItemsCompletedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) DragItemsStartingEventArgs : llm::OS::UI::Xaml::Controls::IDragItemsStartingEventArgs
    {
        DragItemsStartingEventArgs(std::nullptr_t) noexcept {}
        DragItemsStartingEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IDragItemsStartingEventArgs(ptr, take_ownership_from_abi) {}
        DragItemsStartingEventArgs();
    };
    struct __declspec(empty_bases) DropDownButton : llm::OS::UI::Xaml::Controls::IDropDownButton,
        impl::base<DropDownButton, llm::OS::UI::Xaml::Controls::Button, llm::OS::UI::Xaml::Controls::Primitives::ButtonBase, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<DropDownButton, llm::OS::UI::Xaml::Controls::IButton, llm::OS::UI::Xaml::Controls::IButtonWithFlyout, llm::OS::UI::Xaml::Controls::Primitives::IButtonBase, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        DropDownButton(std::nullptr_t) noexcept {}
        DropDownButton(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IDropDownButton(ptr, take_ownership_from_abi) {}
        DropDownButton();
    };
    struct __declspec(empty_bases) DropDownButtonAutomationPeer : llm::OS::UI::Xaml::Controls::IDropDownButtonAutomationPeer,
        impl::base<DropDownButtonAutomationPeer, llm::OS::UI::Xaml::Automation::Peers::ButtonAutomationPeer, llm::OS::UI::Xaml::Automation::Peers::ButtonBaseAutomationPeer, llm::OS::UI::Xaml::Automation::Peers::FrameworkElementAutomationPeer, llm::OS::UI::Xaml::Automation::Peers::AutomationPeer, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<DropDownButtonAutomationPeer, llm::OS::UI::Xaml::Automation::Provider::IExpandCollapseProvider, llm::OS::UI::Xaml::Automation::Peers::IButtonAutomationPeer, llm::OS::UI::Xaml::Automation::Provider::IInvokeProvider, llm::OS::UI::Xaml::Automation::Peers::IButtonBaseAutomationPeer, llm::OS::UI::Xaml::Automation::Peers::IFrameworkElementAutomationPeer, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer2, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer3, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer4, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer5, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer6, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer7, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer8, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer9, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerProtected, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides2, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides3, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides4, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides5, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides6, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides8, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides9, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        DropDownButtonAutomationPeer(std::nullptr_t) noexcept {}
        DropDownButtonAutomationPeer(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IDropDownButtonAutomationPeer(ptr, take_ownership_from_abi) {}
        explicit DropDownButtonAutomationPeer(llm::OS::UI::Xaml::Controls::DropDownButton const& owner);
    };
    struct __declspec(empty_bases) DynamicOverflowItemsChangingEventArgs : llm::OS::UI::Xaml::Controls::IDynamicOverflowItemsChangingEventArgs
    {
        DynamicOverflowItemsChangingEventArgs(std::nullptr_t) noexcept {}
        DynamicOverflowItemsChangingEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IDynamicOverflowItemsChangingEventArgs(ptr, take_ownership_from_abi) {}
        DynamicOverflowItemsChangingEventArgs();
    };
    struct __declspec(empty_bases) FlipView : llm::OS::UI::Xaml::Controls::IFlipView,
        impl::base<FlipView, llm::OS::UI::Xaml::Controls::Primitives::Selector, llm::OS::UI::Xaml::Controls::ItemsControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<FlipView, llm::OS::UI::Xaml::Controls::IFlipView2, llm::OS::UI::Xaml::Controls::Primitives::ISelector, llm::OS::UI::Xaml::Controls::IItemsControl, llm::OS::UI::Xaml::Controls::IItemsControl2, llm::OS::UI::Xaml::Controls::IItemsControl3, llm::OS::UI::Xaml::Controls::IItemsControlOverrides, llm::OS::UI::Xaml::Controls::IItemContainerMapping, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        FlipView(std::nullptr_t) noexcept {}
        FlipView(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IFlipView(ptr, take_ownership_from_abi) {}
        FlipView();
        [[nodiscard]] static auto UseTouchAnimationsForAllNavigationProperty();
    };
    struct __declspec(empty_bases) FlipViewItem : llm::OS::UI::Xaml::Controls::IFlipViewItem,
        impl::base<FlipViewItem, llm::OS::UI::Xaml::Controls::Primitives::SelectorItem, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<FlipViewItem, llm::OS::UI::Xaml::Controls::Primitives::ISelectorItem, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        FlipViewItem(std::nullptr_t) noexcept {}
        FlipViewItem(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IFlipViewItem(ptr, take_ownership_from_abi) {}
        FlipViewItem();
    };
    struct __declspec(empty_bases) Flyout : llm::OS::UI::Xaml::Controls::IFlyout,
        impl::base<Flyout, llm::OS::UI::Xaml::Controls::Primitives::FlyoutBase, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<Flyout, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase2, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase3, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase4, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase5, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase6, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBaseOverrides, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBaseOverrides4, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        Flyout(std::nullptr_t) noexcept {}
        Flyout(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IFlyout(ptr, take_ownership_from_abi) {}
        Flyout();
        using impl::consume_t<Flyout, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase>::ShowAt;
        using impl::consume_t<Flyout, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase5>::ShowAt;
        [[nodiscard]] static auto ContentProperty();
        [[nodiscard]] static auto FlyoutPresenterStyleProperty();
    };
    struct __declspec(empty_bases) FlyoutPresenter : llm::OS::UI::Xaml::Controls::IFlyoutPresenter,
        impl::base<FlyoutPresenter, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<FlyoutPresenter, llm::OS::UI::Xaml::Controls::IFlyoutPresenter2, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        FlyoutPresenter(std::nullptr_t) noexcept {}
        FlyoutPresenter(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IFlyoutPresenter(ptr, take_ownership_from_abi) {}
        FlyoutPresenter();
        [[nodiscard]] static auto IsDefaultShadowEnabledProperty();
    };
    struct __declspec(empty_bases) FocusDisengagedEventArgs : llm::OS::UI::Xaml::Controls::IFocusDisengagedEventArgs,
        impl::base<FocusDisengagedEventArgs, llm::OS::UI::Xaml::RoutedEventArgs>,
        impl::require<FocusDisengagedEventArgs, llm::OS::UI::Xaml::IRoutedEventArgs>
    {
        FocusDisengagedEventArgs(std::nullptr_t) noexcept {}
        FocusDisengagedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IFocusDisengagedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) FocusEngagedEventArgs : llm::OS::UI::Xaml::Controls::IFocusEngagedEventArgs,
        impl::base<FocusEngagedEventArgs, llm::OS::UI::Xaml::RoutedEventArgs>,
        impl::require<FocusEngagedEventArgs, llm::OS::UI::Xaml::Controls::IFocusEngagedEventArgs2, llm::OS::UI::Xaml::IRoutedEventArgs>
    {
        FocusEngagedEventArgs(std::nullptr_t) noexcept {}
        FocusEngagedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IFocusEngagedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) FontIcon : llm::OS::UI::Xaml::Controls::IFontIcon,
        impl::base<FontIcon, llm::OS::UI::Xaml::Controls::IconElement, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<FontIcon, llm::OS::UI::Xaml::Controls::IFontIcon2, llm::OS::UI::Xaml::Controls::IFontIcon3, llm::OS::UI::Xaml::Controls::IIconElement, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        FontIcon(std::nullptr_t) noexcept {}
        FontIcon(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IFontIcon(ptr, take_ownership_from_abi) {}
        FontIcon();
        [[nodiscard]] static auto GlyphProperty();
        [[nodiscard]] static auto FontSizeProperty();
        [[nodiscard]] static auto FontFamilyProperty();
        [[nodiscard]] static auto FontWeightProperty();
        [[nodiscard]] static auto FontStyleProperty();
        [[nodiscard]] static auto IsTextScaleFactorEnabledProperty();
        [[nodiscard]] static auto MirroredWhenRightToLeftProperty();
    };
    struct __declspec(empty_bases) FontIconSource : llm::OS::UI::Xaml::Controls::IFontIconSource,
        impl::base<FontIconSource, llm::OS::UI::Xaml::Controls::IconSource, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<FontIconSource, llm::OS::UI::Xaml::Controls::IIconSource, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        FontIconSource(std::nullptr_t) noexcept {}
        FontIconSource(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IFontIconSource(ptr, take_ownership_from_abi) {}
        FontIconSource();
        [[nodiscard]] static auto GlyphProperty();
        [[nodiscard]] static auto FontSizeProperty();
        [[nodiscard]] static auto FontFamilyProperty();
        [[nodiscard]] static auto FontWeightProperty();
        [[nodiscard]] static auto FontStyleProperty();
        [[nodiscard]] static auto IsTextScaleFactorEnabledProperty();
        [[nodiscard]] static auto MirroredWhenRightToLeftProperty();
    };
    struct __declspec(empty_bases) Frame : llm::OS::UI::Xaml::Controls::IFrame,
        impl::base<Frame, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<Frame, llm::OS::UI::Xaml::Controls::IFrame2, llm::OS::UI::Xaml::Controls::IFrame3, llm::OS::UI::Xaml::Controls::IFrame4, llm::OS::UI::Xaml::Controls::IFrame5, llm::OS::UI::Xaml::Controls::INavigate, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        Frame(std::nullptr_t) noexcept {}
        Frame(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IFrame(ptr, take_ownership_from_abi) {}
        Frame();
        using llm::OS::UI::Xaml::Controls::IFrame::GoBack;
        using impl::consume_t<Frame, llm::OS::UI::Xaml::Controls::IFrame3>::GoBack;
        using llm::OS::UI::Xaml::Controls::IFrame::Navigate;
        using impl::consume_t<Frame, llm::OS::UI::Xaml::Controls::IFrame2>::Navigate;
        using impl::consume_t<Frame, llm::OS::UI::Xaml::Controls::INavigate>::Navigate;
        using llm::OS::UI::Xaml::Controls::IFrame::SetNavigationState;
        using impl::consume_t<Frame, llm::OS::UI::Xaml::Controls::IFrame4>::SetNavigationState;
        [[nodiscard]] static auto CacheSizeProperty();
        [[nodiscard]] static auto CanGoBackProperty();
        [[nodiscard]] static auto CanGoForwardProperty();
        [[nodiscard]] static auto CurrentSourcePageTypeProperty();
        [[nodiscard]] static auto SourcePageTypeProperty();
        [[nodiscard]] static auto BackStackDepthProperty();
        [[nodiscard]] static auto BackStackProperty();
        [[nodiscard]] static auto ForwardStackProperty();
        [[nodiscard]] static auto IsNavigationStackEnabledProperty();
    };
    struct __declspec(empty_bases) Grid : llm::OS::UI::Xaml::Controls::IGrid,
        impl::base<Grid, llm::OS::UI::Xaml::Controls::Panel, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<Grid, llm::OS::UI::Xaml::Controls::IGrid2, llm::OS::UI::Xaml::Controls::IGrid3, llm::OS::UI::Xaml::Controls::IGrid4, llm::OS::UI::Xaml::Controls::IPanel, llm::OS::UI::Xaml::Controls::IPanel2, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        Grid(std::nullptr_t) noexcept {}
        Grid(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IGrid(ptr, take_ownership_from_abi) {}
        Grid();
        [[nodiscard]] static auto RowProperty();
        static auto GetRow(llm::OS::UI::Xaml::FrameworkElement const& element);
        static auto SetRow(llm::OS::UI::Xaml::FrameworkElement const& element, int32_t value);
        [[nodiscard]] static auto ColumnProperty();
        static auto GetColumn(llm::OS::UI::Xaml::FrameworkElement const& element);
        static auto SetColumn(llm::OS::UI::Xaml::FrameworkElement const& element, int32_t value);
        [[nodiscard]] static auto RowSpanProperty();
        static auto GetRowSpan(llm::OS::UI::Xaml::FrameworkElement const& element);
        static auto SetRowSpan(llm::OS::UI::Xaml::FrameworkElement const& element, int32_t value);
        [[nodiscard]] static auto ColumnSpanProperty();
        static auto GetColumnSpan(llm::OS::UI::Xaml::FrameworkElement const& element);
        static auto SetColumnSpan(llm::OS::UI::Xaml::FrameworkElement const& element, int32_t value);
        [[nodiscard]] static auto BorderBrushProperty();
        [[nodiscard]] static auto BorderThicknessProperty();
        [[nodiscard]] static auto CornerRadiusProperty();
        [[nodiscard]] static auto PaddingProperty();
        [[nodiscard]] static auto RowSpacingProperty();
        [[nodiscard]] static auto ColumnSpacingProperty();
        [[nodiscard]] static auto BackgroundSizingProperty();
    };
    struct __declspec(empty_bases) GridView : llm::OS::UI::Xaml::Controls::IGridView,
        impl::base<GridView, llm::OS::UI::Xaml::Controls::ListViewBase, llm::OS::UI::Xaml::Controls::Primitives::Selector, llm::OS::UI::Xaml::Controls::ItemsControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<GridView, llm::OS::UI::Xaml::Controls::IListViewBase, llm::OS::UI::Xaml::Controls::IListViewBase2, llm::OS::UI::Xaml::Controls::IListViewBase3, llm::OS::UI::Xaml::Controls::IListViewBase4, llm::OS::UI::Xaml::Controls::IListViewBase5, llm::OS::UI::Xaml::Controls::IListViewBase6, llm::OS::UI::Xaml::Controls::ISemanticZoomInformation, llm::OS::UI::Xaml::Controls::Primitives::ISelector, llm::OS::UI::Xaml::Controls::IItemsControl, llm::OS::UI::Xaml::Controls::IItemsControl2, llm::OS::UI::Xaml::Controls::IItemsControl3, llm::OS::UI::Xaml::Controls::IItemsControlOverrides, llm::OS::UI::Xaml::Controls::IItemContainerMapping, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        GridView(std::nullptr_t) noexcept {}
        GridView(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IGridView(ptr, take_ownership_from_abi) {}
        GridView();
    };
    struct __declspec(empty_bases) GridViewHeaderItem : llm::OS::UI::Xaml::Controls::IGridViewHeaderItem,
        impl::base<GridViewHeaderItem, llm::OS::UI::Xaml::Controls::ListViewBaseHeaderItem, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<GridViewHeaderItem, llm::OS::UI::Xaml::Controls::IListViewBaseHeaderItem, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        GridViewHeaderItem(std::nullptr_t) noexcept {}
        GridViewHeaderItem(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IGridViewHeaderItem(ptr, take_ownership_from_abi) {}
        GridViewHeaderItem();
    };
    struct __declspec(empty_bases) GridViewItem : llm::OS::UI::Xaml::Controls::IGridViewItem,
        impl::base<GridViewItem, llm::OS::UI::Xaml::Controls::Primitives::SelectorItem, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<GridViewItem, llm::OS::UI::Xaml::Controls::Primitives::ISelectorItem, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        GridViewItem(std::nullptr_t) noexcept {}
        GridViewItem(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IGridViewItem(ptr, take_ownership_from_abi) {}
        GridViewItem();
    };
    struct __declspec(empty_bases) GroupItem : llm::OS::UI::Xaml::Controls::IGroupItem,
        impl::base<GroupItem, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<GroupItem, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        GroupItem(std::nullptr_t) noexcept {}
        GroupItem(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IGroupItem(ptr, take_ownership_from_abi) {}
        GroupItem();
    };
    struct __declspec(empty_bases) GroupStyle : llm::OS::UI::Xaml::Controls::IGroupStyle,
        impl::require<GroupStyle, llm::OS::UI::Xaml::Controls::IGroupStyle2, llm::OS::UI::Xaml::Data::INotifyPropertyChanged>
    {
        GroupStyle(std::nullptr_t) noexcept {}
        GroupStyle(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IGroupStyle(ptr, take_ownership_from_abi) {}
        GroupStyle();
    };
    struct __declspec(empty_bases) GroupStyleSelector : llm::OS::UI::Xaml::Controls::IGroupStyleSelector,
        impl::require<GroupStyleSelector, llm::OS::UI::Xaml::Controls::IGroupStyleSelectorOverrides>
    {
        GroupStyleSelector(std::nullptr_t) noexcept {}
        GroupStyleSelector(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IGroupStyleSelector(ptr, take_ownership_from_abi) {}
        GroupStyleSelector();
    };
    struct __declspec(empty_bases) HandwritingPanelClosedEventArgs : llm::OS::UI::Xaml::Controls::IHandwritingPanelClosedEventArgs
    {
        HandwritingPanelClosedEventArgs(std::nullptr_t) noexcept {}
        HandwritingPanelClosedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IHandwritingPanelClosedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) HandwritingPanelOpenedEventArgs : llm::OS::UI::Xaml::Controls::IHandwritingPanelOpenedEventArgs
    {
        HandwritingPanelOpenedEventArgs(std::nullptr_t) noexcept {}
        HandwritingPanelOpenedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IHandwritingPanelOpenedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) HandwritingView : llm::OS::UI::Xaml::Controls::IHandwritingView,
        impl::base<HandwritingView, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<HandwritingView, llm::OS::UI::Xaml::Controls::IHandwritingView2, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        HandwritingView(std::nullptr_t) noexcept {}
        HandwritingView(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IHandwritingView(ptr, take_ownership_from_abi) {}
        HandwritingView();
        [[nodiscard]] static auto PlacementTargetProperty();
        [[nodiscard]] static auto PlacementAlignmentProperty();
        [[nodiscard]] static auto IsOpenProperty();
        [[nodiscard]] static auto AreCandidatesEnabledProperty();
        [[nodiscard]] static auto IsSwitchToKeyboardEnabledProperty();
        [[nodiscard]] static auto IsCommandBarOpenProperty();
    };
    struct __declspec(empty_bases) HandwritingViewCandidatesChangedEventArgs : llm::OS::UI::Xaml::Controls::IHandwritingViewCandidatesChangedEventArgs
    {
        HandwritingViewCandidatesChangedEventArgs(std::nullptr_t) noexcept {}
        HandwritingViewCandidatesChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IHandwritingViewCandidatesChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) HandwritingViewTextSubmittedEventArgs : llm::OS::UI::Xaml::Controls::IHandwritingViewTextSubmittedEventArgs
    {
        HandwritingViewTextSubmittedEventArgs(std::nullptr_t) noexcept {}
        HandwritingViewTextSubmittedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IHandwritingViewTextSubmittedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) Hub : llm::OS::UI::Xaml::Controls::IHub,
        impl::base<Hub, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<Hub, llm::OS::UI::Xaml::Controls::ISemanticZoomInformation, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        Hub(std::nullptr_t) noexcept {}
        Hub(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IHub(ptr, take_ownership_from_abi) {}
        Hub();
        [[nodiscard]] static auto HeaderProperty();
        [[nodiscard]] static auto HeaderTemplateProperty();
        [[nodiscard]] static auto OrientationProperty();
        [[nodiscard]] static auto DefaultSectionIndexProperty();
        [[nodiscard]] static auto SemanticZoomOwnerProperty();
        [[nodiscard]] static auto IsActiveViewProperty();
        [[nodiscard]] static auto IsZoomedInViewProperty();
    };
    struct __declspec(empty_bases) HubSection : llm::OS::UI::Xaml::Controls::IHubSection,
        impl::base<HubSection, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<HubSection, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        HubSection(std::nullptr_t) noexcept {}
        HubSection(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IHubSection(ptr, take_ownership_from_abi) {}
        HubSection();
        [[nodiscard]] static auto HeaderProperty();
        [[nodiscard]] static auto HeaderTemplateProperty();
        [[nodiscard]] static auto ContentTemplateProperty();
        [[nodiscard]] static auto IsHeaderInteractiveProperty();
    };
    struct __declspec(empty_bases) HubSectionCollection : llm::OS::Foundation::Collections::IVector<llm::OS::UI::Xaml::Controls::HubSection>
    {
        HubSectionCollection(std::nullptr_t) noexcept {}
        HubSectionCollection(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::Foundation::Collections::IVector<llm::OS::UI::Xaml::Controls::HubSection>(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) HubSectionHeaderClickEventArgs : llm::OS::UI::Xaml::Controls::IHubSectionHeaderClickEventArgs
    {
        HubSectionHeaderClickEventArgs(std::nullptr_t) noexcept {}
        HubSectionHeaderClickEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IHubSectionHeaderClickEventArgs(ptr, take_ownership_from_abi) {}
        HubSectionHeaderClickEventArgs();
    };
    struct __declspec(empty_bases) HyperlinkButton : llm::OS::UI::Xaml::Controls::IHyperlinkButton,
        impl::base<HyperlinkButton, llm::OS::UI::Xaml::Controls::Primitives::ButtonBase, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<HyperlinkButton, llm::OS::UI::Xaml::Controls::Primitives::IButtonBase, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        HyperlinkButton(std::nullptr_t) noexcept {}
        HyperlinkButton(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IHyperlinkButton(ptr, take_ownership_from_abi) {}
        HyperlinkButton();
        [[nodiscard]] static auto NavigateUriProperty();
    };
    struct __declspec(empty_bases) IconElement : llm::OS::UI::Xaml::Controls::IIconElement,
        impl::base<IconElement, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<IconElement, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        IconElement(std::nullptr_t) noexcept {}
        IconElement(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IIconElement(ptr, take_ownership_from_abi) {}
        [[nodiscard]] static auto ForegroundProperty();
    };
    struct __declspec(empty_bases) IconSource : llm::OS::UI::Xaml::Controls::IIconSource,
        impl::base<IconSource, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<IconSource, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        IconSource(std::nullptr_t) noexcept {}
        IconSource(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IIconSource(ptr, take_ownership_from_abi) {}
        [[nodiscard]] static auto ForegroundProperty();
    };
    struct __declspec(empty_bases) IconSourceElement : llm::OS::UI::Xaml::Controls::IIconSourceElement,
        impl::base<IconSourceElement, llm::OS::UI::Xaml::Controls::IconElement, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<IconSourceElement, llm::OS::UI::Xaml::Controls::IIconElement, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        IconSourceElement(std::nullptr_t) noexcept {}
        IconSourceElement(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IIconSourceElement(ptr, take_ownership_from_abi) {}
        IconSourceElement();
        [[nodiscard]] static auto IconSourceProperty();
    };
    struct __declspec(empty_bases) Image : llm::OS::UI::Xaml::Controls::IImage,
        impl::base<Image, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<Image, llm::OS::UI::Xaml::Controls::IImage2, llm::OS::UI::Xaml::Controls::IImage3, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        Image(std::nullptr_t) noexcept {}
        Image(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IImage(ptr, take_ownership_from_abi) {}
        Image();
        [[nodiscard]] static auto SourceProperty();
        [[nodiscard]] static auto StretchProperty();
        [[nodiscard]] static auto NineGridProperty();
        [[nodiscard]] static auto PlayToSourceProperty();
    };
    struct __declspec(empty_bases) InkCanvas : llm::OS::UI::Xaml::Controls::IInkCanvas,
        impl::base<InkCanvas, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<InkCanvas, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        InkCanvas(std::nullptr_t) noexcept {}
        InkCanvas(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IInkCanvas(ptr, take_ownership_from_abi) {}
        InkCanvas();
    };
    struct __declspec(empty_bases) InkToolbar : llm::OS::UI::Xaml::Controls::IInkToolbar,
        impl::base<InkToolbar, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<InkToolbar, llm::OS::UI::Xaml::Controls::IInkToolbar2, llm::OS::UI::Xaml::Controls::IInkToolbar3, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        InkToolbar(std::nullptr_t) noexcept {}
        InkToolbar(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IInkToolbar(ptr, take_ownership_from_abi) {}
        InkToolbar();
        [[nodiscard]] static auto InitialControlsProperty();
        [[nodiscard]] static auto ChildrenProperty();
        [[nodiscard]] static auto ActiveToolProperty();
        [[nodiscard]] static auto InkDrawingAttributesProperty();
        [[nodiscard]] static auto IsRulerButtonCheckedProperty();
        [[nodiscard]] static auto TargetInkCanvasProperty();
        [[nodiscard]] static auto IsStencilButtonCheckedProperty();
        [[nodiscard]] static auto ButtonFlyoutPlacementProperty();
        [[nodiscard]] static auto OrientationProperty();
        [[nodiscard]] static auto TargetInkPresenterProperty();
    };
    struct __declspec(empty_bases) InkToolbarBallpointPenButton : llm::OS::UI::Xaml::Controls::IInkToolbarBallpointPenButton,
        impl::base<InkToolbarBallpointPenButton, llm::OS::UI::Xaml::Controls::InkToolbarPenButton, llm::OS::UI::Xaml::Controls::InkToolbarToolButton, llm::OS::UI::Xaml::Controls::RadioButton, llm::OS::UI::Xaml::Controls::Primitives::ToggleButton, llm::OS::UI::Xaml::Controls::Primitives::ButtonBase, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<InkToolbarBallpointPenButton, llm::OS::UI::Xaml::Controls::IInkToolbarPenButton, llm::OS::UI::Xaml::Controls::IInkToolbarToolButton, llm::OS::UI::Xaml::Controls::IRadioButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButtonOverrides, llm::OS::UI::Xaml::Controls::Primitives::IButtonBase, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        InkToolbarBallpointPenButton(std::nullptr_t) noexcept {}
        InkToolbarBallpointPenButton(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IInkToolbarBallpointPenButton(ptr, take_ownership_from_abi) {}
        InkToolbarBallpointPenButton();
    };
    struct __declspec(empty_bases) InkToolbarCustomPen : llm::OS::UI::Xaml::Controls::IInkToolbarCustomPen,
        impl::base<InkToolbarCustomPen, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<InkToolbarCustomPen, llm::OS::UI::Xaml::Controls::IInkToolbarCustomPenOverrides, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        InkToolbarCustomPen(std::nullptr_t) noexcept {}
        InkToolbarCustomPen(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IInkToolbarCustomPen(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) InkToolbarCustomPenButton : llm::OS::UI::Xaml::Controls::IInkToolbarCustomPenButton,
        impl::base<InkToolbarCustomPenButton, llm::OS::UI::Xaml::Controls::InkToolbarPenButton, llm::OS::UI::Xaml::Controls::InkToolbarToolButton, llm::OS::UI::Xaml::Controls::RadioButton, llm::OS::UI::Xaml::Controls::Primitives::ToggleButton, llm::OS::UI::Xaml::Controls::Primitives::ButtonBase, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<InkToolbarCustomPenButton, llm::OS::UI::Xaml::Controls::IInkToolbarPenButton, llm::OS::UI::Xaml::Controls::IInkToolbarToolButton, llm::OS::UI::Xaml::Controls::IRadioButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButtonOverrides, llm::OS::UI::Xaml::Controls::Primitives::IButtonBase, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        InkToolbarCustomPenButton(std::nullptr_t) noexcept {}
        InkToolbarCustomPenButton(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IInkToolbarCustomPenButton(ptr, take_ownership_from_abi) {}
        InkToolbarCustomPenButton();
        [[nodiscard]] static auto CustomPenProperty();
        [[nodiscard]] static auto ConfigurationContentProperty();
    };
    struct __declspec(empty_bases) InkToolbarCustomToggleButton : llm::OS::UI::Xaml::Controls::IInkToolbarCustomToggleButton,
        impl::base<InkToolbarCustomToggleButton, llm::OS::UI::Xaml::Controls::InkToolbarToggleButton, llm::OS::UI::Xaml::Controls::CheckBox, llm::OS::UI::Xaml::Controls::Primitives::ToggleButton, llm::OS::UI::Xaml::Controls::Primitives::ButtonBase, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<InkToolbarCustomToggleButton, llm::OS::UI::Xaml::Controls::IInkToolbarToggleButton, llm::OS::UI::Xaml::Controls::ICheckBox, llm::OS::UI::Xaml::Controls::Primitives::IToggleButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButtonOverrides, llm::OS::UI::Xaml::Controls::Primitives::IButtonBase, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        InkToolbarCustomToggleButton(std::nullptr_t) noexcept {}
        InkToolbarCustomToggleButton(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IInkToolbarCustomToggleButton(ptr, take_ownership_from_abi) {}
        InkToolbarCustomToggleButton();
    };
    struct __declspec(empty_bases) InkToolbarCustomToolButton : llm::OS::UI::Xaml::Controls::IInkToolbarCustomToolButton,
        impl::base<InkToolbarCustomToolButton, llm::OS::UI::Xaml::Controls::InkToolbarToolButton, llm::OS::UI::Xaml::Controls::RadioButton, llm::OS::UI::Xaml::Controls::Primitives::ToggleButton, llm::OS::UI::Xaml::Controls::Primitives::ButtonBase, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<InkToolbarCustomToolButton, llm::OS::UI::Xaml::Controls::IInkToolbarToolButton, llm::OS::UI::Xaml::Controls::IRadioButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButtonOverrides, llm::OS::UI::Xaml::Controls::Primitives::IButtonBase, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        InkToolbarCustomToolButton(std::nullptr_t) noexcept {}
        InkToolbarCustomToolButton(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IInkToolbarCustomToolButton(ptr, take_ownership_from_abi) {}
        InkToolbarCustomToolButton();
        [[nodiscard]] static auto ConfigurationContentProperty();
    };
    struct __declspec(empty_bases) InkToolbarEraserButton : llm::OS::UI::Xaml::Controls::IInkToolbarEraserButton,
        impl::base<InkToolbarEraserButton, llm::OS::UI::Xaml::Controls::InkToolbarToolButton, llm::OS::UI::Xaml::Controls::RadioButton, llm::OS::UI::Xaml::Controls::Primitives::ToggleButton, llm::OS::UI::Xaml::Controls::Primitives::ButtonBase, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<InkToolbarEraserButton, llm::OS::UI::Xaml::Controls::IInkToolbarEraserButton2, llm::OS::UI::Xaml::Controls::IInkToolbarToolButton, llm::OS::UI::Xaml::Controls::IRadioButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButtonOverrides, llm::OS::UI::Xaml::Controls::Primitives::IButtonBase, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        InkToolbarEraserButton(std::nullptr_t) noexcept {}
        InkToolbarEraserButton(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IInkToolbarEraserButton(ptr, take_ownership_from_abi) {}
        InkToolbarEraserButton();
        [[nodiscard]] static auto IsClearAllVisibleProperty();
    };
    struct __declspec(empty_bases) InkToolbarFlyoutItem : llm::OS::UI::Xaml::Controls::IInkToolbarFlyoutItem,
        impl::base<InkToolbarFlyoutItem, llm::OS::UI::Xaml::Controls::Primitives::ButtonBase, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<InkToolbarFlyoutItem, llm::OS::UI::Xaml::Controls::Primitives::IButtonBase, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        InkToolbarFlyoutItem(std::nullptr_t) noexcept {}
        InkToolbarFlyoutItem(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IInkToolbarFlyoutItem(ptr, take_ownership_from_abi) {}
        InkToolbarFlyoutItem();
        [[nodiscard]] static auto KindProperty();
        [[nodiscard]] static auto IsCheckedProperty();
    };
    struct __declspec(empty_bases) InkToolbarHighlighterButton : llm::OS::UI::Xaml::Controls::IInkToolbarHighlighterButton,
        impl::base<InkToolbarHighlighterButton, llm::OS::UI::Xaml::Controls::InkToolbarPenButton, llm::OS::UI::Xaml::Controls::InkToolbarToolButton, llm::OS::UI::Xaml::Controls::RadioButton, llm::OS::UI::Xaml::Controls::Primitives::ToggleButton, llm::OS::UI::Xaml::Controls::Primitives::ButtonBase, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<InkToolbarHighlighterButton, llm::OS::UI::Xaml::Controls::IInkToolbarPenButton, llm::OS::UI::Xaml::Controls::IInkToolbarToolButton, llm::OS::UI::Xaml::Controls::IRadioButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButtonOverrides, llm::OS::UI::Xaml::Controls::Primitives::IButtonBase, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        InkToolbarHighlighterButton(std::nullptr_t) noexcept {}
        InkToolbarHighlighterButton(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IInkToolbarHighlighterButton(ptr, take_ownership_from_abi) {}
        InkToolbarHighlighterButton();
    };
    struct __declspec(empty_bases) InkToolbarIsStencilButtonCheckedChangedEventArgs : llm::OS::UI::Xaml::Controls::IInkToolbarIsStencilButtonCheckedChangedEventArgs
    {
        InkToolbarIsStencilButtonCheckedChangedEventArgs(std::nullptr_t) noexcept {}
        InkToolbarIsStencilButtonCheckedChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IInkToolbarIsStencilButtonCheckedChangedEventArgs(ptr, take_ownership_from_abi) {}
        InkToolbarIsStencilButtonCheckedChangedEventArgs();
    };
    struct __declspec(empty_bases) InkToolbarMenuButton : llm::OS::UI::Xaml::Controls::IInkToolbarMenuButton,
        impl::base<InkToolbarMenuButton, llm::OS::UI::Xaml::Controls::Primitives::ToggleButton, llm::OS::UI::Xaml::Controls::Primitives::ButtonBase, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<InkToolbarMenuButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButtonOverrides, llm::OS::UI::Xaml::Controls::Primitives::IButtonBase, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        InkToolbarMenuButton(std::nullptr_t) noexcept {}
        InkToolbarMenuButton(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IInkToolbarMenuButton(ptr, take_ownership_from_abi) {}
        [[nodiscard]] static auto IsExtensionGlyphShownProperty();
    };
    struct __declspec(empty_bases) InkToolbarPenButton : llm::OS::UI::Xaml::Controls::IInkToolbarPenButton,
        impl::base<InkToolbarPenButton, llm::OS::UI::Xaml::Controls::InkToolbarToolButton, llm::OS::UI::Xaml::Controls::RadioButton, llm::OS::UI::Xaml::Controls::Primitives::ToggleButton, llm::OS::UI::Xaml::Controls::Primitives::ButtonBase, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<InkToolbarPenButton, llm::OS::UI::Xaml::Controls::IInkToolbarToolButton, llm::OS::UI::Xaml::Controls::IRadioButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButtonOverrides, llm::OS::UI::Xaml::Controls::Primitives::IButtonBase, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        InkToolbarPenButton(std::nullptr_t) noexcept {}
        InkToolbarPenButton(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IInkToolbarPenButton(ptr, take_ownership_from_abi) {}
        [[nodiscard]] static auto PaletteProperty();
        [[nodiscard]] static auto MinStrokeWidthProperty();
        [[nodiscard]] static auto MaxStrokeWidthProperty();
        [[nodiscard]] static auto SelectedBrushProperty();
        [[nodiscard]] static auto SelectedBrushIndexProperty();
        [[nodiscard]] static auto SelectedStrokeWidthProperty();
    };
    struct __declspec(empty_bases) InkToolbarPenConfigurationControl : llm::OS::UI::Xaml::Controls::IInkToolbarPenConfigurationControl,
        impl::base<InkToolbarPenConfigurationControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<InkToolbarPenConfigurationControl, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        InkToolbarPenConfigurationControl(std::nullptr_t) noexcept {}
        InkToolbarPenConfigurationControl(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IInkToolbarPenConfigurationControl(ptr, take_ownership_from_abi) {}
        InkToolbarPenConfigurationControl();
        [[nodiscard]] static auto PenButtonProperty();
    };
    struct __declspec(empty_bases) InkToolbarPencilButton : llm::OS::UI::Xaml::Controls::IInkToolbarPencilButton,
        impl::base<InkToolbarPencilButton, llm::OS::UI::Xaml::Controls::InkToolbarPenButton, llm::OS::UI::Xaml::Controls::InkToolbarToolButton, llm::OS::UI::Xaml::Controls::RadioButton, llm::OS::UI::Xaml::Controls::Primitives::ToggleButton, llm::OS::UI::Xaml::Controls::Primitives::ButtonBase, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<InkToolbarPencilButton, llm::OS::UI::Xaml::Controls::IInkToolbarPenButton, llm::OS::UI::Xaml::Controls::IInkToolbarToolButton, llm::OS::UI::Xaml::Controls::IRadioButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButtonOverrides, llm::OS::UI::Xaml::Controls::Primitives::IButtonBase, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        InkToolbarPencilButton(std::nullptr_t) noexcept {}
        InkToolbarPencilButton(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IInkToolbarPencilButton(ptr, take_ownership_from_abi) {}
        InkToolbarPencilButton();
    };
    struct __declspec(empty_bases) InkToolbarRulerButton : llm::OS::UI::Xaml::Controls::IInkToolbarRulerButton,
        impl::base<InkToolbarRulerButton, llm::OS::UI::Xaml::Controls::InkToolbarToggleButton, llm::OS::UI::Xaml::Controls::CheckBox, llm::OS::UI::Xaml::Controls::Primitives::ToggleButton, llm::OS::UI::Xaml::Controls::Primitives::ButtonBase, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<InkToolbarRulerButton, llm::OS::UI::Xaml::Controls::IInkToolbarToggleButton, llm::OS::UI::Xaml::Controls::ICheckBox, llm::OS::UI::Xaml::Controls::Primitives::IToggleButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButtonOverrides, llm::OS::UI::Xaml::Controls::Primitives::IButtonBase, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        InkToolbarRulerButton(std::nullptr_t) noexcept {}
        InkToolbarRulerButton(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IInkToolbarRulerButton(ptr, take_ownership_from_abi) {}
        InkToolbarRulerButton();
        [[nodiscard]] static auto RulerProperty();
    };
    struct __declspec(empty_bases) InkToolbarStencilButton : llm::OS::UI::Xaml::Controls::IInkToolbarStencilButton,
        impl::base<InkToolbarStencilButton, llm::OS::UI::Xaml::Controls::InkToolbarMenuButton, llm::OS::UI::Xaml::Controls::Primitives::ToggleButton, llm::OS::UI::Xaml::Controls::Primitives::ButtonBase, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<InkToolbarStencilButton, llm::OS::UI::Xaml::Controls::IInkToolbarMenuButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButtonOverrides, llm::OS::UI::Xaml::Controls::Primitives::IButtonBase, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        InkToolbarStencilButton(std::nullptr_t) noexcept {}
        InkToolbarStencilButton(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IInkToolbarStencilButton(ptr, take_ownership_from_abi) {}
        InkToolbarStencilButton();
        [[nodiscard]] static auto RulerProperty();
        [[nodiscard]] static auto ProtractorProperty();
        [[nodiscard]] static auto SelectedStencilProperty();
        [[nodiscard]] static auto IsRulerItemVisibleProperty();
        [[nodiscard]] static auto IsProtractorItemVisibleProperty();
    };
    struct __declspec(empty_bases) InkToolbarToggleButton : llm::OS::UI::Xaml::Controls::IInkToolbarToggleButton,
        impl::base<InkToolbarToggleButton, llm::OS::UI::Xaml::Controls::CheckBox, llm::OS::UI::Xaml::Controls::Primitives::ToggleButton, llm::OS::UI::Xaml::Controls::Primitives::ButtonBase, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<InkToolbarToggleButton, llm::OS::UI::Xaml::Controls::ICheckBox, llm::OS::UI::Xaml::Controls::Primitives::IToggleButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButtonOverrides, llm::OS::UI::Xaml::Controls::Primitives::IButtonBase, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        InkToolbarToggleButton(std::nullptr_t) noexcept {}
        InkToolbarToggleButton(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IInkToolbarToggleButton(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) InkToolbarToolButton : llm::OS::UI::Xaml::Controls::IInkToolbarToolButton,
        impl::base<InkToolbarToolButton, llm::OS::UI::Xaml::Controls::RadioButton, llm::OS::UI::Xaml::Controls::Primitives::ToggleButton, llm::OS::UI::Xaml::Controls::Primitives::ButtonBase, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<InkToolbarToolButton, llm::OS::UI::Xaml::Controls::IRadioButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButtonOverrides, llm::OS::UI::Xaml::Controls::Primitives::IButtonBase, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        InkToolbarToolButton(std::nullptr_t) noexcept {}
        InkToolbarToolButton(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IInkToolbarToolButton(ptr, take_ownership_from_abi) {}
        [[nodiscard]] static auto IsExtensionGlyphShownProperty();
    };
    struct __declspec(empty_bases) IsTextTrimmedChangedEventArgs : llm::OS::UI::Xaml::Controls::IIsTextTrimmedChangedEventArgs
    {
        IsTextTrimmedChangedEventArgs(std::nullptr_t) noexcept {}
        IsTextTrimmedChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IIsTextTrimmedChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ItemClickEventArgs : llm::OS::UI::Xaml::Controls::IItemClickEventArgs,
        impl::base<ItemClickEventArgs, llm::OS::UI::Xaml::RoutedEventArgs>,
        impl::require<ItemClickEventArgs, llm::OS::UI::Xaml::IRoutedEventArgs>
    {
        ItemClickEventArgs(std::nullptr_t) noexcept {}
        ItemClickEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IItemClickEventArgs(ptr, take_ownership_from_abi) {}
        ItemClickEventArgs();
    };
    struct __declspec(empty_bases) ItemCollection : llm::OS::Foundation::Collections::IObservableVector<llm::OS::Foundation::IInspectable>
    {
        ItemCollection(std::nullptr_t) noexcept {}
        ItemCollection(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::Foundation::Collections::IObservableVector<llm::OS::Foundation::IInspectable>(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ItemContainerGenerator : llm::OS::UI::Xaml::Controls::IItemContainerGenerator
    {
        ItemContainerGenerator(std::nullptr_t) noexcept {}
        ItemContainerGenerator(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IItemContainerGenerator(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ItemsControl : llm::OS::UI::Xaml::Controls::IItemsControl,
        impl::base<ItemsControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ItemsControl, llm::OS::UI::Xaml::Controls::IItemsControl2, llm::OS::UI::Xaml::Controls::IItemsControl3, llm::OS::UI::Xaml::Controls::IItemsControlOverrides, llm::OS::UI::Xaml::Controls::IItemContainerMapping, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ItemsControl(std::nullptr_t) noexcept {}
        ItemsControl(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IItemsControl(ptr, take_ownership_from_abi) {}
        ItemsControl();
        [[nodiscard]] static auto ItemsSourceProperty();
        [[nodiscard]] static auto ItemTemplateProperty();
        [[nodiscard]] static auto ItemTemplateSelectorProperty();
        [[nodiscard]] static auto ItemsPanelProperty();
        [[nodiscard]] static auto DisplayMemberPathProperty();
        [[nodiscard]] static auto ItemContainerStyleProperty();
        [[nodiscard]] static auto ItemContainerStyleSelectorProperty();
        [[nodiscard]] static auto ItemContainerTransitionsProperty();
        [[nodiscard]] static auto GroupStyleSelectorProperty();
        [[nodiscard]] static auto IsGroupingProperty();
        static auto GetItemsOwner(llm::OS::UI::Xaml::DependencyObject const& element);
        static auto ItemsControlFromItemContainer(llm::OS::UI::Xaml::DependencyObject const& container);
    };
    struct __declspec(empty_bases) ItemsPanelTemplate : llm::OS::UI::Xaml::Controls::IItemsPanelTemplate,
        impl::base<ItemsPanelTemplate, llm::OS::UI::Xaml::FrameworkTemplate, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ItemsPanelTemplate, llm::OS::UI::Xaml::IFrameworkTemplate, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ItemsPanelTemplate(std::nullptr_t) noexcept {}
        ItemsPanelTemplate(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IItemsPanelTemplate(ptr, take_ownership_from_abi) {}
        ItemsPanelTemplate();
    };
    struct __declspec(empty_bases) ItemsPickedEventArgs : llm::OS::UI::Xaml::Controls::IItemsPickedEventArgs,
        impl::base<ItemsPickedEventArgs, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ItemsPickedEventArgs, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ItemsPickedEventArgs(std::nullptr_t) noexcept {}
        ItemsPickedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IItemsPickedEventArgs(ptr, take_ownership_from_abi) {}
        ItemsPickedEventArgs();
    };
    struct __declspec(empty_bases) ItemsPresenter : llm::OS::UI::Xaml::Controls::IItemsPresenter,
        impl::base<ItemsPresenter, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ItemsPresenter, llm::OS::UI::Xaml::Controls::IItemsPresenter2, llm::OS::UI::Xaml::Controls::Primitives::IScrollSnapPointsInfo, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ItemsPresenter(std::nullptr_t) noexcept {}
        ItemsPresenter(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IItemsPresenter(ptr, take_ownership_from_abi) {}
        ItemsPresenter();
        [[nodiscard]] static auto HeaderProperty();
        [[nodiscard]] static auto HeaderTemplateProperty();
        [[nodiscard]] static auto HeaderTransitionsProperty();
        [[nodiscard]] static auto PaddingProperty();
        [[nodiscard]] static auto FooterProperty();
        [[nodiscard]] static auto FooterTemplateProperty();
        [[nodiscard]] static auto FooterTransitionsProperty();
    };
    struct __declspec(empty_bases) ItemsStackPanel : llm::OS::UI::Xaml::Controls::IItemsStackPanel,
        impl::base<ItemsStackPanel, llm::OS::UI::Xaml::Controls::Panel, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ItemsStackPanel, llm::OS::UI::Xaml::Controls::IItemsStackPanel2, llm::OS::UI::Xaml::Controls::IPanel, llm::OS::UI::Xaml::Controls::IPanel2, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ItemsStackPanel(std::nullptr_t) noexcept {}
        ItemsStackPanel(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IItemsStackPanel(ptr, take_ownership_from_abi) {}
        ItemsStackPanel();
        [[nodiscard]] static auto GroupPaddingProperty();
        [[nodiscard]] static auto OrientationProperty();
        [[nodiscard]] static auto GroupHeaderPlacementProperty();
        [[nodiscard]] static auto CacheLengthProperty();
        [[nodiscard]] static auto AreStickyGroupHeadersEnabledProperty();
    };
    struct __declspec(empty_bases) ItemsWrapGrid : llm::OS::UI::Xaml::Controls::IItemsWrapGrid,
        impl::base<ItemsWrapGrid, llm::OS::UI::Xaml::Controls::Panel, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ItemsWrapGrid, llm::OS::UI::Xaml::Controls::IItemsWrapGrid2, llm::OS::UI::Xaml::Controls::IPanel, llm::OS::UI::Xaml::Controls::IPanel2, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ItemsWrapGrid(std::nullptr_t) noexcept {}
        ItemsWrapGrid(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IItemsWrapGrid(ptr, take_ownership_from_abi) {}
        ItemsWrapGrid();
        [[nodiscard]] static auto GroupPaddingProperty();
        [[nodiscard]] static auto OrientationProperty();
        [[nodiscard]] static auto MaximumRowsOrColumnsProperty();
        [[nodiscard]] static auto ItemWidthProperty();
        [[nodiscard]] static auto ItemHeightProperty();
        [[nodiscard]] static auto GroupHeaderPlacementProperty();
        [[nodiscard]] static auto CacheLengthProperty();
        [[nodiscard]] static auto AreStickyGroupHeadersEnabledProperty();
    };
    struct __declspec(empty_bases) ListBox : llm::OS::UI::Xaml::Controls::IListBox,
        impl::base<ListBox, llm::OS::UI::Xaml::Controls::Primitives::Selector, llm::OS::UI::Xaml::Controls::ItemsControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ListBox, llm::OS::UI::Xaml::Controls::IListBox2, llm::OS::UI::Xaml::Controls::Primitives::ISelector, llm::OS::UI::Xaml::Controls::IItemsControl, llm::OS::UI::Xaml::Controls::IItemsControl2, llm::OS::UI::Xaml::Controls::IItemsControl3, llm::OS::UI::Xaml::Controls::IItemsControlOverrides, llm::OS::UI::Xaml::Controls::IItemContainerMapping, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ListBox(std::nullptr_t) noexcept {}
        ListBox(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IListBox(ptr, take_ownership_from_abi) {}
        ListBox();
        [[nodiscard]] static auto SelectionModeProperty();
        [[nodiscard]] static auto SingleSelectionFollowsFocusProperty();
    };
    struct __declspec(empty_bases) ListBoxItem : llm::OS::UI::Xaml::Controls::IListBoxItem,
        impl::base<ListBoxItem, llm::OS::UI::Xaml::Controls::Primitives::SelectorItem, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ListBoxItem, llm::OS::UI::Xaml::Controls::Primitives::ISelectorItem, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ListBoxItem(std::nullptr_t) noexcept {}
        ListBoxItem(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IListBoxItem(ptr, take_ownership_from_abi) {}
        ListBoxItem();
    };
    struct __declspec(empty_bases) ListPickerFlyout : llm::OS::UI::Xaml::Controls::IListPickerFlyout,
        impl::base<ListPickerFlyout, llm::OS::UI::Xaml::Controls::Primitives::PickerFlyoutBase, llm::OS::UI::Xaml::Controls::Primitives::FlyoutBase, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ListPickerFlyout, llm::OS::UI::Xaml::Controls::Primitives::IPickerFlyoutBase, llm::OS::UI::Xaml::Controls::Primitives::IPickerFlyoutBaseOverrides, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase2, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase3, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase4, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase5, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase6, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBaseOverrides, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBaseOverrides4, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ListPickerFlyout(std::nullptr_t) noexcept {}
        ListPickerFlyout(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IListPickerFlyout(ptr, take_ownership_from_abi) {}
        ListPickerFlyout();
        using impl::consume_t<ListPickerFlyout, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase>::ShowAt;
        using impl::consume_t<ListPickerFlyout, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase5>::ShowAt;
        [[nodiscard]] static auto ItemsSourceProperty();
        [[nodiscard]] static auto ItemTemplateProperty();
        [[nodiscard]] static auto DisplayMemberPathProperty();
        [[nodiscard]] static auto SelectionModeProperty();
        [[nodiscard]] static auto SelectedIndexProperty();
        [[nodiscard]] static auto SelectedItemProperty();
        [[nodiscard]] static auto SelectedValueProperty();
        [[nodiscard]] static auto SelectedValuePathProperty();
    };
    struct __declspec(empty_bases) ListPickerFlyoutPresenter : llm::OS::UI::Xaml::Controls::IListPickerFlyoutPresenter,
        impl::base<ListPickerFlyoutPresenter, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ListPickerFlyoutPresenter, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ListPickerFlyoutPresenter(std::nullptr_t) noexcept {}
        ListPickerFlyoutPresenter(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IListPickerFlyoutPresenter(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ListView : llm::OS::UI::Xaml::Controls::IListView,
        impl::base<ListView, llm::OS::UI::Xaml::Controls::ListViewBase, llm::OS::UI::Xaml::Controls::Primitives::Selector, llm::OS::UI::Xaml::Controls::ItemsControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ListView, llm::OS::UI::Xaml::Controls::IListViewBase, llm::OS::UI::Xaml::Controls::IListViewBase2, llm::OS::UI::Xaml::Controls::IListViewBase3, llm::OS::UI::Xaml::Controls::IListViewBase4, llm::OS::UI::Xaml::Controls::IListViewBase5, llm::OS::UI::Xaml::Controls::IListViewBase6, llm::OS::UI::Xaml::Controls::ISemanticZoomInformation, llm::OS::UI::Xaml::Controls::Primitives::ISelector, llm::OS::UI::Xaml::Controls::IItemsControl, llm::OS::UI::Xaml::Controls::IItemsControl2, llm::OS::UI::Xaml::Controls::IItemsControl3, llm::OS::UI::Xaml::Controls::IItemsControlOverrides, llm::OS::UI::Xaml::Controls::IItemContainerMapping, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ListView(std::nullptr_t) noexcept {}
        ListView(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IListView(ptr, take_ownership_from_abi) {}
        ListView();
    };
    struct __declspec(empty_bases) ListViewBase : llm::OS::UI::Xaml::Controls::IListViewBase,
        impl::base<ListViewBase, llm::OS::UI::Xaml::Controls::Primitives::Selector, llm::OS::UI::Xaml::Controls::ItemsControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ListViewBase, llm::OS::UI::Xaml::Controls::IListViewBase2, llm::OS::UI::Xaml::Controls::IListViewBase3, llm::OS::UI::Xaml::Controls::IListViewBase4, llm::OS::UI::Xaml::Controls::IListViewBase5, llm::OS::UI::Xaml::Controls::IListViewBase6, llm::OS::UI::Xaml::Controls::ISemanticZoomInformation, llm::OS::UI::Xaml::Controls::Primitives::ISelector, llm::OS::UI::Xaml::Controls::IItemsControl, llm::OS::UI::Xaml::Controls::IItemsControl2, llm::OS::UI::Xaml::Controls::IItemsControl3, llm::OS::UI::Xaml::Controls::IItemsControlOverrides, llm::OS::UI::Xaml::Controls::IItemContainerMapping, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ListViewBase(std::nullptr_t) noexcept {}
        ListViewBase(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IListViewBase(ptr, take_ownership_from_abi) {}
        [[nodiscard]] static auto SelectionModeProperty();
        [[nodiscard]] static auto IsSwipeEnabledProperty();
        [[nodiscard]] static auto CanDragItemsProperty();
        [[nodiscard]] static auto CanReorderItemsProperty();
        [[nodiscard]] static auto IsItemClickEnabledProperty();
        [[nodiscard]] static auto DataFetchSizeProperty();
        [[nodiscard]] static auto IncrementalLoadingThresholdProperty();
        [[nodiscard]] static auto IncrementalLoadingTriggerProperty();
        [[nodiscard]] static auto SemanticZoomOwnerProperty();
        [[nodiscard]] static auto IsActiveViewProperty();
        [[nodiscard]] static auto IsZoomedInViewProperty();
        [[nodiscard]] static auto HeaderProperty();
        [[nodiscard]] static auto HeaderTemplateProperty();
        [[nodiscard]] static auto HeaderTransitionsProperty();
        [[nodiscard]] static auto ShowsScrollingPlaceholdersProperty();
        [[nodiscard]] static auto FooterProperty();
        [[nodiscard]] static auto FooterTemplateProperty();
        [[nodiscard]] static auto FooterTransitionsProperty();
        [[nodiscard]] static auto ReorderModeProperty();
        [[nodiscard]] static auto IsMultiSelectCheckBoxEnabledProperty();
        [[nodiscard]] static auto SingleSelectionFollowsFocusProperty();
    };
    struct __declspec(empty_bases) ListViewBaseHeaderItem : llm::OS::UI::Xaml::Controls::IListViewBaseHeaderItem,
        impl::base<ListViewBaseHeaderItem, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ListViewBaseHeaderItem, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ListViewBaseHeaderItem(std::nullptr_t) noexcept {}
        ListViewBaseHeaderItem(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IListViewBaseHeaderItem(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ListViewHeaderItem : llm::OS::UI::Xaml::Controls::IListViewHeaderItem,
        impl::base<ListViewHeaderItem, llm::OS::UI::Xaml::Controls::ListViewBaseHeaderItem, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ListViewHeaderItem, llm::OS::UI::Xaml::Controls::IListViewBaseHeaderItem, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ListViewHeaderItem(std::nullptr_t) noexcept {}
        ListViewHeaderItem(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IListViewHeaderItem(ptr, take_ownership_from_abi) {}
        ListViewHeaderItem();
    };
    struct __declspec(empty_bases) ListViewItem : llm::OS::UI::Xaml::Controls::IListViewItem,
        impl::base<ListViewItem, llm::OS::UI::Xaml::Controls::Primitives::SelectorItem, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ListViewItem, llm::OS::UI::Xaml::Controls::Primitives::ISelectorItem, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ListViewItem(std::nullptr_t) noexcept {}
        ListViewItem(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IListViewItem(ptr, take_ownership_from_abi) {}
        ListViewItem();
    };
    struct __declspec(empty_bases) ListViewPersistenceHelper : llm::OS::UI::Xaml::Controls::IListViewPersistenceHelper
    {
        ListViewPersistenceHelper(std::nullptr_t) noexcept {}
        ListViewPersistenceHelper(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IListViewPersistenceHelper(ptr, take_ownership_from_abi) {}
        static auto GetRelativeScrollPosition(llm::OS::UI::Xaml::Controls::ListViewBase const& listViewBase, llm::OS::UI::Xaml::Controls::ListViewItemToKeyHandler const& itemToKeyHandler);
        static auto SetRelativeScrollPositionAsync(llm::OS::UI::Xaml::Controls::ListViewBase const& listViewBase, param::hstring const& relativeScrollPosition, llm::OS::UI::Xaml::Controls::ListViewKeyToItemHandler const& keyToItemHandler);
    };
    struct __declspec(empty_bases) MediaElement : llm::OS::UI::Xaml::Controls::IMediaElement,
        impl::base<MediaElement, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<MediaElement, llm::OS::UI::Xaml::Controls::IMediaElement2, llm::OS::UI::Xaml::Controls::IMediaElement3, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        MediaElement(std::nullptr_t) noexcept {}
        MediaElement(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IMediaElement(ptr, take_ownership_from_abi) {}
        MediaElement();
        [[nodiscard]] static auto PosterSourceProperty();
        [[nodiscard]] static auto SourceProperty();
        [[nodiscard]] static auto IsMutedProperty();
        [[nodiscard]] static auto IsAudioOnlyProperty();
        [[nodiscard]] static auto AutoPlayProperty();
        [[nodiscard]] static auto VolumeProperty();
        [[nodiscard]] static auto BalanceProperty();
        [[nodiscard]] static auto NaturalVideoHeightProperty();
        [[nodiscard]] static auto NaturalVideoWidthProperty();
        [[nodiscard]] static auto NaturalDurationProperty();
        [[nodiscard]] static auto PositionProperty();
        [[nodiscard]] static auto DownloadProgressProperty();
        [[nodiscard]] static auto BufferingProgressProperty();
        [[nodiscard]] static auto DownloadProgressOffsetProperty();
        [[nodiscard]] static auto CurrentStateProperty();
        [[nodiscard]] static auto CanSeekProperty();
        [[nodiscard]] static auto CanPauseProperty();
        [[nodiscard]] static auto AudioStreamCountProperty();
        [[nodiscard]] static auto AudioStreamIndexProperty();
        [[nodiscard]] static auto PlaybackRateProperty();
        [[nodiscard]] static auto IsLoopingProperty();
        [[nodiscard]] static auto PlayToSourceProperty();
        [[nodiscard]] static auto DefaultPlaybackRateProperty();
        [[nodiscard]] static auto AspectRatioWidthProperty();
        [[nodiscard]] static auto AspectRatioHeightProperty();
        [[nodiscard]] static auto RealTimePlaybackProperty();
        [[nodiscard]] static auto AudioCategoryProperty();
        [[nodiscard]] static auto AudioDeviceTypeProperty();
        [[nodiscard]] static auto ProtectionManagerProperty();
        [[nodiscard]] static auto Stereo3DVideoPackingModeProperty();
        [[nodiscard]] static auto Stereo3DVideoRenderModeProperty();
        [[nodiscard]] static auto IsStereo3DVideoProperty();
        [[nodiscard]] static auto ActualStereo3DVideoPackingModeProperty();
        [[nodiscard]] static auto AreTransportControlsEnabledProperty();
        [[nodiscard]] static auto StretchProperty();
        [[nodiscard]] static auto IsFullWindowProperty();
        [[nodiscard]] static auto PlayToPreferredSourceUriProperty();
    };
    struct __declspec(empty_bases) MediaPlayerElement : llm::OS::UI::Xaml::Controls::IMediaPlayerElement,
        impl::base<MediaPlayerElement, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<MediaPlayerElement, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        MediaPlayerElement(std::nullptr_t) noexcept {}
        MediaPlayerElement(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IMediaPlayerElement(ptr, take_ownership_from_abi) {}
        MediaPlayerElement();
        [[nodiscard]] static auto SourceProperty();
        [[nodiscard]] static auto AreTransportControlsEnabledProperty();
        [[nodiscard]] static auto PosterSourceProperty();
        [[nodiscard]] static auto StretchProperty();
        [[nodiscard]] static auto AutoPlayProperty();
        [[nodiscard]] static auto IsFullWindowProperty();
        [[nodiscard]] static auto MediaPlayerProperty();
    };
    struct __declspec(empty_bases) MediaPlayerPresenter : llm::OS::UI::Xaml::Controls::IMediaPlayerPresenter,
        impl::base<MediaPlayerPresenter, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<MediaPlayerPresenter, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        MediaPlayerPresenter(std::nullptr_t) noexcept {}
        MediaPlayerPresenter(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IMediaPlayerPresenter(ptr, take_ownership_from_abi) {}
        MediaPlayerPresenter();
        [[nodiscard]] static auto MediaPlayerProperty();
        [[nodiscard]] static auto StretchProperty();
        [[nodiscard]] static auto IsFullWindowProperty();
    };
    struct __declspec(empty_bases) MediaTransportControls : llm::OS::UI::Xaml::Controls::IMediaTransportControls,
        impl::base<MediaTransportControls, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<MediaTransportControls, llm::OS::UI::Xaml::Controls::IMediaTransportControls2, llm::OS::UI::Xaml::Controls::IMediaTransportControls3, llm::OS::UI::Xaml::Controls::IMediaTransportControls4, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        MediaTransportControls(std::nullptr_t) noexcept {}
        MediaTransportControls(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IMediaTransportControls(ptr, take_ownership_from_abi) {}
        MediaTransportControls();
        [[nodiscard]] static auto IsFullWindowButtonVisibleProperty();
        [[nodiscard]] static auto IsFullWindowEnabledProperty();
        [[nodiscard]] static auto IsZoomButtonVisibleProperty();
        [[nodiscard]] static auto IsZoomEnabledProperty();
        [[nodiscard]] static auto IsFastForwardButtonVisibleProperty();
        [[nodiscard]] static auto IsFastForwardEnabledProperty();
        [[nodiscard]] static auto IsFastRewindButtonVisibleProperty();
        [[nodiscard]] static auto IsFastRewindEnabledProperty();
        [[nodiscard]] static auto IsStopButtonVisibleProperty();
        [[nodiscard]] static auto IsStopEnabledProperty();
        [[nodiscard]] static auto IsVolumeButtonVisibleProperty();
        [[nodiscard]] static auto IsVolumeEnabledProperty();
        [[nodiscard]] static auto IsPlaybackRateButtonVisibleProperty();
        [[nodiscard]] static auto IsPlaybackRateEnabledProperty();
        [[nodiscard]] static auto IsSeekBarVisibleProperty();
        [[nodiscard]] static auto IsSeekEnabledProperty();
        [[nodiscard]] static auto IsCompactProperty();
        [[nodiscard]] static auto IsSkipForwardButtonVisibleProperty();
        [[nodiscard]] static auto IsSkipForwardEnabledProperty();
        [[nodiscard]] static auto IsSkipBackwardButtonVisibleProperty();
        [[nodiscard]] static auto IsSkipBackwardEnabledProperty();
        [[nodiscard]] static auto IsNextTrackButtonVisibleProperty();
        [[nodiscard]] static auto IsPreviousTrackButtonVisibleProperty();
        [[nodiscard]] static auto FastPlayFallbackBehaviourProperty();
        [[nodiscard]] static auto ShowAndHideAutomaticallyProperty();
        [[nodiscard]] static auto IsRepeatEnabledProperty();
        [[nodiscard]] static auto IsRepeatButtonVisibleProperty();
        [[nodiscard]] static auto IsCompactOverlayButtonVisibleProperty();
        [[nodiscard]] static auto IsCompactOverlayEnabledProperty();
    };
    struct __declspec(empty_bases) MediaTransportControlsHelper : llm::OS::UI::Xaml::Controls::IMediaTransportControlsHelper
    {
        MediaTransportControlsHelper(std::nullptr_t) noexcept {}
        MediaTransportControlsHelper(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IMediaTransportControlsHelper(ptr, take_ownership_from_abi) {}
        [[nodiscard]] static auto DropoutOrderProperty();
        static auto GetDropoutOrder(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetDropoutOrder(llm::OS::UI::Xaml::UIElement const& element, llm::OS::Foundation::IReference<int32_t> const& value);
    };
    struct __declspec(empty_bases) MenuBar : llm::OS::UI::Xaml::Controls::IMenuBar,
        impl::base<MenuBar, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<MenuBar, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        MenuBar(std::nullptr_t) noexcept {}
        MenuBar(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IMenuBar(ptr, take_ownership_from_abi) {}
        MenuBar();
        [[nodiscard]] static auto ItemsProperty();
    };
    struct __declspec(empty_bases) MenuBarItem : llm::OS::UI::Xaml::Controls::IMenuBarItem,
        impl::base<MenuBarItem, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<MenuBarItem, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        MenuBarItem(std::nullptr_t) noexcept {}
        MenuBarItem(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IMenuBarItem(ptr, take_ownership_from_abi) {}
        MenuBarItem();
        [[nodiscard]] static auto TitleProperty();
        [[nodiscard]] static auto ItemsProperty();
    };
    struct __declspec(empty_bases) MenuBarItemFlyout : llm::OS::UI::Xaml::Controls::IMenuBarItemFlyout,
        impl::base<MenuBarItemFlyout, llm::OS::UI::Xaml::Controls::MenuFlyout, llm::OS::UI::Xaml::Controls::Primitives::FlyoutBase, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<MenuBarItemFlyout, llm::OS::UI::Xaml::Controls::IMenuFlyout, llm::OS::UI::Xaml::Controls::IMenuFlyout2, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase2, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase3, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase4, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase5, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase6, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBaseOverrides, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBaseOverrides4, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        MenuBarItemFlyout(std::nullptr_t) noexcept {}
        MenuBarItemFlyout(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IMenuBarItemFlyout(ptr, take_ownership_from_abi) {}
        MenuBarItemFlyout();
        using impl::consume_t<MenuBarItemFlyout, llm::OS::UI::Xaml::Controls::IMenuFlyout2>::ShowAt;
        using impl::consume_t<MenuBarItemFlyout, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase>::ShowAt;
        using impl::consume_t<MenuBarItemFlyout, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase5>::ShowAt;
    };
    struct __declspec(empty_bases) MenuFlyout : llm::OS::UI::Xaml::Controls::IMenuFlyout,
        impl::base<MenuFlyout, llm::OS::UI::Xaml::Controls::Primitives::FlyoutBase, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<MenuFlyout, llm::OS::UI::Xaml::Controls::IMenuFlyout2, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase2, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase3, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase4, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase5, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase6, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBaseOverrides, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBaseOverrides4, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        MenuFlyout(std::nullptr_t) noexcept {}
        MenuFlyout(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IMenuFlyout(ptr, take_ownership_from_abi) {}
        MenuFlyout();
        using impl::consume_t<MenuFlyout, llm::OS::UI::Xaml::Controls::IMenuFlyout2>::ShowAt;
        using impl::consume_t<MenuFlyout, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase>::ShowAt;
        using impl::consume_t<MenuFlyout, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase5>::ShowAt;
        [[nodiscard]] static auto MenuFlyoutPresenterStyleProperty();
    };
    struct __declspec(empty_bases) MenuFlyoutItem : llm::OS::UI::Xaml::Controls::IMenuFlyoutItem,
        impl::base<MenuFlyoutItem, llm::OS::UI::Xaml::Controls::MenuFlyoutItemBase, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<MenuFlyoutItem, llm::OS::UI::Xaml::Controls::IMenuFlyoutItem2, llm::OS::UI::Xaml::Controls::IMenuFlyoutItem3, llm::OS::UI::Xaml::Controls::IMenuFlyoutItemBase, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        MenuFlyoutItem(std::nullptr_t) noexcept {}
        MenuFlyoutItem(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IMenuFlyoutItem(ptr, take_ownership_from_abi) {}
        MenuFlyoutItem();
        [[nodiscard]] static auto TextProperty();
        [[nodiscard]] static auto CommandProperty();
        [[nodiscard]] static auto CommandParameterProperty();
        [[nodiscard]] static auto IconProperty();
        [[nodiscard]] static auto KeyboardAcceleratorTextOverrideProperty();
    };
    struct __declspec(empty_bases) MenuFlyoutItemBase : llm::OS::UI::Xaml::Controls::IMenuFlyoutItemBase,
        impl::base<MenuFlyoutItemBase, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<MenuFlyoutItemBase, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        MenuFlyoutItemBase(std::nullptr_t) noexcept {}
        MenuFlyoutItemBase(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IMenuFlyoutItemBase(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) MenuFlyoutPresenter : llm::OS::UI::Xaml::Controls::IMenuFlyoutPresenter,
        impl::base<MenuFlyoutPresenter, llm::OS::UI::Xaml::Controls::ItemsControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<MenuFlyoutPresenter, llm::OS::UI::Xaml::Controls::IMenuFlyoutPresenter2, llm::OS::UI::Xaml::Controls::IMenuFlyoutPresenter3, llm::OS::UI::Xaml::Controls::IItemsControl, llm::OS::UI::Xaml::Controls::IItemsControl2, llm::OS::UI::Xaml::Controls::IItemsControl3, llm::OS::UI::Xaml::Controls::IItemsControlOverrides, llm::OS::UI::Xaml::Controls::IItemContainerMapping, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        MenuFlyoutPresenter(std::nullptr_t) noexcept {}
        MenuFlyoutPresenter(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IMenuFlyoutPresenter(ptr, take_ownership_from_abi) {}
        MenuFlyoutPresenter();
        [[nodiscard]] static auto IsDefaultShadowEnabledProperty();
    };
    struct __declspec(empty_bases) MenuFlyoutSeparator : llm::OS::UI::Xaml::Controls::IMenuFlyoutSeparator,
        impl::base<MenuFlyoutSeparator, llm::OS::UI::Xaml::Controls::MenuFlyoutItemBase, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<MenuFlyoutSeparator, llm::OS::UI::Xaml::Controls::IMenuFlyoutItemBase, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        MenuFlyoutSeparator(std::nullptr_t) noexcept {}
        MenuFlyoutSeparator(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IMenuFlyoutSeparator(ptr, take_ownership_from_abi) {}
        MenuFlyoutSeparator();
    };
    struct __declspec(empty_bases) MenuFlyoutSubItem : llm::OS::UI::Xaml::Controls::IMenuFlyoutSubItem,
        impl::base<MenuFlyoutSubItem, llm::OS::UI::Xaml::Controls::MenuFlyoutItemBase, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<MenuFlyoutSubItem, llm::OS::UI::Xaml::Controls::IMenuFlyoutSubItem2, llm::OS::UI::Xaml::Controls::IMenuFlyoutItemBase, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        MenuFlyoutSubItem(std::nullptr_t) noexcept {}
        MenuFlyoutSubItem(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IMenuFlyoutSubItem(ptr, take_ownership_from_abi) {}
        MenuFlyoutSubItem();
        [[nodiscard]] static auto TextProperty();
        [[nodiscard]] static auto IconProperty();
    };
    struct __declspec(empty_bases) NavigationView : llm::OS::UI::Xaml::Controls::INavigationView,
        impl::base<NavigationView, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<NavigationView, llm::OS::UI::Xaml::Controls::INavigationView2, llm::OS::UI::Xaml::Controls::INavigationView3, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        NavigationView(std::nullptr_t) noexcept {}
        NavigationView(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::INavigationView(ptr, take_ownership_from_abi) {}
        NavigationView();
        [[nodiscard]] static auto IsPaneOpenProperty();
        [[nodiscard]] static auto CompactModeThresholdWidthProperty();
        [[nodiscard]] static auto ExpandedModeThresholdWidthProperty();
        [[nodiscard]] static auto PaneFooterProperty();
        [[nodiscard]] static auto HeaderProperty();
        [[nodiscard]] static auto HeaderTemplateProperty();
        [[nodiscard]] static auto DisplayModeProperty();
        [[nodiscard]] static auto IsSettingsVisibleProperty();
        [[nodiscard]] static auto IsPaneToggleButtonVisibleProperty();
        [[nodiscard]] static auto AlwaysShowHeaderProperty();
        [[nodiscard]] static auto CompactPaneLengthProperty();
        [[nodiscard]] static auto OpenPaneLengthProperty();
        [[nodiscard]] static auto PaneToggleButtonStyleProperty();
        [[nodiscard]] static auto MenuItemsProperty();
        [[nodiscard]] static auto MenuItemsSourceProperty();
        [[nodiscard]] static auto SelectedItemProperty();
        [[nodiscard]] static auto SettingsItemProperty();
        [[nodiscard]] static auto AutoSuggestBoxProperty();
        [[nodiscard]] static auto MenuItemTemplateProperty();
        [[nodiscard]] static auto MenuItemTemplateSelectorProperty();
        [[nodiscard]] static auto MenuItemContainerStyleProperty();
        [[nodiscard]] static auto MenuItemContainerStyleSelectorProperty();
        [[nodiscard]] static auto IsBackButtonVisibleProperty();
        [[nodiscard]] static auto IsBackEnabledProperty();
        [[nodiscard]] static auto PaneTitleProperty();
        [[nodiscard]] static auto PaneDisplayModeProperty();
        [[nodiscard]] static auto PaneHeaderProperty();
        [[nodiscard]] static auto PaneCustomContentProperty();
        [[nodiscard]] static auto ContentOverlayProperty();
        [[nodiscard]] static auto IsPaneVisibleProperty();
        [[nodiscard]] static auto SelectionFollowsFocusProperty();
        [[nodiscard]] static auto TemplateSettingsProperty();
        [[nodiscard]] static auto ShoulderNavigationEnabledProperty();
        [[nodiscard]] static auto OverflowLabelModeProperty();
    };
    struct __declspec(empty_bases) NavigationViewBackRequestedEventArgs : llm::OS::UI::Xaml::Controls::INavigationViewBackRequestedEventArgs
    {
        NavigationViewBackRequestedEventArgs(std::nullptr_t) noexcept {}
        NavigationViewBackRequestedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::INavigationViewBackRequestedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) NavigationViewDisplayModeChangedEventArgs : llm::OS::UI::Xaml::Controls::INavigationViewDisplayModeChangedEventArgs
    {
        NavigationViewDisplayModeChangedEventArgs(std::nullptr_t) noexcept {}
        NavigationViewDisplayModeChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::INavigationViewDisplayModeChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) NavigationViewItem : llm::OS::UI::Xaml::Controls::INavigationViewItem,
        impl::base<NavigationViewItem, llm::OS::UI::Xaml::Controls::NavigationViewItemBase, llm::OS::UI::Xaml::Controls::ListViewItem, llm::OS::UI::Xaml::Controls::Primitives::SelectorItem, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<NavigationViewItem, llm::OS::UI::Xaml::Controls::INavigationViewItem2, llm::OS::UI::Xaml::Controls::INavigationViewItemBase, llm::OS::UI::Xaml::Controls::IListViewItem, llm::OS::UI::Xaml::Controls::Primitives::ISelectorItem, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        NavigationViewItem(std::nullptr_t) noexcept {}
        NavigationViewItem(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::INavigationViewItem(ptr, take_ownership_from_abi) {}
        NavigationViewItem();
        [[nodiscard]] static auto IconProperty();
        [[nodiscard]] static auto CompactPaneLengthProperty();
        [[nodiscard]] static auto SelectsOnInvokedProperty();
    };
    struct __declspec(empty_bases) NavigationViewItemBase : llm::OS::UI::Xaml::Controls::INavigationViewItemBase,
        impl::base<NavigationViewItemBase, llm::OS::UI::Xaml::Controls::ListViewItem, llm::OS::UI::Xaml::Controls::Primitives::SelectorItem, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<NavigationViewItemBase, llm::OS::UI::Xaml::Controls::IListViewItem, llm::OS::UI::Xaml::Controls::Primitives::ISelectorItem, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        NavigationViewItemBase(std::nullptr_t) noexcept {}
        NavigationViewItemBase(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::INavigationViewItemBase(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) NavigationViewItemHeader : llm::OS::UI::Xaml::Controls::INavigationViewItemHeader,
        impl::base<NavigationViewItemHeader, llm::OS::UI::Xaml::Controls::NavigationViewItemBase, llm::OS::UI::Xaml::Controls::ListViewItem, llm::OS::UI::Xaml::Controls::Primitives::SelectorItem, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<NavigationViewItemHeader, llm::OS::UI::Xaml::Controls::INavigationViewItemBase, llm::OS::UI::Xaml::Controls::IListViewItem, llm::OS::UI::Xaml::Controls::Primitives::ISelectorItem, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        NavigationViewItemHeader(std::nullptr_t) noexcept {}
        NavigationViewItemHeader(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::INavigationViewItemHeader(ptr, take_ownership_from_abi) {}
        NavigationViewItemHeader();
    };
    struct __declspec(empty_bases) NavigationViewItemInvokedEventArgs : llm::OS::UI::Xaml::Controls::INavigationViewItemInvokedEventArgs,
        impl::require<NavigationViewItemInvokedEventArgs, llm::OS::UI::Xaml::Controls::INavigationViewItemInvokedEventArgs2>
    {
        NavigationViewItemInvokedEventArgs(std::nullptr_t) noexcept {}
        NavigationViewItemInvokedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::INavigationViewItemInvokedEventArgs(ptr, take_ownership_from_abi) {}
        NavigationViewItemInvokedEventArgs();
    };
    struct __declspec(empty_bases) NavigationViewItemSeparator : llm::OS::UI::Xaml::Controls::INavigationViewItemSeparator,
        impl::base<NavigationViewItemSeparator, llm::OS::UI::Xaml::Controls::NavigationViewItemBase, llm::OS::UI::Xaml::Controls::ListViewItem, llm::OS::UI::Xaml::Controls::Primitives::SelectorItem, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<NavigationViewItemSeparator, llm::OS::UI::Xaml::Controls::INavigationViewItemBase, llm::OS::UI::Xaml::Controls::IListViewItem, llm::OS::UI::Xaml::Controls::Primitives::ISelectorItem, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        NavigationViewItemSeparator(std::nullptr_t) noexcept {}
        NavigationViewItemSeparator(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::INavigationViewItemSeparator(ptr, take_ownership_from_abi) {}
        NavigationViewItemSeparator();
    };
    struct __declspec(empty_bases) NavigationViewList : llm::OS::UI::Xaml::Controls::INavigationViewList,
        impl::base<NavigationViewList, llm::OS::UI::Xaml::Controls::ListView, llm::OS::UI::Xaml::Controls::ListViewBase, llm::OS::UI::Xaml::Controls::Primitives::Selector, llm::OS::UI::Xaml::Controls::ItemsControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<NavigationViewList, llm::OS::UI::Xaml::Controls::IListView, llm::OS::UI::Xaml::Controls::IListViewBase, llm::OS::UI::Xaml::Controls::IListViewBase2, llm::OS::UI::Xaml::Controls::IListViewBase3, llm::OS::UI::Xaml::Controls::IListViewBase4, llm::OS::UI::Xaml::Controls::IListViewBase5, llm::OS::UI::Xaml::Controls::IListViewBase6, llm::OS::UI::Xaml::Controls::ISemanticZoomInformation, llm::OS::UI::Xaml::Controls::Primitives::ISelector, llm::OS::UI::Xaml::Controls::IItemsControl, llm::OS::UI::Xaml::Controls::IItemsControl2, llm::OS::UI::Xaml::Controls::IItemsControl3, llm::OS::UI::Xaml::Controls::IItemsControlOverrides, llm::OS::UI::Xaml::Controls::IItemContainerMapping, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        NavigationViewList(std::nullptr_t) noexcept {}
        NavigationViewList(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::INavigationViewList(ptr, take_ownership_from_abi) {}
        NavigationViewList();
    };
    struct __declspec(empty_bases) NavigationViewPaneClosingEventArgs : llm::OS::UI::Xaml::Controls::INavigationViewPaneClosingEventArgs
    {
        NavigationViewPaneClosingEventArgs(std::nullptr_t) noexcept {}
        NavigationViewPaneClosingEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::INavigationViewPaneClosingEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) NavigationViewSelectionChangedEventArgs : llm::OS::UI::Xaml::Controls::INavigationViewSelectionChangedEventArgs,
        impl::require<NavigationViewSelectionChangedEventArgs, llm::OS::UI::Xaml::Controls::INavigationViewSelectionChangedEventArgs2>
    {
        NavigationViewSelectionChangedEventArgs(std::nullptr_t) noexcept {}
        NavigationViewSelectionChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::INavigationViewSelectionChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) NavigationViewTemplateSettings : llm::OS::UI::Xaml::Controls::INavigationViewTemplateSettings,
        impl::base<NavigationViewTemplateSettings, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<NavigationViewTemplateSettings, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        NavigationViewTemplateSettings(std::nullptr_t) noexcept {}
        NavigationViewTemplateSettings(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::INavigationViewTemplateSettings(ptr, take_ownership_from_abi) {}
        NavigationViewTemplateSettings();
        [[nodiscard]] static auto TopPaddingProperty();
        [[nodiscard]] static auto OverflowButtonVisibilityProperty();
        [[nodiscard]] static auto PaneToggleButtonVisibilityProperty();
        [[nodiscard]] static auto BackButtonVisibilityProperty();
        [[nodiscard]] static auto TopPaneVisibilityProperty();
        [[nodiscard]] static auto LeftPaneVisibilityProperty();
        [[nodiscard]] static auto SingleSelectionFollowsFocusProperty();
    };
    struct __declspec(empty_bases) NotifyEventArgs : llm::OS::UI::Xaml::Controls::INotifyEventArgs,
        impl::require<NotifyEventArgs, llm::OS::UI::Xaml::Controls::INotifyEventArgs2>
    {
        NotifyEventArgs(std::nullptr_t) noexcept {}
        NotifyEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::INotifyEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) Page : llm::OS::UI::Xaml::Controls::IPage,
        impl::base<Page, llm::OS::UI::Xaml::Controls::UserControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<Page, llm::OS::UI::Xaml::Controls::IPageOverrides, llm::OS::UI::Xaml::Controls::IUserControl, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        Page(std::nullptr_t) noexcept {}
        Page(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IPage(ptr, take_ownership_from_abi) {}
        Page();
        [[nodiscard]] static auto FrameProperty();
        [[nodiscard]] static auto TopAppBarProperty();
        [[nodiscard]] static auto BottomAppBarProperty();
    };
    struct __declspec(empty_bases) Panel : llm::OS::UI::Xaml::Controls::IPanel,
        impl::base<Panel, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<Panel, llm::OS::UI::Xaml::Controls::IPanel2, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        Panel(std::nullptr_t) noexcept {}
        Panel(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IPanel(ptr, take_ownership_from_abi) {}
        [[nodiscard]] static auto BackgroundProperty();
        [[nodiscard]] static auto IsItemsHostProperty();
        [[nodiscard]] static auto ChildrenTransitionsProperty();
    };
    struct __declspec(empty_bases) ParallaxView : llm::OS::UI::Xaml::Controls::IParallaxView,
        impl::base<ParallaxView, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ParallaxView, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ParallaxView(std::nullptr_t) noexcept {}
        ParallaxView(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IParallaxView(ptr, take_ownership_from_abi) {}
        ParallaxView();
        [[nodiscard]] static auto ChildProperty();
        [[nodiscard]] static auto HorizontalSourceEndOffsetProperty();
        [[nodiscard]] static auto HorizontalSourceOffsetKindProperty();
        [[nodiscard]] static auto HorizontalSourceStartOffsetProperty();
        [[nodiscard]] static auto MaxHorizontalShiftRatioProperty();
        [[nodiscard]] static auto HorizontalShiftProperty();
        [[nodiscard]] static auto IsHorizontalShiftClampedProperty();
        [[nodiscard]] static auto IsVerticalShiftClampedProperty();
        [[nodiscard]] static auto SourceProperty();
        [[nodiscard]] static auto VerticalSourceEndOffsetProperty();
        [[nodiscard]] static auto VerticalSourceOffsetKindProperty();
        [[nodiscard]] static auto VerticalSourceStartOffsetProperty();
        [[nodiscard]] static auto MaxVerticalShiftRatioProperty();
        [[nodiscard]] static auto VerticalShiftProperty();
    };
    struct __declspec(empty_bases) PasswordBox : llm::OS::UI::Xaml::Controls::IPasswordBox,
        impl::base<PasswordBox, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<PasswordBox, llm::OS::UI::Xaml::Controls::IPasswordBox2, llm::OS::UI::Xaml::Controls::IPasswordBox3, llm::OS::UI::Xaml::Controls::IPasswordBox4, llm::OS::UI::Xaml::Controls::IPasswordBox5, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        PasswordBox(std::nullptr_t) noexcept {}
        PasswordBox(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IPasswordBox(ptr, take_ownership_from_abi) {}
        PasswordBox();
        [[nodiscard]] static auto PasswordProperty();
        [[nodiscard]] static auto PasswordCharProperty();
        [[nodiscard]] static auto IsPasswordRevealButtonEnabledProperty();
        [[nodiscard]] static auto MaxLengthProperty();
        [[nodiscard]] static auto HeaderProperty();
        [[nodiscard]] static auto HeaderTemplateProperty();
        [[nodiscard]] static auto PlaceholderTextProperty();
        [[nodiscard]] static auto SelectionHighlightColorProperty();
        [[nodiscard]] static auto PreventKeyboardDisplayOnProgrammaticFocusProperty();
        [[nodiscard]] static auto PasswordRevealModeProperty();
        [[nodiscard]] static auto TextReadingOrderProperty();
        [[nodiscard]] static auto InputScopeProperty();
        [[nodiscard]] static auto CanPasteClipboardContentProperty();
        [[nodiscard]] static auto SelectionFlyoutProperty();
        [[nodiscard]] static auto DescriptionProperty();
    };
    struct __declspec(empty_bases) PasswordBoxPasswordChangingEventArgs : llm::OS::UI::Xaml::Controls::IPasswordBoxPasswordChangingEventArgs
    {
        PasswordBoxPasswordChangingEventArgs(std::nullptr_t) noexcept {}
        PasswordBoxPasswordChangingEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IPasswordBoxPasswordChangingEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) PathIcon : llm::OS::UI::Xaml::Controls::IPathIcon,
        impl::base<PathIcon, llm::OS::UI::Xaml::Controls::IconElement, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<PathIcon, llm::OS::UI::Xaml::Controls::IIconElement, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        PathIcon(std::nullptr_t) noexcept {}
        PathIcon(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IPathIcon(ptr, take_ownership_from_abi) {}
        PathIcon();
        [[nodiscard]] static auto DataProperty();
    };
    struct __declspec(empty_bases) PathIconSource : llm::OS::UI::Xaml::Controls::IPathIconSource,
        impl::base<PathIconSource, llm::OS::UI::Xaml::Controls::IconSource, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<PathIconSource, llm::OS::UI::Xaml::Controls::IIconSource, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        PathIconSource(std::nullptr_t) noexcept {}
        PathIconSource(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IPathIconSource(ptr, take_ownership_from_abi) {}
        PathIconSource();
        [[nodiscard]] static auto DataProperty();
    };
    struct __declspec(empty_bases) PersonPicture : llm::OS::UI::Xaml::Controls::IPersonPicture,
        impl::base<PersonPicture, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<PersonPicture, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        PersonPicture(std::nullptr_t) noexcept {}
        PersonPicture(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IPersonPicture(ptr, take_ownership_from_abi) {}
        PersonPicture();
        [[nodiscard]] static auto BadgeNumberProperty();
        [[nodiscard]] static auto BadgeGlyphProperty();
        [[nodiscard]] static auto BadgeImageSourceProperty();
        [[nodiscard]] static auto BadgeTextProperty();
        [[nodiscard]] static auto IsGroupProperty();
        [[nodiscard]] static auto ContactProperty();
        [[nodiscard]] static auto DisplayNameProperty();
        [[nodiscard]] static auto InitialsProperty();
        [[nodiscard]] static auto PreferSmallImageProperty();
        [[nodiscard]] static auto ProfilePictureProperty();
    };
    struct __declspec(empty_bases) PickerConfirmedEventArgs : llm::OS::UI::Xaml::Controls::IPickerConfirmedEventArgs,
        impl::base<PickerConfirmedEventArgs, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<PickerConfirmedEventArgs, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        PickerConfirmedEventArgs(std::nullptr_t) noexcept {}
        PickerConfirmedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IPickerConfirmedEventArgs(ptr, take_ownership_from_abi) {}
        PickerConfirmedEventArgs();
    };
    struct __declspec(empty_bases) PickerFlyout : llm::OS::UI::Xaml::Controls::IPickerFlyout,
        impl::base<PickerFlyout, llm::OS::UI::Xaml::Controls::Primitives::PickerFlyoutBase, llm::OS::UI::Xaml::Controls::Primitives::FlyoutBase, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<PickerFlyout, llm::OS::UI::Xaml::Controls::Primitives::IPickerFlyoutBase, llm::OS::UI::Xaml::Controls::Primitives::IPickerFlyoutBaseOverrides, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase2, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase3, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase4, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase5, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase6, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBaseOverrides, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBaseOverrides4, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        PickerFlyout(std::nullptr_t) noexcept {}
        PickerFlyout(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IPickerFlyout(ptr, take_ownership_from_abi) {}
        PickerFlyout();
        using impl::consume_t<PickerFlyout, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase>::ShowAt;
        using impl::consume_t<PickerFlyout, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase5>::ShowAt;
        [[nodiscard]] static auto ContentProperty();
        [[nodiscard]] static auto ConfirmationButtonsVisibleProperty();
    };
    struct __declspec(empty_bases) PickerFlyoutPresenter : llm::OS::UI::Xaml::Controls::IPickerFlyoutPresenter,
        impl::base<PickerFlyoutPresenter, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<PickerFlyoutPresenter, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        PickerFlyoutPresenter(std::nullptr_t) noexcept {}
        PickerFlyoutPresenter(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IPickerFlyoutPresenter(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) Pivot : llm::OS::UI::Xaml::Controls::IPivot,
        impl::base<Pivot, llm::OS::UI::Xaml::Controls::ItemsControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<Pivot, llm::OS::UI::Xaml::Controls::IPivot2, llm::OS::UI::Xaml::Controls::IPivot3, llm::OS::UI::Xaml::Controls::IItemsControl, llm::OS::UI::Xaml::Controls::IItemsControl2, llm::OS::UI::Xaml::Controls::IItemsControl3, llm::OS::UI::Xaml::Controls::IItemsControlOverrides, llm::OS::UI::Xaml::Controls::IItemContainerMapping, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        Pivot(std::nullptr_t) noexcept {}
        Pivot(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IPivot(ptr, take_ownership_from_abi) {}
        Pivot();
        [[nodiscard]] static auto TitleProperty();
        [[nodiscard]] static auto TitleTemplateProperty();
        [[nodiscard]] static auto HeaderTemplateProperty();
        [[nodiscard]] static auto SelectedIndexProperty();
        [[nodiscard]] static auto SelectedItemProperty();
        [[nodiscard]] static auto IsLockedProperty();
        [[nodiscard]] static auto SlideInAnimationGroupProperty();
        static auto GetSlideInAnimationGroup(llm::OS::UI::Xaml::FrameworkElement const& element);
        static auto SetSlideInAnimationGroup(llm::OS::UI::Xaml::FrameworkElement const& element, llm::OS::UI::Xaml::Controls::PivotSlideInAnimationGroup const& value);
        [[nodiscard]] static auto LeftHeaderProperty();
        [[nodiscard]] static auto LeftHeaderTemplateProperty();
        [[nodiscard]] static auto RightHeaderProperty();
        [[nodiscard]] static auto RightHeaderTemplateProperty();
        [[nodiscard]] static auto HeaderFocusVisualPlacementProperty();
        [[nodiscard]] static auto IsHeaderItemsCarouselEnabledProperty();
    };
    struct __declspec(empty_bases) PivotItem : llm::OS::UI::Xaml::Controls::IPivotItem,
        impl::base<PivotItem, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<PivotItem, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        PivotItem(std::nullptr_t) noexcept {}
        PivotItem(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IPivotItem(ptr, take_ownership_from_abi) {}
        PivotItem();
        [[nodiscard]] static auto HeaderProperty();
    };
    struct __declspec(empty_bases) PivotItemEventArgs : llm::OS::UI::Xaml::Controls::IPivotItemEventArgs
    {
        PivotItemEventArgs(std::nullptr_t) noexcept {}
        PivotItemEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IPivotItemEventArgs(ptr, take_ownership_from_abi) {}
        PivotItemEventArgs();
    };
    struct __declspec(empty_bases) ProgressBar : llm::OS::UI::Xaml::Controls::IProgressBar,
        impl::base<ProgressBar, llm::OS::UI::Xaml::Controls::Primitives::RangeBase, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ProgressBar, llm::OS::UI::Xaml::Controls::Primitives::IRangeBase, llm::OS::UI::Xaml::Controls::Primitives::IRangeBaseOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ProgressBar(std::nullptr_t) noexcept {}
        ProgressBar(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IProgressBar(ptr, take_ownership_from_abi) {}
        ProgressBar();
        [[nodiscard]] static auto IsIndeterminateProperty();
        [[nodiscard]] static auto ShowErrorProperty();
        [[nodiscard]] static auto ShowPausedProperty();
    };
    struct __declspec(empty_bases) ProgressRing : llm::OS::UI::Xaml::Controls::IProgressRing,
        impl::base<ProgressRing, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ProgressRing, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ProgressRing(std::nullptr_t) noexcept {}
        ProgressRing(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IProgressRing(ptr, take_ownership_from_abi) {}
        ProgressRing();
        [[nodiscard]] static auto IsActiveProperty();
    };
    struct __declspec(empty_bases) RadioButton : llm::OS::UI::Xaml::Controls::IRadioButton,
        impl::base<RadioButton, llm::OS::UI::Xaml::Controls::Primitives::ToggleButton, llm::OS::UI::Xaml::Controls::Primitives::ButtonBase, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<RadioButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButton, llm::OS::UI::Xaml::Controls::Primitives::IToggleButtonOverrides, llm::OS::UI::Xaml::Controls::Primitives::IButtonBase, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        RadioButton(std::nullptr_t) noexcept {}
        RadioButton(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IRadioButton(ptr, take_ownership_from_abi) {}
        RadioButton();
        [[nodiscard]] static auto GroupNameProperty();
    };
    struct __declspec(empty_bases) RatingControl : llm::OS::UI::Xaml::Controls::IRatingControl,
        impl::base<RatingControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<RatingControl, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        RatingControl(std::nullptr_t) noexcept {}
        RatingControl(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IRatingControl(ptr, take_ownership_from_abi) {}
        RatingControl();
        [[nodiscard]] static auto CaptionProperty();
        [[nodiscard]] static auto InitialSetValueProperty();
        [[nodiscard]] static auto IsClearEnabledProperty();
        [[nodiscard]] static auto IsReadOnlyProperty();
        [[nodiscard]] static auto MaxRatingProperty();
        [[nodiscard]] static auto PlaceholderValueProperty();
        [[nodiscard]] static auto ItemInfoProperty();
        [[nodiscard]] static auto ValueProperty();
    };
    struct __declspec(empty_bases) RatingItemFontInfo : llm::OS::UI::Xaml::Controls::IRatingItemFontInfo,
        impl::base<RatingItemFontInfo, llm::OS::UI::Xaml::Controls::RatingItemInfo, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<RatingItemFontInfo, llm::OS::UI::Xaml::Controls::IRatingItemInfo, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        RatingItemFontInfo(std::nullptr_t) noexcept {}
        RatingItemFontInfo(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IRatingItemFontInfo(ptr, take_ownership_from_abi) {}
        RatingItemFontInfo();
        [[nodiscard]] static auto DisabledGlyphProperty();
        [[nodiscard]] static auto GlyphProperty();
        [[nodiscard]] static auto PlaceholderGlyphProperty();
        [[nodiscard]] static auto PointerOverGlyphProperty();
        [[nodiscard]] static auto PointerOverPlaceholderGlyphProperty();
        [[nodiscard]] static auto UnsetGlyphProperty();
    };
    struct __declspec(empty_bases) RatingItemImageInfo : llm::OS::UI::Xaml::Controls::IRatingItemImageInfo,
        impl::base<RatingItemImageInfo, llm::OS::UI::Xaml::Controls::RatingItemInfo, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<RatingItemImageInfo, llm::OS::UI::Xaml::Controls::IRatingItemInfo, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        RatingItemImageInfo(std::nullptr_t) noexcept {}
        RatingItemImageInfo(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IRatingItemImageInfo(ptr, take_ownership_from_abi) {}
        RatingItemImageInfo();
        [[nodiscard]] static auto DisabledImageProperty();
        [[nodiscard]] static auto ImageProperty();
        [[nodiscard]] static auto PlaceholderImageProperty();
        [[nodiscard]] static auto PointerOverImageProperty();
        [[nodiscard]] static auto PointerOverPlaceholderImageProperty();
        [[nodiscard]] static auto UnsetImageProperty();
    };
    struct __declspec(empty_bases) RatingItemInfo : llm::OS::UI::Xaml::Controls::IRatingItemInfo,
        impl::base<RatingItemInfo, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<RatingItemInfo, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        RatingItemInfo(std::nullptr_t) noexcept {}
        RatingItemInfo(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IRatingItemInfo(ptr, take_ownership_from_abi) {}
        RatingItemInfo();
    };
    struct __declspec(empty_bases) RefreshContainer : llm::OS::UI::Xaml::Controls::IRefreshContainer,
        impl::base<RefreshContainer, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<RefreshContainer, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        RefreshContainer(std::nullptr_t) noexcept {}
        RefreshContainer(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IRefreshContainer(ptr, take_ownership_from_abi) {}
        RefreshContainer();
        [[nodiscard]] static auto VisualizerProperty();
        [[nodiscard]] static auto PullDirectionProperty();
    };
    struct __declspec(empty_bases) RefreshInteractionRatioChangedEventArgs : llm::OS::UI::Xaml::Controls::IRefreshInteractionRatioChangedEventArgs
    {
        RefreshInteractionRatioChangedEventArgs(std::nullptr_t) noexcept {}
        RefreshInteractionRatioChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IRefreshInteractionRatioChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) RefreshRequestedEventArgs : llm::OS::UI::Xaml::Controls::IRefreshRequestedEventArgs
    {
        RefreshRequestedEventArgs(std::nullptr_t) noexcept {}
        RefreshRequestedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IRefreshRequestedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) RefreshStateChangedEventArgs : llm::OS::UI::Xaml::Controls::IRefreshStateChangedEventArgs
    {
        RefreshStateChangedEventArgs(std::nullptr_t) noexcept {}
        RefreshStateChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IRefreshStateChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) RefreshVisualizer : llm::OS::UI::Xaml::Controls::IRefreshVisualizer,
        impl::base<RefreshVisualizer, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<RefreshVisualizer, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        RefreshVisualizer(std::nullptr_t) noexcept {}
        RefreshVisualizer(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IRefreshVisualizer(ptr, take_ownership_from_abi) {}
        RefreshVisualizer();
        [[nodiscard]] static auto InfoProviderProperty();
        [[nodiscard]] static auto OrientationProperty();
        [[nodiscard]] static auto ContentProperty();
        [[nodiscard]] static auto StateProperty();
    };
    struct __declspec(empty_bases) RelativePanel : llm::OS::UI::Xaml::Controls::IRelativePanel,
        impl::base<RelativePanel, llm::OS::UI::Xaml::Controls::Panel, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<RelativePanel, llm::OS::UI::Xaml::Controls::IRelativePanel2, llm::OS::UI::Xaml::Controls::IPanel, llm::OS::UI::Xaml::Controls::IPanel2, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        RelativePanel(std::nullptr_t) noexcept {}
        RelativePanel(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IRelativePanel(ptr, take_ownership_from_abi) {}
        RelativePanel();
        [[nodiscard]] static auto LeftOfProperty();
        static auto GetLeftOf(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetLeftOf(llm::OS::UI::Xaml::UIElement const& element, llm::OS::Foundation::IInspectable const& value);
        [[nodiscard]] static auto AboveProperty();
        static auto GetAbove(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetAbove(llm::OS::UI::Xaml::UIElement const& element, llm::OS::Foundation::IInspectable const& value);
        [[nodiscard]] static auto RightOfProperty();
        static auto GetRightOf(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetRightOf(llm::OS::UI::Xaml::UIElement const& element, llm::OS::Foundation::IInspectable const& value);
        [[nodiscard]] static auto BelowProperty();
        static auto GetBelow(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetBelow(llm::OS::UI::Xaml::UIElement const& element, llm::OS::Foundation::IInspectable const& value);
        [[nodiscard]] static auto AlignHorizontalCenterWithProperty();
        static auto GetAlignHorizontalCenterWith(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetAlignHorizontalCenterWith(llm::OS::UI::Xaml::UIElement const& element, llm::OS::Foundation::IInspectable const& value);
        [[nodiscard]] static auto AlignVerticalCenterWithProperty();
        static auto GetAlignVerticalCenterWith(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetAlignVerticalCenterWith(llm::OS::UI::Xaml::UIElement const& element, llm::OS::Foundation::IInspectable const& value);
        [[nodiscard]] static auto AlignLeftWithProperty();
        static auto GetAlignLeftWith(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetAlignLeftWith(llm::OS::UI::Xaml::UIElement const& element, llm::OS::Foundation::IInspectable const& value);
        [[nodiscard]] static auto AlignTopWithProperty();
        static auto GetAlignTopWith(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetAlignTopWith(llm::OS::UI::Xaml::UIElement const& element, llm::OS::Foundation::IInspectable const& value);
        [[nodiscard]] static auto AlignRightWithProperty();
        static auto GetAlignRightWith(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetAlignRightWith(llm::OS::UI::Xaml::UIElement const& element, llm::OS::Foundation::IInspectable const& value);
        [[nodiscard]] static auto AlignBottomWithProperty();
        static auto GetAlignBottomWith(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetAlignBottomWith(llm::OS::UI::Xaml::UIElement const& element, llm::OS::Foundation::IInspectable const& value);
        [[nodiscard]] static auto AlignLeftWithPanelProperty();
        static auto GetAlignLeftWithPanel(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetAlignLeftWithPanel(llm::OS::UI::Xaml::UIElement const& element, bool value);
        [[nodiscard]] static auto AlignTopWithPanelProperty();
        static auto GetAlignTopWithPanel(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetAlignTopWithPanel(llm::OS::UI::Xaml::UIElement const& element, bool value);
        [[nodiscard]] static auto AlignRightWithPanelProperty();
        static auto GetAlignRightWithPanel(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetAlignRightWithPanel(llm::OS::UI::Xaml::UIElement const& element, bool value);
        [[nodiscard]] static auto AlignBottomWithPanelProperty();
        static auto GetAlignBottomWithPanel(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetAlignBottomWithPanel(llm::OS::UI::Xaml::UIElement const& element, bool value);
        [[nodiscard]] static auto AlignHorizontalCenterWithPanelProperty();
        static auto GetAlignHorizontalCenterWithPanel(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetAlignHorizontalCenterWithPanel(llm::OS::UI::Xaml::UIElement const& element, bool value);
        [[nodiscard]] static auto AlignVerticalCenterWithPanelProperty();
        static auto GetAlignVerticalCenterWithPanel(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetAlignVerticalCenterWithPanel(llm::OS::UI::Xaml::UIElement const& element, bool value);
        [[nodiscard]] static auto BorderBrushProperty();
        [[nodiscard]] static auto BorderThicknessProperty();
        [[nodiscard]] static auto CornerRadiusProperty();
        [[nodiscard]] static auto PaddingProperty();
        [[nodiscard]] static auto BackgroundSizingProperty();
    };
    struct __declspec(empty_bases) RichEditBox : llm::OS::UI::Xaml::Controls::IRichEditBox,
        impl::base<RichEditBox, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<RichEditBox, llm::OS::UI::Xaml::Controls::IRichEditBox2, llm::OS::UI::Xaml::Controls::IRichEditBox3, llm::OS::UI::Xaml::Controls::IRichEditBox4, llm::OS::UI::Xaml::Controls::IRichEditBox5, llm::OS::UI::Xaml::Controls::IRichEditBox6, llm::OS::UI::Xaml::Controls::IRichEditBox7, llm::OS::UI::Xaml::Controls::IRichEditBox8, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        RichEditBox(std::nullptr_t) noexcept {}
        RichEditBox(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IRichEditBox(ptr, take_ownership_from_abi) {}
        RichEditBox();
        [[nodiscard]] static auto IsReadOnlyProperty();
        [[nodiscard]] static auto AcceptsReturnProperty();
        [[nodiscard]] static auto TextAlignmentProperty();
        [[nodiscard]] static auto TextWrappingProperty();
        [[nodiscard]] static auto IsSpellCheckEnabledProperty();
        [[nodiscard]] static auto IsTextPredictionEnabledProperty();
        [[nodiscard]] static auto InputScopeProperty();
        [[nodiscard]] static auto HeaderProperty();
        [[nodiscard]] static auto HeaderTemplateProperty();
        [[nodiscard]] static auto PlaceholderTextProperty();
        [[nodiscard]] static auto SelectionHighlightColorProperty();
        [[nodiscard]] static auto PreventKeyboardDisplayOnProgrammaticFocusProperty();
        [[nodiscard]] static auto IsColorFontEnabledProperty();
        [[nodiscard]] static auto DesiredCandidateWindowAlignmentProperty();
        [[nodiscard]] static auto TextReadingOrderProperty();
        [[nodiscard]] static auto ClipboardCopyFormatProperty();
        [[nodiscard]] static auto SelectionHighlightColorWhenNotFocusedProperty();
        [[nodiscard]] static auto MaxLengthProperty();
        [[nodiscard]] static auto HorizontalTextAlignmentProperty();
        [[nodiscard]] static auto CharacterCasingProperty();
        [[nodiscard]] static auto DisabledFormattingAcceleratorsProperty();
        [[nodiscard]] static auto ContentLinkForegroundColorProperty();
        [[nodiscard]] static auto ContentLinkBackgroundColorProperty();
        [[nodiscard]] static auto ContentLinkProvidersProperty();
        [[nodiscard]] static auto HandwritingViewProperty();
        [[nodiscard]] static auto IsHandwritingViewEnabledProperty();
        [[nodiscard]] static auto SelectionFlyoutProperty();
        [[nodiscard]] static auto ProofingMenuFlyoutProperty();
        [[nodiscard]] static auto DescriptionProperty();
    };
    struct __declspec(empty_bases) RichEditBoxSelectionChangingEventArgs : llm::OS::UI::Xaml::Controls::IRichEditBoxSelectionChangingEventArgs
    {
        RichEditBoxSelectionChangingEventArgs(std::nullptr_t) noexcept {}
        RichEditBoxSelectionChangingEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IRichEditBoxSelectionChangingEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) RichEditBoxTextChangingEventArgs : llm::OS::UI::Xaml::Controls::IRichEditBoxTextChangingEventArgs,
        impl::require<RichEditBoxTextChangingEventArgs, llm::OS::UI::Xaml::Controls::IRichEditBoxTextChangingEventArgs2>
    {
        RichEditBoxTextChangingEventArgs(std::nullptr_t) noexcept {}
        RichEditBoxTextChangingEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IRichEditBoxTextChangingEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) RichTextBlock : llm::OS::UI::Xaml::Controls::IRichTextBlock,
        impl::base<RichTextBlock, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<RichTextBlock, llm::OS::UI::Xaml::Controls::IRichTextBlock2, llm::OS::UI::Xaml::Controls::IRichTextBlock3, llm::OS::UI::Xaml::Controls::IRichTextBlock4, llm::OS::UI::Xaml::Controls::IRichTextBlock5, llm::OS::UI::Xaml::Controls::IRichTextBlock6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        RichTextBlock(std::nullptr_t) noexcept {}
        RichTextBlock(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IRichTextBlock(ptr, take_ownership_from_abi) {}
        RichTextBlock();
        [[nodiscard]] static auto FontSizeProperty();
        [[nodiscard]] static auto FontFamilyProperty();
        [[nodiscard]] static auto FontWeightProperty();
        [[nodiscard]] static auto FontStyleProperty();
        [[nodiscard]] static auto FontStretchProperty();
        [[nodiscard]] static auto ForegroundProperty();
        [[nodiscard]] static auto TextWrappingProperty();
        [[nodiscard]] static auto TextTrimmingProperty();
        [[nodiscard]] static auto TextAlignmentProperty();
        [[nodiscard]] static auto PaddingProperty();
        [[nodiscard]] static auto LineHeightProperty();
        [[nodiscard]] static auto LineStackingStrategyProperty();
        [[nodiscard]] static auto CharacterSpacingProperty();
        [[nodiscard]] static auto OverflowContentTargetProperty();
        [[nodiscard]] static auto IsTextSelectionEnabledProperty();
        [[nodiscard]] static auto HasOverflowContentProperty();
        [[nodiscard]] static auto SelectedTextProperty();
        [[nodiscard]] static auto TextIndentProperty();
        [[nodiscard]] static auto MaxLinesProperty();
        [[nodiscard]] static auto TextLineBoundsProperty();
        [[nodiscard]] static auto SelectionHighlightColorProperty();
        [[nodiscard]] static auto OpticalMarginAlignmentProperty();
        [[nodiscard]] static auto IsColorFontEnabledProperty();
        [[nodiscard]] static auto TextReadingOrderProperty();
        [[nodiscard]] static auto IsTextScaleFactorEnabledProperty();
        [[nodiscard]] static auto TextDecorationsProperty();
        [[nodiscard]] static auto IsTextTrimmedProperty();
        [[nodiscard]] static auto HorizontalTextAlignmentProperty();
        [[nodiscard]] static auto SelectionFlyoutProperty();
    };
    struct __declspec(empty_bases) RichTextBlockOverflow : llm::OS::UI::Xaml::Controls::IRichTextBlockOverflow,
        impl::base<RichTextBlockOverflow, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<RichTextBlockOverflow, llm::OS::UI::Xaml::Controls::IRichTextBlockOverflow2, llm::OS::UI::Xaml::Controls::IRichTextBlockOverflow3, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        RichTextBlockOverflow(std::nullptr_t) noexcept {}
        RichTextBlockOverflow(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IRichTextBlockOverflow(ptr, take_ownership_from_abi) {}
        RichTextBlockOverflow();
        [[nodiscard]] static auto OverflowContentTargetProperty();
        [[nodiscard]] static auto PaddingProperty();
        [[nodiscard]] static auto HasOverflowContentProperty();
        [[nodiscard]] static auto MaxLinesProperty();
        [[nodiscard]] static auto IsTextTrimmedProperty();
    };
    struct __declspec(empty_bases) RowDefinition : llm::OS::UI::Xaml::Controls::IRowDefinition,
        impl::base<RowDefinition, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<RowDefinition, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        RowDefinition(std::nullptr_t) noexcept {}
        RowDefinition(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IRowDefinition(ptr, take_ownership_from_abi) {}
        RowDefinition();
        [[nodiscard]] static auto HeightProperty();
        [[nodiscard]] static auto MaxHeightProperty();
        [[nodiscard]] static auto MinHeightProperty();
    };
    struct __declspec(empty_bases) RowDefinitionCollection : llm::OS::Foundation::Collections::IVector<llm::OS::UI::Xaml::Controls::RowDefinition>
    {
        RowDefinitionCollection(std::nullptr_t) noexcept {}
        RowDefinitionCollection(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::Foundation::Collections::IVector<llm::OS::UI::Xaml::Controls::RowDefinition>(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ScrollContentPresenter : llm::OS::UI::Xaml::Controls::IScrollContentPresenter,
        impl::base<ScrollContentPresenter, llm::OS::UI::Xaml::Controls::ContentPresenter, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ScrollContentPresenter, llm::OS::UI::Xaml::Controls::IScrollContentPresenter2, llm::OS::UI::Xaml::Controls::IContentPresenter, llm::OS::UI::Xaml::Controls::IContentPresenter2, llm::OS::UI::Xaml::Controls::IContentPresenter3, llm::OS::UI::Xaml::Controls::IContentPresenter4, llm::OS::UI::Xaml::Controls::IContentPresenter5, llm::OS::UI::Xaml::Controls::IContentPresenterOverrides, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ScrollContentPresenter(std::nullptr_t) noexcept {}
        ScrollContentPresenter(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IScrollContentPresenter(ptr, take_ownership_from_abi) {}
        ScrollContentPresenter();
        [[nodiscard]] static auto CanContentRenderOutsideBoundsProperty();
        [[nodiscard]] static auto SizesContentToTemplatedParentProperty();
    };
    struct __declspec(empty_bases) ScrollViewer : llm::OS::UI::Xaml::Controls::IScrollViewer,
        impl::base<ScrollViewer, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ScrollViewer, llm::OS::UI::Xaml::Controls::IScrollViewer2, llm::OS::UI::Xaml::Controls::IScrollViewer3, llm::OS::UI::Xaml::Controls::IScrollViewer4, llm::OS::UI::Xaml::Controls::IScrollAnchorProvider, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ScrollViewer(std::nullptr_t) noexcept {}
        ScrollViewer(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IScrollViewer(ptr, take_ownership_from_abi) {}
        ScrollViewer();
        [[nodiscard]] static auto HorizontalSnapPointsAlignmentProperty();
        [[nodiscard]] static auto VerticalSnapPointsAlignmentProperty();
        [[nodiscard]] static auto HorizontalSnapPointsTypeProperty();
        [[nodiscard]] static auto VerticalSnapPointsTypeProperty();
        [[nodiscard]] static auto ZoomSnapPointsTypeProperty();
        [[nodiscard]] static auto HorizontalOffsetProperty();
        [[nodiscard]] static auto ViewportWidthProperty();
        [[nodiscard]] static auto ScrollableWidthProperty();
        [[nodiscard]] static auto ComputedHorizontalScrollBarVisibilityProperty();
        [[nodiscard]] static auto ExtentWidthProperty();
        [[nodiscard]] static auto VerticalOffsetProperty();
        [[nodiscard]] static auto ViewportHeightProperty();
        [[nodiscard]] static auto ScrollableHeightProperty();
        [[nodiscard]] static auto ComputedVerticalScrollBarVisibilityProperty();
        [[nodiscard]] static auto ExtentHeightProperty();
        [[nodiscard]] static auto MinZoomFactorProperty();
        [[nodiscard]] static auto MaxZoomFactorProperty();
        [[nodiscard]] static auto ZoomFactorProperty();
        [[nodiscard]] static auto ZoomSnapPointsProperty();
        [[nodiscard]] static auto HorizontalScrollBarVisibilityProperty();
        static auto GetHorizontalScrollBarVisibility(llm::OS::UI::Xaml::DependencyObject const& element);
        static auto SetHorizontalScrollBarVisibility(llm::OS::UI::Xaml::DependencyObject const& element, llm::OS::UI::Xaml::Controls::ScrollBarVisibility const& horizontalScrollBarVisibility);
        [[nodiscard]] static auto VerticalScrollBarVisibilityProperty();
        static auto GetVerticalScrollBarVisibility(llm::OS::UI::Xaml::DependencyObject const& element);
        static auto SetVerticalScrollBarVisibility(llm::OS::UI::Xaml::DependencyObject const& element, llm::OS::UI::Xaml::Controls::ScrollBarVisibility const& verticalScrollBarVisibility);
        [[nodiscard]] static auto IsHorizontalRailEnabledProperty();
        static auto GetIsHorizontalRailEnabled(llm::OS::UI::Xaml::DependencyObject const& element);
        static auto SetIsHorizontalRailEnabled(llm::OS::UI::Xaml::DependencyObject const& element, bool isHorizontalRailEnabled);
        [[nodiscard]] static auto IsVerticalRailEnabledProperty();
        static auto GetIsVerticalRailEnabled(llm::OS::UI::Xaml::DependencyObject const& element);
        static auto SetIsVerticalRailEnabled(llm::OS::UI::Xaml::DependencyObject const& element, bool isVerticalRailEnabled);
        [[nodiscard]] static auto IsHorizontalScrollChainingEnabledProperty();
        static auto GetIsHorizontalScrollChainingEnabled(llm::OS::UI::Xaml::DependencyObject const& element);
        static auto SetIsHorizontalScrollChainingEnabled(llm::OS::UI::Xaml::DependencyObject const& element, bool isHorizontalScrollChainingEnabled);
        [[nodiscard]] static auto IsVerticalScrollChainingEnabledProperty();
        static auto GetIsVerticalScrollChainingEnabled(llm::OS::UI::Xaml::DependencyObject const& element);
        static auto SetIsVerticalScrollChainingEnabled(llm::OS::UI::Xaml::DependencyObject const& element, bool isVerticalScrollChainingEnabled);
        [[nodiscard]] static auto IsZoomChainingEnabledProperty();
        static auto GetIsZoomChainingEnabled(llm::OS::UI::Xaml::DependencyObject const& element);
        static auto SetIsZoomChainingEnabled(llm::OS::UI::Xaml::DependencyObject const& element, bool isZoomChainingEnabled);
        [[nodiscard]] static auto IsScrollInertiaEnabledProperty();
        static auto GetIsScrollInertiaEnabled(llm::OS::UI::Xaml::DependencyObject const& element);
        static auto SetIsScrollInertiaEnabled(llm::OS::UI::Xaml::DependencyObject const& element, bool isScrollInertiaEnabled);
        [[nodiscard]] static auto IsZoomInertiaEnabledProperty();
        static auto GetIsZoomInertiaEnabled(llm::OS::UI::Xaml::DependencyObject const& element);
        static auto SetIsZoomInertiaEnabled(llm::OS::UI::Xaml::DependencyObject const& element, bool isZoomInertiaEnabled);
        [[nodiscard]] static auto HorizontalScrollModeProperty();
        static auto GetHorizontalScrollMode(llm::OS::UI::Xaml::DependencyObject const& element);
        static auto SetHorizontalScrollMode(llm::OS::UI::Xaml::DependencyObject const& element, llm::OS::UI::Xaml::Controls::ScrollMode const& horizontalScrollMode);
        [[nodiscard]] static auto VerticalScrollModeProperty();
        static auto GetVerticalScrollMode(llm::OS::UI::Xaml::DependencyObject const& element);
        static auto SetVerticalScrollMode(llm::OS::UI::Xaml::DependencyObject const& element, llm::OS::UI::Xaml::Controls::ScrollMode const& verticalScrollMode);
        [[nodiscard]] static auto ZoomModeProperty();
        static auto GetZoomMode(llm::OS::UI::Xaml::DependencyObject const& element);
        static auto SetZoomMode(llm::OS::UI::Xaml::DependencyObject const& element, llm::OS::UI::Xaml::Controls::ZoomMode const& zoomMode);
        [[nodiscard]] static auto IsDeferredScrollingEnabledProperty();
        static auto GetIsDeferredScrollingEnabled(llm::OS::UI::Xaml::DependencyObject const& element);
        static auto SetIsDeferredScrollingEnabled(llm::OS::UI::Xaml::DependencyObject const& element, bool isDeferredScrollingEnabled);
        [[nodiscard]] static auto BringIntoViewOnFocusChangeProperty();
        static auto GetBringIntoViewOnFocusChange(llm::OS::UI::Xaml::DependencyObject const& element);
        static auto SetBringIntoViewOnFocusChange(llm::OS::UI::Xaml::DependencyObject const& element, bool bringIntoViewOnFocusChange);
        [[nodiscard]] static auto TopLeftHeaderProperty();
        [[nodiscard]] static auto LeftHeaderProperty();
        [[nodiscard]] static auto TopHeaderProperty();
        [[nodiscard]] static auto ReduceViewportForCoreInputViewOcclusionsProperty();
        [[nodiscard]] static auto HorizontalAnchorRatioProperty();
        [[nodiscard]] static auto VerticalAnchorRatioProperty();
        [[nodiscard]] static auto CanContentRenderOutsideBoundsProperty();
        static auto GetCanContentRenderOutsideBounds(llm::OS::UI::Xaml::DependencyObject const& element);
        static auto SetCanContentRenderOutsideBounds(llm::OS::UI::Xaml::DependencyObject const& element, bool canContentRenderOutsideBounds);
    };
    struct __declspec(empty_bases) ScrollViewerView : llm::OS::UI::Xaml::Controls::IScrollViewerView
    {
        ScrollViewerView(std::nullptr_t) noexcept {}
        ScrollViewerView(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IScrollViewerView(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ScrollViewerViewChangedEventArgs : llm::OS::UI::Xaml::Controls::IScrollViewerViewChangedEventArgs
    {
        ScrollViewerViewChangedEventArgs(std::nullptr_t) noexcept {}
        ScrollViewerViewChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IScrollViewerViewChangedEventArgs(ptr, take_ownership_from_abi) {}
        ScrollViewerViewChangedEventArgs();
    };
    struct __declspec(empty_bases) ScrollViewerViewChangingEventArgs : llm::OS::UI::Xaml::Controls::IScrollViewerViewChangingEventArgs
    {
        ScrollViewerViewChangingEventArgs(std::nullptr_t) noexcept {}
        ScrollViewerViewChangingEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IScrollViewerViewChangingEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) SearchBox : llm::OS::UI::Xaml::Controls::ISearchBox,
        impl::base<SearchBox, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<SearchBox, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        SearchBox(std::nullptr_t) noexcept {}
        SearchBox(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISearchBox(ptr, take_ownership_from_abi) {}
        SearchBox();
        [[nodiscard]] static auto SearchHistoryEnabledProperty();
        [[nodiscard]] static auto SearchHistoryContextProperty();
        [[nodiscard]] static auto PlaceholderTextProperty();
        [[nodiscard]] static auto QueryTextProperty();
        [[nodiscard]] static auto FocusOnKeyboardInputProperty();
        [[nodiscard]] static auto ChooseSuggestionOnEnterProperty();
    };
    struct __declspec(empty_bases) SearchBoxQueryChangedEventArgs : llm::OS::UI::Xaml::Controls::ISearchBoxQueryChangedEventArgs
    {
        SearchBoxQueryChangedEventArgs(std::nullptr_t) noexcept {}
        SearchBoxQueryChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISearchBoxQueryChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) SearchBoxQuerySubmittedEventArgs : llm::OS::UI::Xaml::Controls::ISearchBoxQuerySubmittedEventArgs
    {
        SearchBoxQuerySubmittedEventArgs(std::nullptr_t) noexcept {}
        SearchBoxQuerySubmittedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISearchBoxQuerySubmittedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) SearchBoxResultSuggestionChosenEventArgs : llm::OS::UI::Xaml::Controls::ISearchBoxResultSuggestionChosenEventArgs
    {
        SearchBoxResultSuggestionChosenEventArgs(std::nullptr_t) noexcept {}
        SearchBoxResultSuggestionChosenEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISearchBoxResultSuggestionChosenEventArgs(ptr, take_ownership_from_abi) {}
        SearchBoxResultSuggestionChosenEventArgs();
    };
    struct __declspec(empty_bases) SearchBoxSuggestionsRequestedEventArgs : llm::OS::UI::Xaml::Controls::ISearchBoxSuggestionsRequestedEventArgs
    {
        SearchBoxSuggestionsRequestedEventArgs(std::nullptr_t) noexcept {}
        SearchBoxSuggestionsRequestedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISearchBoxSuggestionsRequestedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) SectionsInViewChangedEventArgs : llm::OS::UI::Xaml::Controls::ISectionsInViewChangedEventArgs
    {
        SectionsInViewChangedEventArgs(std::nullptr_t) noexcept {}
        SectionsInViewChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISectionsInViewChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) SelectionChangedEventArgs : llm::OS::UI::Xaml::Controls::ISelectionChangedEventArgs,
        impl::base<SelectionChangedEventArgs, llm::OS::UI::Xaml::RoutedEventArgs>,
        impl::require<SelectionChangedEventArgs, llm::OS::UI::Xaml::IRoutedEventArgs>
    {
        SelectionChangedEventArgs(std::nullptr_t) noexcept {}
        SelectionChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISelectionChangedEventArgs(ptr, take_ownership_from_abi) {}
        SelectionChangedEventArgs(param::vector<llm::OS::Foundation::IInspectable> const& removedItems, param::vector<llm::OS::Foundation::IInspectable> const& addedItems);
    };
    struct __declspec(empty_bases) SemanticZoom : llm::OS::UI::Xaml::Controls::ISemanticZoom,
        impl::base<SemanticZoom, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<SemanticZoom, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        SemanticZoom(std::nullptr_t) noexcept {}
        SemanticZoom(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISemanticZoom(ptr, take_ownership_from_abi) {}
        SemanticZoom();
        [[nodiscard]] static auto ZoomedInViewProperty();
        [[nodiscard]] static auto ZoomedOutViewProperty();
        [[nodiscard]] static auto IsZoomedInViewActiveProperty();
        [[nodiscard]] static auto CanChangeViewsProperty();
        [[nodiscard]] static auto IsZoomOutButtonEnabledProperty();
    };
    struct __declspec(empty_bases) SemanticZoomLocation : llm::OS::UI::Xaml::Controls::ISemanticZoomLocation
    {
        SemanticZoomLocation(std::nullptr_t) noexcept {}
        SemanticZoomLocation(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISemanticZoomLocation(ptr, take_ownership_from_abi) {}
        SemanticZoomLocation();
    };
    struct __declspec(empty_bases) SemanticZoomViewChangedEventArgs : llm::OS::UI::Xaml::Controls::ISemanticZoomViewChangedEventArgs
    {
        SemanticZoomViewChangedEventArgs(std::nullptr_t) noexcept {}
        SemanticZoomViewChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISemanticZoomViewChangedEventArgs(ptr, take_ownership_from_abi) {}
        SemanticZoomViewChangedEventArgs();
    };
    struct __declspec(empty_bases) SettingsFlyout : llm::OS::UI::Xaml::Controls::ISettingsFlyout,
        impl::base<SettingsFlyout, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<SettingsFlyout, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        SettingsFlyout(std::nullptr_t) noexcept {}
        SettingsFlyout(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISettingsFlyout(ptr, take_ownership_from_abi) {}
        SettingsFlyout();
        [[nodiscard]] static auto TitleProperty();
        [[nodiscard]] static auto HeaderBackgroundProperty();
        [[nodiscard]] static auto HeaderForegroundProperty();
        [[nodiscard]] static auto IconSourceProperty();
    };
    struct __declspec(empty_bases) Slider : llm::OS::UI::Xaml::Controls::ISlider,
        impl::base<Slider, llm::OS::UI::Xaml::Controls::Primitives::RangeBase, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<Slider, llm::OS::UI::Xaml::Controls::ISlider2, llm::OS::UI::Xaml::Controls::Primitives::IRangeBase, llm::OS::UI::Xaml::Controls::Primitives::IRangeBaseOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        Slider(std::nullptr_t) noexcept {}
        Slider(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISlider(ptr, take_ownership_from_abi) {}
        Slider();
        [[nodiscard]] static auto IntermediateValueProperty();
        [[nodiscard]] static auto StepFrequencyProperty();
        [[nodiscard]] static auto SnapsToProperty();
        [[nodiscard]] static auto TickFrequencyProperty();
        [[nodiscard]] static auto TickPlacementProperty();
        [[nodiscard]] static auto OrientationProperty();
        [[nodiscard]] static auto IsDirectionReversedProperty();
        [[nodiscard]] static auto IsThumbToolTipEnabledProperty();
        [[nodiscard]] static auto ThumbToolTipValueConverterProperty();
        [[nodiscard]] static auto HeaderProperty();
        [[nodiscard]] static auto HeaderTemplateProperty();
    };
    struct __declspec(empty_bases) SplitButton : llm::OS::UI::Xaml::Controls::ISplitButton,
        impl::base<SplitButton, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<SplitButton, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        SplitButton(std::nullptr_t) noexcept {}
        SplitButton(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISplitButton(ptr, take_ownership_from_abi) {}
        SplitButton();
        [[nodiscard]] static auto FlyoutProperty();
        [[nodiscard]] static auto CommandProperty();
        [[nodiscard]] static auto CommandParameterProperty();
    };
    struct __declspec(empty_bases) SplitButtonAutomationPeer : llm::OS::UI::Xaml::Controls::ISplitButtonAutomationPeer,
        impl::base<SplitButtonAutomationPeer, llm::OS::UI::Xaml::Automation::Peers::FrameworkElementAutomationPeer, llm::OS::UI::Xaml::Automation::Peers::AutomationPeer, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<SplitButtonAutomationPeer, llm::OS::UI::Xaml::Automation::Provider::IExpandCollapseProvider, llm::OS::UI::Xaml::Automation::Provider::IInvokeProvider, llm::OS::UI::Xaml::Automation::Peers::IFrameworkElementAutomationPeer, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer2, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer3, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer4, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer5, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer6, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer7, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer8, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer9, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerProtected, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides2, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides3, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides4, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides5, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides6, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides8, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides9, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        SplitButtonAutomationPeer(std::nullptr_t) noexcept {}
        SplitButtonAutomationPeer(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISplitButtonAutomationPeer(ptr, take_ownership_from_abi) {}
        explicit SplitButtonAutomationPeer(llm::OS::UI::Xaml::Controls::SplitButton const& owner);
    };
    struct __declspec(empty_bases) SplitButtonClickEventArgs : llm::OS::UI::Xaml::Controls::ISplitButtonClickEventArgs
    {
        SplitButtonClickEventArgs(std::nullptr_t) noexcept {}
        SplitButtonClickEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISplitButtonClickEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) SplitView : llm::OS::UI::Xaml::Controls::ISplitView,
        impl::base<SplitView, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<SplitView, llm::OS::UI::Xaml::Controls::ISplitView2, llm::OS::UI::Xaml::Controls::ISplitView3, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        SplitView(std::nullptr_t) noexcept {}
        SplitView(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISplitView(ptr, take_ownership_from_abi) {}
        SplitView();
        [[nodiscard]] static auto ContentProperty();
        [[nodiscard]] static auto PaneProperty();
        [[nodiscard]] static auto IsPaneOpenProperty();
        [[nodiscard]] static auto OpenPaneLengthProperty();
        [[nodiscard]] static auto CompactPaneLengthProperty();
        [[nodiscard]] static auto PanePlacementProperty();
        [[nodiscard]] static auto DisplayModeProperty();
        [[nodiscard]] static auto TemplateSettingsProperty();
        [[nodiscard]] static auto PaneBackgroundProperty();
        [[nodiscard]] static auto LightDismissOverlayModeProperty();
    };
    struct __declspec(empty_bases) SplitViewPaneClosingEventArgs : llm::OS::UI::Xaml::Controls::ISplitViewPaneClosingEventArgs
    {
        SplitViewPaneClosingEventArgs(std::nullptr_t) noexcept {}
        SplitViewPaneClosingEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISplitViewPaneClosingEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) StackPanel : llm::OS::UI::Xaml::Controls::IStackPanel,
        impl::base<StackPanel, llm::OS::UI::Xaml::Controls::Panel, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<StackPanel, llm::OS::UI::Xaml::Controls::IStackPanel2, llm::OS::UI::Xaml::Controls::IStackPanel4, llm::OS::UI::Xaml::Controls::IStackPanel5, llm::OS::UI::Xaml::Controls::Primitives::IScrollSnapPointsInfo, llm::OS::UI::Xaml::Controls::IInsertionPanel, llm::OS::UI::Xaml::Controls::IPanel, llm::OS::UI::Xaml::Controls::IPanel2, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        StackPanel(std::nullptr_t) noexcept {}
        StackPanel(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IStackPanel(ptr, take_ownership_from_abi) {}
        StackPanel();
        [[nodiscard]] static auto AreScrollSnapPointsRegularProperty();
        [[nodiscard]] static auto OrientationProperty();
        [[nodiscard]] static auto BorderBrushProperty();
        [[nodiscard]] static auto BorderThicknessProperty();
        [[nodiscard]] static auto CornerRadiusProperty();
        [[nodiscard]] static auto PaddingProperty();
        [[nodiscard]] static auto SpacingProperty();
        [[nodiscard]] static auto BackgroundSizingProperty();
    };
    struct __declspec(empty_bases) StyleSelector : llm::OS::UI::Xaml::Controls::IStyleSelector,
        impl::require<StyleSelector, llm::OS::UI::Xaml::Controls::IStyleSelectorOverrides>
    {
        StyleSelector(std::nullptr_t) noexcept {}
        StyleSelector(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IStyleSelector(ptr, take_ownership_from_abi) {}
        StyleSelector();
    };
    struct __declspec(empty_bases) SwapChainBackgroundPanel : llm::OS::UI::Xaml::Controls::ISwapChainBackgroundPanel,
        impl::base<SwapChainBackgroundPanel, llm::OS::UI::Xaml::Controls::Grid, llm::OS::UI::Xaml::Controls::Panel, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<SwapChainBackgroundPanel, llm::OS::UI::Xaml::Controls::ISwapChainBackgroundPanel2, llm::OS::UI::Xaml::Controls::IGrid, llm::OS::UI::Xaml::Controls::IGrid2, llm::OS::UI::Xaml::Controls::IGrid3, llm::OS::UI::Xaml::Controls::IGrid4, llm::OS::UI::Xaml::Controls::IPanel, llm::OS::UI::Xaml::Controls::IPanel2, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        SwapChainBackgroundPanel(std::nullptr_t) noexcept {}
        SwapChainBackgroundPanel(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISwapChainBackgroundPanel(ptr, take_ownership_from_abi) {}
        SwapChainBackgroundPanel();
    };
    struct __declspec(empty_bases) SwapChainPanel : llm::OS::UI::Xaml::Controls::ISwapChainPanel,
        impl::base<SwapChainPanel, llm::OS::UI::Xaml::Controls::Grid, llm::OS::UI::Xaml::Controls::Panel, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<SwapChainPanel, llm::OS::UI::Xaml::Controls::IGrid, llm::OS::UI::Xaml::Controls::IGrid2, llm::OS::UI::Xaml::Controls::IGrid3, llm::OS::UI::Xaml::Controls::IGrid4, llm::OS::UI::Xaml::Controls::IPanel, llm::OS::UI::Xaml::Controls::IPanel2, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        SwapChainPanel(std::nullptr_t) noexcept {}
        SwapChainPanel(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISwapChainPanel(ptr, take_ownership_from_abi) {}
        SwapChainPanel();
        [[nodiscard]] static auto CompositionScaleXProperty();
        [[nodiscard]] static auto CompositionScaleYProperty();
    };
    struct __declspec(empty_bases) SwipeControl : llm::OS::UI::Xaml::Controls::ISwipeControl,
        impl::base<SwipeControl, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<SwipeControl, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        SwipeControl(std::nullptr_t) noexcept {}
        SwipeControl(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISwipeControl(ptr, take_ownership_from_abi) {}
        SwipeControl();
        [[nodiscard]] static auto LeftItemsProperty();
        [[nodiscard]] static auto RightItemsProperty();
        [[nodiscard]] static auto TopItemsProperty();
        [[nodiscard]] static auto BottomItemsProperty();
    };
    struct __declspec(empty_bases) SwipeItem : llm::OS::UI::Xaml::Controls::ISwipeItem,
        impl::base<SwipeItem, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<SwipeItem, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        SwipeItem(std::nullptr_t) noexcept {}
        SwipeItem(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISwipeItem(ptr, take_ownership_from_abi) {}
        SwipeItem();
        [[nodiscard]] static auto IconSourceProperty();
        [[nodiscard]] static auto TextProperty();
        [[nodiscard]] static auto BackgroundProperty();
        [[nodiscard]] static auto ForegroundProperty();
        [[nodiscard]] static auto CommandProperty();
        [[nodiscard]] static auto CommandParameterProperty();
        [[nodiscard]] static auto BehaviorOnInvokedProperty();
    };
    struct __declspec(empty_bases) SwipeItemInvokedEventArgs : llm::OS::UI::Xaml::Controls::ISwipeItemInvokedEventArgs
    {
        SwipeItemInvokedEventArgs(std::nullptr_t) noexcept {}
        SwipeItemInvokedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISwipeItemInvokedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) SwipeItems : llm::OS::UI::Xaml::Controls::ISwipeItems,
        impl::base<SwipeItems, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<SwipeItems, llm::OS::Foundation::Collections::IIterable<llm::OS::UI::Xaml::Controls::SwipeItem>, llm::OS::Foundation::Collections::IVector<llm::OS::UI::Xaml::Controls::SwipeItem>, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        SwipeItems(std::nullptr_t) noexcept {}
        SwipeItems(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISwipeItems(ptr, take_ownership_from_abi) {}
        SwipeItems();
        [[nodiscard]] static auto ModeProperty();
    };
    struct __declspec(empty_bases) SymbolIcon : llm::OS::UI::Xaml::Controls::ISymbolIcon,
        impl::base<SymbolIcon, llm::OS::UI::Xaml::Controls::IconElement, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<SymbolIcon, llm::OS::UI::Xaml::Controls::IIconElement, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        SymbolIcon(std::nullptr_t) noexcept {}
        SymbolIcon(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISymbolIcon(ptr, take_ownership_from_abi) {}
        SymbolIcon();
        explicit SymbolIcon(llm::OS::UI::Xaml::Controls::Symbol const& symbol);
        [[nodiscard]] static auto SymbolProperty();
    };
    struct __declspec(empty_bases) SymbolIconSource : llm::OS::UI::Xaml::Controls::ISymbolIconSource,
        impl::base<SymbolIconSource, llm::OS::UI::Xaml::Controls::IconSource, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<SymbolIconSource, llm::OS::UI::Xaml::Controls::IIconSource, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        SymbolIconSource(std::nullptr_t) noexcept {}
        SymbolIconSource(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ISymbolIconSource(ptr, take_ownership_from_abi) {}
        SymbolIconSource();
        [[nodiscard]] static auto SymbolProperty();
    };
    struct __declspec(empty_bases) TextBlock : llm::OS::UI::Xaml::Controls::ITextBlock,
        impl::base<TextBlock, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<TextBlock, llm::OS::UI::Xaml::Controls::ITextBlock2, llm::OS::UI::Xaml::Controls::ITextBlock3, llm::OS::UI::Xaml::Controls::ITextBlock4, llm::OS::UI::Xaml::Controls::ITextBlock5, llm::OS::UI::Xaml::Controls::ITextBlock6, llm::OS::UI::Xaml::Controls::ITextBlock7, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        TextBlock(std::nullptr_t) noexcept {}
        TextBlock(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITextBlock(ptr, take_ownership_from_abi) {}
        TextBlock();
        [[nodiscard]] static auto FontSizeProperty();
        [[nodiscard]] static auto FontFamilyProperty();
        [[nodiscard]] static auto FontWeightProperty();
        [[nodiscard]] static auto FontStyleProperty();
        [[nodiscard]] static auto FontStretchProperty();
        [[nodiscard]] static auto CharacterSpacingProperty();
        [[nodiscard]] static auto ForegroundProperty();
        [[nodiscard]] static auto TextWrappingProperty();
        [[nodiscard]] static auto TextTrimmingProperty();
        [[nodiscard]] static auto TextAlignmentProperty();
        [[nodiscard]] static auto TextProperty();
        [[nodiscard]] static auto PaddingProperty();
        [[nodiscard]] static auto LineHeightProperty();
        [[nodiscard]] static auto LineStackingStrategyProperty();
        [[nodiscard]] static auto IsTextSelectionEnabledProperty();
        [[nodiscard]] static auto SelectedTextProperty();
        [[nodiscard]] static auto SelectionHighlightColorProperty();
        [[nodiscard]] static auto MaxLinesProperty();
        [[nodiscard]] static auto TextLineBoundsProperty();
        [[nodiscard]] static auto OpticalMarginAlignmentProperty();
        [[nodiscard]] static auto IsColorFontEnabledProperty();
        [[nodiscard]] static auto TextReadingOrderProperty();
        [[nodiscard]] static auto IsTextScaleFactorEnabledProperty();
        [[nodiscard]] static auto TextDecorationsProperty();
        [[nodiscard]] static auto IsTextTrimmedProperty();
        [[nodiscard]] static auto HorizontalTextAlignmentProperty();
        [[nodiscard]] static auto SelectionFlyoutProperty();
    };
    struct __declspec(empty_bases) TextBox : llm::OS::UI::Xaml::Controls::ITextBox,
        impl::base<TextBox, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<TextBox, llm::OS::UI::Xaml::Controls::ITextBox2, llm::OS::UI::Xaml::Controls::ITextBox3, llm::OS::UI::Xaml::Controls::ITextBox4, llm::OS::UI::Xaml::Controls::ITextBox5, llm::OS::UI::Xaml::Controls::ITextBox6, llm::OS::UI::Xaml::Controls::ITextBox7, llm::OS::UI::Xaml::Controls::ITextBox8, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        TextBox(std::nullptr_t) noexcept {}
        TextBox(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITextBox(ptr, take_ownership_from_abi) {}
        TextBox();
        [[nodiscard]] static auto TextProperty();
        [[nodiscard]] static auto MaxLengthProperty();
        [[nodiscard]] static auto IsReadOnlyProperty();
        [[nodiscard]] static auto AcceptsReturnProperty();
        [[nodiscard]] static auto TextAlignmentProperty();
        [[nodiscard]] static auto TextWrappingProperty();
        [[nodiscard]] static auto IsSpellCheckEnabledProperty();
        [[nodiscard]] static auto IsTextPredictionEnabledProperty();
        [[nodiscard]] static auto InputScopeProperty();
        [[nodiscard]] static auto HeaderProperty();
        [[nodiscard]] static auto HeaderTemplateProperty();
        [[nodiscard]] static auto PlaceholderTextProperty();
        [[nodiscard]] static auto SelectionHighlightColorProperty();
        [[nodiscard]] static auto PreventKeyboardDisplayOnProgrammaticFocusProperty();
        [[nodiscard]] static auto IsColorFontEnabledProperty();
        [[nodiscard]] static auto DesiredCandidateWindowAlignmentProperty();
        [[nodiscard]] static auto TextReadingOrderProperty();
        [[nodiscard]] static auto SelectionHighlightColorWhenNotFocusedProperty();
        [[nodiscard]] static auto HorizontalTextAlignmentProperty();
        [[nodiscard]] static auto CharacterCasingProperty();
        [[nodiscard]] static auto PlaceholderForegroundProperty();
        [[nodiscard]] static auto HandwritingViewProperty();
        [[nodiscard]] static auto IsHandwritingViewEnabledProperty();
        [[nodiscard]] static auto CanPasteClipboardContentProperty();
        [[nodiscard]] static auto CanUndoProperty();
        [[nodiscard]] static auto CanRedoProperty();
        [[nodiscard]] static auto SelectionFlyoutProperty();
        [[nodiscard]] static auto ProofingMenuFlyoutProperty();
        [[nodiscard]] static auto DescriptionProperty();
    };
    struct __declspec(empty_bases) TextBoxBeforeTextChangingEventArgs : llm::OS::UI::Xaml::Controls::ITextBoxBeforeTextChangingEventArgs
    {
        TextBoxBeforeTextChangingEventArgs(std::nullptr_t) noexcept {}
        TextBoxBeforeTextChangingEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITextBoxBeforeTextChangingEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) TextBoxSelectionChangingEventArgs : llm::OS::UI::Xaml::Controls::ITextBoxSelectionChangingEventArgs
    {
        TextBoxSelectionChangingEventArgs(std::nullptr_t) noexcept {}
        TextBoxSelectionChangingEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITextBoxSelectionChangingEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) TextBoxTextChangingEventArgs : llm::OS::UI::Xaml::Controls::ITextBoxTextChangingEventArgs,
        impl::require<TextBoxTextChangingEventArgs, llm::OS::UI::Xaml::Controls::ITextBoxTextChangingEventArgs2>
    {
        TextBoxTextChangingEventArgs(std::nullptr_t) noexcept {}
        TextBoxTextChangingEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITextBoxTextChangingEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) TextChangedEventArgs : llm::OS::UI::Xaml::Controls::ITextChangedEventArgs,
        impl::base<TextChangedEventArgs, llm::OS::UI::Xaml::RoutedEventArgs>,
        impl::require<TextChangedEventArgs, llm::OS::UI::Xaml::IRoutedEventArgs>
    {
        TextChangedEventArgs(std::nullptr_t) noexcept {}
        TextChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITextChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) TextCommandBarFlyout : llm::OS::UI::Xaml::Controls::ITextCommandBarFlyout,
        impl::base<TextCommandBarFlyout, llm::OS::UI::Xaml::Controls::CommandBarFlyout, llm::OS::UI::Xaml::Controls::Primitives::FlyoutBase, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<TextCommandBarFlyout, llm::OS::UI::Xaml::Controls::ICommandBarFlyout, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase2, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase3, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase4, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase5, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase6, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBaseOverrides, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBaseOverrides4, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        TextCommandBarFlyout(std::nullptr_t) noexcept {}
        TextCommandBarFlyout(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITextCommandBarFlyout(ptr, take_ownership_from_abi) {}
        TextCommandBarFlyout();
        using impl::consume_t<TextCommandBarFlyout, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase>::ShowAt;
        using impl::consume_t<TextCommandBarFlyout, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase5>::ShowAt;
    };
    struct __declspec(empty_bases) TextCompositionChangedEventArgs : llm::OS::UI::Xaml::Controls::ITextCompositionChangedEventArgs
    {
        TextCompositionChangedEventArgs(std::nullptr_t) noexcept {}
        TextCompositionChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITextCompositionChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) TextCompositionEndedEventArgs : llm::OS::UI::Xaml::Controls::ITextCompositionEndedEventArgs
    {
        TextCompositionEndedEventArgs(std::nullptr_t) noexcept {}
        TextCompositionEndedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITextCompositionEndedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) TextCompositionStartedEventArgs : llm::OS::UI::Xaml::Controls::ITextCompositionStartedEventArgs
    {
        TextCompositionStartedEventArgs(std::nullptr_t) noexcept {}
        TextCompositionStartedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITextCompositionStartedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) TextControlCopyingToClipboardEventArgs : llm::OS::UI::Xaml::Controls::ITextControlCopyingToClipboardEventArgs
    {
        TextControlCopyingToClipboardEventArgs(std::nullptr_t) noexcept {}
        TextControlCopyingToClipboardEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITextControlCopyingToClipboardEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) TextControlCuttingToClipboardEventArgs : llm::OS::UI::Xaml::Controls::ITextControlCuttingToClipboardEventArgs
    {
        TextControlCuttingToClipboardEventArgs(std::nullptr_t) noexcept {}
        TextControlCuttingToClipboardEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITextControlCuttingToClipboardEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) TextControlPasteEventArgs : llm::OS::UI::Xaml::Controls::ITextControlPasteEventArgs
    {
        TextControlPasteEventArgs(std::nullptr_t) noexcept {}
        TextControlPasteEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITextControlPasteEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) TimePickedEventArgs : llm::OS::UI::Xaml::Controls::ITimePickedEventArgs,
        impl::base<TimePickedEventArgs, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<TimePickedEventArgs, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        TimePickedEventArgs(std::nullptr_t) noexcept {}
        TimePickedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITimePickedEventArgs(ptr, take_ownership_from_abi) {}
        TimePickedEventArgs();
    };
    struct __declspec(empty_bases) TimePicker : llm::OS::UI::Xaml::Controls::ITimePicker,
        impl::base<TimePicker, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<TimePicker, llm::OS::UI::Xaml::Controls::ITimePicker2, llm::OS::UI::Xaml::Controls::ITimePicker3, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        TimePicker(std::nullptr_t) noexcept {}
        TimePicker(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITimePicker(ptr, take_ownership_from_abi) {}
        TimePicker();
        [[nodiscard]] static auto HeaderProperty();
        [[nodiscard]] static auto HeaderTemplateProperty();
        [[nodiscard]] static auto ClockIdentifierProperty();
        [[nodiscard]] static auto MinuteIncrementProperty();
        [[nodiscard]] static auto TimeProperty();
        [[nodiscard]] static auto LightDismissOverlayModeProperty();
        [[nodiscard]] static auto SelectedTimeProperty();
    };
    struct __declspec(empty_bases) TimePickerFlyout : llm::OS::UI::Xaml::Controls::ITimePickerFlyout,
        impl::base<TimePickerFlyout, llm::OS::UI::Xaml::Controls::Primitives::PickerFlyoutBase, llm::OS::UI::Xaml::Controls::Primitives::FlyoutBase, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<TimePickerFlyout, llm::OS::UI::Xaml::Controls::Primitives::IPickerFlyoutBase, llm::OS::UI::Xaml::Controls::Primitives::IPickerFlyoutBaseOverrides, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase2, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase3, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase4, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase5, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase6, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBaseOverrides, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBaseOverrides4, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        TimePickerFlyout(std::nullptr_t) noexcept {}
        TimePickerFlyout(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITimePickerFlyout(ptr, take_ownership_from_abi) {}
        TimePickerFlyout();
        using impl::consume_t<TimePickerFlyout, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase>::ShowAt;
        using impl::consume_t<TimePickerFlyout, llm::OS::UI::Xaml::Controls::Primitives::IFlyoutBase5>::ShowAt;
        [[nodiscard]] static auto ClockIdentifierProperty();
        [[nodiscard]] static auto TimeProperty();
        [[nodiscard]] static auto MinuteIncrementProperty();
    };
    struct __declspec(empty_bases) TimePickerFlyoutPresenter : llm::OS::UI::Xaml::Controls::ITimePickerFlyoutPresenter,
        impl::base<TimePickerFlyoutPresenter, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<TimePickerFlyoutPresenter, llm::OS::UI::Xaml::Controls::ITimePickerFlyoutPresenter2, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        TimePickerFlyoutPresenter(std::nullptr_t) noexcept {}
        TimePickerFlyoutPresenter(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITimePickerFlyoutPresenter(ptr, take_ownership_from_abi) {}
        [[nodiscard]] static auto IsDefaultShadowEnabledProperty();
    };
    struct __declspec(empty_bases) TimePickerSelectedValueChangedEventArgs : llm::OS::UI::Xaml::Controls::ITimePickerSelectedValueChangedEventArgs
    {
        TimePickerSelectedValueChangedEventArgs(std::nullptr_t) noexcept {}
        TimePickerSelectedValueChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITimePickerSelectedValueChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) TimePickerValueChangedEventArgs : llm::OS::UI::Xaml::Controls::ITimePickerValueChangedEventArgs
    {
        TimePickerValueChangedEventArgs(std::nullptr_t) noexcept {}
        TimePickerValueChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITimePickerValueChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ToggleMenuFlyoutItem : llm::OS::UI::Xaml::Controls::IToggleMenuFlyoutItem,
        impl::base<ToggleMenuFlyoutItem, llm::OS::UI::Xaml::Controls::MenuFlyoutItem, llm::OS::UI::Xaml::Controls::MenuFlyoutItemBase, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ToggleMenuFlyoutItem, llm::OS::UI::Xaml::Controls::IMenuFlyoutItem, llm::OS::UI::Xaml::Controls::IMenuFlyoutItem2, llm::OS::UI::Xaml::Controls::IMenuFlyoutItem3, llm::OS::UI::Xaml::Controls::IMenuFlyoutItemBase, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ToggleMenuFlyoutItem(std::nullptr_t) noexcept {}
        ToggleMenuFlyoutItem(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IToggleMenuFlyoutItem(ptr, take_ownership_from_abi) {}
        ToggleMenuFlyoutItem();
        [[nodiscard]] static auto IsCheckedProperty();
    };
    struct __declspec(empty_bases) ToggleSplitButton : llm::OS::UI::Xaml::Controls::IToggleSplitButton,
        impl::base<ToggleSplitButton, llm::OS::UI::Xaml::Controls::SplitButton, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ToggleSplitButton, llm::OS::UI::Xaml::Controls::ISplitButton, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ToggleSplitButton(std::nullptr_t) noexcept {}
        ToggleSplitButton(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IToggleSplitButton(ptr, take_ownership_from_abi) {}
        ToggleSplitButton();
    };
    struct __declspec(empty_bases) ToggleSplitButtonAutomationPeer : llm::OS::UI::Xaml::Controls::IToggleSplitButtonAutomationPeer,
        impl::base<ToggleSplitButtonAutomationPeer, llm::OS::UI::Xaml::Automation::Peers::FrameworkElementAutomationPeer, llm::OS::UI::Xaml::Automation::Peers::AutomationPeer, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ToggleSplitButtonAutomationPeer, llm::OS::UI::Xaml::Automation::Provider::IExpandCollapseProvider, llm::OS::UI::Xaml::Automation::Provider::IToggleProvider, llm::OS::UI::Xaml::Automation::Peers::IFrameworkElementAutomationPeer, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer2, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer3, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer4, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer5, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer6, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer7, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer8, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeer9, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerProtected, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides2, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides3, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides4, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides5, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides6, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides8, llm::OS::UI::Xaml::Automation::Peers::IAutomationPeerOverrides9, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ToggleSplitButtonAutomationPeer(std::nullptr_t) noexcept {}
        ToggleSplitButtonAutomationPeer(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IToggleSplitButtonAutomationPeer(ptr, take_ownership_from_abi) {}
        explicit ToggleSplitButtonAutomationPeer(llm::OS::UI::Xaml::Controls::ToggleSplitButton const& owner);
    };
    struct __declspec(empty_bases) ToggleSplitButtonIsCheckedChangedEventArgs : llm::OS::UI::Xaml::Controls::IToggleSplitButtonIsCheckedChangedEventArgs
    {
        ToggleSplitButtonIsCheckedChangedEventArgs(std::nullptr_t) noexcept {}
        ToggleSplitButtonIsCheckedChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IToggleSplitButtonIsCheckedChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ToggleSwitch : llm::OS::UI::Xaml::Controls::IToggleSwitch,
        impl::base<ToggleSwitch, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ToggleSwitch, llm::OS::UI::Xaml::Controls::IToggleSwitchOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ToggleSwitch(std::nullptr_t) noexcept {}
        ToggleSwitch(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IToggleSwitch(ptr, take_ownership_from_abi) {}
        ToggleSwitch();
        [[nodiscard]] static auto IsOnProperty();
        [[nodiscard]] static auto HeaderProperty();
        [[nodiscard]] static auto HeaderTemplateProperty();
        [[nodiscard]] static auto OnContentProperty();
        [[nodiscard]] static auto OnContentTemplateProperty();
        [[nodiscard]] static auto OffContentProperty();
        [[nodiscard]] static auto OffContentTemplateProperty();
    };
    struct __declspec(empty_bases) ToolTip : llm::OS::UI::Xaml::Controls::IToolTip,
        impl::base<ToolTip, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<ToolTip, llm::OS::UI::Xaml::Controls::IToolTip2, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        ToolTip(std::nullptr_t) noexcept {}
        ToolTip(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IToolTip(ptr, take_ownership_from_abi) {}
        ToolTip();
        [[nodiscard]] static auto HorizontalOffsetProperty();
        [[nodiscard]] static auto IsOpenProperty();
        [[nodiscard]] static auto PlacementProperty();
        [[nodiscard]] static auto PlacementTargetProperty();
        [[nodiscard]] static auto VerticalOffsetProperty();
        [[nodiscard]] static auto PlacementRectProperty();
    };
    struct __declspec(empty_bases) ToolTipService : llm::OS::UI::Xaml::Controls::IToolTipService
    {
        ToolTipService(std::nullptr_t) noexcept {}
        ToolTipService(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IToolTipService(ptr, take_ownership_from_abi) {}
        [[nodiscard]] static auto PlacementProperty();
        static auto GetPlacement(llm::OS::UI::Xaml::DependencyObject const& element);
        static auto SetPlacement(llm::OS::UI::Xaml::DependencyObject const& element, llm::OS::UI::Xaml::Controls::Primitives::PlacementMode const& value);
        [[nodiscard]] static auto PlacementTargetProperty();
        static auto GetPlacementTarget(llm::OS::UI::Xaml::DependencyObject const& element);
        static auto SetPlacementTarget(llm::OS::UI::Xaml::DependencyObject const& element, llm::OS::UI::Xaml::UIElement const& value);
        [[nodiscard]] static auto ToolTipProperty();
        static auto GetToolTip(llm::OS::UI::Xaml::DependencyObject const& element);
        static auto SetToolTip(llm::OS::UI::Xaml::DependencyObject const& element, llm::OS::Foundation::IInspectable const& value);
    };
    struct __declspec(empty_bases) TreeView : llm::OS::UI::Xaml::Controls::ITreeView,
        impl::base<TreeView, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<TreeView, llm::OS::UI::Xaml::Controls::ITreeView2, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        TreeView(std::nullptr_t) noexcept {}
        TreeView(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITreeView(ptr, take_ownership_from_abi) {}
        TreeView();
        [[nodiscard]] static auto SelectionModeProperty();
        [[nodiscard]] static auto CanDragItemsProperty();
        [[nodiscard]] static auto CanReorderItemsProperty();
        [[nodiscard]] static auto ItemTemplateProperty();
        [[nodiscard]] static auto ItemTemplateSelectorProperty();
        [[nodiscard]] static auto ItemContainerStyleProperty();
        [[nodiscard]] static auto ItemContainerStyleSelectorProperty();
        [[nodiscard]] static auto ItemContainerTransitionsProperty();
        [[nodiscard]] static auto ItemsSourceProperty();
    };
    struct __declspec(empty_bases) TreeViewCollapsedEventArgs : llm::OS::UI::Xaml::Controls::ITreeViewCollapsedEventArgs,
        impl::require<TreeViewCollapsedEventArgs, llm::OS::UI::Xaml::Controls::ITreeViewCollapsedEventArgs2>
    {
        TreeViewCollapsedEventArgs(std::nullptr_t) noexcept {}
        TreeViewCollapsedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITreeViewCollapsedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) TreeViewDragItemsCompletedEventArgs : llm::OS::UI::Xaml::Controls::ITreeViewDragItemsCompletedEventArgs
    {
        TreeViewDragItemsCompletedEventArgs(std::nullptr_t) noexcept {}
        TreeViewDragItemsCompletedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITreeViewDragItemsCompletedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) TreeViewDragItemsStartingEventArgs : llm::OS::UI::Xaml::Controls::ITreeViewDragItemsStartingEventArgs
    {
        TreeViewDragItemsStartingEventArgs(std::nullptr_t) noexcept {}
        TreeViewDragItemsStartingEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITreeViewDragItemsStartingEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) TreeViewExpandingEventArgs : llm::OS::UI::Xaml::Controls::ITreeViewExpandingEventArgs,
        impl::require<TreeViewExpandingEventArgs, llm::OS::UI::Xaml::Controls::ITreeViewExpandingEventArgs2>
    {
        TreeViewExpandingEventArgs(std::nullptr_t) noexcept {}
        TreeViewExpandingEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITreeViewExpandingEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) TreeViewItem : llm::OS::UI::Xaml::Controls::ITreeViewItem,
        impl::base<TreeViewItem, llm::OS::UI::Xaml::Controls::ListViewItem, llm::OS::UI::Xaml::Controls::Primitives::SelectorItem, llm::OS::UI::Xaml::Controls::ContentControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<TreeViewItem, llm::OS::UI::Xaml::Controls::ITreeViewItem2, llm::OS::UI::Xaml::Controls::IListViewItem, llm::OS::UI::Xaml::Controls::Primitives::ISelectorItem, llm::OS::UI::Xaml::Controls::IContentControl, llm::OS::UI::Xaml::Controls::IContentControl2, llm::OS::UI::Xaml::Controls::IContentControlOverrides, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        TreeViewItem(std::nullptr_t) noexcept {}
        TreeViewItem(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITreeViewItem(ptr, take_ownership_from_abi) {}
        TreeViewItem();
        [[nodiscard]] static auto GlyphOpacityProperty();
        [[nodiscard]] static auto GlyphBrushProperty();
        [[nodiscard]] static auto ExpandedGlyphProperty();
        [[nodiscard]] static auto CollapsedGlyphProperty();
        [[nodiscard]] static auto GlyphSizeProperty();
        [[nodiscard]] static auto IsExpandedProperty();
        [[nodiscard]] static auto TreeViewItemTemplateSettingsProperty();
        [[nodiscard]] static auto HasUnrealizedChildrenProperty();
        [[nodiscard]] static auto ItemsSourceProperty();
    };
    struct __declspec(empty_bases) TreeViewItemInvokedEventArgs : llm::OS::UI::Xaml::Controls::ITreeViewItemInvokedEventArgs
    {
        TreeViewItemInvokedEventArgs(std::nullptr_t) noexcept {}
        TreeViewItemInvokedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITreeViewItemInvokedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) TreeViewItemTemplateSettings : llm::OS::UI::Xaml::Controls::ITreeViewItemTemplateSettings,
        impl::base<TreeViewItemTemplateSettings, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<TreeViewItemTemplateSettings, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        TreeViewItemTemplateSettings(std::nullptr_t) noexcept {}
        TreeViewItemTemplateSettings(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITreeViewItemTemplateSettings(ptr, take_ownership_from_abi) {}
        TreeViewItemTemplateSettings();
        [[nodiscard]] static auto ExpandedGlyphVisibilityProperty();
        [[nodiscard]] static auto CollapsedGlyphVisibilityProperty();
        [[nodiscard]] static auto IndentationProperty();
        [[nodiscard]] static auto DragItemsCountProperty();
    };
    struct __declspec(empty_bases) TreeViewList : llm::OS::UI::Xaml::Controls::ITreeViewList,
        impl::base<TreeViewList, llm::OS::UI::Xaml::Controls::ListView, llm::OS::UI::Xaml::Controls::ListViewBase, llm::OS::UI::Xaml::Controls::Primitives::Selector, llm::OS::UI::Xaml::Controls::ItemsControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<TreeViewList, llm::OS::UI::Xaml::Controls::IListView, llm::OS::UI::Xaml::Controls::IListViewBase, llm::OS::UI::Xaml::Controls::IListViewBase2, llm::OS::UI::Xaml::Controls::IListViewBase3, llm::OS::UI::Xaml::Controls::IListViewBase4, llm::OS::UI::Xaml::Controls::IListViewBase5, llm::OS::UI::Xaml::Controls::IListViewBase6, llm::OS::UI::Xaml::Controls::ISemanticZoomInformation, llm::OS::UI::Xaml::Controls::Primitives::ISelector, llm::OS::UI::Xaml::Controls::IItemsControl, llm::OS::UI::Xaml::Controls::IItemsControl2, llm::OS::UI::Xaml::Controls::IItemsControl3, llm::OS::UI::Xaml::Controls::IItemsControlOverrides, llm::OS::UI::Xaml::Controls::IItemContainerMapping, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        TreeViewList(std::nullptr_t) noexcept {}
        TreeViewList(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITreeViewList(ptr, take_ownership_from_abi) {}
        TreeViewList();
    };
    struct __declspec(empty_bases) TreeViewNode : llm::OS::UI::Xaml::Controls::ITreeViewNode,
        impl::base<TreeViewNode, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<TreeViewNode, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        TreeViewNode(std::nullptr_t) noexcept {}
        TreeViewNode(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITreeViewNode(ptr, take_ownership_from_abi) {}
        TreeViewNode();
        [[nodiscard]] static auto ContentProperty();
        [[nodiscard]] static auto DepthProperty();
        [[nodiscard]] static auto IsExpandedProperty();
        [[nodiscard]] static auto HasChildrenProperty();
    };
    struct __declspec(empty_bases) TwoPaneView : llm::OS::UI::Xaml::Controls::ITwoPaneView,
        impl::base<TwoPaneView, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<TwoPaneView, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        TwoPaneView(std::nullptr_t) noexcept {}
        TwoPaneView(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::ITwoPaneView(ptr, take_ownership_from_abi) {}
        TwoPaneView();
        [[nodiscard]] static auto Pane1Property();
        [[nodiscard]] static auto Pane2Property();
        [[nodiscard]] static auto Pane1LengthProperty();
        [[nodiscard]] static auto Pane2LengthProperty();
        [[nodiscard]] static auto PanePriorityProperty();
        [[nodiscard]] static auto ModeProperty();
        [[nodiscard]] static auto WideModeConfigurationProperty();
        [[nodiscard]] static auto TallModeConfigurationProperty();
        [[nodiscard]] static auto MinWideModeWidthProperty();
        [[nodiscard]] static auto MinTallModeHeightProperty();
    };
    struct __declspec(empty_bases) UIElementCollection : llm::OS::Foundation::Collections::IVector<llm::OS::UI::Xaml::UIElement>,
        impl::require<UIElementCollection, llm::OS::UI::Xaml::Controls::IUIElementCollection>
    {
        UIElementCollection(std::nullptr_t) noexcept {}
        UIElementCollection(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::Foundation::Collections::IVector<llm::OS::UI::Xaml::UIElement>(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) UserControl : llm::OS::UI::Xaml::Controls::IUserControl,
        impl::base<UserControl, llm::OS::UI::Xaml::Controls::Control, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<UserControl, llm::OS::UI::Xaml::Controls::IControl, llm::OS::UI::Xaml::Controls::IControl2, llm::OS::UI::Xaml::Controls::IControl3, llm::OS::UI::Xaml::Controls::IControl4, llm::OS::UI::Xaml::Controls::IControl5, llm::OS::UI::Xaml::Controls::IControl7, llm::OS::UI::Xaml::Controls::IControlProtected, llm::OS::UI::Xaml::Controls::IControlOverrides, llm::OS::UI::Xaml::Controls::IControlOverrides6, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        UserControl(std::nullptr_t) noexcept {}
        UserControl(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IUserControl(ptr, take_ownership_from_abi) {}
        UserControl();
        [[nodiscard]] static auto ContentProperty();
    };
    struct __declspec(empty_bases) VariableSizedWrapGrid : llm::OS::UI::Xaml::Controls::IVariableSizedWrapGrid,
        impl::base<VariableSizedWrapGrid, llm::OS::UI::Xaml::Controls::Panel, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<VariableSizedWrapGrid, llm::OS::UI::Xaml::Controls::IPanel, llm::OS::UI::Xaml::Controls::IPanel2, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        VariableSizedWrapGrid(std::nullptr_t) noexcept {}
        VariableSizedWrapGrid(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IVariableSizedWrapGrid(ptr, take_ownership_from_abi) {}
        VariableSizedWrapGrid();
        [[nodiscard]] static auto ItemHeightProperty();
        [[nodiscard]] static auto ItemWidthProperty();
        [[nodiscard]] static auto OrientationProperty();
        [[nodiscard]] static auto HorizontalChildrenAlignmentProperty();
        [[nodiscard]] static auto VerticalChildrenAlignmentProperty();
        [[nodiscard]] static auto MaximumRowsOrColumnsProperty();
        [[nodiscard]] static auto RowSpanProperty();
        static auto GetRowSpan(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetRowSpan(llm::OS::UI::Xaml::UIElement const& element, int32_t value);
        [[nodiscard]] static auto ColumnSpanProperty();
        static auto GetColumnSpan(llm::OS::UI::Xaml::UIElement const& element);
        static auto SetColumnSpan(llm::OS::UI::Xaml::UIElement const& element, int32_t value);
    };
    struct __declspec(empty_bases) Viewbox : llm::OS::UI::Xaml::Controls::IViewbox,
        impl::base<Viewbox, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<Viewbox, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        Viewbox(std::nullptr_t) noexcept {}
        Viewbox(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IViewbox(ptr, take_ownership_from_abi) {}
        Viewbox();
        [[nodiscard]] static auto StretchProperty();
        [[nodiscard]] static auto StretchDirectionProperty();
    };
    struct __declspec(empty_bases) VirtualizingPanel : llm::OS::UI::Xaml::Controls::IVirtualizingPanel,
        impl::base<VirtualizingPanel, llm::OS::UI::Xaml::Controls::Panel, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<VirtualizingPanel, llm::OS::UI::Xaml::Controls::IVirtualizingPanelProtected, llm::OS::UI::Xaml::Controls::IVirtualizingPanelOverrides, llm::OS::UI::Xaml::Controls::IPanel, llm::OS::UI::Xaml::Controls::IPanel2, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        VirtualizingPanel(std::nullptr_t) noexcept {}
        VirtualizingPanel(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IVirtualizingPanel(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) VirtualizingStackPanel : llm::OS::UI::Xaml::Controls::IVirtualizingStackPanel,
        impl::base<VirtualizingStackPanel, llm::OS::UI::Xaml::Controls::Primitives::OrientedVirtualizingPanel, llm::OS::UI::Xaml::Controls::VirtualizingPanel, llm::OS::UI::Xaml::Controls::Panel, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<VirtualizingStackPanel, llm::OS::UI::Xaml::Controls::IVirtualizingStackPanelOverrides, llm::OS::UI::Xaml::Controls::Primitives::IOrientedVirtualizingPanel, llm::OS::UI::Xaml::Controls::Primitives::IScrollSnapPointsInfo, llm::OS::UI::Xaml::Controls::IInsertionPanel, llm::OS::UI::Xaml::Controls::IVirtualizingPanel, llm::OS::UI::Xaml::Controls::IVirtualizingPanelProtected, llm::OS::UI::Xaml::Controls::IVirtualizingPanelOverrides, llm::OS::UI::Xaml::Controls::IPanel, llm::OS::UI::Xaml::Controls::IPanel2, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        VirtualizingStackPanel(std::nullptr_t) noexcept {}
        VirtualizingStackPanel(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IVirtualizingStackPanel(ptr, take_ownership_from_abi) {}
        VirtualizingStackPanel();
        [[nodiscard]] static auto AreScrollSnapPointsRegularProperty();
        [[nodiscard]] static auto OrientationProperty();
        [[nodiscard]] static auto VirtualizationModeProperty();
        static auto GetVirtualizationMode(llm::OS::UI::Xaml::DependencyObject const& element);
        static auto SetVirtualizationMode(llm::OS::UI::Xaml::DependencyObject const& element, llm::OS::UI::Xaml::Controls::VirtualizationMode const& value);
        [[nodiscard]] static auto IsVirtualizingProperty();
        static auto GetIsVirtualizing(llm::OS::UI::Xaml::DependencyObject const& o);
    };
    struct __declspec(empty_bases) WebView : llm::OS::UI::Xaml::Controls::IWebView,
        impl::base<WebView, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<WebView, llm::OS::UI::Xaml::Controls::IWebView2, llm::OS::UI::Xaml::Controls::IWebView3, llm::OS::UI::Xaml::Controls::IWebView4, llm::OS::UI::Xaml::Controls::IWebView5, llm::OS::UI::Xaml::Controls::IWebView6, llm::OS::UI::Xaml::Controls::IWebView7, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        WebView(std::nullptr_t) noexcept {}
        WebView(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IWebView(ptr, take_ownership_from_abi) {}
        WebView();
        explicit WebView(llm::OS::UI::Xaml::Controls::WebViewExecutionMode const& executionMode);
        [[nodiscard]] static auto AnyScriptNotifyUri();
        [[nodiscard]] static auto SourceProperty();
        [[nodiscard]] static auto AllowedScriptNotifyUrisProperty();
        [[nodiscard]] static auto DataTransferPackageProperty();
        [[nodiscard]] static auto CanGoBackProperty();
        [[nodiscard]] static auto CanGoForwardProperty();
        [[nodiscard]] static auto DocumentTitleProperty();
        [[nodiscard]] static auto DefaultBackgroundColorProperty();
        [[nodiscard]] static auto ContainsFullScreenElementProperty();
        [[nodiscard]] static auto DefaultExecutionMode();
        static auto ClearTemporaryWebDataAsync();
        [[nodiscard]] static auto XYFocusLeftProperty();
        [[nodiscard]] static auto XYFocusRightProperty();
        [[nodiscard]] static auto XYFocusUpProperty();
        [[nodiscard]] static auto XYFocusDownProperty();
    };
    struct __declspec(empty_bases) WebViewBrush : llm::OS::UI::Xaml::Controls::IWebViewBrush,
        impl::base<WebViewBrush, llm::OS::UI::Xaml::Media::TileBrush, llm::OS::UI::Xaml::Media::Brush, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<WebViewBrush, llm::OS::UI::Xaml::Media::ITileBrush, llm::OS::UI::Xaml::Media::IBrush, llm::OS::UI::Xaml::Media::IBrushOverrides2, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        WebViewBrush(std::nullptr_t) noexcept {}
        WebViewBrush(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IWebViewBrush(ptr, take_ownership_from_abi) {}
        WebViewBrush();
        [[nodiscard]] static auto SourceNameProperty();
    };
    struct __declspec(empty_bases) WebViewContentLoadingEventArgs : llm::OS::UI::Xaml::Controls::IWebViewContentLoadingEventArgs
    {
        WebViewContentLoadingEventArgs(std::nullptr_t) noexcept {}
        WebViewContentLoadingEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IWebViewContentLoadingEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) WebViewDOMContentLoadedEventArgs : llm::OS::UI::Xaml::Controls::IWebViewDOMContentLoadedEventArgs
    {
        WebViewDOMContentLoadedEventArgs(std::nullptr_t) noexcept {}
        WebViewDOMContentLoadedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IWebViewDOMContentLoadedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) WebViewDeferredPermissionRequest : llm::OS::UI::Xaml::Controls::IWebViewDeferredPermissionRequest
    {
        WebViewDeferredPermissionRequest(std::nullptr_t) noexcept {}
        WebViewDeferredPermissionRequest(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IWebViewDeferredPermissionRequest(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) WebViewLongRunningScriptDetectedEventArgs : llm::OS::UI::Xaml::Controls::IWebViewLongRunningScriptDetectedEventArgs
    {
        WebViewLongRunningScriptDetectedEventArgs(std::nullptr_t) noexcept {}
        WebViewLongRunningScriptDetectedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IWebViewLongRunningScriptDetectedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) WebViewNavigationCompletedEventArgs : llm::OS::UI::Xaml::Controls::IWebViewNavigationCompletedEventArgs
    {
        WebViewNavigationCompletedEventArgs(std::nullptr_t) noexcept {}
        WebViewNavigationCompletedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IWebViewNavigationCompletedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) WebViewNavigationFailedEventArgs : llm::OS::UI::Xaml::Controls::IWebViewNavigationFailedEventArgs
    {
        WebViewNavigationFailedEventArgs(std::nullptr_t) noexcept {}
        WebViewNavigationFailedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IWebViewNavigationFailedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) WebViewNavigationStartingEventArgs : llm::OS::UI::Xaml::Controls::IWebViewNavigationStartingEventArgs
    {
        WebViewNavigationStartingEventArgs(std::nullptr_t) noexcept {}
        WebViewNavigationStartingEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IWebViewNavigationStartingEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) WebViewNewWindowRequestedEventArgs : llm::OS::UI::Xaml::Controls::IWebViewNewWindowRequestedEventArgs
    {
        WebViewNewWindowRequestedEventArgs(std::nullptr_t) noexcept {}
        WebViewNewWindowRequestedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IWebViewNewWindowRequestedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) WebViewPermissionRequest : llm::OS::UI::Xaml::Controls::IWebViewPermissionRequest
    {
        WebViewPermissionRequest(std::nullptr_t) noexcept {}
        WebViewPermissionRequest(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IWebViewPermissionRequest(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) WebViewPermissionRequestedEventArgs : llm::OS::UI::Xaml::Controls::IWebViewPermissionRequestedEventArgs
    {
        WebViewPermissionRequestedEventArgs(std::nullptr_t) noexcept {}
        WebViewPermissionRequestedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IWebViewPermissionRequestedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) WebViewSeparateProcessLostEventArgs : llm::OS::UI::Xaml::Controls::IWebViewSeparateProcessLostEventArgs
    {
        WebViewSeparateProcessLostEventArgs(std::nullptr_t) noexcept {}
        WebViewSeparateProcessLostEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IWebViewSeparateProcessLostEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) WebViewSettings : llm::OS::UI::Xaml::Controls::IWebViewSettings
    {
        WebViewSettings(std::nullptr_t) noexcept {}
        WebViewSettings(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IWebViewSettings(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) WebViewUnsupportedUriSchemeIdentifiedEventArgs : llm::OS::UI::Xaml::Controls::IWebViewUnsupportedUriSchemeIdentifiedEventArgs
    {
        WebViewUnsupportedUriSchemeIdentifiedEventArgs(std::nullptr_t) noexcept {}
        WebViewUnsupportedUriSchemeIdentifiedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IWebViewUnsupportedUriSchemeIdentifiedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) WebViewUnviewableContentIdentifiedEventArgs : llm::OS::UI::Xaml::Controls::IWebViewUnviewableContentIdentifiedEventArgs,
        impl::require<WebViewUnviewableContentIdentifiedEventArgs, llm::OS::UI::Xaml::Controls::IWebViewUnviewableContentIdentifiedEventArgs2>
    {
        WebViewUnviewableContentIdentifiedEventArgs(std::nullptr_t) noexcept {}
        WebViewUnviewableContentIdentifiedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IWebViewUnviewableContentIdentifiedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) WebViewWebResourceRequestedEventArgs : llm::OS::UI::Xaml::Controls::IWebViewWebResourceRequestedEventArgs
    {
        WebViewWebResourceRequestedEventArgs(std::nullptr_t) noexcept {}
        WebViewWebResourceRequestedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IWebViewWebResourceRequestedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) WrapGrid : llm::OS::UI::Xaml::Controls::IWrapGrid,
        impl::base<WrapGrid, llm::OS::UI::Xaml::Controls::Primitives::OrientedVirtualizingPanel, llm::OS::UI::Xaml::Controls::VirtualizingPanel, llm::OS::UI::Xaml::Controls::Panel, llm::OS::UI::Xaml::FrameworkElement, llm::OS::UI::Xaml::UIElement, llm::OS::UI::Xaml::DependencyObject>,
        impl::require<WrapGrid, llm::OS::UI::Xaml::Controls::Primitives::IOrientedVirtualizingPanel, llm::OS::UI::Xaml::Controls::Primitives::IScrollSnapPointsInfo, llm::OS::UI::Xaml::Controls::IInsertionPanel, llm::OS::UI::Xaml::Controls::IVirtualizingPanel, llm::OS::UI::Xaml::Controls::IVirtualizingPanelProtected, llm::OS::UI::Xaml::Controls::IVirtualizingPanelOverrides, llm::OS::UI::Xaml::Controls::IPanel, llm::OS::UI::Xaml::Controls::IPanel2, llm::OS::UI::Xaml::IFrameworkElement, llm::OS::UI::Xaml::IFrameworkElement2, llm::OS::UI::Xaml::IFrameworkElement3, llm::OS::UI::Xaml::IFrameworkElement4, llm::OS::UI::Xaml::IFrameworkElement6, llm::OS::UI::Xaml::IFrameworkElement7, llm::OS::UI::Xaml::IFrameworkElementProtected7, llm::OS::UI::Xaml::IFrameworkElementOverrides, llm::OS::UI::Xaml::IFrameworkElementOverrides2, llm::OS::UI::Xaml::IUIElement, llm::OS::UI::Xaml::IUIElement2, llm::OS::UI::Xaml::IUIElement3, llm::OS::UI::Xaml::IUIElement4, llm::OS::UI::Xaml::IUIElement5, llm::OS::UI::Xaml::IUIElement7, llm::OS::UI::Xaml::IUIElement8, llm::OS::UI::Xaml::IUIElement9, llm::OS::UI::Xaml::IUIElement10, llm::OS::UI::Xaml::IUIElementOverrides, llm::OS::UI::Xaml::IUIElementOverrides7, llm::OS::UI::Xaml::IUIElementOverrides8, llm::OS::UI::Xaml::IUIElementOverrides9, llm::OS::UI::Composition::IAnimationObject, llm::OS::UI::Composition::IVisualElement, llm::OS::UI::Xaml::IDependencyObject, llm::OS::UI::Xaml::IDependencyObject2>
    {
        WrapGrid(std::nullptr_t) noexcept {}
        WrapGrid(void* ptr, take_ownership_from_abi_t) noexcept : llm::OS::UI::Xaml::Controls::IWrapGrid(ptr, take_ownership_from_abi) {}
        WrapGrid();
        [[nodiscard]] static auto ItemWidthProperty();
        [[nodiscard]] static auto ItemHeightProperty();
        [[nodiscard]] static auto OrientationProperty();
        [[nodiscard]] static auto HorizontalChildrenAlignmentProperty();
        [[nodiscard]] static auto VerticalChildrenAlignmentProperty();
        [[nodiscard]] static auto MaximumRowsOrColumnsProperty();
    };
    template <typename D>
    class IAppBarOverridesT
    {
        D& shim() noexcept { return *static_cast<D*>(this); }
        D const& shim() const noexcept { return *static_cast<const D*>(this); }
    public:
        using IAppBarOverrides = llm::OS::UI::Xaml::Controls::IAppBarOverrides;
        LLM_IMPL_AUTO(void) OnClosed(llm::OS::Foundation::IInspectable const& e) const;
        LLM_IMPL_AUTO(void) OnOpened(llm::OS::Foundation::IInspectable const& e) const;
    };
    template <typename D>
    class IAppBarOverrides3T
    {
        D& shim() noexcept { return *static_cast<D*>(this); }
        D const& shim() const noexcept { return *static_cast<const D*>(this); }
    public:
        using IAppBarOverrides3 = llm::OS::UI::Xaml::Controls::IAppBarOverrides3;
        LLM_IMPL_AUTO(void) OnClosing(llm::OS::Foundation::IInspectable const& e) const;
        LLM_IMPL_AUTO(void) OnOpening(llm::OS::Foundation::IInspectable const& e) const;
    };
    template <typename D>
    class IComboBoxOverridesT
    {
        D& shim() noexcept { return *static_cast<D*>(this); }
        D const& shim() const noexcept { return *static_cast<const D*>(this); }
    public:
        using IComboBoxOverrides = llm::OS::UI::Xaml::Controls::IComboBoxOverrides;
        LLM_IMPL_AUTO(void) OnDropDownClosed(llm::OS::Foundation::IInspectable const& e) const;
        LLM_IMPL_AUTO(void) OnDropDownOpened(llm::OS::Foundation::IInspectable const& e) const;
    };
    template <typename D>
    class IContentControlOverridesT
    {
        D& shim() noexcept { return *static_cast<D*>(this); }
        D const& shim() const noexcept { return *static_cast<const D*>(this); }
    public:
        using IContentControlOverrides = llm::OS::UI::Xaml::Controls::IContentControlOverrides;
        LLM_IMPL_AUTO(void) OnContentChanged(llm::OS::Foundation::IInspectable const& oldContent, llm::OS::Foundation::IInspectable const& newContent) const;
        LLM_IMPL_AUTO(void) OnContentTemplateChanged(llm::OS::UI::Xaml::DataTemplate const& oldContentTemplate, llm::OS::UI::Xaml::DataTemplate const& newContentTemplate) const;
        LLM_IMPL_AUTO(void) OnContentTemplateSelectorChanged(llm::OS::UI::Xaml::Controls::DataTemplateSelector const& oldContentTemplateSelector, llm::OS::UI::Xaml::Controls::DataTemplateSelector const& newContentTemplateSelector) const;
    };
    template <typename D>
    class IContentPresenterOverridesT
    {
        D& shim() noexcept { return *static_cast<D*>(this); }
        D const& shim() const noexcept { return *static_cast<const D*>(this); }
    public:
        using IContentPresenterOverrides = llm::OS::UI::Xaml::Controls::IContentPresenterOverrides;
        LLM_IMPL_AUTO(void) OnContentTemplateChanged(llm::OS::UI::Xaml::DataTemplate const& oldContentTemplate, llm::OS::UI::Xaml::DataTemplate const& newContentTemplate) const;
        LLM_IMPL_AUTO(void) OnContentTemplateSelectorChanged(llm::OS::UI::Xaml::Controls::DataTemplateSelector const& oldContentTemplateSelector, llm::OS::UI::Xaml::Controls::DataTemplateSelector const& newContentTemplateSelector) const;
    };
    template <typename D>
    class IControlOverridesT
    {
        D& shim() noexcept { return *static_cast<D*>(this); }
        D const& shim() const noexcept { return *static_cast<const D*>(this); }
    public:
        using IControlOverrides = llm::OS::UI::Xaml::Controls::IControlOverrides;
        LLM_IMPL_AUTO(void) OnPointerEntered(llm::OS::UI::Xaml::Input::PointerRoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnPointerPressed(llm::OS::UI::Xaml::Input::PointerRoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnPointerMoved(llm::OS::UI::Xaml::Input::PointerRoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnPointerReleased(llm::OS::UI::Xaml::Input::PointerRoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnPointerExited(llm::OS::UI::Xaml::Input::PointerRoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnPointerCaptureLost(llm::OS::UI::Xaml::Input::PointerRoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnPointerCanceled(llm::OS::UI::Xaml::Input::PointerRoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnPointerWheelChanged(llm::OS::UI::Xaml::Input::PointerRoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnTapped(llm::OS::UI::Xaml::Input::TappedRoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnDoubleTapped(llm::OS::UI::Xaml::Input::DoubleTappedRoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnHolding(llm::OS::UI::Xaml::Input::HoldingRoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnRightTapped(llm::OS::UI::Xaml::Input::RightTappedRoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnManipulationStarting(llm::OS::UI::Xaml::Input::ManipulationStartingRoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnManipulationInertiaStarting(llm::OS::UI::Xaml::Input::ManipulationInertiaStartingRoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnManipulationStarted(llm::OS::UI::Xaml::Input::ManipulationStartedRoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnManipulationDelta(llm::OS::UI::Xaml::Input::ManipulationDeltaRoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnManipulationCompleted(llm::OS::UI::Xaml::Input::ManipulationCompletedRoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnKeyUp(llm::OS::UI::Xaml::Input::KeyRoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnKeyDown(llm::OS::UI::Xaml::Input::KeyRoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnGotFocus(llm::OS::UI::Xaml::RoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnLostFocus(llm::OS::UI::Xaml::RoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnDragEnter(llm::OS::UI::Xaml::DragEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnDragLeave(llm::OS::UI::Xaml::DragEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnDragOver(llm::OS::UI::Xaml::DragEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnDrop(llm::OS::UI::Xaml::DragEventArgs const& e) const;
    };
    template <typename D>
    class IControlOverrides6T
    {
        D& shim() noexcept { return *static_cast<D*>(this); }
        D const& shim() const noexcept { return *static_cast<const D*>(this); }
    public:
        using IControlOverrides6 = llm::OS::UI::Xaml::Controls::IControlOverrides6;
        LLM_IMPL_AUTO(void) OnPreviewKeyDown(llm::OS::UI::Xaml::Input::KeyRoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnPreviewKeyUp(llm::OS::UI::Xaml::Input::KeyRoutedEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnCharacterReceived(llm::OS::UI::Xaml::Input::CharacterReceivedRoutedEventArgs const& e) const;
    };
    template <typename D>
    class IDataTemplateSelectorOverridesT
    {
        D& shim() noexcept { return *static_cast<D*>(this); }
        D const& shim() const noexcept { return *static_cast<const D*>(this); }
    public:
        using IDataTemplateSelectorOverrides = llm::OS::UI::Xaml::Controls::IDataTemplateSelectorOverrides;
        LLM_IMPL_AUTO(llm::OS::UI::Xaml::DataTemplate) SelectTemplateCore(llm::OS::Foundation::IInspectable const& item, llm::OS::UI::Xaml::DependencyObject const& container) const;
    };
    template <typename D>
    class IDataTemplateSelectorOverrides2T
    {
        D& shim() noexcept { return *static_cast<D*>(this); }
        D const& shim() const noexcept { return *static_cast<const D*>(this); }
    public:
        using IDataTemplateSelectorOverrides2 = llm::OS::UI::Xaml::Controls::IDataTemplateSelectorOverrides2;
        LLM_IMPL_AUTO(llm::OS::UI::Xaml::DataTemplate) SelectTemplateCore(llm::OS::Foundation::IInspectable const& item) const;
    };
    template <typename D>
    class IGroupStyleSelectorOverridesT
    {
        D& shim() noexcept { return *static_cast<D*>(this); }
        D const& shim() const noexcept { return *static_cast<const D*>(this); }
    public:
        using IGroupStyleSelectorOverrides = llm::OS::UI::Xaml::Controls::IGroupStyleSelectorOverrides;
        LLM_IMPL_AUTO(llm::OS::UI::Xaml::Controls::GroupStyle) SelectGroupStyleCore(llm::OS::Foundation::IInspectable const& group, uint32_t level) const;
    };
    template <typename D>
    class IInkToolbarCustomPenOverridesT
    {
        D& shim() noexcept { return *static_cast<D*>(this); }
        D const& shim() const noexcept { return *static_cast<const D*>(this); }
    public:
        using IInkToolbarCustomPenOverrides = llm::OS::UI::Xaml::Controls::IInkToolbarCustomPenOverrides;
        LLM_IMPL_AUTO(llm::OS::UI::Input::Inking::InkDrawingAttributes) CreateInkDrawingAttributesCore(llm::OS::UI::Xaml::Media::Brush const& brush, double strokeWidth) const;
    };
    template <typename D>
    class IItemsControlOverridesT
    {
        D& shim() noexcept { return *static_cast<D*>(this); }
        D const& shim() const noexcept { return *static_cast<const D*>(this); }
    public:
        using IItemsControlOverrides = llm::OS::UI::Xaml::Controls::IItemsControlOverrides;
        LLM_IMPL_AUTO(bool) IsItemItsOwnContainerOverride(llm::OS::Foundation::IInspectable const& item) const;
        LLM_IMPL_AUTO(llm::OS::UI::Xaml::DependencyObject) GetContainerForItemOverride() const;
        LLM_IMPL_AUTO(void) ClearContainerForItemOverride(llm::OS::UI::Xaml::DependencyObject const& element, llm::OS::Foundation::IInspectable const& item) const;
        LLM_IMPL_AUTO(void) PrepareContainerForItemOverride(llm::OS::UI::Xaml::DependencyObject const& element, llm::OS::Foundation::IInspectable const& item) const;
        LLM_IMPL_AUTO(void) OnItemsChanged(llm::OS::Foundation::IInspectable const& e) const;
        LLM_IMPL_AUTO(void) OnItemContainerStyleChanged(llm::OS::UI::Xaml::Style const& oldItemContainerStyle, llm::OS::UI::Xaml::Style const& newItemContainerStyle) const;
        LLM_IMPL_AUTO(void) OnItemContainerStyleSelectorChanged(llm::OS::UI::Xaml::Controls::StyleSelector const& oldItemContainerStyleSelector, llm::OS::UI::Xaml::Controls::StyleSelector const& newItemContainerStyleSelector) const;
        LLM_IMPL_AUTO(void) OnItemTemplateChanged(llm::OS::UI::Xaml::DataTemplate const& oldItemTemplate, llm::OS::UI::Xaml::DataTemplate const& newItemTemplate) const;
        LLM_IMPL_AUTO(void) OnItemTemplateSelectorChanged(llm::OS::UI::Xaml::Controls::DataTemplateSelector const& oldItemTemplateSelector, llm::OS::UI::Xaml::Controls::DataTemplateSelector const& newItemTemplateSelector) const;
        LLM_IMPL_AUTO(void) OnGroupStyleSelectorChanged(llm::OS::UI::Xaml::Controls::GroupStyleSelector const& oldGroupStyleSelector, llm::OS::UI::Xaml::Controls::GroupStyleSelector const& newGroupStyleSelector) const;
    };
    template <typename D>
    class IPageOverridesT
    {
        D& shim() noexcept { return *static_cast<D*>(this); }
        D const& shim() const noexcept { return *static_cast<const D*>(this); }
    public:
        using IPageOverrides = llm::OS::UI::Xaml::Controls::IPageOverrides;
        LLM_IMPL_AUTO(void) OnNavigatedFrom(llm::OS::UI::Xaml::Navigation::NavigationEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnNavigatedTo(llm::OS::UI::Xaml::Navigation::NavigationEventArgs const& e) const;
        LLM_IMPL_AUTO(void) OnNavigatingFrom(llm::OS::UI::Xaml::Navigation::NavigatingCancelEventArgs const& e) const;
    };
    template <typename D>
    class IStyleSelectorOverridesT
    {
        D& shim() noexcept { return *static_cast<D*>(this); }
        D const& shim() const noexcept { return *static_cast<const D*>(this); }
    public:
        using IStyleSelectorOverrides = llm::OS::UI::Xaml::Controls::IStyleSelectorOverrides;
        LLM_IMPL_AUTO(llm::OS::UI::Xaml::Style) SelectStyleCore(llm::OS::Foundation::IInspectable const& item, llm::OS::UI::Xaml::DependencyObject const& container) const;
    };
    template <typename D>
    class IToggleSwitchOverridesT
    {
        D& shim() noexcept { return *static_cast<D*>(this); }
        D const& shim() const noexcept { return *static_cast<const D*>(this); }
    public:
        using IToggleSwitchOverrides = llm::OS::UI::Xaml::Controls::IToggleSwitchOverrides;
        LLM_IMPL_AUTO(void) OnToggled() const;
        LLM_IMPL_AUTO(void) OnOnContentChanged(llm::OS::Foundation::IInspectable const& oldContent, llm::OS::Foundation::IInspectable const& newContent) const;
        LLM_IMPL_AUTO(void) OnOffContentChanged(llm::OS::Foundation::IInspectable const& oldContent, llm::OS::Foundation::IInspectable const& newContent) const;
        LLM_IMPL_AUTO(void) OnHeaderChanged(llm::OS::Foundation::IInspectable const& oldContent, llm::OS::Foundation::IInspectable const& newContent) const;
    };
    template <typename D>
    class IVirtualizingPanelOverridesT
    {
        D& shim() noexcept { return *static_cast<D*>(this); }
        D const& shim() const noexcept { return *static_cast<const D*>(this); }
    public:
        using IVirtualizingPanelOverrides = llm::OS::UI::Xaml::Controls::IVirtualizingPanelOverrides;
        LLM_IMPL_AUTO(void) OnItemsChanged(llm::OS::Foundation::IInspectable const& sender, llm::OS::UI::Xaml::Controls::Primitives::ItemsChangedEventArgs const& args) const;
        LLM_IMPL_AUTO(void) OnClearChildren() const;
        LLM_IMPL_AUTO(void) BringIndexIntoView(int32_t index) const;
    };
    template <typename D>
    class IVirtualizingStackPanelOverridesT
    {
        D& shim() noexcept { return *static_cast<D*>(this); }
        D const& shim() const noexcept { return *static_cast<const D*>(this); }
    public:
        using IVirtualizingStackPanelOverrides = llm::OS::UI::Xaml::Controls::IVirtualizingStackPanelOverrides;
        LLM_IMPL_AUTO(void) OnCleanUpVirtualizedItem(llm::OS::UI::Xaml::Controls::CleanUpVirtualizedItemEventArgs const& e) const;
    };
}
#endif
