// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#pragma once
#include <wrl.h>
#include <wrl/wrappers/corewrappers.h>
#include <string>

#include "AdaptiveCards.ObjectModel.Uwp.h"
#include <BaseCardElement.h>
#include <BaseActionElement.h>
#include <ChoiceInput.h>
#include <Column.h>
#include <Fact.h>
#include <Image.h>
#include <Inline.h>
#include <MediaSource.h>
#include <ToggleVisibilityTarget.h>
#include <windows.foundation.collections.h>
#include <ParseContext.h>
#include "AdaptiveCardParseWarning.h"
#include "RemoteResourceInformation.h"

class bad_string_conversion : public std::exception
{
public:
    bad_string_conversion() : _dwErr(GetLastError()) {}

private:
    DWORD _dwErr;
};

HRESULT WStringToHString(std::wstring_view in, _Outptr_ HSTRING* out) noexcept;

std::string WStringToString(std::wstring_view in);
std::wstring StringToWString(std::string_view in);

// This function is needed to deal with the fact that non-windows platforms handle Unicode without the need for wchar_t.
// (which has a platform specific implementation) It converts a std::string to an HSTRING.
HRESULT UTF8ToHString(std::string_view in, _Outptr_ HSTRING* out) noexcept;

// This function is needed to deal with the fact that non-windows platforms handle Unicode without the need for wchar_t.
// (which has a platform specific implementation) It converts from HSTRING to a standard std::string.
HRESULT HStringToUTF8(HSTRING in, std::string& out) noexcept;

std::string HStringToUTF8(HSTRING in);

inline bool Boolify(const boolean value) noexcept
{
    return (value > 0);
}

HRESULT GetColorFromString(const std::string& colorString, _Out_ ABI::Windows::UI::Color* color) noexcept;

HRESULT GenerateSharedElement(_In_ ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveCardElement* items,
                              std::shared_ptr<AdaptiveCards::BaseCardElement>& containedElement);

HRESULT GenerateSharedElements(_In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveCardElement*>* items,
                               std::vector<std::shared_ptr<AdaptiveCards::BaseCardElement>>& containedElements);

HRESULT GenerateSharedAction(_In_ ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveActionElement* action,
                             std::shared_ptr<AdaptiveCards::BaseActionElement>& sharedAction);

HRESULT GenerateSharedActions(_In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveActionElement*>* items,
                              std::vector<std::shared_ptr<AdaptiveCards::BaseActionElement>>& containedElements);

HRESULT GenerateSharedRequirements(
    _In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::AdaptiveRequirement*>* adaptiveRequirements,
    std::unordered_map<std::string, AdaptiveCards::SemanticVersion>& sharedRequirements) noexcept;

HRESULT GenerateSharedImages(_In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::AdaptiveImage*>* items,
                             std::vector<std::shared_ptr<AdaptiveCards::Image>>& containedElements);

HRESULT GenerateSharedFacts(_In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::AdaptiveFact*>* items,
                            std::vector<std::shared_ptr<AdaptiveCards::Fact>>& containedElements);

HRESULT GenerateSharedChoices(_In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::AdaptiveChoiceInput*>* items,
                              std::vector<std::shared_ptr<AdaptiveCards::ChoiceInput>>& containedElements);

HRESULT GenerateSharedMediaSources(
    _In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::AdaptiveMediaSource*>* items,
    std::vector<std::shared_ptr<AdaptiveCards::MediaSource>>& containedElements);

HRESULT GenerateSharedInlines(_In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveInline*>* items,
                              std::vector<std::shared_ptr<AdaptiveCards::Inline>>& containedElements);

HRESULT GenerateSharedToggleElements(
    _In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::AdaptiveToggleVisibilityTarget*>* items,
    std::vector<std::shared_ptr<AdaptiveCards::ToggleVisibilityTarget>>& containedElements);

HRESULT GenerateElementProjection(_In_ const std::shared_ptr<AdaptiveCards::BaseCardElement>& baseElement,
                                  _COM_Outptr_ ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveCardElement** projectedElement) noexcept;

HRESULT GenerateContainedElementsProjection(
    const std::vector<std::shared_ptr<AdaptiveCards::BaseCardElement>>& containedElements,
    _In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveCardElement*>* projectedParentContainer) noexcept;

HRESULT GenerateActionsProjection(
    const std::vector<std::shared_ptr<AdaptiveCards::BaseActionElement>>& actions,
    _In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveActionElement*>* projectedParentContainer) noexcept;

HRESULT GenerateActionProjection(const std::shared_ptr<AdaptiveCards::BaseActionElement>& action,
                                 _COM_Outptr_ ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveActionElement** projectedAction) noexcept;

HRESULT GenerateColumnsProjection(
    const std::vector<std::shared_ptr<AdaptiveCards::Column>>& containedElements,
    _In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::AdaptiveColumn*>* projectedParentContainer) noexcept;

HRESULT GenerateFactsProjection(
    const std::vector<std::shared_ptr<AdaptiveCards::Fact>>& containedElements,
    _In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::AdaptiveFact*>* projectedParentContainer) noexcept;

HRESULT GenerateInlinesProjection(
    const std::vector<std::shared_ptr<AdaptiveCards::Inline>>& containedElements,
    _In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveInline*>* projectedParentContainer) noexcept;

HRESULT GenerateRequirementsProjection(
    const std::unordered_map<std::string, AdaptiveCards::SemanticVersion>& sharedRequirements,
    _In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::AdaptiveRequirement*>* projectedRequirementVector) noexcept;

HRESULT GenerateImagesProjection(
    const std::vector<std::shared_ptr<AdaptiveCards::Image>>& containedElements,
    _In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::AdaptiveImage*>* projectedParentContainer) noexcept;

HRESULT GenerateInputChoicesProjection(
    const std::vector<std::shared_ptr<AdaptiveCards::ChoiceInput>>& containedElements,
    _In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::AdaptiveChoiceInput*>* projectedParentContainer) noexcept;

HRESULT GenerateMediaSourcesProjection(
    const std::vector<std::shared_ptr<AdaptiveCards::MediaSource>>& containedElements,
    _In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::AdaptiveMediaSource*>* projectedParentContainer) noexcept;

HRESULT GenerateToggleTargetProjection(
    const std::vector<std::shared_ptr<AdaptiveCards::ToggleVisibilityTarget>>& containedElements,
    _In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::AdaptiveToggleVisibilityTarget*>* projectedParentContainer) noexcept;

HRESULT GenerateSeparatorProjection(const std::shared_ptr<AdaptiveCards::Separator>& sharedSeparator,
                                    _COM_Outptr_ ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveSeparator** projectedSeparator) noexcept;

HRESULT GenerateSharedSeparator(_In_ ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveSeparator* separator,
                                _Out_ std::shared_ptr<AdaptiveCards::Separator>* sharedSeparatorOut) noexcept;

HRESULT StringToJsonObject(const std::string& inputString, _COM_Outptr_ ABI::Windows::Data::Json::IJsonObject** result);
HRESULT HStringToJsonObject(const HSTRING& inputHString, _COM_Outptr_ ABI::Windows::Data::Json::IJsonObject** result);
HRESULT JsonObjectToHString(_In_ ABI::Windows::Data::Json::IJsonObject* inputJson, _Outptr_ HSTRING* result);
HRESULT JsonObjectToString(_In_ ABI::Windows::Data::Json::IJsonObject* inputJson, std::string& result);

HRESULT StringToJsonValue(const std::string inputString, _COM_Outptr_ ABI::Windows::Data::Json::IJsonValue** result);
HRESULT HStringToJsonValue(const HSTRING& inputHString, _COM_Outptr_ ABI::Windows::Data::Json::IJsonValue** result);
HRESULT JsonValueToHString(_In_ ABI::Windows::Data::Json::IJsonValue* inputJsonValue, _Outptr_ HSTRING* result);
HRESULT JsonValueToString(_In_ ABI::Windows::Data::Json::IJsonValue* inputJsonValue, std::string& result);

HRESULT JsonCppToJsonObject(const Json::Value& jsonCppValue, _COM_Outptr_ ABI::Windows::Data::Json::IJsonObject** result);
HRESULT JsonObjectToJsonCpp(_In_ ABI::Windows::Data::Json::IJsonObject* jsonObject, _Out_ Json::Value* jsonCppValue);

HRESULT ProjectedActionTypeToHString(ABI::AdaptiveCards::Rendering::Uwp::ActionType projectedActionType, _Outptr_ HSTRING* result);
HRESULT ProjectedElementTypeToHString(ABI::AdaptiveCards::Rendering::Uwp::ElementType projectedElementType,
                                      _Outptr_ HSTRING* result);

HRESULT MeetsRequirements(_In_ ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveCardElement* cardElement,
                          _In_ ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveFeatureRegistration* featureRegistration,
                          _Out_ bool* meetsRequirements);

HRESULT IsBackgroundImageValid(_In_ ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveBackgroundImage* backgroundImageElement,
                               _Out_ BOOL* isValid);

// Peek interface to help get implementation types from winrt interfaces
struct DECLSPEC_UUID("defc7d5f-b4e5-4a74-80be-d87bd50a2f45") ITypePeek : IInspectable
{
    virtual void* PeekAt(REFIID riid) = 0;
    template<typename Q> void* PeekHelper(REFIID riid, Q* pQ) { return (__uuidof(Q) == riid) ? pQ : nullptr; }

protected:
    virtual ~ITypePeek() {}
};

template<typename T, typename R> Microsoft::WRL::ComPtr<T> PeekInnards(R r)
{
    Microsoft::WRL::ComPtr<T> inner;
    Microsoft::WRL::ComPtr<ITypePeek> peeker;

    if (r && SUCCEEDED(r->QueryInterface(__uuidof(ITypePeek), &peeker)))
    {
        inner = reinterpret_cast<T*>(peeker->PeekAt(__uuidof(T)));
    }
    return inner;
}

void RemoteResourceElementToRemoteResourceInformationVector(_In_ ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveElementWithRemoteResources* remoteResources,
                                                            std::vector<AdaptiveCards::RemoteResourceInformation>& resourceUris);

HRESULT SharedWarningsToAdaptiveWarnings(
    const std::vector<std::shared_ptr<AdaptiveCards::AdaptiveCardParseWarning>>& sharedWarnings,
    _In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::AdaptiveWarning*>* adaptiveWarnings);

HRESULT AdaptiveWarningsToSharedWarnings(
    _In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::AdaptiveWarning*>* adaptiveWarnings,
    std::vector<std::shared_ptr<AdaptiveCards::AdaptiveCardParseWarning>>& sharedWarnings);

ABI::Windows::UI::Color GenerateLHoverColor(const ABI::Windows::UI::Color& originalColor);

ABI::Windows::Foundation::DateTime GetDateTime(unsigned int year, unsigned int month, unsigned int day);

HRESULT GetDateTimeReference(unsigned int year,
                             unsigned int month,
                             unsigned int day,
                             _COM_Outptr_ ABI::Windows::Foundation::IReference<ABI::Windows::Foundation::DateTime>** dateTimeReference);

ABI::AdaptiveCards::Rendering::Uwp::FallbackType MapSharedFallbackTypeToUwp(const AdaptiveCards::FallbackType type);
AdaptiveCards::FallbackType MapUwpFallbackTypeToShared(const ABI::AdaptiveCards::Rendering::Uwp::FallbackType type);

HRESULT CopyTextElement(_In_ ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveTextElement* textElement,
                        _COM_Outptr_ ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveTextElement** copiedTextElement);

HRESULT GetAdaptiveActionParserRegistrationFromSharedModel(
    const std::shared_ptr<AdaptiveCards::ActionParserRegistration>& sharedActionParserRegistration,
    _COM_Outptr_ ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveActionParserRegistration** adaptiveActionParserRegistration);

HRESULT GetAdaptiveElementParserRegistrationFromSharedModel(
    const std::shared_ptr<AdaptiveCards::ElementParserRegistration>& sharedElementParserRegistration,
    _COM_Outptr_ ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveElementParserRegistration** adaptiveElementParserRegistration);

template<typename T, typename TInterface, typename C>
HRESULT IterateOverVectorWithFailure(_In_ ABI::Windows::Foundation::Collections::IVector<T*>* vector, const boolean stopOnFailure, C iterationCallback)
{
    Microsoft::WRL::ComPtr<ABI::Windows::Foundation::Collections::IVector<T*>> localVector(vector);
    ComPtr<IIterable<T*>> vectorIterable;
    HRESULT hr = localVector.As<IIterable<T*>>(&vectorIterable);

    if (SUCCEEDED(hr))
    {
        Microsoft::WRL::ComPtr<IIterator<T*>> vectorIterator;
        vectorIterable->First(&vectorIterator);

        boolean hasCurrent = false;
        hr = vectorIterator->get_HasCurrent(&hasCurrent);
        while (SUCCEEDED(hr) && hasCurrent)
        {
            Microsoft::WRL::ComPtr<TInterface> current = nullptr;
            if (FAILED(vectorIterator->get_Current(current.GetAddressOf())))
            {
                return S_OK;
            }

            hr = iterationCallback(current.Get());
            if (stopOnFailure && FAILED(hr))
            {
                return hr;
            }

            hr = vectorIterator->MoveNext(&hasCurrent);
        }
    }

    return hr;
}

template<typename T, typename C>
HRESULT IterateOverVectorWithFailure(_In_ ABI::Windows::Foundation::Collections::IVector<T*>* vector, const boolean stopOnFailure, C iterationCallback)
{
    return IterateOverVectorWithFailure<T, T, C>(vector, stopOnFailure, iterationCallback);
}

template<typename T, typename TInterface, typename C>
void IterateOverVector(_In_ ABI::Windows::Foundation::Collections::IVector<T*>* vector, C iterationCallback)
{
    Microsoft::WRL::ComPtr<ABI::Windows::Foundation::Collections::IVector<T*>> localVector(vector);
    ComPtr<IIterable<T*>> vectorIterable;
    THROW_IF_FAILED(localVector.As<IIterable<T*>>(&vectorIterable));

    Microsoft::WRL::ComPtr<IIterator<T*>> vectorIterator;
    if (FAILED(vectorIterable->First(&vectorIterator)))
    {
        return;
    }

    boolean hasCurrent = false;
    HRESULT hr = vectorIterator->get_HasCurrent(&hasCurrent);
    while (SUCCEEDED(hr) && hasCurrent)
    {
        Microsoft::WRL::ComPtr<TInterface> current = nullptr;
        hr = vectorIterator->get_Current(current.GetAddressOf());
        if (FAILED(hr))
        {
            break;
        }

        iterationCallback(current.Get());
        hr = vectorIterator->MoveNext(&hasCurrent);
    }
}

template<typename T, typename C>
void IterateOverVector(_In_ ABI::Windows::Foundation::Collections::IVector<T*>* vector, C iterationCallback)
{
    IterateOverVector<T, T, C>(vector, iterationCallback);
}
