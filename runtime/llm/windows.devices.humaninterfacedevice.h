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
#ifndef LLM_OS_Devices_HumanInterfaceDevice_H
#define LLM_OS_Devices_HumanInterfaceDevice_H
#include "llm/base.h"
static_assert(llm::check_version(CPPLLM_VERSION, "2.0.220110.5"), "Mismatched C++/WinRT headers.");
#define CPPLLM_VERSION "2.0.220110.5"
#include "llm/Windows.Devices.h"
#include "llm/impl/Windows.Foundation.2.h"
#include "llm/impl/Windows.Foundation.Collections.2.h"
#include "llm/impl/Windows.Storage.2.h"
#include "llm/impl/Windows.Storage.Streams.2.h"
#include "llm/impl/Windows.Devices.HumanInterfaceDevice.2.h"
namespace llm::impl
{
    template <typename D> LLM_IMPL_AUTO(uint32_t) consume_Windows_Devices_HumanInterfaceDevice_IHidBooleanControl<D>::Id() const
    {
        uint32_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControl)->get_Id(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint16_t) consume_Windows_Devices_HumanInterfaceDevice_IHidBooleanControl<D>::UsagePage() const
    {
        uint16_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControl)->get_UsagePage(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint16_t) consume_Windows_Devices_HumanInterfaceDevice_IHidBooleanControl<D>::UsageId() const
    {
        uint16_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControl)->get_UsageId(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_Devices_HumanInterfaceDevice_IHidBooleanControl<D>::IsActive() const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControl)->get_IsActive(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_Devices_HumanInterfaceDevice_IHidBooleanControl<D>::IsActive(bool value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControl)->put_IsActive(value));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidBooleanControlDescription) consume_Windows_Devices_HumanInterfaceDevice_IHidBooleanControl<D>::ControlDescription() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControl)->get_ControlDescription(&value));
        return llm::OS::Devices::HumanInterfaceDevice::HidBooleanControlDescription{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(uint32_t) consume_Windows_Devices_HumanInterfaceDevice_IHidBooleanControlDescription<D>::Id() const
    {
        uint32_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControlDescription)->get_Id(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint16_t) consume_Windows_Devices_HumanInterfaceDevice_IHidBooleanControlDescription<D>::ReportId() const
    {
        uint16_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControlDescription)->get_ReportId(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidReportType) consume_Windows_Devices_HumanInterfaceDevice_IHidBooleanControlDescription<D>::ReportType() const
    {
        llm::OS::Devices::HumanInterfaceDevice::HidReportType value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControlDescription)->get_ReportType(reinterpret_cast<int32_t*>(&value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint16_t) consume_Windows_Devices_HumanInterfaceDevice_IHidBooleanControlDescription<D>::UsagePage() const
    {
        uint16_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControlDescription)->get_UsagePage(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint16_t) consume_Windows_Devices_HumanInterfaceDevice_IHidBooleanControlDescription<D>::UsageId() const
    {
        uint16_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControlDescription)->get_UsageId(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Collections::IVectorView<llm::OS::Devices::HumanInterfaceDevice::HidCollection>) consume_Windows_Devices_HumanInterfaceDevice_IHidBooleanControlDescription<D>::ParentCollections() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControlDescription)->get_ParentCollections(&value));
        return llm::OS::Foundation::Collections::IVectorView<llm::OS::Devices::HumanInterfaceDevice::HidCollection>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_Devices_HumanInterfaceDevice_IHidBooleanControlDescription2<D>::IsAbsolute() const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControlDescription2)->get_IsAbsolute(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint32_t) consume_Windows_Devices_HumanInterfaceDevice_IHidCollection<D>::Id() const
    {
        uint32_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidCollection)->get_Id(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidCollectionType) consume_Windows_Devices_HumanInterfaceDevice_IHidCollection<D>::Type() const
    {
        llm::OS::Devices::HumanInterfaceDevice::HidCollectionType value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidCollection)->get_Type(reinterpret_cast<int32_t*>(&value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint32_t) consume_Windows_Devices_HumanInterfaceDevice_IHidCollection<D>::UsagePage() const
    {
        uint32_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidCollection)->get_UsagePage(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint32_t) consume_Windows_Devices_HumanInterfaceDevice_IHidCollection<D>::UsageId() const
    {
        uint32_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidCollection)->get_UsageId(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint16_t) consume_Windows_Devices_HumanInterfaceDevice_IHidDevice<D>::VendorId() const
    {
        uint16_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDevice)->get_VendorId(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint16_t) consume_Windows_Devices_HumanInterfaceDevice_IHidDevice<D>::ProductId() const
    {
        uint16_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDevice)->get_ProductId(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint16_t) consume_Windows_Devices_HumanInterfaceDevice_IHidDevice<D>::Version() const
    {
        uint16_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDevice)->get_Version(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint16_t) consume_Windows_Devices_HumanInterfaceDevice_IHidDevice<D>::UsagePage() const
    {
        uint16_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDevice)->get_UsagePage(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint16_t) consume_Windows_Devices_HumanInterfaceDevice_IHidDevice<D>::UsageId() const
    {
        uint16_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDevice)->get_UsageId(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<llm::OS::Devices::HumanInterfaceDevice::HidInputReport>) consume_Windows_Devices_HumanInterfaceDevice_IHidDevice<D>::GetInputReportAsync() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDevice)->GetInputReportAsync(&value));
        return llm::OS::Foundation::IAsyncOperation<llm::OS::Devices::HumanInterfaceDevice::HidInputReport>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<llm::OS::Devices::HumanInterfaceDevice::HidInputReport>) consume_Windows_Devices_HumanInterfaceDevice_IHidDevice<D>::GetInputReportAsync(uint16_t reportId) const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDevice)->GetInputReportByIdAsync(reportId, &value));
        return llm::OS::Foundation::IAsyncOperation<llm::OS::Devices::HumanInterfaceDevice::HidInputReport>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<llm::OS::Devices::HumanInterfaceDevice::HidFeatureReport>) consume_Windows_Devices_HumanInterfaceDevice_IHidDevice<D>::GetFeatureReportAsync() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDevice)->GetFeatureReportAsync(&value));
        return llm::OS::Foundation::IAsyncOperation<llm::OS::Devices::HumanInterfaceDevice::HidFeatureReport>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<llm::OS::Devices::HumanInterfaceDevice::HidFeatureReport>) consume_Windows_Devices_HumanInterfaceDevice_IHidDevice<D>::GetFeatureReportAsync(uint16_t reportId) const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDevice)->GetFeatureReportByIdAsync(reportId, &value));
        return llm::OS::Foundation::IAsyncOperation<llm::OS::Devices::HumanInterfaceDevice::HidFeatureReport>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidOutputReport) consume_Windows_Devices_HumanInterfaceDevice_IHidDevice<D>::CreateOutputReport() const
    {
        void* outputReport{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDevice)->CreateOutputReport(&outputReport));
        return llm::OS::Devices::HumanInterfaceDevice::HidOutputReport{ outputReport, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidOutputReport) consume_Windows_Devices_HumanInterfaceDevice_IHidDevice<D>::CreateOutputReport(uint16_t reportId) const
    {
        void* outputReport{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDevice)->CreateOutputReportById(reportId, &outputReport));
        return llm::OS::Devices::HumanInterfaceDevice::HidOutputReport{ outputReport, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidFeatureReport) consume_Windows_Devices_HumanInterfaceDevice_IHidDevice<D>::CreateFeatureReport() const
    {
        void* featureReport{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDevice)->CreateFeatureReport(&featureReport));
        return llm::OS::Devices::HumanInterfaceDevice::HidFeatureReport{ featureReport, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidFeatureReport) consume_Windows_Devices_HumanInterfaceDevice_IHidDevice<D>::CreateFeatureReport(uint16_t reportId) const
    {
        void* featureReport{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDevice)->CreateFeatureReportById(reportId, &featureReport));
        return llm::OS::Devices::HumanInterfaceDevice::HidFeatureReport{ featureReport, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<uint32_t>) consume_Windows_Devices_HumanInterfaceDevice_IHidDevice<D>::SendOutputReportAsync(llm::OS::Devices::HumanInterfaceDevice::HidOutputReport const& outputReport) const
    {
        void* operation{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDevice)->SendOutputReportAsync(*(void**)(&outputReport), &operation));
        return llm::OS::Foundation::IAsyncOperation<uint32_t>{ operation, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<uint32_t>) consume_Windows_Devices_HumanInterfaceDevice_IHidDevice<D>::SendFeatureReportAsync(llm::OS::Devices::HumanInterfaceDevice::HidFeatureReport const& featureReport) const
    {
        void* operation{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDevice)->SendFeatureReportAsync(*(void**)(&featureReport), &operation));
        return llm::OS::Foundation::IAsyncOperation<uint32_t>{ operation, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Collections::IVectorView<llm::OS::Devices::HumanInterfaceDevice::HidBooleanControlDescription>) consume_Windows_Devices_HumanInterfaceDevice_IHidDevice<D>::GetBooleanControlDescriptions(llm::OS::Devices::HumanInterfaceDevice::HidReportType const& reportType, uint16_t usagePage, uint16_t usageId) const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDevice)->GetBooleanControlDescriptions(static_cast<int32_t>(reportType), usagePage, usageId, &value));
        return llm::OS::Foundation::Collections::IVectorView<llm::OS::Devices::HumanInterfaceDevice::HidBooleanControlDescription>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Collections::IVectorView<llm::OS::Devices::HumanInterfaceDevice::HidNumericControlDescription>) consume_Windows_Devices_HumanInterfaceDevice_IHidDevice<D>::GetNumericControlDescriptions(llm::OS::Devices::HumanInterfaceDevice::HidReportType const& reportType, uint16_t usagePage, uint16_t usageId) const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDevice)->GetNumericControlDescriptions(static_cast<int32_t>(reportType), usagePage, usageId, &value));
        return llm::OS::Foundation::Collections::IVectorView<llm::OS::Devices::HumanInterfaceDevice::HidNumericControlDescription>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::event_token) consume_Windows_Devices_HumanInterfaceDevice_IHidDevice<D>::InputReportReceived(llm::OS::Foundation::TypedEventHandler<llm::OS::Devices::HumanInterfaceDevice::HidDevice, llm::OS::Devices::HumanInterfaceDevice::HidInputReportReceivedEventArgs> const& reportHandler) const
    {
        llm::event_token token{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDevice)->add_InputReportReceived(*(void**)(&reportHandler), put_abi(token)));
        return token;
    }
    template <typename D> typename consume_Windows_Devices_HumanInterfaceDevice_IHidDevice<D>::InputReportReceived_revoker consume_Windows_Devices_HumanInterfaceDevice_IHidDevice<D>::InputReportReceived(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::Devices::HumanInterfaceDevice::HidDevice, llm::OS::Devices::HumanInterfaceDevice::HidInputReportReceivedEventArgs> const& reportHandler) const
    {
        return impl::make_event_revoker<D, InputReportReceived_revoker>(this, InputReportReceived(reportHandler));
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_Devices_HumanInterfaceDevice_IHidDevice<D>::InputReportReceived(llm::event_token const& token) const noexcept
    {
        LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDevice)->remove_InputReportReceived(impl::bind_in(token));
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_Devices_HumanInterfaceDevice_IHidDeviceStatics<D>::GetDeviceSelector(uint16_t usagePage, uint16_t usageId) const
    {
        void* selector{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDeviceStatics)->GetDeviceSelector(usagePage, usageId, &selector));
        return hstring{ selector, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_Devices_HumanInterfaceDevice_IHidDeviceStatics<D>::GetDeviceSelector(uint16_t usagePage, uint16_t usageId, uint16_t vendorId, uint16_t productId) const
    {
        void* selector{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDeviceStatics)->GetDeviceSelectorVidPid(usagePage, usageId, vendorId, productId, &selector));
        return hstring{ selector, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<llm::OS::Devices::HumanInterfaceDevice::HidDevice>) consume_Windows_Devices_HumanInterfaceDevice_IHidDeviceStatics<D>::FromIdAsync(param::hstring const& deviceId, llm::OS::Storage::FileAccessMode const& accessMode) const
    {
        void* hidDevice{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidDeviceStatics)->FromIdAsync(*(void**)(&deviceId), static_cast<int32_t>(accessMode), &hidDevice));
        return llm::OS::Foundation::IAsyncOperation<llm::OS::Devices::HumanInterfaceDevice::HidDevice>{ hidDevice, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(uint16_t) consume_Windows_Devices_HumanInterfaceDevice_IHidFeatureReport<D>::Id() const
    {
        uint16_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidFeatureReport)->get_Id(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Storage::Streams::IBuffer) consume_Windows_Devices_HumanInterfaceDevice_IHidFeatureReport<D>::Data() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidFeatureReport)->get_Data(&value));
        return llm::OS::Storage::Streams::IBuffer{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_Devices_HumanInterfaceDevice_IHidFeatureReport<D>::Data(llm::OS::Storage::Streams::IBuffer const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidFeatureReport)->put_Data(*(void**)(&value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl) consume_Windows_Devices_HumanInterfaceDevice_IHidFeatureReport<D>::GetBooleanControl(uint16_t usagePage, uint16_t usageId) const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidFeatureReport)->GetBooleanControl(usagePage, usageId, &value));
        return llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl) consume_Windows_Devices_HumanInterfaceDevice_IHidFeatureReport<D>::GetBooleanControlByDescription(llm::OS::Devices::HumanInterfaceDevice::HidBooleanControlDescription const& controlDescription) const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidFeatureReport)->GetBooleanControlByDescription(*(void**)(&controlDescription), &value));
        return llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidNumericControl) consume_Windows_Devices_HumanInterfaceDevice_IHidFeatureReport<D>::GetNumericControl(uint16_t usagePage, uint16_t usageId) const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidFeatureReport)->GetNumericControl(usagePage, usageId, &value));
        return llm::OS::Devices::HumanInterfaceDevice::HidNumericControl{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidNumericControl) consume_Windows_Devices_HumanInterfaceDevice_IHidFeatureReport<D>::GetNumericControlByDescription(llm::OS::Devices::HumanInterfaceDevice::HidNumericControlDescription const& controlDescription) const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidFeatureReport)->GetNumericControlByDescription(*(void**)(&controlDescription), &value));
        return llm::OS::Devices::HumanInterfaceDevice::HidNumericControl{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(uint16_t) consume_Windows_Devices_HumanInterfaceDevice_IHidInputReport<D>::Id() const
    {
        uint16_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidInputReport)->get_Id(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Storage::Streams::IBuffer) consume_Windows_Devices_HumanInterfaceDevice_IHidInputReport<D>::Data() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidInputReport)->get_Data(&value));
        return llm::OS::Storage::Streams::IBuffer{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Collections::IVectorView<llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl>) consume_Windows_Devices_HumanInterfaceDevice_IHidInputReport<D>::ActivatedBooleanControls() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidInputReport)->get_ActivatedBooleanControls(&value));
        return llm::OS::Foundation::Collections::IVectorView<llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Collections::IVectorView<llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl>) consume_Windows_Devices_HumanInterfaceDevice_IHidInputReport<D>::TransitionedBooleanControls() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidInputReport)->get_TransitionedBooleanControls(&value));
        return llm::OS::Foundation::Collections::IVectorView<llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl) consume_Windows_Devices_HumanInterfaceDevice_IHidInputReport<D>::GetBooleanControl(uint16_t usagePage, uint16_t usageId) const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidInputReport)->GetBooleanControl(usagePage, usageId, &value));
        return llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl) consume_Windows_Devices_HumanInterfaceDevice_IHidInputReport<D>::GetBooleanControlByDescription(llm::OS::Devices::HumanInterfaceDevice::HidBooleanControlDescription const& controlDescription) const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidInputReport)->GetBooleanControlByDescription(*(void**)(&controlDescription), &value));
        return llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidNumericControl) consume_Windows_Devices_HumanInterfaceDevice_IHidInputReport<D>::GetNumericControl(uint16_t usagePage, uint16_t usageId) const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidInputReport)->GetNumericControl(usagePage, usageId, &value));
        return llm::OS::Devices::HumanInterfaceDevice::HidNumericControl{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidNumericControl) consume_Windows_Devices_HumanInterfaceDevice_IHidInputReport<D>::GetNumericControlByDescription(llm::OS::Devices::HumanInterfaceDevice::HidNumericControlDescription const& controlDescription) const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidInputReport)->GetNumericControlByDescription(*(void**)(&controlDescription), &value));
        return llm::OS::Devices::HumanInterfaceDevice::HidNumericControl{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidInputReport) consume_Windows_Devices_HumanInterfaceDevice_IHidInputReportReceivedEventArgs<D>::Report() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidInputReportReceivedEventArgs)->get_Report(&value));
        return llm::OS::Devices::HumanInterfaceDevice::HidInputReport{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(uint32_t) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControl<D>::Id() const
    {
        uint32_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControl)->get_Id(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControl<D>::IsGrouped() const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControl)->get_IsGrouped(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint16_t) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControl<D>::UsagePage() const
    {
        uint16_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControl)->get_UsagePage(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint16_t) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControl<D>::UsageId() const
    {
        uint16_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControl)->get_UsageId(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(int64_t) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControl<D>::Value() const
    {
        int64_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControl)->get_Value(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControl<D>::Value(int64_t value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControl)->put_Value(value));
    }
    template <typename D> LLM_IMPL_AUTO(int64_t) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControl<D>::ScaledValue() const
    {
        int64_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControl)->get_ScaledValue(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControl<D>::ScaledValue(int64_t value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControl)->put_ScaledValue(value));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidNumericControlDescription) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControl<D>::ControlDescription() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControl)->get_ControlDescription(&value));
        return llm::OS::Devices::HumanInterfaceDevice::HidNumericControlDescription{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(uint32_t) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControlDescription<D>::Id() const
    {
        uint32_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControlDescription)->get_Id(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint16_t) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControlDescription<D>::ReportId() const
    {
        uint16_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControlDescription)->get_ReportId(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidReportType) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControlDescription<D>::ReportType() const
    {
        llm::OS::Devices::HumanInterfaceDevice::HidReportType value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControlDescription)->get_ReportType(reinterpret_cast<int32_t*>(&value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint32_t) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControlDescription<D>::ReportSize() const
    {
        uint32_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControlDescription)->get_ReportSize(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint32_t) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControlDescription<D>::ReportCount() const
    {
        uint32_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControlDescription)->get_ReportCount(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint16_t) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControlDescription<D>::UsagePage() const
    {
        uint16_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControlDescription)->get_UsagePage(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint16_t) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControlDescription<D>::UsageId() const
    {
        uint16_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControlDescription)->get_UsageId(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(int32_t) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControlDescription<D>::LogicalMinimum() const
    {
        int32_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControlDescription)->get_LogicalMinimum(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(int32_t) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControlDescription<D>::LogicalMaximum() const
    {
        int32_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControlDescription)->get_LogicalMaximum(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(int32_t) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControlDescription<D>::PhysicalMinimum() const
    {
        int32_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControlDescription)->get_PhysicalMinimum(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(int32_t) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControlDescription<D>::PhysicalMaximum() const
    {
        int32_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControlDescription)->get_PhysicalMaximum(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint32_t) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControlDescription<D>::UnitExponent() const
    {
        uint32_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControlDescription)->get_UnitExponent(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(uint32_t) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControlDescription<D>::Unit() const
    {
        uint32_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControlDescription)->get_Unit(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControlDescription<D>::IsAbsolute() const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControlDescription)->get_IsAbsolute(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControlDescription<D>::HasNull() const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControlDescription)->get_HasNull(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Collections::IVectorView<llm::OS::Devices::HumanInterfaceDevice::HidCollection>) consume_Windows_Devices_HumanInterfaceDevice_IHidNumericControlDescription<D>::ParentCollections() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidNumericControlDescription)->get_ParentCollections(&value));
        return llm::OS::Foundation::Collections::IVectorView<llm::OS::Devices::HumanInterfaceDevice::HidCollection>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(uint16_t) consume_Windows_Devices_HumanInterfaceDevice_IHidOutputReport<D>::Id() const
    {
        uint16_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidOutputReport)->get_Id(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Storage::Streams::IBuffer) consume_Windows_Devices_HumanInterfaceDevice_IHidOutputReport<D>::Data() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidOutputReport)->get_Data(&value));
        return llm::OS::Storage::Streams::IBuffer{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_Devices_HumanInterfaceDevice_IHidOutputReport<D>::Data(llm::OS::Storage::Streams::IBuffer const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidOutputReport)->put_Data(*(void**)(&value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl) consume_Windows_Devices_HumanInterfaceDevice_IHidOutputReport<D>::GetBooleanControl(uint16_t usagePage, uint16_t usageId) const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidOutputReport)->GetBooleanControl(usagePage, usageId, &value));
        return llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl) consume_Windows_Devices_HumanInterfaceDevice_IHidOutputReport<D>::GetBooleanControlByDescription(llm::OS::Devices::HumanInterfaceDevice::HidBooleanControlDescription const& controlDescription) const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidOutputReport)->GetBooleanControlByDescription(*(void**)(&controlDescription), &value));
        return llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidNumericControl) consume_Windows_Devices_HumanInterfaceDevice_IHidOutputReport<D>::GetNumericControl(uint16_t usagePage, uint16_t usageId) const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidOutputReport)->GetNumericControl(usagePage, usageId, &value));
        return llm::OS::Devices::HumanInterfaceDevice::HidNumericControl{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Devices::HumanInterfaceDevice::HidNumericControl) consume_Windows_Devices_HumanInterfaceDevice_IHidOutputReport<D>::GetNumericControlByDescription(llm::OS::Devices::HumanInterfaceDevice::HidNumericControlDescription const& controlDescription) const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Devices::HumanInterfaceDevice::IHidOutputReport)->GetNumericControlByDescription(*(void**)(&controlDescription), &value));
        return llm::OS::Devices::HumanInterfaceDevice::HidNumericControl{ value, take_ownership_from_abi };
    }
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControl> : produce_base<D, llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControl>
    {
        int32_t __stdcall get_Id(uint32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint32_t>(this->shim().Id());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_UsagePage(uint16_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint16_t>(this->shim().UsagePage());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_UsageId(uint16_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint16_t>(this->shim().UsageId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_IsActive(bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().IsActive());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_IsActive(bool value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().IsActive(value);
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_ControlDescription(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidBooleanControlDescription>(this->shim().ControlDescription());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControlDescription> : produce_base<D, llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControlDescription>
    {
        int32_t __stdcall get_Id(uint32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint32_t>(this->shim().Id());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_ReportId(uint16_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint16_t>(this->shim().ReportId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_ReportType(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidReportType>(this->shim().ReportType());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_UsagePage(uint16_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint16_t>(this->shim().UsagePage());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_UsageId(uint16_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint16_t>(this->shim().UsageId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_ParentCollections(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::Collections::IVectorView<llm::OS::Devices::HumanInterfaceDevice::HidCollection>>(this->shim().ParentCollections());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControlDescription2> : produce_base<D, llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControlDescription2>
    {
        int32_t __stdcall get_IsAbsolute(bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().IsAbsolute());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Devices::HumanInterfaceDevice::IHidCollection> : produce_base<D, llm::OS::Devices::HumanInterfaceDevice::IHidCollection>
    {
        int32_t __stdcall get_Id(uint32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint32_t>(this->shim().Id());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Type(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidCollectionType>(this->shim().Type());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_UsagePage(uint32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint32_t>(this->shim().UsagePage());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_UsageId(uint32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint32_t>(this->shim().UsageId());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Devices::HumanInterfaceDevice::IHidDevice> : produce_base<D, llm::OS::Devices::HumanInterfaceDevice::IHidDevice>
    {
        int32_t __stdcall get_VendorId(uint16_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint16_t>(this->shim().VendorId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_ProductId(uint16_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint16_t>(this->shim().ProductId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Version(uint16_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint16_t>(this->shim().Version());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_UsagePage(uint16_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint16_t>(this->shim().UsagePage());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_UsageId(uint16_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint16_t>(this->shim().UsageId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetInputReportAsync(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::IAsyncOperation<llm::OS::Devices::HumanInterfaceDevice::HidInputReport>>(this->shim().GetInputReportAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetInputReportByIdAsync(uint16_t reportId, void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::IAsyncOperation<llm::OS::Devices::HumanInterfaceDevice::HidInputReport>>(this->shim().GetInputReportAsync(reportId));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetFeatureReportAsync(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::IAsyncOperation<llm::OS::Devices::HumanInterfaceDevice::HidFeatureReport>>(this->shim().GetFeatureReportAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetFeatureReportByIdAsync(uint16_t reportId, void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::IAsyncOperation<llm::OS::Devices::HumanInterfaceDevice::HidFeatureReport>>(this->shim().GetFeatureReportAsync(reportId));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall CreateOutputReport(void** outputReport) noexcept final try
        {
            clear_abi(outputReport);
            typename D::abi_guard guard(this->shim());
            *outputReport = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidOutputReport>(this->shim().CreateOutputReport());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall CreateOutputReportById(uint16_t reportId, void** outputReport) noexcept final try
        {
            clear_abi(outputReport);
            typename D::abi_guard guard(this->shim());
            *outputReport = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidOutputReport>(this->shim().CreateOutputReport(reportId));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall CreateFeatureReport(void** featureReport) noexcept final try
        {
            clear_abi(featureReport);
            typename D::abi_guard guard(this->shim());
            *featureReport = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidFeatureReport>(this->shim().CreateFeatureReport());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall CreateFeatureReportById(uint16_t reportId, void** featureReport) noexcept final try
        {
            clear_abi(featureReport);
            typename D::abi_guard guard(this->shim());
            *featureReport = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidFeatureReport>(this->shim().CreateFeatureReport(reportId));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall SendOutputReportAsync(void* outputReport, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<llm::OS::Foundation::IAsyncOperation<uint32_t>>(this->shim().SendOutputReportAsync(*reinterpret_cast<llm::OS::Devices::HumanInterfaceDevice::HidOutputReport const*>(&outputReport)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall SendFeatureReportAsync(void* featureReport, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<llm::OS::Foundation::IAsyncOperation<uint32_t>>(this->shim().SendFeatureReportAsync(*reinterpret_cast<llm::OS::Devices::HumanInterfaceDevice::HidFeatureReport const*>(&featureReport)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetBooleanControlDescriptions(int32_t reportType, uint16_t usagePage, uint16_t usageId, void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::Collections::IVectorView<llm::OS::Devices::HumanInterfaceDevice::HidBooleanControlDescription>>(this->shim().GetBooleanControlDescriptions(*reinterpret_cast<llm::OS::Devices::HumanInterfaceDevice::HidReportType const*>(&reportType), usagePage, usageId));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetNumericControlDescriptions(int32_t reportType, uint16_t usagePage, uint16_t usageId, void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::Collections::IVectorView<llm::OS::Devices::HumanInterfaceDevice::HidNumericControlDescription>>(this->shim().GetNumericControlDescriptions(*reinterpret_cast<llm::OS::Devices::HumanInterfaceDevice::HidReportType const*>(&reportType), usagePage, usageId));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall add_InputReportReceived(void* reportHandler, llm::event_token* token) noexcept final try
        {
            zero_abi<llm::event_token>(token);
            typename D::abi_guard guard(this->shim());
            *token = detach_from<llm::event_token>(this->shim().InputReportReceived(*reinterpret_cast<llm::OS::Foundation::TypedEventHandler<llm::OS::Devices::HumanInterfaceDevice::HidDevice, llm::OS::Devices::HumanInterfaceDevice::HidInputReportReceivedEventArgs> const*>(&reportHandler)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall remove_InputReportReceived(llm::event_token token) noexcept final
        {
            typename D::abi_guard guard(this->shim());
            this->shim().InputReportReceived(*reinterpret_cast<llm::event_token const*>(&token));
            return 0;
        }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Devices::HumanInterfaceDevice::IHidDeviceStatics> : produce_base<D, llm::OS::Devices::HumanInterfaceDevice::IHidDeviceStatics>
    {
        int32_t __stdcall GetDeviceSelector(uint16_t usagePage, uint16_t usageId, void** selector) noexcept final try
        {
            clear_abi(selector);
            typename D::abi_guard guard(this->shim());
            *selector = detach_from<hstring>(this->shim().GetDeviceSelector(usagePage, usageId));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetDeviceSelectorVidPid(uint16_t usagePage, uint16_t usageId, uint16_t vendorId, uint16_t productId, void** selector) noexcept final try
        {
            clear_abi(selector);
            typename D::abi_guard guard(this->shim());
            *selector = detach_from<hstring>(this->shim().GetDeviceSelector(usagePage, usageId, vendorId, productId));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall FromIdAsync(void* deviceId, int32_t accessMode, void** hidDevice) noexcept final try
        {
            clear_abi(hidDevice);
            typename D::abi_guard guard(this->shim());
            *hidDevice = detach_from<llm::OS::Foundation::IAsyncOperation<llm::OS::Devices::HumanInterfaceDevice::HidDevice>>(this->shim().FromIdAsync(*reinterpret_cast<hstring const*>(&deviceId), *reinterpret_cast<llm::OS::Storage::FileAccessMode const*>(&accessMode)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Devices::HumanInterfaceDevice::IHidFeatureReport> : produce_base<D, llm::OS::Devices::HumanInterfaceDevice::IHidFeatureReport>
    {
        int32_t __stdcall get_Id(uint16_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint16_t>(this->shim().Id());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Data(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Storage::Streams::IBuffer>(this->shim().Data());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Data(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Data(*reinterpret_cast<llm::OS::Storage::Streams::IBuffer const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetBooleanControl(uint16_t usagePage, uint16_t usageId, void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl>(this->shim().GetBooleanControl(usagePage, usageId));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetBooleanControlByDescription(void* controlDescription, void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl>(this->shim().GetBooleanControlByDescription(*reinterpret_cast<llm::OS::Devices::HumanInterfaceDevice::HidBooleanControlDescription const*>(&controlDescription)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetNumericControl(uint16_t usagePage, uint16_t usageId, void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidNumericControl>(this->shim().GetNumericControl(usagePage, usageId));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetNumericControlByDescription(void* controlDescription, void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidNumericControl>(this->shim().GetNumericControlByDescription(*reinterpret_cast<llm::OS::Devices::HumanInterfaceDevice::HidNumericControlDescription const*>(&controlDescription)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Devices::HumanInterfaceDevice::IHidInputReport> : produce_base<D, llm::OS::Devices::HumanInterfaceDevice::IHidInputReport>
    {
        int32_t __stdcall get_Id(uint16_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint16_t>(this->shim().Id());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Data(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Storage::Streams::IBuffer>(this->shim().Data());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_ActivatedBooleanControls(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::Collections::IVectorView<llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl>>(this->shim().ActivatedBooleanControls());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_TransitionedBooleanControls(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::Collections::IVectorView<llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl>>(this->shim().TransitionedBooleanControls());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetBooleanControl(uint16_t usagePage, uint16_t usageId, void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl>(this->shim().GetBooleanControl(usagePage, usageId));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetBooleanControlByDescription(void* controlDescription, void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl>(this->shim().GetBooleanControlByDescription(*reinterpret_cast<llm::OS::Devices::HumanInterfaceDevice::HidBooleanControlDescription const*>(&controlDescription)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetNumericControl(uint16_t usagePage, uint16_t usageId, void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidNumericControl>(this->shim().GetNumericControl(usagePage, usageId));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetNumericControlByDescription(void* controlDescription, void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidNumericControl>(this->shim().GetNumericControlByDescription(*reinterpret_cast<llm::OS::Devices::HumanInterfaceDevice::HidNumericControlDescription const*>(&controlDescription)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Devices::HumanInterfaceDevice::IHidInputReportReceivedEventArgs> : produce_base<D, llm::OS::Devices::HumanInterfaceDevice::IHidInputReportReceivedEventArgs>
    {
        int32_t __stdcall get_Report(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidInputReport>(this->shim().Report());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Devices::HumanInterfaceDevice::IHidNumericControl> : produce_base<D, llm::OS::Devices::HumanInterfaceDevice::IHidNumericControl>
    {
        int32_t __stdcall get_Id(uint32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint32_t>(this->shim().Id());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_IsGrouped(bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().IsGrouped());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_UsagePage(uint16_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint16_t>(this->shim().UsagePage());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_UsageId(uint16_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint16_t>(this->shim().UsageId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Value(int64_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<int64_t>(this->shim().Value());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Value(int64_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Value(value);
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_ScaledValue(int64_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<int64_t>(this->shim().ScaledValue());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_ScaledValue(int64_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().ScaledValue(value);
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_ControlDescription(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidNumericControlDescription>(this->shim().ControlDescription());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Devices::HumanInterfaceDevice::IHidNumericControlDescription> : produce_base<D, llm::OS::Devices::HumanInterfaceDevice::IHidNumericControlDescription>
    {
        int32_t __stdcall get_Id(uint32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint32_t>(this->shim().Id());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_ReportId(uint16_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint16_t>(this->shim().ReportId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_ReportType(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidReportType>(this->shim().ReportType());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_ReportSize(uint32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint32_t>(this->shim().ReportSize());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_ReportCount(uint32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint32_t>(this->shim().ReportCount());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_UsagePage(uint16_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint16_t>(this->shim().UsagePage());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_UsageId(uint16_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint16_t>(this->shim().UsageId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_LogicalMinimum(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<int32_t>(this->shim().LogicalMinimum());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_LogicalMaximum(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<int32_t>(this->shim().LogicalMaximum());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_PhysicalMinimum(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<int32_t>(this->shim().PhysicalMinimum());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_PhysicalMaximum(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<int32_t>(this->shim().PhysicalMaximum());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_UnitExponent(uint32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint32_t>(this->shim().UnitExponent());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Unit(uint32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint32_t>(this->shim().Unit());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_IsAbsolute(bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().IsAbsolute());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_HasNull(bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().HasNull());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_ParentCollections(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::Collections::IVectorView<llm::OS::Devices::HumanInterfaceDevice::HidCollection>>(this->shim().ParentCollections());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Devices::HumanInterfaceDevice::IHidOutputReport> : produce_base<D, llm::OS::Devices::HumanInterfaceDevice::IHidOutputReport>
    {
        int32_t __stdcall get_Id(uint16_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint16_t>(this->shim().Id());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Data(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Storage::Streams::IBuffer>(this->shim().Data());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Data(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Data(*reinterpret_cast<llm::OS::Storage::Streams::IBuffer const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetBooleanControl(uint16_t usagePage, uint16_t usageId, void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl>(this->shim().GetBooleanControl(usagePage, usageId));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetBooleanControlByDescription(void* controlDescription, void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl>(this->shim().GetBooleanControlByDescription(*reinterpret_cast<llm::OS::Devices::HumanInterfaceDevice::HidBooleanControlDescription const*>(&controlDescription)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetNumericControl(uint16_t usagePage, uint16_t usageId, void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidNumericControl>(this->shim().GetNumericControl(usagePage, usageId));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetNumericControlByDescription(void* controlDescription, void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Devices::HumanInterfaceDevice::HidNumericControl>(this->shim().GetNumericControlByDescription(*reinterpret_cast<llm::OS::Devices::HumanInterfaceDevice::HidNumericControlDescription const*>(&controlDescription)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
}
LLM_EXPORT namespace llm::OS::Devices::HumanInterfaceDevice
{
    inline auto HidDevice::GetDeviceSelector(uint16_t usagePage, uint16_t usageId)
    {
        return impl::call_factory<HidDevice, IHidDeviceStatics>([&](IHidDeviceStatics const& f) { return f.GetDeviceSelector(usagePage, usageId); });
    }
    inline auto HidDevice::GetDeviceSelector(uint16_t usagePage, uint16_t usageId, uint16_t vendorId, uint16_t productId)
    {
        return impl::call_factory<HidDevice, IHidDeviceStatics>([&](IHidDeviceStatics const& f) { return f.GetDeviceSelector(usagePage, usageId, vendorId, productId); });
    }
    inline auto HidDevice::FromIdAsync(param::hstring const& deviceId, llm::OS::Storage::FileAccessMode const& accessMode)
    {
        return impl::call_factory<HidDevice, IHidDeviceStatics>([&](IHidDeviceStatics const& f) { return f.FromIdAsync(deviceId, accessMode); });
    }
}
namespace std
{
#ifndef LLM_LEAN_AND_MEAN
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControl> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControlDescription> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::IHidBooleanControlDescription2> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::IHidCollection> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::IHidDevice> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::IHidDeviceStatics> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::IHidFeatureReport> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::IHidInputReport> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::IHidInputReportReceivedEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::IHidNumericControl> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::IHidNumericControlDescription> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::IHidOutputReport> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::HidBooleanControl> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::HidBooleanControlDescription> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::HidCollection> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::HidDevice> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::HidFeatureReport> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::HidInputReport> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::HidInputReportReceivedEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::HidNumericControl> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::HidNumericControlDescription> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Devices::HumanInterfaceDevice::HidOutputReport> : llm::impl::hash_base {};
#endif
#ifdef __cpp_lib_format
#endif
}
#endif
