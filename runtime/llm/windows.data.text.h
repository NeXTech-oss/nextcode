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
#ifndef LLM_OS_Data_Text_H
#define LLM_OS_Data_Text_H
#include "llm/base.h"
static_assert(llm::check_version(CPPLLM_VERSION, "2.0.220110.5"), "Mismatched C++/WinRT headers.");
#define CPPLLM_VERSION "2.0.220110.5"
#include "llm/impl/Windows.Foundation.2.h"
#include "llm/impl/Windows.Foundation.Collections.2.h"
#include "llm/impl/Windows.UI.Text.Core.2.h"
#include "llm/impl/Windows.Data.Text.2.h"
namespace llm::impl
{
    template <typename D> LLM_IMPL_AUTO(llm::OS::Data::Text::TextSegment) consume_Windows_Data_Text_IAlternateWordForm<D>::SourceTextSegment() const
    {
        llm::OS::Data::Text::TextSegment value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IAlternateWordForm)->get_SourceTextSegment(put_abi(value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_Data_Text_IAlternateWordForm<D>::AlternateText() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IAlternateWordForm)->get_AlternateText(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Data::Text::AlternateNormalizationFormat) consume_Windows_Data_Text_IAlternateWordForm<D>::NormalizationFormat() const
    {
        llm::OS::Data::Text::AlternateNormalizationFormat value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IAlternateWordForm)->get_NormalizationFormat(reinterpret_cast<int32_t*>(&value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_Data_Text_ISelectableWordSegment<D>::Text() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ISelectableWordSegment)->get_Text(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Data::Text::TextSegment) consume_Windows_Data_Text_ISelectableWordSegment<D>::SourceTextSegment() const
    {
        llm::OS::Data::Text::TextSegment value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ISelectableWordSegment)->get_SourceTextSegment(put_abi(value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_Data_Text_ISelectableWordsSegmenter<D>::ResolvedLanguage() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ISelectableWordsSegmenter)->get_ResolvedLanguage(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Data::Text::SelectableWordSegment) consume_Windows_Data_Text_ISelectableWordsSegmenter<D>::GetTokenAt(param::hstring const& text, uint32_t startIndex) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ISelectableWordsSegmenter)->GetTokenAt(*(void**)(&text), startIndex, &result));
        return llm::OS::Data::Text::SelectableWordSegment{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Collections::IVectorView<llm::OS::Data::Text::SelectableWordSegment>) consume_Windows_Data_Text_ISelectableWordsSegmenter<D>::GetTokens(param::hstring const& text) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ISelectableWordsSegmenter)->GetTokens(*(void**)(&text), &result));
        return llm::OS::Foundation::Collections::IVectorView<llm::OS::Data::Text::SelectableWordSegment>{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_Data_Text_ISelectableWordsSegmenter<D>::Tokenize(param::hstring const& text, uint32_t startIndex, llm::OS::Data::Text::SelectableWordSegmentsTokenizingHandler const& handler) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ISelectableWordsSegmenter)->Tokenize(*(void**)(&text), startIndex, *(void**)(&handler)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Data::Text::SelectableWordsSegmenter) consume_Windows_Data_Text_ISelectableWordsSegmenterFactory<D>::CreateWithLanguage(param::hstring const& language) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ISelectableWordsSegmenterFactory)->CreateWithLanguage(*(void**)(&language), &result));
        return llm::OS::Data::Text::SelectableWordsSegmenter{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Collections::IVectorView<llm::OS::Data::Text::TextSegment>) consume_Windows_Data_Text_ISemanticTextQuery<D>::Find(param::hstring const& content) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ISemanticTextQuery)->Find(*(void**)(&content), &result));
        return llm::OS::Foundation::Collections::IVectorView<llm::OS::Data::Text::TextSegment>{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Collections::IVectorView<llm::OS::Data::Text::TextSegment>) consume_Windows_Data_Text_ISemanticTextQuery<D>::FindInProperty(param::hstring const& propertyContent, param::hstring const& propertyName) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ISemanticTextQuery)->FindInProperty(*(void**)(&propertyContent), *(void**)(&propertyName), &result));
        return llm::OS::Foundation::Collections::IVectorView<llm::OS::Data::Text::TextSegment>{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Data::Text::SemanticTextQuery) consume_Windows_Data_Text_ISemanticTextQueryFactory<D>::Create(param::hstring const& aqsFilter) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ISemanticTextQueryFactory)->Create(*(void**)(&aqsFilter), &result));
        return llm::OS::Data::Text::SemanticTextQuery{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Data::Text::SemanticTextQuery) consume_Windows_Data_Text_ISemanticTextQueryFactory<D>::CreateWithLanguage(param::hstring const& aqsFilter, param::hstring const& filterLanguage) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ISemanticTextQueryFactory)->CreateWithLanguage(*(void**)(&aqsFilter), *(void**)(&filterLanguage), &result));
        return llm::OS::Data::Text::SemanticTextQuery{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_Data_Text_ITextConversionGenerator<D>::ResolvedLanguage() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ITextConversionGenerator)->get_ResolvedLanguage(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_Data_Text_ITextConversionGenerator<D>::LanguageAvailableButNotInstalled() const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ITextConversionGenerator)->get_LanguageAvailableButNotInstalled(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<hstring>>) consume_Windows_Data_Text_ITextConversionGenerator<D>::GetCandidatesAsync(param::hstring const& input) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ITextConversionGenerator)->GetCandidatesAsync(*(void**)(&input), &result));
        return llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<hstring>>{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<hstring>>) consume_Windows_Data_Text_ITextConversionGenerator<D>::GetCandidatesAsync(param::hstring const& input, uint32_t maxCandidates) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ITextConversionGenerator)->GetCandidatesWithMaxCountAsync(*(void**)(&input), maxCandidates, &result));
        return llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<hstring>>{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Data::Text::TextConversionGenerator) consume_Windows_Data_Text_ITextConversionGeneratorFactory<D>::Create(param::hstring const& languageTag) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ITextConversionGeneratorFactory)->Create(*(void**)(&languageTag), &result));
        return llm::OS::Data::Text::TextConversionGenerator{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_Data_Text_ITextPhoneme<D>::DisplayText() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ITextPhoneme)->get_DisplayText(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_Data_Text_ITextPhoneme<D>::ReadingText() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ITextPhoneme)->get_ReadingText(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_Data_Text_ITextPredictionGenerator<D>::ResolvedLanguage() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ITextPredictionGenerator)->get_ResolvedLanguage(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_Data_Text_ITextPredictionGenerator<D>::LanguageAvailableButNotInstalled() const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ITextPredictionGenerator)->get_LanguageAvailableButNotInstalled(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<hstring>>) consume_Windows_Data_Text_ITextPredictionGenerator<D>::GetCandidatesAsync(param::hstring const& input) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ITextPredictionGenerator)->GetCandidatesAsync(*(void**)(&input), &result));
        return llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<hstring>>{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<hstring>>) consume_Windows_Data_Text_ITextPredictionGenerator<D>::GetCandidatesAsync(param::hstring const& input, uint32_t maxCandidates) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ITextPredictionGenerator)->GetCandidatesWithMaxCountAsync(*(void**)(&input), maxCandidates, &result));
        return llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<hstring>>{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<hstring>>) consume_Windows_Data_Text_ITextPredictionGenerator2<D>::GetCandidatesAsync(param::hstring const& input, uint32_t maxCandidates, llm::OS::Data::Text::TextPredictionOptions const& predictionOptions, param::async_iterable<hstring> const& previousStrings) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ITextPredictionGenerator2)->GetCandidatesWithParametersAsync(*(void**)(&input), maxCandidates, static_cast<uint32_t>(predictionOptions), *(void**)(&previousStrings), &result));
        return llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<hstring>>{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<hstring>>) consume_Windows_Data_Text_ITextPredictionGenerator2<D>::GetNextWordCandidatesAsync(uint32_t maxCandidates, param::async_iterable<hstring> const& previousStrings) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ITextPredictionGenerator2)->GetNextWordCandidatesAsync(maxCandidates, *(void**)(&previousStrings), &result));
        return llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<hstring>>{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::UI::Text::Core::CoreTextInputScope) consume_Windows_Data_Text_ITextPredictionGenerator2<D>::InputScope() const
    {
        llm::OS::UI::Text::Core::CoreTextInputScope value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ITextPredictionGenerator2)->get_InputScope(reinterpret_cast<int32_t*>(&value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_Data_Text_ITextPredictionGenerator2<D>::InputScope(llm::OS::UI::Text::Core::CoreTextInputScope const& value) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ITextPredictionGenerator2)->put_InputScope(static_cast<int32_t>(value)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Data::Text::TextPredictionGenerator) consume_Windows_Data_Text_ITextPredictionGeneratorFactory<D>::Create(param::hstring const& languageTag) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ITextPredictionGeneratorFactory)->Create(*(void**)(&languageTag), &result));
        return llm::OS::Data::Text::TextPredictionGenerator{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_Data_Text_ITextReverseConversionGenerator<D>::ResolvedLanguage() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ITextReverseConversionGenerator)->get_ResolvedLanguage(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_Data_Text_ITextReverseConversionGenerator<D>::LanguageAvailableButNotInstalled() const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ITextReverseConversionGenerator)->get_LanguageAvailableButNotInstalled(&value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<hstring>) consume_Windows_Data_Text_ITextReverseConversionGenerator<D>::ConvertBackAsync(param::hstring const& input) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ITextReverseConversionGenerator)->ConvertBackAsync(*(void**)(&input), &result));
        return llm::OS::Foundation::IAsyncOperation<hstring>{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<llm::OS::Data::Text::TextPhoneme>>) consume_Windows_Data_Text_ITextReverseConversionGenerator2<D>::GetPhonemesAsync(param::hstring const& input) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ITextReverseConversionGenerator2)->GetPhonemesAsync(*(void**)(&input), &result));
        return llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<llm::OS::Data::Text::TextPhoneme>>{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Data::Text::TextReverseConversionGenerator) consume_Windows_Data_Text_ITextReverseConversionGeneratorFactory<D>::Create(param::hstring const& languageTag) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::ITextReverseConversionGeneratorFactory)->Create(*(void**)(&languageTag), &result));
        return llm::OS::Data::Text::TextReverseConversionGenerator{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(uint32_t) consume_Windows_Data_Text_IUnicodeCharactersStatics<D>::GetCodepointFromSurrogatePair(uint32_t highSurrogate, uint32_t lowSurrogate) const
    {
        uint32_t codepoint{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IUnicodeCharactersStatics)->GetCodepointFromSurrogatePair(highSurrogate, lowSurrogate, &codepoint));
        return codepoint;
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_Data_Text_IUnicodeCharactersStatics<D>::GetSurrogatePairFromCodepoint(uint32_t codepoint, char16_t& highSurrogate, char16_t& lowSurrogate) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IUnicodeCharactersStatics)->GetSurrogatePairFromCodepoint(codepoint, &highSurrogate, &lowSurrogate));
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_Data_Text_IUnicodeCharactersStatics<D>::IsHighSurrogate(uint32_t codepoint) const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IUnicodeCharactersStatics)->IsHighSurrogate(codepoint, &value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_Data_Text_IUnicodeCharactersStatics<D>::IsLowSurrogate(uint32_t codepoint) const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IUnicodeCharactersStatics)->IsLowSurrogate(codepoint, &value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_Data_Text_IUnicodeCharactersStatics<D>::IsSupplementary(uint32_t codepoint) const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IUnicodeCharactersStatics)->IsSupplementary(codepoint, &value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_Data_Text_IUnicodeCharactersStatics<D>::IsNoncharacter(uint32_t codepoint) const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IUnicodeCharactersStatics)->IsNoncharacter(codepoint, &value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_Data_Text_IUnicodeCharactersStatics<D>::IsWhitespace(uint32_t codepoint) const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IUnicodeCharactersStatics)->IsWhitespace(codepoint, &value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_Data_Text_IUnicodeCharactersStatics<D>::IsAlphabetic(uint32_t codepoint) const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IUnicodeCharactersStatics)->IsAlphabetic(codepoint, &value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_Data_Text_IUnicodeCharactersStatics<D>::IsCased(uint32_t codepoint) const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IUnicodeCharactersStatics)->IsCased(codepoint, &value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_Data_Text_IUnicodeCharactersStatics<D>::IsUppercase(uint32_t codepoint) const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IUnicodeCharactersStatics)->IsUppercase(codepoint, &value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_Data_Text_IUnicodeCharactersStatics<D>::IsLowercase(uint32_t codepoint) const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IUnicodeCharactersStatics)->IsLowercase(codepoint, &value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_Data_Text_IUnicodeCharactersStatics<D>::IsIdStart(uint32_t codepoint) const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IUnicodeCharactersStatics)->IsIdStart(codepoint, &value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_Data_Text_IUnicodeCharactersStatics<D>::IsIdContinue(uint32_t codepoint) const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IUnicodeCharactersStatics)->IsIdContinue(codepoint, &value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_Data_Text_IUnicodeCharactersStatics<D>::IsGraphemeBase(uint32_t codepoint) const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IUnicodeCharactersStatics)->IsGraphemeBase(codepoint, &value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(bool) consume_Windows_Data_Text_IUnicodeCharactersStatics<D>::IsGraphemeExtend(uint32_t codepoint) const
    {
        bool value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IUnicodeCharactersStatics)->IsGraphemeExtend(codepoint, &value));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Data::Text::UnicodeNumericType) consume_Windows_Data_Text_IUnicodeCharactersStatics<D>::GetNumericType(uint32_t codepoint) const
    {
        llm::OS::Data::Text::UnicodeNumericType value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IUnicodeCharactersStatics)->GetNumericType(codepoint, reinterpret_cast<int32_t*>(&value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Data::Text::UnicodeGeneralCategory) consume_Windows_Data_Text_IUnicodeCharactersStatics<D>::GetGeneralCategory(uint32_t codepoint) const
    {
        llm::OS::Data::Text::UnicodeGeneralCategory value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IUnicodeCharactersStatics)->GetGeneralCategory(codepoint, reinterpret_cast<int32_t*>(&value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_Data_Text_IWordSegment<D>::Text() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IWordSegment)->get_Text(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Data::Text::TextSegment) consume_Windows_Data_Text_IWordSegment<D>::SourceTextSegment() const
    {
        llm::OS::Data::Text::TextSegment value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IWordSegment)->get_SourceTextSegment(put_abi(value)));
        return value;
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Collections::IVectorView<llm::OS::Data::Text::AlternateWordForm>) consume_Windows_Data_Text_IWordSegment<D>::AlternateForms() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IWordSegment)->get_AlternateForms(&value));
        return llm::OS::Foundation::Collections::IVectorView<llm::OS::Data::Text::AlternateWordForm>{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(hstring) consume_Windows_Data_Text_IWordsSegmenter<D>::ResolvedLanguage() const
    {
        void* value{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IWordsSegmenter)->get_ResolvedLanguage(&value));
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Data::Text::WordSegment) consume_Windows_Data_Text_IWordsSegmenter<D>::GetTokenAt(param::hstring const& text, uint32_t startIndex) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IWordsSegmenter)->GetTokenAt(*(void**)(&text), startIndex, &result));
        return llm::OS::Data::Text::WordSegment{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Foundation::Collections::IVectorView<llm::OS::Data::Text::WordSegment>) consume_Windows_Data_Text_IWordsSegmenter<D>::GetTokens(param::hstring const& text) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IWordsSegmenter)->GetTokens(*(void**)(&text), &result));
        return llm::OS::Foundation::Collections::IVectorView<llm::OS::Data::Text::WordSegment>{ result, take_ownership_from_abi };
    }
    template <typename D> LLM_IMPL_AUTO(void) consume_Windows_Data_Text_IWordsSegmenter<D>::Tokenize(param::hstring const& text, uint32_t startIndex, llm::OS::Data::Text::WordSegmentsTokenizingHandler const& handler) const
    {
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IWordsSegmenter)->Tokenize(*(void**)(&text), startIndex, *(void**)(&handler)));
    }
    template <typename D> LLM_IMPL_AUTO(llm::OS::Data::Text::WordsSegmenter) consume_Windows_Data_Text_IWordsSegmenterFactory<D>::CreateWithLanguage(param::hstring const& language) const
    {
        void* result{};
        check_hresult(LLM_IMPL_SHIM(llm::OS::Data::Text::IWordsSegmenterFactory)->CreateWithLanguage(*(void**)(&language), &result));
        return llm::OS::Data::Text::WordsSegmenter{ result, take_ownership_from_abi };
    }
    template <typename H> struct delegate<llm::OS::Data::Text::SelectableWordSegmentsTokenizingHandler, H> final : implements_delegate<llm::OS::Data::Text::SelectableWordSegmentsTokenizingHandler, H>
    {
        delegate(H&& handler) : implements_delegate<llm::OS::Data::Text::SelectableWordSegmentsTokenizingHandler, H>(std::forward<H>(handler)) {}

        int32_t __stdcall Invoke(void* precedingWords, void* words) noexcept final try
        {
            (*this)(*reinterpret_cast<llm::OS::Foundation::Collections::IIterable<llm::OS::Data::Text::SelectableWordSegment> const*>(&precedingWords), *reinterpret_cast<llm::OS::Foundation::Collections::IIterable<llm::OS::Data::Text::SelectableWordSegment> const*>(&words));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
    template <typename H> struct delegate<llm::OS::Data::Text::WordSegmentsTokenizingHandler, H> final : implements_delegate<llm::OS::Data::Text::WordSegmentsTokenizingHandler, H>
    {
        delegate(H&& handler) : implements_delegate<llm::OS::Data::Text::WordSegmentsTokenizingHandler, H>(std::forward<H>(handler)) {}

        int32_t __stdcall Invoke(void* precedingWords, void* words) noexcept final try
        {
            (*this)(*reinterpret_cast<llm::OS::Foundation::Collections::IIterable<llm::OS::Data::Text::WordSegment> const*>(&precedingWords), *reinterpret_cast<llm::OS::Foundation::Collections::IIterable<llm::OS::Data::Text::WordSegment> const*>(&words));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Data::Text::IAlternateWordForm> : produce_base<D, llm::OS::Data::Text::IAlternateWordForm>
    {
        int32_t __stdcall get_SourceTextSegment(struct struct_Windows_Data_Text_TextSegment* value) noexcept final try
        {
            zero_abi<llm::OS::Data::Text::TextSegment>(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Data::Text::TextSegment>(this->shim().SourceTextSegment());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_AlternateText(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().AlternateText());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_NormalizationFormat(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Data::Text::AlternateNormalizationFormat>(this->shim().NormalizationFormat());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Data::Text::ISelectableWordSegment> : produce_base<D, llm::OS::Data::Text::ISelectableWordSegment>
    {
        int32_t __stdcall get_Text(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().Text());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_SourceTextSegment(struct struct_Windows_Data_Text_TextSegment* value) noexcept final try
        {
            zero_abi<llm::OS::Data::Text::TextSegment>(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Data::Text::TextSegment>(this->shim().SourceTextSegment());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Data::Text::ISelectableWordsSegmenter> : produce_base<D, llm::OS::Data::Text::ISelectableWordsSegmenter>
    {
        int32_t __stdcall get_ResolvedLanguage(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().ResolvedLanguage());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetTokenAt(void* text, uint32_t startIndex, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Data::Text::SelectableWordSegment>(this->shim().GetTokenAt(*reinterpret_cast<hstring const*>(&text), startIndex));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetTokens(void* text, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::Collections::IVectorView<llm::OS::Data::Text::SelectableWordSegment>>(this->shim().GetTokens(*reinterpret_cast<hstring const*>(&text)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall Tokenize(void* text, uint32_t startIndex, void* handler) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Tokenize(*reinterpret_cast<hstring const*>(&text), startIndex, *reinterpret_cast<llm::OS::Data::Text::SelectableWordSegmentsTokenizingHandler const*>(&handler));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Data::Text::ISelectableWordsSegmenterFactory> : produce_base<D, llm::OS::Data::Text::ISelectableWordsSegmenterFactory>
    {
        int32_t __stdcall CreateWithLanguage(void* language, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Data::Text::SelectableWordsSegmenter>(this->shim().CreateWithLanguage(*reinterpret_cast<hstring const*>(&language)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Data::Text::ISemanticTextQuery> : produce_base<D, llm::OS::Data::Text::ISemanticTextQuery>
    {
        int32_t __stdcall Find(void* content, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::Collections::IVectorView<llm::OS::Data::Text::TextSegment>>(this->shim().Find(*reinterpret_cast<hstring const*>(&content)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall FindInProperty(void* propertyContent, void* propertyName, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::Collections::IVectorView<llm::OS::Data::Text::TextSegment>>(this->shim().FindInProperty(*reinterpret_cast<hstring const*>(&propertyContent), *reinterpret_cast<hstring const*>(&propertyName)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Data::Text::ISemanticTextQueryFactory> : produce_base<D, llm::OS::Data::Text::ISemanticTextQueryFactory>
    {
        int32_t __stdcall Create(void* aqsFilter, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Data::Text::SemanticTextQuery>(this->shim().Create(*reinterpret_cast<hstring const*>(&aqsFilter)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall CreateWithLanguage(void* aqsFilter, void* filterLanguage, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Data::Text::SemanticTextQuery>(this->shim().CreateWithLanguage(*reinterpret_cast<hstring const*>(&aqsFilter), *reinterpret_cast<hstring const*>(&filterLanguage)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Data::Text::ITextConversionGenerator> : produce_base<D, llm::OS::Data::Text::ITextConversionGenerator>
    {
        int32_t __stdcall get_ResolvedLanguage(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().ResolvedLanguage());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_LanguageAvailableButNotInstalled(bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().LanguageAvailableButNotInstalled());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetCandidatesAsync(void* input, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<hstring>>>(this->shim().GetCandidatesAsync(*reinterpret_cast<hstring const*>(&input)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetCandidatesWithMaxCountAsync(void* input, uint32_t maxCandidates, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<hstring>>>(this->shim().GetCandidatesAsync(*reinterpret_cast<hstring const*>(&input), maxCandidates));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Data::Text::ITextConversionGeneratorFactory> : produce_base<D, llm::OS::Data::Text::ITextConversionGeneratorFactory>
    {
        int32_t __stdcall Create(void* languageTag, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Data::Text::TextConversionGenerator>(this->shim().Create(*reinterpret_cast<hstring const*>(&languageTag)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Data::Text::ITextPhoneme> : produce_base<D, llm::OS::Data::Text::ITextPhoneme>
    {
        int32_t __stdcall get_DisplayText(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().DisplayText());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_ReadingText(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().ReadingText());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Data::Text::ITextPredictionGenerator> : produce_base<D, llm::OS::Data::Text::ITextPredictionGenerator>
    {
        int32_t __stdcall get_ResolvedLanguage(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().ResolvedLanguage());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_LanguageAvailableButNotInstalled(bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().LanguageAvailableButNotInstalled());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetCandidatesAsync(void* input, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<hstring>>>(this->shim().GetCandidatesAsync(*reinterpret_cast<hstring const*>(&input)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetCandidatesWithMaxCountAsync(void* input, uint32_t maxCandidates, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<hstring>>>(this->shim().GetCandidatesAsync(*reinterpret_cast<hstring const*>(&input), maxCandidates));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Data::Text::ITextPredictionGenerator2> : produce_base<D, llm::OS::Data::Text::ITextPredictionGenerator2>
    {
        int32_t __stdcall GetCandidatesWithParametersAsync(void* input, uint32_t maxCandidates, uint32_t predictionOptions, void* previousStrings, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<hstring>>>(this->shim().GetCandidatesAsync(*reinterpret_cast<hstring const*>(&input), maxCandidates, *reinterpret_cast<llm::OS::Data::Text::TextPredictionOptions const*>(&predictionOptions), *reinterpret_cast<llm::OS::Foundation::Collections::IIterable<hstring> const*>(&previousStrings)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetNextWordCandidatesAsync(uint32_t maxCandidates, void* previousStrings, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<hstring>>>(this->shim().GetNextWordCandidatesAsync(maxCandidates, *reinterpret_cast<llm::OS::Foundation::Collections::IIterable<hstring> const*>(&previousStrings)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_InputScope(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::UI::Text::Core::CoreTextInputScope>(this->shim().InputScope());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall put_InputScope(int32_t value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().InputScope(*reinterpret_cast<llm::OS::UI::Text::Core::CoreTextInputScope const*>(&value));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Data::Text::ITextPredictionGeneratorFactory> : produce_base<D, llm::OS::Data::Text::ITextPredictionGeneratorFactory>
    {
        int32_t __stdcall Create(void* languageTag, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Data::Text::TextPredictionGenerator>(this->shim().Create(*reinterpret_cast<hstring const*>(&languageTag)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Data::Text::ITextReverseConversionGenerator> : produce_base<D, llm::OS::Data::Text::ITextReverseConversionGenerator>
    {
        int32_t __stdcall get_ResolvedLanguage(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().ResolvedLanguage());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_LanguageAvailableButNotInstalled(bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().LanguageAvailableButNotInstalled());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall ConvertBackAsync(void* input, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncOperation<hstring>>(this->shim().ConvertBackAsync(*reinterpret_cast<hstring const*>(&input)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Data::Text::ITextReverseConversionGenerator2> : produce_base<D, llm::OS::Data::Text::ITextReverseConversionGenerator2>
    {
        int32_t __stdcall GetPhonemesAsync(void* input, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::IAsyncOperation<llm::OS::Foundation::Collections::IVectorView<llm::OS::Data::Text::TextPhoneme>>>(this->shim().GetPhonemesAsync(*reinterpret_cast<hstring const*>(&input)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Data::Text::ITextReverseConversionGeneratorFactory> : produce_base<D, llm::OS::Data::Text::ITextReverseConversionGeneratorFactory>
    {
        int32_t __stdcall Create(void* languageTag, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Data::Text::TextReverseConversionGenerator>(this->shim().Create(*reinterpret_cast<hstring const*>(&languageTag)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Data::Text::IUnicodeCharactersStatics> : produce_base<D, llm::OS::Data::Text::IUnicodeCharactersStatics>
    {
        int32_t __stdcall GetCodepointFromSurrogatePair(uint32_t highSurrogate, uint32_t lowSurrogate, uint32_t* codepoint) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *codepoint = detach_from<uint32_t>(this->shim().GetCodepointFromSurrogatePair(highSurrogate, lowSurrogate));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetSurrogatePairFromCodepoint(uint32_t codepoint, char16_t* highSurrogate, char16_t* lowSurrogate) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().GetSurrogatePairFromCodepoint(codepoint, *highSurrogate, *lowSurrogate);
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall IsHighSurrogate(uint32_t codepoint, bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().IsHighSurrogate(codepoint));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall IsLowSurrogate(uint32_t codepoint, bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().IsLowSurrogate(codepoint));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall IsSupplementary(uint32_t codepoint, bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().IsSupplementary(codepoint));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall IsNoncharacter(uint32_t codepoint, bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().IsNoncharacter(codepoint));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall IsWhitespace(uint32_t codepoint, bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().IsWhitespace(codepoint));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall IsAlphabetic(uint32_t codepoint, bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().IsAlphabetic(codepoint));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall IsCased(uint32_t codepoint, bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().IsCased(codepoint));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall IsUppercase(uint32_t codepoint, bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().IsUppercase(codepoint));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall IsLowercase(uint32_t codepoint, bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().IsLowercase(codepoint));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall IsIdStart(uint32_t codepoint, bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().IsIdStart(codepoint));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall IsIdContinue(uint32_t codepoint, bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().IsIdContinue(codepoint));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall IsGraphemeBase(uint32_t codepoint, bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().IsGraphemeBase(codepoint));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall IsGraphemeExtend(uint32_t codepoint, bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().IsGraphemeExtend(codepoint));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetNumericType(uint32_t codepoint, int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Data::Text::UnicodeNumericType>(this->shim().GetNumericType(codepoint));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetGeneralCategory(uint32_t codepoint, int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Data::Text::UnicodeGeneralCategory>(this->shim().GetGeneralCategory(codepoint));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Data::Text::IWordSegment> : produce_base<D, llm::OS::Data::Text::IWordSegment>
    {
        int32_t __stdcall get_Text(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().Text());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_SourceTextSegment(struct struct_Windows_Data_Text_TextSegment* value) noexcept final try
        {
            zero_abi<llm::OS::Data::Text::TextSegment>(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Data::Text::TextSegment>(this->shim().SourceTextSegment());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_AlternateForms(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<llm::OS::Foundation::Collections::IVectorView<llm::OS::Data::Text::AlternateWordForm>>(this->shim().AlternateForms());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Data::Text::IWordsSegmenter> : produce_base<D, llm::OS::Data::Text::IWordsSegmenter>
    {
        int32_t __stdcall get_ResolvedLanguage(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().ResolvedLanguage());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetTokenAt(void* text, uint32_t startIndex, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Data::Text::WordSegment>(this->shim().GetTokenAt(*reinterpret_cast<hstring const*>(&text), startIndex));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetTokens(void* text, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Foundation::Collections::IVectorView<llm::OS::Data::Text::WordSegment>>(this->shim().GetTokens(*reinterpret_cast<hstring const*>(&text)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall Tokenize(void* text, uint32_t startIndex, void* handler) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Tokenize(*reinterpret_cast<hstring const*>(&text), startIndex, *reinterpret_cast<llm::OS::Data::Text::WordSegmentsTokenizingHandler const*>(&handler));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef LLM_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, llm::OS::Data::Text::IWordsSegmenterFactory> : produce_base<D, llm::OS::Data::Text::IWordsSegmenterFactory>
    {
        int32_t __stdcall CreateWithLanguage(void* language, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<llm::OS::Data::Text::WordsSegmenter>(this->shim().CreateWithLanguage(*reinterpret_cast<hstring const*>(&language)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
}
LLM_EXPORT namespace llm::OS::Data::Text
{
    constexpr auto operator|(TextPredictionOptions const left, TextPredictionOptions const right) noexcept
    {
        return static_cast<TextPredictionOptions>(impl::to_underlying_type(left) | impl::to_underlying_type(right));
    }
    constexpr auto operator|=(TextPredictionOptions& left, TextPredictionOptions const right) noexcept
    {
        left = left | right;
        return left;
    }
    constexpr auto operator&(TextPredictionOptions const left, TextPredictionOptions const right) noexcept
    {
        return static_cast<TextPredictionOptions>(impl::to_underlying_type(left) & impl::to_underlying_type(right));
    }
    constexpr auto operator&=(TextPredictionOptions& left, TextPredictionOptions const right) noexcept
    {
        left = left & right;
        return left;
    }
    constexpr auto operator~(TextPredictionOptions const value) noexcept
    {
        return static_cast<TextPredictionOptions>(~impl::to_underlying_type(value));
    }
    constexpr auto operator^(TextPredictionOptions const left, TextPredictionOptions const right) noexcept
    {
        return static_cast<TextPredictionOptions>(impl::to_underlying_type(left) ^ impl::to_underlying_type(right));
    }
    constexpr auto operator^=(TextPredictionOptions& left, TextPredictionOptions const right) noexcept
    {
        left = left ^ right;
        return left;
    }
    inline SelectableWordsSegmenter::SelectableWordsSegmenter(param::hstring const& language) :
        SelectableWordsSegmenter(impl::call_factory<SelectableWordsSegmenter, ISelectableWordsSegmenterFactory>([&](ISelectableWordsSegmenterFactory const& f) { return f.CreateWithLanguage(language); }))
    {
    }
    inline SemanticTextQuery::SemanticTextQuery(param::hstring const& aqsFilter) :
        SemanticTextQuery(impl::call_factory<SemanticTextQuery, ISemanticTextQueryFactory>([&](ISemanticTextQueryFactory const& f) { return f.Create(aqsFilter); }))
    {
    }
    inline SemanticTextQuery::SemanticTextQuery(param::hstring const& aqsFilter, param::hstring const& filterLanguage) :
        SemanticTextQuery(impl::call_factory<SemanticTextQuery, ISemanticTextQueryFactory>([&](ISemanticTextQueryFactory const& f) { return f.CreateWithLanguage(aqsFilter, filterLanguage); }))
    {
    }
    inline TextConversionGenerator::TextConversionGenerator(param::hstring const& languageTag) :
        TextConversionGenerator(impl::call_factory<TextConversionGenerator, ITextConversionGeneratorFactory>([&](ITextConversionGeneratorFactory const& f) { return f.Create(languageTag); }))
    {
    }
    inline TextPredictionGenerator::TextPredictionGenerator(param::hstring const& languageTag) :
        TextPredictionGenerator(impl::call_factory<TextPredictionGenerator, ITextPredictionGeneratorFactory>([&](ITextPredictionGeneratorFactory const& f) { return f.Create(languageTag); }))
    {
    }
    inline TextReverseConversionGenerator::TextReverseConversionGenerator(param::hstring const& languageTag) :
        TextReverseConversionGenerator(impl::call_factory<TextReverseConversionGenerator, ITextReverseConversionGeneratorFactory>([&](ITextReverseConversionGeneratorFactory const& f) { return f.Create(languageTag); }))
    {
    }
    inline auto UnicodeCharacters::GetCodepointFromSurrogatePair(uint32_t highSurrogate, uint32_t lowSurrogate)
    {
        return impl::call_factory<UnicodeCharacters, IUnicodeCharactersStatics>([&](IUnicodeCharactersStatics const& f) { return f.GetCodepointFromSurrogatePair(highSurrogate, lowSurrogate); });
    }
    inline auto UnicodeCharacters::GetSurrogatePairFromCodepoint(uint32_t codepoint, char16_t& highSurrogate, char16_t& lowSurrogate)
    {
        impl::call_factory<UnicodeCharacters, IUnicodeCharactersStatics>([&](IUnicodeCharactersStatics const& f) { return f.GetSurrogatePairFromCodepoint(codepoint, highSurrogate, lowSurrogate); });
    }
    inline auto UnicodeCharacters::IsHighSurrogate(uint32_t codepoint)
    {
        return impl::call_factory<UnicodeCharacters, IUnicodeCharactersStatics>([&](IUnicodeCharactersStatics const& f) { return f.IsHighSurrogate(codepoint); });
    }
    inline auto UnicodeCharacters::IsLowSurrogate(uint32_t codepoint)
    {
        return impl::call_factory<UnicodeCharacters, IUnicodeCharactersStatics>([&](IUnicodeCharactersStatics const& f) { return f.IsLowSurrogate(codepoint); });
    }
    inline auto UnicodeCharacters::IsSupplementary(uint32_t codepoint)
    {
        return impl::call_factory<UnicodeCharacters, IUnicodeCharactersStatics>([&](IUnicodeCharactersStatics const& f) { return f.IsSupplementary(codepoint); });
    }
    inline auto UnicodeCharacters::IsNoncharacter(uint32_t codepoint)
    {
        return impl::call_factory<UnicodeCharacters, IUnicodeCharactersStatics>([&](IUnicodeCharactersStatics const& f) { return f.IsNoncharacter(codepoint); });
    }
    inline auto UnicodeCharacters::IsWhitespace(uint32_t codepoint)
    {
        return impl::call_factory<UnicodeCharacters, IUnicodeCharactersStatics>([&](IUnicodeCharactersStatics const& f) { return f.IsWhitespace(codepoint); });
    }
    inline auto UnicodeCharacters::IsAlphabetic(uint32_t codepoint)
    {
        return impl::call_factory<UnicodeCharacters, IUnicodeCharactersStatics>([&](IUnicodeCharactersStatics const& f) { return f.IsAlphabetic(codepoint); });
    }
    inline auto UnicodeCharacters::IsCased(uint32_t codepoint)
    {
        return impl::call_factory<UnicodeCharacters, IUnicodeCharactersStatics>([&](IUnicodeCharactersStatics const& f) { return f.IsCased(codepoint); });
    }
    inline auto UnicodeCharacters::IsUppercase(uint32_t codepoint)
    {
        return impl::call_factory<UnicodeCharacters, IUnicodeCharactersStatics>([&](IUnicodeCharactersStatics const& f) { return f.IsUppercase(codepoint); });
    }
    inline auto UnicodeCharacters::IsLowercase(uint32_t codepoint)
    {
        return impl::call_factory<UnicodeCharacters, IUnicodeCharactersStatics>([&](IUnicodeCharactersStatics const& f) { return f.IsLowercase(codepoint); });
    }
    inline auto UnicodeCharacters::IsIdStart(uint32_t codepoint)
    {
        return impl::call_factory<UnicodeCharacters, IUnicodeCharactersStatics>([&](IUnicodeCharactersStatics const& f) { return f.IsIdStart(codepoint); });
    }
    inline auto UnicodeCharacters::IsIdContinue(uint32_t codepoint)
    {
        return impl::call_factory<UnicodeCharacters, IUnicodeCharactersStatics>([&](IUnicodeCharactersStatics const& f) { return f.IsIdContinue(codepoint); });
    }
    inline auto UnicodeCharacters::IsGraphemeBase(uint32_t codepoint)
    {
        return impl::call_factory<UnicodeCharacters, IUnicodeCharactersStatics>([&](IUnicodeCharactersStatics const& f) { return f.IsGraphemeBase(codepoint); });
    }
    inline auto UnicodeCharacters::IsGraphemeExtend(uint32_t codepoint)
    {
        return impl::call_factory<UnicodeCharacters, IUnicodeCharactersStatics>([&](IUnicodeCharactersStatics const& f) { return f.IsGraphemeExtend(codepoint); });
    }
    inline auto UnicodeCharacters::GetNumericType(uint32_t codepoint)
    {
        return impl::call_factory<UnicodeCharacters, IUnicodeCharactersStatics>([&](IUnicodeCharactersStatics const& f) { return f.GetNumericType(codepoint); });
    }
    inline auto UnicodeCharacters::GetGeneralCategory(uint32_t codepoint)
    {
        return impl::call_factory<UnicodeCharacters, IUnicodeCharactersStatics>([&](IUnicodeCharactersStatics const& f) { return f.GetGeneralCategory(codepoint); });
    }
    inline WordsSegmenter::WordsSegmenter(param::hstring const& language) :
        WordsSegmenter(impl::call_factory<WordsSegmenter, IWordsSegmenterFactory>([&](IWordsSegmenterFactory const& f) { return f.CreateWithLanguage(language); }))
    {
    }
    template <typename L> SelectableWordSegmentsTokenizingHandler::SelectableWordSegmentsTokenizingHandler(L handler) :
        SelectableWordSegmentsTokenizingHandler(impl::make_delegate<SelectableWordSegmentsTokenizingHandler>(std::forward<L>(handler)))
    {
    }
    template <typename F> SelectableWordSegmentsTokenizingHandler::SelectableWordSegmentsTokenizingHandler(F* handler) :
        SelectableWordSegmentsTokenizingHandler([=](auto&&... args) { return handler(args...); })
    {
    }
    template <typename O, typename M> SelectableWordSegmentsTokenizingHandler::SelectableWordSegmentsTokenizingHandler(O* object, M method) :
        SelectableWordSegmentsTokenizingHandler([=](auto&&... args) { return ((*object).*(method))(args...); })
    {
    }
    template <typename O, typename M> SelectableWordSegmentsTokenizingHandler::SelectableWordSegmentsTokenizingHandler(com_ptr<O>&& object, M method) :
        SelectableWordSegmentsTokenizingHandler([o = std::move(object), method](auto&&... args) { return ((*o).*(method))(args...); })
    {
    }
    template <typename O, typename M> SelectableWordSegmentsTokenizingHandler::SelectableWordSegmentsTokenizingHandler(weak_ref<O>&& object, M method) :
        SelectableWordSegmentsTokenizingHandler([o = std::move(object), method](auto&&... args) { if (auto s = o.get()) { ((*s).*(method))(args...); } })
    {
    }
    inline auto SelectableWordSegmentsTokenizingHandler::operator()(param::iterable<llm::OS::Data::Text::SelectableWordSegment> const& precedingWords, param::iterable<llm::OS::Data::Text::SelectableWordSegment> const& words) const
    {
        check_hresult((*(impl::abi_t<SelectableWordSegmentsTokenizingHandler>**)this)->Invoke(*(void**)(&precedingWords), *(void**)(&words)));
    }
    template <typename L> WordSegmentsTokenizingHandler::WordSegmentsTokenizingHandler(L handler) :
        WordSegmentsTokenizingHandler(impl::make_delegate<WordSegmentsTokenizingHandler>(std::forward<L>(handler)))
    {
    }
    template <typename F> WordSegmentsTokenizingHandler::WordSegmentsTokenizingHandler(F* handler) :
        WordSegmentsTokenizingHandler([=](auto&&... args) { return handler(args...); })
    {
    }
    template <typename O, typename M> WordSegmentsTokenizingHandler::WordSegmentsTokenizingHandler(O* object, M method) :
        WordSegmentsTokenizingHandler([=](auto&&... args) { return ((*object).*(method))(args...); })
    {
    }
    template <typename O, typename M> WordSegmentsTokenizingHandler::WordSegmentsTokenizingHandler(com_ptr<O>&& object, M method) :
        WordSegmentsTokenizingHandler([o = std::move(object), method](auto&&... args) { return ((*o).*(method))(args...); })
    {
    }
    template <typename O, typename M> WordSegmentsTokenizingHandler::WordSegmentsTokenizingHandler(weak_ref<O>&& object, M method) :
        WordSegmentsTokenizingHandler([o = std::move(object), method](auto&&... args) { if (auto s = o.get()) { ((*s).*(method))(args...); } })
    {
    }
    inline auto WordSegmentsTokenizingHandler::operator()(param::iterable<llm::OS::Data::Text::WordSegment> const& precedingWords, param::iterable<llm::OS::Data::Text::WordSegment> const& words) const
    {
        check_hresult((*(impl::abi_t<WordSegmentsTokenizingHandler>**)this)->Invoke(*(void**)(&precedingWords), *(void**)(&words)));
    }
}
namespace std
{
#ifndef LLM_LEAN_AND_MEAN
    template<> struct hash<llm::OS::Data::Text::IAlternateWordForm> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::ISelectableWordSegment> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::ISelectableWordsSegmenter> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::ISelectableWordsSegmenterFactory> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::ISemanticTextQuery> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::ISemanticTextQueryFactory> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::ITextConversionGenerator> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::ITextConversionGeneratorFactory> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::ITextPhoneme> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::ITextPredictionGenerator> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::ITextPredictionGenerator2> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::ITextPredictionGeneratorFactory> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::ITextReverseConversionGenerator> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::ITextReverseConversionGenerator2> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::ITextReverseConversionGeneratorFactory> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::IUnicodeCharactersStatics> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::IWordSegment> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::IWordsSegmenter> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::IWordsSegmenterFactory> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::AlternateWordForm> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::SelectableWordSegment> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::SelectableWordsSegmenter> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::SemanticTextQuery> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::TextConversionGenerator> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::TextPhoneme> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::TextPredictionGenerator> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::TextReverseConversionGenerator> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::UnicodeCharacters> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::WordSegment> : llm::impl::hash_base {};
    template<> struct hash<llm::OS::Data::Text::WordsSegmenter> : llm::impl::hash_base {};
#endif
#ifdef __cpp_lib_format
#endif
}
#endif
