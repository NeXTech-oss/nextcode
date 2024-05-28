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
#ifndef LLM_OS_ApplicationModel_Appointments_DataProvider_H
#define LLM_OS_ApplicationModel_Appointments_DataProvider_H
#include "llm/base.h"
static_assert(llm::check_version(CPPLLM_VERSION, "2.0.220110.5"), "Mismatched C++/WinRT headers.");
#define CPPLLM_VERSION "2.0.220110.5"
#include "llm/Windows.ApplicationModel.Appointments.h"
#include "llm/impl/Windows.ApplicationModel.Appointments.2.h"
#include "llm/impl/Windows.Foundation.2.h"
#include "llm/impl/Windows.Foundation.Collections.2.h"
#include "llm/impl/Windows.ApplicationModel.Appointments.DataProvider.2.h"
namespace llm::impl
{
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarCancelMeetingRequest<D>::AppointmentCalendarLocalId() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCancelMeetingRequest)->get_AppointmentCalendarLocalId(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarCancelMeetingRequest<D>::AppointmentLocalId() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCancelMeetingRequest)->get_AppointmentLocalId(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarCancelMeetingRequest<D>::AppointmentOriginalStartTime() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCancelMeetingRequest)->get_AppointmentOriginalStartTime(&value));
        return llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarCancelMeetingRequest<D>::Subject() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCancelMeetingRequest)->get_Subject(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarCancelMeetingRequest<D>::Comment() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCancelMeetingRequest)->get_Comment(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarCancelMeetingRequest<D>::NotifyInvitees() const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCancelMeetingRequest)->get_NotifyInvitees(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarCancelMeetingRequest<D>::ReportCompletedAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCancelMeetingRequest)->ReportCompletedAsync(&result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarCancelMeetingRequest<D>::ReportFailedAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCancelMeetingRequest)->ReportFailedAsync(&result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarCancelMeetingRequest) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarCancelMeetingRequestEventArgs<D>::Request() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCancelMeetingRequestEventArgs)->get_Request(&value));
        return llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarCancelMeetingRequest{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Deferral) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarCancelMeetingRequestEventArgs<D>::GetDeferral() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCancelMeetingRequestEventArgs)->GetDeferral(&value));
        return llm::OS::Foundation::Deferral{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarCreateOrUpdateAppointmentRequest<D>::AppointmentCalendarLocalId() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCreateOrUpdateAppointmentRequest)->get_AppointmentCalendarLocalId(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::Appointments::Appointment) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarCreateOrUpdateAppointmentRequest<D>::Appointment() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCreateOrUpdateAppointmentRequest)->get_Appointment(&value));
        return llm::OS::ApplicationModel::Appointments::Appointment{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarCreateOrUpdateAppointmentRequest<D>::NotifyInvitees() const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCreateOrUpdateAppointmentRequest)->get_NotifyInvitees(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Collections::IVectorView<hstring>) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarCreateOrUpdateAppointmentRequest<D>::ChangedProperties() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCreateOrUpdateAppointmentRequest)->get_ChangedProperties(&value));
        return llm::OS::Foundation::Collections::IVectorView<hstring>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarCreateOrUpdateAppointmentRequest<D>::ReportCompletedAsync(llm::OS::ApplicationModel::Appointments::Appointment const& createdOrUpdatedAppointment) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCreateOrUpdateAppointmentRequest)->ReportCompletedAsync(*(void**)(&createdOrUpdatedAppointment), &result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarCreateOrUpdateAppointmentRequest<D>::ReportFailedAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCreateOrUpdateAppointmentRequest)->ReportFailedAsync(&result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarCreateOrUpdateAppointmentRequest) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarCreateOrUpdateAppointmentRequestEventArgs<D>::Request() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCreateOrUpdateAppointmentRequestEventArgs)->get_Request(&value));
        return llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarCreateOrUpdateAppointmentRequest{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Deferral) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarCreateOrUpdateAppointmentRequestEventArgs<D>::GetDeferral() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCreateOrUpdateAppointmentRequestEventArgs)->GetDeferral(&value));
        return llm::OS::Foundation::Deferral{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarForwardMeetingRequest<D>::AppointmentCalendarLocalId() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarForwardMeetingRequest)->get_AppointmentCalendarLocalId(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarForwardMeetingRequest<D>::AppointmentLocalId() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarForwardMeetingRequest)->get_AppointmentLocalId(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarForwardMeetingRequest<D>::AppointmentOriginalStartTime() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarForwardMeetingRequest)->get_AppointmentOriginalStartTime(&value));
        return llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Collections::IVectorView<llm::OS::ApplicationModel::Appointments::AppointmentInvitee>) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarForwardMeetingRequest<D>::Invitees() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarForwardMeetingRequest)->get_Invitees(&value));
        return llm::OS::Foundation::Collections::IVectorView<llm::OS::ApplicationModel::Appointments::AppointmentInvitee>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarForwardMeetingRequest<D>::Subject() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarForwardMeetingRequest)->get_Subject(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarForwardMeetingRequest<D>::ForwardHeader() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarForwardMeetingRequest)->get_ForwardHeader(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarForwardMeetingRequest<D>::Comment() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarForwardMeetingRequest)->get_Comment(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarForwardMeetingRequest<D>::ReportCompletedAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarForwardMeetingRequest)->ReportCompletedAsync(&result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarForwardMeetingRequest<D>::ReportFailedAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarForwardMeetingRequest)->ReportFailedAsync(&result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarForwardMeetingRequest) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarForwardMeetingRequestEventArgs<D>::Request() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarForwardMeetingRequestEventArgs)->get_Request(&value));
        return llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarForwardMeetingRequest{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Deferral) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarForwardMeetingRequestEventArgs<D>::GetDeferral() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarForwardMeetingRequestEventArgs)->GetDeferral(&value));
        return llm::OS::Foundation::Deferral{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarProposeNewTimeForMeetingRequest<D>::AppointmentCalendarLocalId() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarProposeNewTimeForMeetingRequest)->get_AppointmentCalendarLocalId(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarProposeNewTimeForMeetingRequest<D>::AppointmentLocalId() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarProposeNewTimeForMeetingRequest)->get_AppointmentLocalId(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarProposeNewTimeForMeetingRequest<D>::AppointmentOriginalStartTime() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarProposeNewTimeForMeetingRequest)->get_AppointmentOriginalStartTime(&value));
        return llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::DateTime) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarProposeNewTimeForMeetingRequest<D>::NewStartTime() const
    {
        llm::OS::Foundation::DateTime value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarProposeNewTimeForMeetingRequest)->get_NewStartTime(put_abi(value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::TimeSpan) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarProposeNewTimeForMeetingRequest<D>::NewDuration() const
    {
        llm::OS::Foundation::TimeSpan value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarProposeNewTimeForMeetingRequest)->get_NewDuration(put_abi(value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarProposeNewTimeForMeetingRequest<D>::Subject() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarProposeNewTimeForMeetingRequest)->get_Subject(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarProposeNewTimeForMeetingRequest<D>::Comment() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarProposeNewTimeForMeetingRequest)->get_Comment(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarProposeNewTimeForMeetingRequest<D>::ReportCompletedAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarProposeNewTimeForMeetingRequest)->ReportCompletedAsync(&result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarProposeNewTimeForMeetingRequest<D>::ReportFailedAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarProposeNewTimeForMeetingRequest)->ReportFailedAsync(&result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarProposeNewTimeForMeetingRequest) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarProposeNewTimeForMeetingRequestEventArgs<D>::Request() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarProposeNewTimeForMeetingRequestEventArgs)->get_Request(&value));
        return llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarProposeNewTimeForMeetingRequest{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Deferral) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarProposeNewTimeForMeetingRequestEventArgs<D>::GetDeferral() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarProposeNewTimeForMeetingRequestEventArgs)->GetDeferral(&value));
        return llm::OS::Foundation::Deferral{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarSyncManagerSyncRequest<D>::AppointmentCalendarLocalId() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarSyncManagerSyncRequest)->get_AppointmentCalendarLocalId(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarSyncManagerSyncRequest<D>::ReportCompletedAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarSyncManagerSyncRequest)->ReportCompletedAsync(&result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarSyncManagerSyncRequest<D>::ReportFailedAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarSyncManagerSyncRequest)->ReportFailedAsync(&result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarSyncManagerSyncRequest) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarSyncManagerSyncRequestEventArgs<D>::Request() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarSyncManagerSyncRequestEventArgs)->get_Request(&value));
        return llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarSyncManagerSyncRequest{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Deferral) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarSyncManagerSyncRequestEventArgs<D>::GetDeferral() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarSyncManagerSyncRequestEventArgs)->GetDeferral(&value));
        return llm::OS::Foundation::Deferral{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarUpdateMeetingResponseRequest<D>::AppointmentCalendarLocalId() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarUpdateMeetingResponseRequest)->get_AppointmentCalendarLocalId(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarUpdateMeetingResponseRequest<D>::AppointmentLocalId() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarUpdateMeetingResponseRequest)->get_AppointmentLocalId(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarUpdateMeetingResponseRequest<D>::AppointmentOriginalStartTime() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarUpdateMeetingResponseRequest)->get_AppointmentOriginalStartTime(&value));
        return llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::Appointments::AppointmentParticipantResponse) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarUpdateMeetingResponseRequest<D>::Response() const
    {
        llm::OS::ApplicationModel::Appointments::AppointmentParticipantResponse response{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarUpdateMeetingResponseRequest)->get_Response(reinterpret_cast<int32_t*>(&response)));
        return response;
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarUpdateMeetingResponseRequest<D>::Subject() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarUpdateMeetingResponseRequest)->get_Subject(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarUpdateMeetingResponseRequest<D>::Comment() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarUpdateMeetingResponseRequest)->get_Comment(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarUpdateMeetingResponseRequest<D>::SendUpdate() const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarUpdateMeetingResponseRequest)->get_SendUpdate(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarUpdateMeetingResponseRequest<D>::ReportCompletedAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarUpdateMeetingResponseRequest)->ReportCompletedAsync(&result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarUpdateMeetingResponseRequest<D>::ReportFailedAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarUpdateMeetingResponseRequest)->ReportFailedAsync(&result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarUpdateMeetingResponseRequest) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarUpdateMeetingResponseRequestEventArgs<D>::Request() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarUpdateMeetingResponseRequestEventArgs)->get_Request(&value));
        return llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarUpdateMeetingResponseRequest{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Deferral) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentCalendarUpdateMeetingResponseRequestEventArgs<D>::GetDeferral() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarUpdateMeetingResponseRequestEventArgs)->GetDeferral(&value));
        return llm::OS::Foundation::Deferral{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::event_token) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::SyncRequested(llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection, llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarSyncManagerSyncRequestEventArgs> const& handler) const
    {
        llm::event_token token{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentDataProviderConnection)->add_SyncRequested(*(void**)(&handler), put_abi(token)));
        return token;
    }
    template <typename D> typename consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::SyncRequested_revoker consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::SyncRequested(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection, llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarSyncManagerSyncRequestEventArgs> const& handler) const
    {
        return impl::make_event_revoker<D, SyncRequested_revoker>(this, SyncRequested(handler));
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::SyncRequested(llm::event_token const& token) const noexcept
    {
        LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentDataProviderConnection)->remove_SyncRequested(impl::bind_in(token));
    }
    template <typename D> LLM_IMPL_AUTO(llm::event_token) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::CreateOrUpdateAppointmentRequested(llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection, llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarCreateOrUpdateAppointmentRequestEventArgs> const& handler) const
    {
        llm::event_token token{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentDataProviderConnection)->add_CreateOrUpdateAppointmentRequested(*(void**)(&handler), put_abi(token)));
        return token;
    }
    template <typename D> typename consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::CreateOrUpdateAppointmentRequested_revoker consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::CreateOrUpdateAppointmentRequested(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection, llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarCreateOrUpdateAppointmentRequestEventArgs> const& handler) const
    {
        return impl::make_event_revoker<D, CreateOrUpdateAppointmentRequested_revoker>(this, CreateOrUpdateAppointmentRequested(handler));
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::CreateOrUpdateAppointmentRequested(llm::event_token const& token) const noexcept
    {
        LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentDataProviderConnection)->remove_CreateOrUpdateAppointmentRequested(impl::bind_in(token));
    }
    template <typename D> LLM_IMPL_AUTO(llm::event_token) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::CancelMeetingRequested(llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection, llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarCancelMeetingRequestEventArgs> const& handler) const
    {
        llm::event_token token{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentDataProviderConnection)->add_CancelMeetingRequested(*(void**)(&handler), put_abi(token)));
        return token;
    }
    template <typename D> typename consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::CancelMeetingRequested_revoker consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::CancelMeetingRequested(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection, llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarCancelMeetingRequestEventArgs> const& handler) const
    {
        return impl::make_event_revoker<D, CancelMeetingRequested_revoker>(this, CancelMeetingRequested(handler));
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::CancelMeetingRequested(llm::event_token const& token) const noexcept
    {
        LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentDataProviderConnection)->remove_CancelMeetingRequested(impl::bind_in(token));
    }
    template <typename D> LLM_IMPL_AUTO(llm::event_token) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::ForwardMeetingRequested(llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection, llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarForwardMeetingRequestEventArgs> const& handler) const
    {
        llm::event_token token{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentDataProviderConnection)->add_ForwardMeetingRequested(*(void**)(&handler), put_abi(token)));
        return token;
    }
    template <typename D> typename consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::ForwardMeetingRequested_revoker consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::ForwardMeetingRequested(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection, llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarForwardMeetingRequestEventArgs> const& handler) const
    {
        return impl::make_event_revoker<D, ForwardMeetingRequested_revoker>(this, ForwardMeetingRequested(handler));
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::ForwardMeetingRequested(llm::event_token const& token) const noexcept
    {
        LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentDataProviderConnection)->remove_ForwardMeetingRequested(impl::bind_in(token));
    }
    template <typename D> LLM_IMPL_AUTO(llm::event_token) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::ProposeNewTimeForMeetingRequested(llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection, llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarProposeNewTimeForMeetingRequestEventArgs> const& handler) const
    {
        llm::event_token token{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentDataProviderConnection)->add_ProposeNewTimeForMeetingRequested(*(void**)(&handler), put_abi(token)));
        return token;
    }
    template <typename D> typename consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::ProposeNewTimeForMeetingRequested_revoker consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::ProposeNewTimeForMeetingRequested(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection, llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarProposeNewTimeForMeetingRequestEventArgs> const& handler) const
    {
        return impl::make_event_revoker<D, ProposeNewTimeForMeetingRequested_revoker>(this, ProposeNewTimeForMeetingRequested(handler));
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::ProposeNewTimeForMeetingRequested(llm::event_token const& token) const noexcept
    {
        LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentDataProviderConnection)->remove_ProposeNewTimeForMeetingRequested(impl::bind_in(token));
    }
    template <typename D> LLM_IMPL_AUTO(llm::event_token) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::UpdateMeetingResponseRequested(llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection, llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarUpdateMeetingResponseRequestEventArgs> const& handler) const
    {
        llm::event_token token{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentDataProviderConnection)->add_UpdateMeetingResponseRequested(*(void**)(&handler), put_abi(token)));
        return token;
    }
    template <typename D> typename consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::UpdateMeetingResponseRequested_revoker consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::UpdateMeetingResponseRequested(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection, llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarUpdateMeetingResponseRequestEventArgs> const& handler) const
    {
        return impl::make_event_revoker<D, UpdateMeetingResponseRequested_revoker>(this, UpdateMeetingResponseRequested(handler));
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::UpdateMeetingResponseRequested(llm::event_token const& token) const noexcept
    {
        LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentDataProviderConnection)->remove_UpdateMeetingResponseRequested(impl::bind_in(token));
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderConnection<D>::Start() const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentDataProviderConnection)->Start());
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection) consume_Windows_ApplicationModel_Appointments_DataProvider_IAppointmentDataProviderTriggerDetails<D>::Connection() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentDataProviderTriggerDetails)->get_Connection(&value));
        return llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection{ value, take_ownership_from_abi };
    }
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCancelMeetingRequest> : produce_base<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCancelMeetingRequest>
    {
        int32_t __stdcall get_AppointmentCalendarLocalId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().AppointmentCalendarLocalId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_AppointmentLocalId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().AppointmentLocalId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_AppointmentOriginalStartTime(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>>(this->shim().AppointmentOriginalStartTime());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Subject(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().Subject());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Comment(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().Comment());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_NotifyInvitees(bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().NotifyInvitees());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall ReportCompletedAsync(void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncAction>(this->shim().ReportCompletedAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall ReportFailedAsync(void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncAction>(this->shim().ReportFailedAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCancelMeetingRequestEventArgs> : produce_base<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCancelMeetingRequestEventArgs>
    {
        int32_t __stdcall get_Request(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarCancelMeetingRequest>(this->shim().Request());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetDeferral(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::Deferral>(this->shim().GetDeferral());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCreateOrUpdateAppointmentRequest> : produce_base<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCreateOrUpdateAppointmentRequest>
    {
        int32_t __stdcall get_AppointmentCalendarLocalId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().AppointmentCalendarLocalId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Appointment(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::Appointments::Appointment>(this->shim().Appointment());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_NotifyInvitees(bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().NotifyInvitees());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_ChangedProperties(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::Collections::IVectorView<hstring>>(this->shim().ChangedProperties());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall ReportCompletedAsync(void* createdOrUpdatedAppointment, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncAction>(this->shim().ReportCompletedAsync(*reinterpret_cast<llm::OS::ApplicationModel::Appointments::Appointment const*>(&createdOrUpdatedAppointment)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall ReportFailedAsync(void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncAction>(this->shim().ReportFailedAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCreateOrUpdateAppointmentRequestEventArgs> : produce_base<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCreateOrUpdateAppointmentRequestEventArgs>
    {
        int32_t __stdcall get_Request(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarCreateOrUpdateAppointmentRequest>(this->shim().Request());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetDeferral(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::Deferral>(this->shim().GetDeferral());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarForwardMeetingRequest> : produce_base<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarForwardMeetingRequest>
    {
        int32_t __stdcall get_AppointmentCalendarLocalId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().AppointmentCalendarLocalId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_AppointmentLocalId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().AppointmentLocalId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_AppointmentOriginalStartTime(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>>(this->shim().AppointmentOriginalStartTime());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Invitees(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::Collections::IVectorView<llm::OS::ApplicationModel::Appointments::AppointmentInvitee>>(this->shim().Invitees());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Subject(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().Subject());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_ForwardHeader(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().ForwardHeader());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Comment(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().Comment());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall ReportCompletedAsync(void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncAction>(this->shim().ReportCompletedAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall ReportFailedAsync(void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncAction>(this->shim().ReportFailedAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarForwardMeetingRequestEventArgs> : produce_base<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarForwardMeetingRequestEventArgs>
    {
        int32_t __stdcall get_Request(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarForwardMeetingRequest>(this->shim().Request());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetDeferral(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::Deferral>(this->shim().GetDeferral());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarProposeNewTimeForMeetingRequest> : produce_base<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarProposeNewTimeForMeetingRequest>
    {
        int32_t __stdcall get_AppointmentCalendarLocalId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().AppointmentCalendarLocalId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_AppointmentLocalId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().AppointmentLocalId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_AppointmentOriginalStartTime(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>>(this->shim().AppointmentOriginalStartTime());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_NewStartTime(int64_t* value) noexcept final try
        {
            zero_abi<llm::OS::Foundation::DateTime>(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::DateTime>(this->shim().NewStartTime());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_NewDuration(int64_t* value) noexcept final try
        {
            zero_abi<llm::OS::Foundation::TimeSpan>(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::TimeSpan>(this->shim().NewDuration());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Subject(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().Subject());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Comment(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().Comment());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall ReportCompletedAsync(void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncAction>(this->shim().ReportCompletedAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall ReportFailedAsync(void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncAction>(this->shim().ReportFailedAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarProposeNewTimeForMeetingRequestEventArgs> : produce_base<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarProposeNewTimeForMeetingRequestEventArgs>
    {
        int32_t __stdcall get_Request(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarProposeNewTimeForMeetingRequest>(this->shim().Request());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetDeferral(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::Deferral>(this->shim().GetDeferral());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarSyncManagerSyncRequest> : produce_base<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarSyncManagerSyncRequest>
    {
        int32_t __stdcall get_AppointmentCalendarLocalId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().AppointmentCalendarLocalId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall ReportCompletedAsync(void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncAction>(this->shim().ReportCompletedAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall ReportFailedAsync(void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncAction>(this->shim().ReportFailedAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarSyncManagerSyncRequestEventArgs> : produce_base<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarSyncManagerSyncRequestEventArgs>
    {
        int32_t __stdcall get_Request(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarSyncManagerSyncRequest>(this->shim().Request());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetDeferral(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::Deferral>(this->shim().GetDeferral());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarUpdateMeetingResponseRequest> : produce_base<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarUpdateMeetingResponseRequest>
    {
        int32_t __stdcall get_AppointmentCalendarLocalId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().AppointmentCalendarLocalId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_AppointmentLocalId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().AppointmentLocalId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_AppointmentOriginalStartTime(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>>(this->shim().AppointmentOriginalStartTime());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Response(int32_t* response) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *response = detach_from<llm::OS::ApplicationModel::Appointments::AppointmentParticipantResponse>(this->shim().Response());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Subject(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().Subject());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Comment(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().Comment());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_SendUpdate(bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().SendUpdate());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall ReportCompletedAsync(void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncAction>(this->shim().ReportCompletedAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall ReportFailedAsync(void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncAction>(this->shim().ReportFailedAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarUpdateMeetingResponseRequestEventArgs> : produce_base<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarUpdateMeetingResponseRequestEventArgs>
    {
        int32_t __stdcall get_Request(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarUpdateMeetingResponseRequest>(this->shim().Request());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetDeferral(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::Deferral>(this->shim().GetDeferral());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentDataProviderConnection> : produce_base<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentDataProviderConnection>
    {
        int32_t __stdcall add_SyncRequested(void* handler, llm::event_token* token) noexcept final try
        {
            zero_abi<llm::event_token>(token);
            typename D::abi_guard guard(this->shim());
            *token = detach_from<llm::event_token>(this->shim().SyncRequested(*reinterpret_cast<llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection, llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarSyncManagerSyncRequestEventArgs> const*>(&handler)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall remove_SyncRequested(llm::event_token token) noexcept final
        {
            typename D::abi_guard guard(this->shim());
            this->shim().SyncRequested(*reinterpret_cast<llm::event_token const*>(&token));
            return 0;
        }
        int32_t __stdcall add_CreateOrUpdateAppointmentRequested(void* handler, llm::event_token* token) noexcept final try
        {
            zero_abi<llm::event_token>(token);
            typename D::abi_guard guard(this->shim());
            *token = detach_from<llm::event_token>(this->shim().CreateOrUpdateAppointmentRequested(*reinterpret_cast<llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection, llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarCreateOrUpdateAppointmentRequestEventArgs> const*>(&handler)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall remove_CreateOrUpdateAppointmentRequested(llm::event_token token) noexcept final
        {
            typename D::abi_guard guard(this->shim());
            this->shim().CreateOrUpdateAppointmentRequested(*reinterpret_cast<llm::event_token const*>(&token));
            return 0;
        }
        int32_t __stdcall add_CancelMeetingRequested(void* handler, llm::event_token* token) noexcept final try
        {
            zero_abi<llm::event_token>(token);
            typename D::abi_guard guard(this->shim());
            *token = detach_from<llm::event_token>(this->shim().CancelMeetingRequested(*reinterpret_cast<llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection, llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarCancelMeetingRequestEventArgs> const*>(&handler)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall remove_CancelMeetingRequested(llm::event_token token) noexcept final
        {
            typename D::abi_guard guard(this->shim());
            this->shim().CancelMeetingRequested(*reinterpret_cast<llm::event_token const*>(&token));
            return 0;
        }
        int32_t __stdcall add_ForwardMeetingRequested(void* handler, llm::event_token* token) noexcept final try
        {
            zero_abi<llm::event_token>(token);
            typename D::abi_guard guard(this->shim());
            *token = detach_from<llm::event_token>(this->shim().ForwardMeetingRequested(*reinterpret_cast<llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection, llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarForwardMeetingRequestEventArgs> const*>(&handler)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall remove_ForwardMeetingRequested(llm::event_token token) noexcept final
        {
            typename D::abi_guard guard(this->shim());
            this->shim().ForwardMeetingRequested(*reinterpret_cast<llm::event_token const*>(&token));
            return 0;
        }
        int32_t __stdcall add_ProposeNewTimeForMeetingRequested(void* handler, llm::event_token* token) noexcept final try
        {
            zero_abi<llm::event_token>(token);
            typename D::abi_guard guard(this->shim());
            *token = detach_from<llm::event_token>(this->shim().ProposeNewTimeForMeetingRequested(*reinterpret_cast<llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection, llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarProposeNewTimeForMeetingRequestEventArgs> const*>(&handler)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall remove_ProposeNewTimeForMeetingRequested(llm::event_token token) noexcept final
        {
            typename D::abi_guard guard(this->shim());
            this->shim().ProposeNewTimeForMeetingRequested(*reinterpret_cast<llm::event_token const*>(&token));
            return 0;
        }
        int32_t __stdcall add_UpdateMeetingResponseRequested(void* handler, llm::event_token* token) noexcept final try
        {
            zero_abi<llm::event_token>(token);
            typename D::abi_guard guard(this->shim());
            *token = detach_from<llm::event_token>(this->shim().UpdateMeetingResponseRequested(*reinterpret_cast<llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection, llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarUpdateMeetingResponseRequestEventArgs> const*>(&handler)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall remove_UpdateMeetingResponseRequested(llm::event_token token) noexcept final
        {
            typename D::abi_guard guard(this->shim());
            this->shim().UpdateMeetingResponseRequested(*reinterpret_cast<llm::event_token const*>(&token));
            return 0;
        }
        int32_t __stdcall Start() noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Start();
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentDataProviderTriggerDetails> : produce_base<D, llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentDataProviderTriggerDetails>
    {
        int32_t __stdcall get_Connection(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection>(this->shim().Connection());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
}
LLM_EXPORT namespace llm::OS::ApplicationModel::Appointments::DataProvider
{
}
namespace std
{
#ifndef LLM_LEAN_AND_MEAN
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCancelMeetingRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCancelMeetingRequestEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCreateOrUpdateAppointmentRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarCreateOrUpdateAppointmentRequestEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarForwardMeetingRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarForwardMeetingRequestEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarProposeNewTimeForMeetingRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarProposeNewTimeForMeetingRequestEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarSyncManagerSyncRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarSyncManagerSyncRequestEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarUpdateMeetingResponseRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentCalendarUpdateMeetingResponseRequestEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentDataProviderConnection> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::IAppointmentDataProviderTriggerDetails> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarCancelMeetingRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarCancelMeetingRequestEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarCreateOrUpdateAppointmentRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarCreateOrUpdateAppointmentRequestEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarForwardMeetingRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarForwardMeetingRequestEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarProposeNewTimeForMeetingRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarProposeNewTimeForMeetingRequestEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarSyncManagerSyncRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarSyncManagerSyncRequestEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarUpdateMeetingResponseRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentCalendarUpdateMeetingResponseRequestEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderConnection> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::Appointments::DataProvider::AppointmentDataProviderTriggerDetails> : llm::impl::hash_base {};
#endif
#ifdef __cpp_lib_format
#endif
}
#endif
