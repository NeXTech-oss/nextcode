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
#ifndef LLM_OS_ApplicationModel_UserDataTasks_DataProvider_H
#define LLM_OS_ApplicationModel_UserDataTasks_DataProvider_H
#include "llm/base.h"
static_assert(llm::check_version(CPPLLM_VERSION, "2.0.220110.5"), "Mismatched C++/WinRT headers.");
#define CPPLLM_VERSION "2.0.220110.5"
#include "llm/Windows.ApplicationModel.UserDataTasks.h"
#include "llm/impl/Windows.ApplicationModel.UserDataTasks.2.h"
#include "llm/impl/Windows.Foundation.2.h"
#include "llm/impl/Windows.ApplicationModel.UserDataTasks.DataProvider.2.h"
namespace llm::impl
{
    template <typename D> LLM_IMPL_AUTO(llm::event_token) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderConnection<D>::CreateOrUpdateTaskRequested(llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskDataProviderConnection, llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListCreateOrUpdateTaskRequestEventArgs> const& handler) const
    {
        llm::event_token token{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskDataProviderConnection)->add_CreateOrUpdateTaskRequested(*(void**)(&handler), put_abi(token)));
        return token;
    }
    template <typename D> typename consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderConnection<D>::CreateOrUpdateTaskRequested_revoker consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderConnection<D>::CreateOrUpdateTaskRequested(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskDataProviderConnection, llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListCreateOrUpdateTaskRequestEventArgs> const& handler) const
    {
        return impl::make_event_revoker<D, CreateOrUpdateTaskRequested_revoker>(this, CreateOrUpdateTaskRequested(handler));
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderConnection<D>::CreateOrUpdateTaskRequested(llm::event_token const& token) const noexcept
    {
        LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskDataProviderConnection)->remove_CreateOrUpdateTaskRequested(impl::bind_in(token));
    }
    template <typename D> LLM_IMPL_AUTO(llm::event_token) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderConnection<D>::SyncRequested(llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskDataProviderConnection, llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListSyncManagerSyncRequestEventArgs> const& handler) const
    {
        llm::event_token token{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskDataProviderConnection)->add_SyncRequested(*(void**)(&handler), put_abi(token)));
        return token;
    }
    template <typename D> typename consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderConnection<D>::SyncRequested_revoker consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderConnection<D>::SyncRequested(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskDataProviderConnection, llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListSyncManagerSyncRequestEventArgs> const& handler) const
    {
        return impl::make_event_revoker<D, SyncRequested_revoker>(this, SyncRequested(handler));
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderConnection<D>::SyncRequested(llm::event_token const& token) const noexcept
    {
        LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskDataProviderConnection)->remove_SyncRequested(impl::bind_in(token));
    }
    template <typename D> LLM_IMPL_AUTO(llm::event_token) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderConnection<D>::SkipOccurrenceRequested(llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskDataProviderConnection, llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListSkipOccurrenceRequestEventArgs> const& handler) const
    {
        llm::event_token token{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskDataProviderConnection)->add_SkipOccurrenceRequested(*(void**)(&handler), put_abi(token)));
        return token;
    }
    template <typename D> typename consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderConnection<D>::SkipOccurrenceRequested_revoker consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderConnection<D>::SkipOccurrenceRequested(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskDataProviderConnection, llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListSkipOccurrenceRequestEventArgs> const& handler) const
    {
        return impl::make_event_revoker<D, SkipOccurrenceRequested_revoker>(this, SkipOccurrenceRequested(handler));
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderConnection<D>::SkipOccurrenceRequested(llm::event_token const& token) const noexcept
    {
        LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskDataProviderConnection)->remove_SkipOccurrenceRequested(impl::bind_in(token));
    }
    template <typename D> LLM_IMPL_AUTO(llm::event_token) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderConnection<D>::CompleteTaskRequested(llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskDataProviderConnection, llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListCompleteTaskRequestEventArgs> const& handler) const
    {
        llm::event_token token{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskDataProviderConnection)->add_CompleteTaskRequested(*(void**)(&handler), put_abi(token)));
        return token;
    }
    template <typename D> typename consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderConnection<D>::CompleteTaskRequested_revoker consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderConnection<D>::CompleteTaskRequested(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskDataProviderConnection, llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListCompleteTaskRequestEventArgs> const& handler) const
    {
        return impl::make_event_revoker<D, CompleteTaskRequested_revoker>(this, CompleteTaskRequested(handler));
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderConnection<D>::CompleteTaskRequested(llm::event_token const& token) const noexcept
    {
        LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskDataProviderConnection)->remove_CompleteTaskRequested(impl::bind_in(token));
    }
    template <typename D> LLM_IMPL_AUTO(llm::event_token) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderConnection<D>::DeleteTaskRequested(llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskDataProviderConnection, llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListDeleteTaskRequestEventArgs> const& handler) const
    {
        llm::event_token token{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskDataProviderConnection)->add_DeleteTaskRequested(*(void**)(&handler), put_abi(token)));
        return token;
    }
    template <typename D> typename consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderConnection<D>::DeleteTaskRequested_revoker consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderConnection<D>::DeleteTaskRequested(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskDataProviderConnection, llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListDeleteTaskRequestEventArgs> const& handler) const
    {
        return impl::make_event_revoker<D, DeleteTaskRequested_revoker>(this, DeleteTaskRequested(handler));
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderConnection<D>::DeleteTaskRequested(llm::event_token const& token) const noexcept
    {
        LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskDataProviderConnection)->remove_DeleteTaskRequested(impl::bind_in(token));
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderConnection<D>::Start() const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskDataProviderConnection)->Start());
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskDataProviderConnection) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskDataProviderTriggerDetails<D>::Connection() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskDataProviderTriggerDetails)->get_Connection(&value));
        return llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskDataProviderConnection{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListCompleteTaskRequest<D>::TaskListId() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCompleteTaskRequest)->get_TaskListId(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListCompleteTaskRequest<D>::TaskId() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCompleteTaskRequest)->get_TaskId(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListCompleteTaskRequest<D>::ReportCompletedAsync(param::hstring const& completedTaskId) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCompleteTaskRequest)->ReportCompletedAsync(*(void**)(&completedTaskId), &result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListCompleteTaskRequest<D>::ReportFailedAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCompleteTaskRequest)->ReportFailedAsync(&result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListCompleteTaskRequest) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListCompleteTaskRequestEventArgs<D>::Request() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCompleteTaskRequestEventArgs)->get_Request(&value));
        return llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListCompleteTaskRequest{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Deferral) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListCompleteTaskRequestEventArgs<D>::GetDeferral() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCompleteTaskRequestEventArgs)->GetDeferral(&value));
        return llm::OS::Foundation::Deferral{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListCreateOrUpdateTaskRequest<D>::TaskListId() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCreateOrUpdateTaskRequest)->get_TaskListId(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::UserDataTask) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListCreateOrUpdateTaskRequest<D>::Task() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCreateOrUpdateTaskRequest)->get_Task(&value));
        return llm::OS::ApplicationModel::UserDataTasks::UserDataTask{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListCreateOrUpdateTaskRequest<D>::ReportCompletedAsync(llm::OS::ApplicationModel::UserDataTasks::UserDataTask const& createdOrUpdatedUserDataTask) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCreateOrUpdateTaskRequest)->ReportCompletedAsync(*(void**)(&createdOrUpdatedUserDataTask), &result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListCreateOrUpdateTaskRequest<D>::ReportFailedAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCreateOrUpdateTaskRequest)->ReportFailedAsync(&result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListCreateOrUpdateTaskRequest) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListCreateOrUpdateTaskRequestEventArgs<D>::Request() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCreateOrUpdateTaskRequestEventArgs)->get_Request(&value));
        return llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListCreateOrUpdateTaskRequest{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Deferral) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListCreateOrUpdateTaskRequestEventArgs<D>::GetDeferral() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCreateOrUpdateTaskRequestEventArgs)->GetDeferral(&value));
        return llm::OS::Foundation::Deferral{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListDeleteTaskRequest<D>::TaskListId() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListDeleteTaskRequest)->get_TaskListId(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListDeleteTaskRequest<D>::TaskId() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListDeleteTaskRequest)->get_TaskId(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListDeleteTaskRequest<D>::ReportCompletedAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListDeleteTaskRequest)->ReportCompletedAsync(&result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListDeleteTaskRequest<D>::ReportFailedAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListDeleteTaskRequest)->ReportFailedAsync(&result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListDeleteTaskRequest) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListDeleteTaskRequestEventArgs<D>::Request() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListDeleteTaskRequestEventArgs)->get_Request(&value));
        return llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListDeleteTaskRequest{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Deferral) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListDeleteTaskRequestEventArgs<D>::GetDeferral() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListDeleteTaskRequestEventArgs)->GetDeferral(&value));
        return llm::OS::Foundation::Deferral{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListSkipOccurrenceRequest<D>::TaskListId() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSkipOccurrenceRequest)->get_TaskListId(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListSkipOccurrenceRequest<D>::TaskId() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSkipOccurrenceRequest)->get_TaskId(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListSkipOccurrenceRequest<D>::ReportCompletedAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSkipOccurrenceRequest)->ReportCompletedAsync(&result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListSkipOccurrenceRequest<D>::ReportFailedAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSkipOccurrenceRequest)->ReportFailedAsync(&result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListSkipOccurrenceRequest) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListSkipOccurrenceRequestEventArgs<D>::Request() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSkipOccurrenceRequestEventArgs)->get_Request(&value));
        return llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListSkipOccurrenceRequest{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Deferral) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListSkipOccurrenceRequestEventArgs<D>::GetDeferral() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSkipOccurrenceRequestEventArgs)->GetDeferral(&value));
        return llm::OS::Foundation::Deferral{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListSyncManagerSyncRequest<D>::TaskListId() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSyncManagerSyncRequest)->get_TaskListId(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListSyncManagerSyncRequest<D>::ReportCompletedAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSyncManagerSyncRequest)->ReportCompletedAsync(&result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListSyncManagerSyncRequest<D>::ReportFailedAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSyncManagerSyncRequest)->ReportFailedAsync(&result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListSyncManagerSyncRequest) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListSyncManagerSyncRequestEventArgs<D>::Request() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSyncManagerSyncRequestEventArgs)->get_Request(&value));
        return llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListSyncManagerSyncRequest{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Deferral) consume_Windows_ApplicationModel_UserDataTasks_DataProvider_IUserDataTaskListSyncManagerSyncRequestEventArgs<D>::GetDeferral() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSyncManagerSyncRequestEventArgs)->GetDeferral(&value));
        return llm::OS::Foundation::Deferral{ value, take_ownership_from_abi };
    }
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskDataProviderConnection> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskDataProviderConnection>
    {
        int32_t __stdcall add_CreateOrUpdateTaskRequested(void* handler, llm::event_token* token) noexcept final try
        {
            zero_abi<llm::event_token>(token);
            typename D::abi_guard guard(this->shim());
            *token = detach_from<llm::event_token>(this->shim().CreateOrUpdateTaskRequested(*reinterpret_cast<llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskDataProviderConnection, llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListCreateOrUpdateTaskRequestEventArgs> const*>(&handler)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall remove_CreateOrUpdateTaskRequested(llm::event_token token) noexcept final
        {
            typename D::abi_guard guard(this->shim());
            this->shim().CreateOrUpdateTaskRequested(*reinterpret_cast<llm::event_token const*>(&token));
            return 0;
        }
        int32_t __stdcall add_SyncRequested(void* handler, llm::event_token* token) noexcept final try
        {
            zero_abi<llm::event_token>(token);
            typename D::abi_guard guard(this->shim());
            *token = detach_from<llm::event_token>(this->shim().SyncRequested(*reinterpret_cast<llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskDataProviderConnection, llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListSyncManagerSyncRequestEventArgs> const*>(&handler)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall remove_SyncRequested(llm::event_token token) noexcept final
        {
            typename D::abi_guard guard(this->shim());
            this->shim().SyncRequested(*reinterpret_cast<llm::event_token const*>(&token));
            return 0;
        }
        int32_t __stdcall add_SkipOccurrenceRequested(void* handler, llm::event_token* token) noexcept final try
        {
            zero_abi<llm::event_token>(token);
            typename D::abi_guard guard(this->shim());
            *token = detach_from<llm::event_token>(this->shim().SkipOccurrenceRequested(*reinterpret_cast<llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskDataProviderConnection, llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListSkipOccurrenceRequestEventArgs> const*>(&handler)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall remove_SkipOccurrenceRequested(llm::event_token token) noexcept final
        {
            typename D::abi_guard guard(this->shim());
            this->shim().SkipOccurrenceRequested(*reinterpret_cast<llm::event_token const*>(&token));
            return 0;
        }
        int32_t __stdcall add_CompleteTaskRequested(void* handler, llm::event_token* token) noexcept final try
        {
            zero_abi<llm::event_token>(token);
            typename D::abi_guard guard(this->shim());
            *token = detach_from<llm::event_token>(this->shim().CompleteTaskRequested(*reinterpret_cast<llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskDataProviderConnection, llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListCompleteTaskRequestEventArgs> const*>(&handler)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall remove_CompleteTaskRequested(llm::event_token token) noexcept final
        {
            typename D::abi_guard guard(this->shim());
            this->shim().CompleteTaskRequested(*reinterpret_cast<llm::event_token const*>(&token));
            return 0;
        }
        int32_t __stdcall add_DeleteTaskRequested(void* handler, llm::event_token* token) noexcept final try
        {
            zero_abi<llm::event_token>(token);
            typename D::abi_guard guard(this->shim());
            *token = detach_from<llm::event_token>(this->shim().DeleteTaskRequested(*reinterpret_cast<llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskDataProviderConnection, llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListDeleteTaskRequestEventArgs> const*>(&handler)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall remove_DeleteTaskRequested(llm::event_token token) noexcept final
        {
            typename D::abi_guard guard(this->shim());
            this->shim().DeleteTaskRequested(*reinterpret_cast<llm::event_token const*>(&token));
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
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskDataProviderTriggerDetails> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskDataProviderTriggerDetails>
    {
        int32_t __stdcall get_Connection(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskDataProviderConnection>(this->shim().Connection());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCompleteTaskRequest> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCompleteTaskRequest>
    {
        int32_t __stdcall get_TaskListId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().TaskListId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_TaskId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().TaskId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall ReportCompletedAsync(void* completedTaskId, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncAction>(this->shim().ReportCompletedAsync(*reinterpret_cast<hstring const*>(&completedTaskId)));
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
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCompleteTaskRequestEventArgs> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCompleteTaskRequestEventArgs>
    {
        int32_t __stdcall get_Request(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListCompleteTaskRequest>(this->shim().Request());
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
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCreateOrUpdateTaskRequest> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCreateOrUpdateTaskRequest>
    {
        int32_t __stdcall get_TaskListId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().TaskListId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Task(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::UserDataTask>(this->shim().Task());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall ReportCompletedAsync(void* createdOrUpdatedUserDataTask, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncAction>(this->shim().ReportCompletedAsync(*reinterpret_cast<llm::OS::ApplicationModel::UserDataTasks::UserDataTask const*>(&createdOrUpdatedUserDataTask)));
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
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCreateOrUpdateTaskRequestEventArgs> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCreateOrUpdateTaskRequestEventArgs>
    {
        int32_t __stdcall get_Request(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListCreateOrUpdateTaskRequest>(this->shim().Request());
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
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListDeleteTaskRequest> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListDeleteTaskRequest>
    {
        int32_t __stdcall get_TaskListId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().TaskListId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_TaskId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().TaskId());
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
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListDeleteTaskRequestEventArgs> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListDeleteTaskRequestEventArgs>
    {
        int32_t __stdcall get_Request(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListDeleteTaskRequest>(this->shim().Request());
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
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSkipOccurrenceRequest> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSkipOccurrenceRequest>
    {
        int32_t __stdcall get_TaskListId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().TaskListId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_TaskId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().TaskId());
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
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSkipOccurrenceRequestEventArgs> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSkipOccurrenceRequestEventArgs>
    {
        int32_t __stdcall get_Request(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListSkipOccurrenceRequest>(this->shim().Request());
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
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSyncManagerSyncRequest> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSyncManagerSyncRequest>
    {
        int32_t __stdcall get_TaskListId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().TaskListId());
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
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSyncManagerSyncRequestEventArgs> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSyncManagerSyncRequestEventArgs>
    {
        int32_t __stdcall get_Request(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListSyncManagerSyncRequest>(this->shim().Request());
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
}
LLM_EXPORT namespace llm::OS::ApplicationModel::UserDataTasks::DataProvider
{
}
namespace std
{
#ifndef LLM_LEAN_AND_MEAN
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskDataProviderConnection> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskDataProviderTriggerDetails> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCompleteTaskRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCompleteTaskRequestEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCreateOrUpdateTaskRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListCreateOrUpdateTaskRequestEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListDeleteTaskRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListDeleteTaskRequestEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSkipOccurrenceRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSkipOccurrenceRequestEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSyncManagerSyncRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::IUserDataTaskListSyncManagerSyncRequestEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskDataProviderConnection> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskDataProviderTriggerDetails> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListCompleteTaskRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListCompleteTaskRequestEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListCreateOrUpdateTaskRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListCreateOrUpdateTaskRequestEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListDeleteTaskRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListDeleteTaskRequestEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListSkipOccurrenceRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListSkipOccurrenceRequestEventArgs> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListSyncManagerSyncRequest> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::DataProvider::UserDataTaskListSyncManagerSyncRequestEventArgs> : llm::impl::hash_base {};
#endif
#ifdef __cpp_lib_format
#endif
}
#endif
