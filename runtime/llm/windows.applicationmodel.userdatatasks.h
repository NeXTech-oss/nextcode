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
#ifndef LLM_OS_ApplicationModel_UserDataTasks_H
#define LLM_OS_ApplicationModel_UserDataTasks_H
#include "llm/base.h"
static_assert(llm::check_version(CPPLLM_VERSION, "2.0.220110.5"), "Mismatched C++/WinRT headers.");
#define CPPLLM_VERSION "2.0.220110.5"
#include "llm/Windows.ApplicationModel.h"
#include "llm/impl/Windows.Foundation.2.h"
#include "llm/impl/Windows.Foundation.Collections.2.h"
#include "llm/impl/Windows.System.2.h"
#include "llm/impl/Windows.ApplicationModel.UserDataTasks.2.h"
namespace llm::impl
{
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::Id() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->get_Id(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::ListId() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->get_ListId(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::RemoteId() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->get_RemoteId(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::RemoteId(param::hstring const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->put_RemoteId(*(void**)(&value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::CompletedDate() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->get_CompletedDate(&value));
        return llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::CompletedDate(llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime> const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->put_CompletedDate(*(void**)(&value)));
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::Details() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->get_Details(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::Details(param::hstring const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->put_Details(*(void**)(&value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskDetailsKind) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::DetailsKind() const
    {
        llm::OS::ApplicationModel::UserDataTasks::UserDataTaskDetailsKind value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->get_DetailsKind(reinterpret_cast<int32_t*>(&value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::DetailsKind(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskDetailsKind const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->put_DetailsKind(static_cast<int32_t>(value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::DueDate() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->get_DueDate(&value));
        return llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::DueDate(llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime> const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->put_DueDate(*(void**)(&value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskKind) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::Kind() const
    {
        llm::OS::ApplicationModel::UserDataTasks::UserDataTaskKind value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->get_Kind(reinterpret_cast<int32_t*>(&value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskPriority) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::Priority() const
    {
        llm::OS::ApplicationModel::UserDataTasks::UserDataTaskPriority value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->get_Priority(reinterpret_cast<int32_t*>(&value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::Priority(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskPriority const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->put_Priority(static_cast<int32_t>(value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRecurrenceProperties) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::RecurrenceProperties() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->get_RecurrenceProperties(&value));
        return llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRecurrenceProperties{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::RecurrenceProperties(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRecurrenceProperties const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->put_RecurrenceProperties(*(void**)(&value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRegenerationProperties) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::RegenerationProperties() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->get_RegenerationProperties(&value));
        return llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRegenerationProperties{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::RegenerationProperties(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRegenerationProperties const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->put_RegenerationProperties(*(void**)(&value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::Reminder() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->get_Reminder(&value));
        return llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::Reminder(llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime> const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->put_Reminder(*(void**)(&value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskSensitivity) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::Sensitivity() const
    {
        llm::OS::ApplicationModel::UserDataTasks::UserDataTaskSensitivity value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->get_Sensitivity(reinterpret_cast<int32_t*>(&value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::Sensitivity(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskSensitivity const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->put_Sensitivity(static_cast<int32_t>(value)));
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::Subject() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->get_Subject(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::Subject(param::hstring const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->put_Subject(*(void**)(&value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::StartDate() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->get_StartDate(&value));
        return llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTask<D>::StartDate(llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime> const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTask)->put_StartDate(*(void**)(&value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Collections::IVectorView<llm::OS::ApplicationModel::UserDataTasks::UserDataTask>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskBatch<D>::Tasks() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskBatch)->get_Tasks(&value));
        return llm::OS::Foundation::Collections::IVectorView<llm::OS::ApplicationModel::UserDataTasks::UserDataTask>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskList<D>::Id() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList)->get_Id(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskList<D>::UserDataAccountId() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList)->get_UserDataAccountId(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskList<D>::DisplayName() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList)->get_DisplayName(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskList<D>::DisplayName(param::hstring const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList)->put_DisplayName(*(void**)(&value)));
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskList<D>::SourceDisplayName() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList)->get_SourceDisplayName(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListOtherAppReadAccess) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskList<D>::OtherAppReadAccess() const
    {
        llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListOtherAppReadAccess value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList)->get_OtherAppReadAccess(reinterpret_cast<int32_t*>(&value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskList<D>::OtherAppReadAccess(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListOtherAppReadAccess const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList)->put_OtherAppReadAccess(static_cast<int32_t>(value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListOtherAppWriteAccess) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskList<D>::OtherAppWriteAccess() const
    {
        llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListOtherAppWriteAccess value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList)->get_OtherAppWriteAccess(reinterpret_cast<int32_t*>(&value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskList<D>::OtherAppWriteAccess(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListOtherAppWriteAccess const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList)->put_OtherAppWriteAccess(static_cast<int32_t>(value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListLimitedWriteOperations) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskList<D>::LimitedWriteOperations() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList)->get_LimitedWriteOperations(&value));
        return llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListLimitedWriteOperations{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListSyncManager) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskList<D>::SyncManager() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList)->get_SyncManager(&value));
        return llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListSyncManager{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskList<D>::RegisterSyncManagerAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList)->RegisterSyncManagerAsync(&result));
        return llm::OS::Foundation::IAsyncAction{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskReader) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskList<D>::GetTaskReader() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList)->GetTaskReader(&result));
        return llm::OS::ApplicationModel::UserDataTasks::UserDataTaskReader{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskReader) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskList<D>::GetTaskReader(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskQueryOptions const& options) const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList)->GetTaskReaderWithOptions(*(void**)(&options), &value));
        return llm::OS::ApplicationModel::UserDataTasks::UserDataTaskReader{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<llm::OS::ApplicationModel::UserDataTasks::UserDataTask>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskList<D>::GetTaskAsync(param::hstring const& userDataTask) const
    {
        void* operation{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList)->GetTaskAsync(*(void**)(&userDataTask), &operation));
        return llm::OS::Foundation::IAsyncOperation<llm::OS::ApplicationModel::UserDataTasks::UserDataTask>{ operation, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskList<D>::SaveTaskAsync(llm::OS::ApplicationModel::UserDataTasks::UserDataTask const& userDataTask) const
    {
        void* action{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList)->SaveTaskAsync(*(void**)(&userDataTask), &action));
        return llm::OS::Foundation::IAsyncAction{ action, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskList<D>::DeleteTaskAsync(param::hstring const& userDataTaskId) const
    {
        void* action{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList)->DeleteTaskAsync(*(void**)(&userDataTaskId), &action));
        return llm::OS::Foundation::IAsyncAction{ action, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskList<D>::DeleteAsync() const
    {
        void* asyncAction{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList)->DeleteAsync(&asyncAction));
        return llm::OS::Foundation::IAsyncAction{ asyncAction, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncAction) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskList<D>::SaveAsync() const
    {
        void* asyncAction{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList)->SaveAsync(&asyncAction));
        return llm::OS::Foundation::IAsyncAction{ asyncAction, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<hstring>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskListLimitedWriteOperations<D>::TryCompleteTaskAsync(param::hstring const& userDataTaskId) const
    {
        void* operation{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskListLimitedWriteOperations)->TryCompleteTaskAsync(*(void**)(&userDataTaskId), &operation));
        return llm::OS::Foundation::IAsyncOperation<hstring>{ operation, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<bool>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskListLimitedWriteOperations<D>::TryCreateOrUpdateTaskAsync(llm::OS::ApplicationModel::UserDataTasks::UserDataTask const& userDataTask) const
    {
        void* operation{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskListLimitedWriteOperations)->TryCreateOrUpdateTaskAsync(*(void**)(&userDataTask), &operation));
        return llm::OS::Foundation::IAsyncOperation<bool>{ operation, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<bool>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskListLimitedWriteOperations<D>::TryDeleteTaskAsync(param::hstring const& userDataTaskId) const
    {
        void* operation{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskListLimitedWriteOperations)->TryDeleteTaskAsync(*(void**)(&userDataTaskId), &operation));
        return llm::OS::Foundation::IAsyncOperation<bool>{ operation, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<bool>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskListLimitedWriteOperations<D>::TrySkipOccurrenceAsync(param::hstring const& userDataTaskId) const
    {
        void* operation{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskListLimitedWriteOperations)->TrySkipOccurrenceAsync(*(void**)(&userDataTaskId), &operation));
        return llm::OS::Foundation::IAsyncOperation<bool>{ operation, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::DateTime) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskListSyncManager<D>::LastAttemptedSyncTime() const
    {
        llm::OS::Foundation::DateTime value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskListSyncManager)->get_LastAttemptedSyncTime(put_abi(value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskListSyncManager<D>::LastAttemptedSyncTime(llm::OS::Foundation::DateTime const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskListSyncManager)->put_LastAttemptedSyncTime(impl::bind_in(value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::DateTime) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskListSyncManager<D>::LastSuccessfulSyncTime() const
    {
        llm::OS::Foundation::DateTime value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskListSyncManager)->get_LastSuccessfulSyncTime(put_abi(value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskListSyncManager<D>::LastSuccessfulSyncTime(llm::OS::Foundation::DateTime const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskListSyncManager)->put_LastSuccessfulSyncTime(impl::bind_in(value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListSyncStatus) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskListSyncManager<D>::Status() const
    {
        llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListSyncStatus value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskListSyncManager)->get_Status(reinterpret_cast<int32_t*>(&value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskListSyncManager<D>::Status(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListSyncStatus const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskListSyncManager)->put_Status(static_cast<int32_t>(value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<bool>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskListSyncManager<D>::SyncAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskListSyncManager)->SyncAsync(&result));
        return llm::OS::Foundation::IAsyncOperation<bool>{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::event_token) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskListSyncManager<D>::SyncStatusChanged(llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListSyncManager, llm::OS::Foundation::IInspectable> const& handler) const
    {
        llm::event_token token{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskListSyncManager)->add_SyncStatusChanged(*(void**)(&handler), put_abi(token)));
        return token;
    }
    template <typename D> typename consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskListSyncManager<D>::SyncStatusChanged_revoker consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskListSyncManager<D>::SyncStatusChanged(auto_revoke_t, llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListSyncManager, llm::OS::Foundation::IInspectable> const& handler) const
    {
        return impl::make_event_revoker<D, SyncStatusChanged_revoker>(this, SyncStatusChanged(handler));
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskListSyncManager<D>::SyncStatusChanged(llm::event_token const& token) const noexcept
    {
        LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskListSyncManager)->remove_SyncStatusChanged(impl::bind_in(token));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskStore>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskManager<D>::RequestStoreAsync(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskStoreAccessType const& accessType) const
    {
        void* operation{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskManager)->RequestStoreAsync(static_cast<int32_t>(accessType), &operation));
        return llm::OS::Foundation::IAsyncOperation<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskStore>{ operation, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::System::User) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskManager<D>::User() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskManager)->get_User(&value));
        return llm::OS::System::User{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskManager) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskManagerStatics<D>::GetDefault() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskManagerStatics)->GetDefault(&result));
        return llm::OS::ApplicationModel::UserDataTasks::UserDataTaskManager{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskManager) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskManagerStatics<D>::GetForUser(llm::OS::System::User const& user) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskManagerStatics)->GetForUser(*(void**)(&user), &result));
        return llm::OS::ApplicationModel::UserDataTasks::UserDataTaskManager{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskQuerySortProperty) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskQueryOptions<D>::SortProperty() const
    {
        llm::OS::ApplicationModel::UserDataTasks::UserDataTaskQuerySortProperty value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskQueryOptions)->get_SortProperty(reinterpret_cast<int32_t*>(&value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskQueryOptions<D>::SortProperty(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskQuerySortProperty const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskQueryOptions)->put_SortProperty(static_cast<int32_t>(value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskQueryKind) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskQueryOptions<D>::Kind() const
    {
        llm::OS::ApplicationModel::UserDataTasks::UserDataTaskQueryKind value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskQueryOptions)->get_Kind(reinterpret_cast<int32_t*>(&value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskQueryOptions<D>::Kind(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskQueryKind const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskQueryOptions)->put_Kind(static_cast<int32_t>(value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskBatch>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskReader<D>::ReadBatchAsync() const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskReader)->ReadBatchAsync(&result));
        return llm::OS::Foundation::IAsyncOperation<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskBatch>{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRecurrenceUnit) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRecurrenceProperties<D>::Unit() const
    {
        llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRecurrenceUnit value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRecurrenceProperties)->get_Unit(reinterpret_cast<int32_t*>(&value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRecurrenceProperties<D>::Unit(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRecurrenceUnit const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRecurrenceProperties)->put_Unit(static_cast<int32_t>(value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IReference<int32_t>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRecurrenceProperties<D>::Occurrences() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRecurrenceProperties)->get_Occurrences(&value));
        return llm::OS::Foundation::IReference<int32_t>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRecurrenceProperties<D>::Occurrences(llm::OS::Foundation::IReference<int32_t> const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRecurrenceProperties)->put_Occurrences(*(void**)(&value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRecurrenceProperties<D>::Until() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRecurrenceProperties)->get_Until(&value));
        return llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRecurrenceProperties<D>::Until(llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime> const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRecurrenceProperties)->put_Until(*(void**)(&value)));
    }
    template <typename D> LLM_IMPL_AUTO(int32_t) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRecurrenceProperties<D>::Interval() const
    {
        int32_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRecurrenceProperties)->get_Interval(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRecurrenceProperties<D>::Interval(int32_t value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRecurrenceProperties)->put_Interval(value));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IReference<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskDaysOfWeek>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRecurrenceProperties<D>::DaysOfWeek() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRecurrenceProperties)->get_DaysOfWeek(&value));
        return llm::OS::Foundation::IReference<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskDaysOfWeek>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRecurrenceProperties<D>::DaysOfWeek(llm::OS::Foundation::IReference<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskDaysOfWeek> const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRecurrenceProperties)->put_DaysOfWeek(*(void**)(&value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IReference<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskWeekOfMonth>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRecurrenceProperties<D>::WeekOfMonth() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRecurrenceProperties)->get_WeekOfMonth(&value));
        return llm::OS::Foundation::IReference<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskWeekOfMonth>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRecurrenceProperties<D>::WeekOfMonth(llm::OS::Foundation::IReference<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskWeekOfMonth> const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRecurrenceProperties)->put_WeekOfMonth(*(void**)(&value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IReference<int32_t>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRecurrenceProperties<D>::Month() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRecurrenceProperties)->get_Month(&value));
        return llm::OS::Foundation::IReference<int32_t>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRecurrenceProperties<D>::Month(llm::OS::Foundation::IReference<int32_t> const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRecurrenceProperties)->put_Month(*(void**)(&value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IReference<int32_t>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRecurrenceProperties<D>::Day() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRecurrenceProperties)->get_Day(&value));
        return llm::OS::Foundation::IReference<int32_t>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRecurrenceProperties<D>::Day(llm::OS::Foundation::IReference<int32_t> const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRecurrenceProperties)->put_Day(*(void**)(&value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRegenerationUnit) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRegenerationProperties<D>::Unit() const
    {
        llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRegenerationUnit value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRegenerationProperties)->get_Unit(reinterpret_cast<int32_t*>(&value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRegenerationProperties<D>::Unit(llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRegenerationUnit const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRegenerationProperties)->put_Unit(static_cast<int32_t>(value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IReference<int32_t>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRegenerationProperties<D>::Occurrences() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRegenerationProperties)->get_Occurrences(&value));
        return llm::OS::Foundation::IReference<int32_t>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRegenerationProperties<D>::Occurrences(llm::OS::Foundation::IReference<int32_t> const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRegenerationProperties)->put_Occurrences(*(void**)(&value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRegenerationProperties<D>::Until() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRegenerationProperties)->get_Until(&value));
        return llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRegenerationProperties<D>::Until(llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime> const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRegenerationProperties)->put_Until(*(void**)(&value)));
    }
    template <typename D> LLM_IMPL_AUTO(int32_t) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRegenerationProperties<D>::Interval() const
    {
        int32_t value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRegenerationProperties)->get_Interval(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskRegenerationProperties<D>::Interval(int32_t value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRegenerationProperties)->put_Interval(value));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskList>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskStore<D>::CreateListAsync(param::hstring const& name) const
    {
        void* operation{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskStore)->CreateListAsync(*(void**)(&name), &operation));
        return llm::OS::Foundation::IAsyncOperation<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskList>{ operation, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskList>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskStore<D>::CreateListAsync(param::hstring const& name, param::hstring const& userDataAccountId) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskStore)->CreateListInAccountAsync(*(void**)(&name), *(void**)(&userDataAccountId), &result));
        return llm::OS::Foundation::IAsyncOperation<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskList>{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskList>>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskStore<D>::FindListsAsync() const
    {
        void* operation{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskStore)->FindListsAsync(&operation));
        return llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskList>>{ operation, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskList>) consume_Windows_ApplicationModel_UserDataTasks_IUserDataTaskStore<D>::GetListAsync(param::hstring const& taskListId) const
    {
        void* operation{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskStore)->GetListAsync(*(void**)(&taskListId), &operation));
        return llm::OS::Foundation::IAsyncOperation<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskList>{ operation, take_ownership_from_abi };
    }
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTask> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTask>
    {
        int32_t __stdcall get_Id(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().Id());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_ListId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().ListId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_RemoteId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().RemoteId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_RemoteId(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().RemoteId(*reinterpret_cast<hstring const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_CompletedDate(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>>(this->shim().CompletedDate());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_CompletedDate(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().CompletedDate(*reinterpret_cast<llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime> const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Details(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().Details());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Details(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Details(*reinterpret_cast<hstring const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_DetailsKind(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskDetailsKind>(this->shim().DetailsKind());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_DetailsKind(int32_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().DetailsKind(*reinterpret_cast<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskDetailsKind const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_DueDate(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>>(this->shim().DueDate());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_DueDate(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().DueDate(*reinterpret_cast<llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime> const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Kind(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskKind>(this->shim().Kind());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Priority(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskPriority>(this->shim().Priority());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Priority(int32_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Priority(*reinterpret_cast<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskPriority const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_RecurrenceProperties(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRecurrenceProperties>(this->shim().RecurrenceProperties());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_RecurrenceProperties(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().RecurrenceProperties(*reinterpret_cast<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRecurrenceProperties const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_RegenerationProperties(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRegenerationProperties>(this->shim().RegenerationProperties());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_RegenerationProperties(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().RegenerationProperties(*reinterpret_cast<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRegenerationProperties const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Reminder(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>>(this->shim().Reminder());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Reminder(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Reminder(*reinterpret_cast<llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime> const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Sensitivity(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskSensitivity>(this->shim().Sensitivity());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Sensitivity(int32_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Sensitivity(*reinterpret_cast<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskSensitivity const*>(&value));
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
        int32_t __stdcall put_Subject(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Subject(*reinterpret_cast<hstring const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_StartDate(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>>(this->shim().StartDate());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_StartDate(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().StartDate(*reinterpret_cast<llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime> const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskBatch> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskBatch>
    {
        int32_t __stdcall get_Tasks(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::Collections::IVectorView<llm::OS::ApplicationModel::UserDataTasks::UserDataTask>>(this->shim().Tasks());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList>
    {
        int32_t __stdcall get_Id(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().Id());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_UserDataAccountId(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().UserDataAccountId());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_DisplayName(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().DisplayName());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_DisplayName(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().DisplayName(*reinterpret_cast<hstring const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_SourceDisplayName(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().SourceDisplayName());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_OtherAppReadAccess(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListOtherAppReadAccess>(this->shim().OtherAppReadAccess());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_OtherAppReadAccess(int32_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().OtherAppReadAccess(*reinterpret_cast<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListOtherAppReadAccess const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_OtherAppWriteAccess(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListOtherAppWriteAccess>(this->shim().OtherAppWriteAccess());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_OtherAppWriteAccess(int32_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().OtherAppWriteAccess(*reinterpret_cast<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListOtherAppWriteAccess const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_LimitedWriteOperations(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListLimitedWriteOperations>(this->shim().LimitedWriteOperations());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_SyncManager(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListSyncManager>(this->shim().SyncManager());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall RegisterSyncManagerAsync(void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncAction>(this->shim().RegisterSyncManagerAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetTaskReader(void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskReader>(this->shim().GetTaskReader());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetTaskReaderWithOptions(void* options, void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskReader>(this->shim().GetTaskReader(*reinterpret_cast<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskQueryOptions const*>(&options)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetTaskAsync(void* userDataTask, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<llm::OS::Foundation::IAsyncOperation<llm::OS::ApplicationModel::UserDataTasks::UserDataTask>>(this->shim().GetTaskAsync(*reinterpret_cast<hstring const*>(&userDataTask)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall SaveTaskAsync(void* userDataTask, void** action) noexcept final try
        {
            clear_abi(action);
            typename D::abi_guard guard(this->shim());
            *action = detach_from<llm::OS::Foundation::IAsyncAction>(this->shim().SaveTaskAsync(*reinterpret_cast<llm::OS::ApplicationModel::UserDataTasks::UserDataTask const*>(&userDataTask)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall DeleteTaskAsync(void* userDataTaskId, void** action) noexcept final try
        {
            clear_abi(action);
            typename D::abi_guard guard(this->shim());
            *action = detach_from<llm::OS::Foundation::IAsyncAction>(this->shim().DeleteTaskAsync(*reinterpret_cast<hstring const*>(&userDataTaskId)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall DeleteAsync(void** asyncAction) noexcept final try
        {
            clear_abi(asyncAction);
            typename D::abi_guard guard(this->shim());
            *asyncAction = detach_from<llm::OS::Foundation::IAsyncAction>(this->shim().DeleteAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall SaveAsync(void** asyncAction) noexcept final try
        {
            clear_abi(asyncAction);
            typename D::abi_guard guard(this->shim());
            *asyncAction = detach_from<llm::OS::Foundation::IAsyncAction>(this->shim().SaveAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskListLimitedWriteOperations> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskListLimitedWriteOperations>
    {
        int32_t __stdcall TryCompleteTaskAsync(void* userDataTaskId, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<llm::OS::Foundation::IAsyncOperation<hstring>>(this->shim().TryCompleteTaskAsync(*reinterpret_cast<hstring const*>(&userDataTaskId)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall TryCreateOrUpdateTaskAsync(void* userDataTask, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<llm::OS::Foundation::IAsyncOperation<bool>>(this->shim().TryCreateOrUpdateTaskAsync(*reinterpret_cast<llm::OS::ApplicationModel::UserDataTasks::UserDataTask const*>(&userDataTask)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall TryDeleteTaskAsync(void* userDataTaskId, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<llm::OS::Foundation::IAsyncOperation<bool>>(this->shim().TryDeleteTaskAsync(*reinterpret_cast<hstring const*>(&userDataTaskId)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall TrySkipOccurrenceAsync(void* userDataTaskId, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<llm::OS::Foundation::IAsyncOperation<bool>>(this->shim().TrySkipOccurrenceAsync(*reinterpret_cast<hstring const*>(&userDataTaskId)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskListSyncManager> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskListSyncManager>
    {
        int32_t __stdcall get_LastAttemptedSyncTime(int64_t* value) noexcept final try
        {
            zero_abi<llm::OS::Foundation::DateTime>(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::DateTime>(this->shim().LastAttemptedSyncTime());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_LastAttemptedSyncTime(int64_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().LastAttemptedSyncTime(*reinterpret_cast<llm::OS::Foundation::DateTime const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_LastSuccessfulSyncTime(int64_t* value) noexcept final try
        {
            zero_abi<llm::OS::Foundation::DateTime>(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::DateTime>(this->shim().LastSuccessfulSyncTime());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_LastSuccessfulSyncTime(int64_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().LastSuccessfulSyncTime(*reinterpret_cast<llm::OS::Foundation::DateTime const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Status(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListSyncStatus>(this->shim().Status());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Status(int32_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Status(*reinterpret_cast<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListSyncStatus const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall SyncAsync(void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncOperation<bool>>(this->shim().SyncAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall add_SyncStatusChanged(void* handler, llm::event_token* token) noexcept final try
        {
            zero_abi<llm::event_token>(token);
            typename D::abi_guard guard(this->shim());
            *token = detach_from<llm::event_token>(this->shim().SyncStatusChanged(*reinterpret_cast<llm::OS::Foundation::TypedEventHandler<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListSyncManager, llm::OS::Foundation::IInspectable> const*>(&handler)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall remove_SyncStatusChanged(llm::event_token token) noexcept final
        {
            typename D::abi_guard guard(this->shim());
            this->shim().SyncStatusChanged(*reinterpret_cast<llm::event_token const*>(&token));
            return 0;
        }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskManager> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskManager>
    {
        int32_t __stdcall RequestStoreAsync(int32_t accessType, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<llm::OS::Foundation::IAsyncOperation<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskStore>>(this->shim().RequestStoreAsync(*reinterpret_cast<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskStoreAccessType const*>(&accessType)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_User(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::System::User>(this->shim().User());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskManagerStatics> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskManagerStatics>
    {
        int32_t __stdcall GetDefault(void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskManager>(this->shim().GetDefault());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetForUser(void* user, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskManager>(this->shim().GetForUser(*reinterpret_cast<llm::OS::System::User const*>(&user)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskQueryOptions> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskQueryOptions>
    {
        int32_t __stdcall get_SortProperty(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskQuerySortProperty>(this->shim().SortProperty());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_SortProperty(int32_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().SortProperty(*reinterpret_cast<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskQuerySortProperty const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Kind(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskQueryKind>(this->shim().Kind());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Kind(int32_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Kind(*reinterpret_cast<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskQueryKind const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskReader> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskReader>
    {
        int32_t __stdcall ReadBatchAsync(void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncOperation<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskBatch>>(this->shim().ReadBatchAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRecurrenceProperties> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRecurrenceProperties>
    {
        int32_t __stdcall get_Unit(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRecurrenceUnit>(this->shim().Unit());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Unit(int32_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Unit(*reinterpret_cast<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRecurrenceUnit const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Occurrences(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::IReference<int32_t>>(this->shim().Occurrences());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Occurrences(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Occurrences(*reinterpret_cast<llm::OS::Foundation::IReference<int32_t> const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Until(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>>(this->shim().Until());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Until(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Until(*reinterpret_cast<llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime> const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Interval(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<int32_t>(this->shim().Interval());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Interval(int32_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Interval(value);
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_DaysOfWeek(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::IReference<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskDaysOfWeek>>(this->shim().DaysOfWeek());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_DaysOfWeek(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().DaysOfWeek(*reinterpret_cast<llm::OS::Foundation::IReference<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskDaysOfWeek> const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_WeekOfMonth(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::IReference<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskWeekOfMonth>>(this->shim().WeekOfMonth());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_WeekOfMonth(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().WeekOfMonth(*reinterpret_cast<llm::OS::Foundation::IReference<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskWeekOfMonth> const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Month(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::IReference<int32_t>>(this->shim().Month());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Month(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Month(*reinterpret_cast<llm::OS::Foundation::IReference<int32_t> const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Day(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::IReference<int32_t>>(this->shim().Day());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Day(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Day(*reinterpret_cast<llm::OS::Foundation::IReference<int32_t> const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRegenerationProperties> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRegenerationProperties>
    {
        int32_t __stdcall get_Unit(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRegenerationUnit>(this->shim().Unit());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Unit(int32_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Unit(*reinterpret_cast<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRegenerationUnit const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Occurrences(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::IReference<int32_t>>(this->shim().Occurrences());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Occurrences(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Occurrences(*reinterpret_cast<llm::OS::Foundation::IReference<int32_t> const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Until(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime>>(this->shim().Until());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Until(void* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Until(*reinterpret_cast<llm::OS::Foundation::IReference<llm::OS::Foundation::DateTime> const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_Interval(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<int32_t>(this->shim().Interval());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_Interval(int32_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Interval(value);
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskStore> : produce_base<D, llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskStore>
    {
        int32_t __stdcall CreateListAsync(void* name, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<llm::OS::Foundation::IAsyncOperation<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskList>>(this->shim().CreateListAsync(*reinterpret_cast<hstring const*>(&name)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall CreateListInAccountAsync(void* name, void* userDataAccountId, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncOperation<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskList>>(this->shim().CreateListAsync(*reinterpret_cast<hstring const*>(&name), *reinterpret_cast<hstring const*>(&userDataAccountId)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall FindListsAsync(void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskList>>>(this->shim().FindListsAsync());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetListAsync(void* taskListId, void** operation) noexcept final try
        {
            clear_abi(operation);
            typename D::abi_guard guard(this->shim());
            *operation = detach_from<llm::OS::Foundation::IAsyncOperation<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskList>>(this->shim().GetListAsync(*reinterpret_cast<hstring const*>(&taskListId)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
}
LLM_EXPORT namespace llm::OS::ApplicationModel::UserDataTasks
{
    constexpr auto operator|(UserDataTaskDaysOfWeek const left, UserDataTaskDaysOfWeek const right) noexcept
    {
        return static_cast<UserDataTaskDaysOfWeek>(impl::to_underlying_type(left) | impl::to_underlying_type(right));
    }
    constexpr auto operator|=(UserDataTaskDaysOfWeek& left, UserDataTaskDaysOfWeek const right) noexcept
    {
        left = left | right;
        return left;
    }
    constexpr auto operator&(UserDataTaskDaysOfWeek const left, UserDataTaskDaysOfWeek const right) noexcept
    {
        return static_cast<UserDataTaskDaysOfWeek>(impl::to_underlying_type(left) & impl::to_underlying_type(right));
    }
    constexpr auto operator&=(UserDataTaskDaysOfWeek& left, UserDataTaskDaysOfWeek const right) noexcept
    {
        left = left & right;
        return left;
    }
    constexpr auto operator~(UserDataTaskDaysOfWeek const value) noexcept
    {
        return static_cast<UserDataTaskDaysOfWeek>(~impl::to_underlying_type(value));
    }
    constexpr auto operator^(UserDataTaskDaysOfWeek const left, UserDataTaskDaysOfWeek const right) noexcept
    {
        return static_cast<UserDataTaskDaysOfWeek>(impl::to_underlying_type(left) ^ impl::to_underlying_type(right));
    }
    constexpr auto operator^=(UserDataTaskDaysOfWeek& left, UserDataTaskDaysOfWeek const right) noexcept
    {
        left = left ^ right;
        return left;
    }
    inline UserDataTask::UserDataTask() :
        UserDataTask(impl::call_factory_cast<UserDataTask(*)(llm::OS::Foundation::IActivationFactory const&), UserDataTask>([](llm::OS::Foundation::IActivationFactory const& f) { return f.template ActivateInstance<UserDataTask>(); }))
    {
    }
    inline auto UserDataTaskManager::GetDefault()
    {
        return impl::call_factory_cast<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskManager(*)(IUserDataTaskManagerStatics const&), UserDataTaskManager, IUserDataTaskManagerStatics>([](IUserDataTaskManagerStatics const& f) { return f.GetDefault(); });
    }
    inline auto UserDataTaskManager::GetForUser(llm::OS::System::User const& user)
    {
        return impl::call_factory<UserDataTaskManager, IUserDataTaskManagerStatics>([&](IUserDataTaskManagerStatics const& f) { return f.GetForUser(user); });
    }
    inline UserDataTaskQueryOptions::UserDataTaskQueryOptions() :
        UserDataTaskQueryOptions(impl::call_factory_cast<UserDataTaskQueryOptions(*)(llm::OS::Foundation::IActivationFactory const&), UserDataTaskQueryOptions>([](llm::OS::Foundation::IActivationFactory const& f) { return f.template ActivateInstance<UserDataTaskQueryOptions>(); }))
    {
    }
    inline UserDataTaskRecurrenceProperties::UserDataTaskRecurrenceProperties() :
        UserDataTaskRecurrenceProperties(impl::call_factory_cast<UserDataTaskRecurrenceProperties(*)(llm::OS::Foundation::IActivationFactory const&), UserDataTaskRecurrenceProperties>([](llm::OS::Foundation::IActivationFactory const& f) { return f.template ActivateInstance<UserDataTaskRecurrenceProperties>(); }))
    {
    }
    inline UserDataTaskRegenerationProperties::UserDataTaskRegenerationProperties() :
        UserDataTaskRegenerationProperties(impl::call_factory_cast<UserDataTaskRegenerationProperties(*)(llm::OS::Foundation::IActivationFactory const&), UserDataTaskRegenerationProperties>([](llm::OS::Foundation::IActivationFactory const& f) { return f.template ActivateInstance<UserDataTaskRegenerationProperties>(); }))
    {
    }
}
namespace std
{
#ifndef LLM_LEAN_AND_MEAN
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::IUserDataTask> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskBatch> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskList> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskListLimitedWriteOperations> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskListSyncManager> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskManager> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskManagerStatics> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskQueryOptions> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskReader> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRecurrenceProperties> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskRegenerationProperties> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::IUserDataTaskStore> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::UserDataTask> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskBatch> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskList> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListLimitedWriteOperations> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskListSyncManager> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskManager> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskQueryOptions> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskReader> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRecurrenceProperties> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskRegenerationProperties> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::ApplicationModel::UserDataTasks::UserDataTaskStore> : llm::impl::hash_base {};
#endif
#ifdef __cpp_lib_format
#endif
}
#endif
