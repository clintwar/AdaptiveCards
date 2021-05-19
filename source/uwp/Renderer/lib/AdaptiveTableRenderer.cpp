// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#include "pch.h"

#include "AdaptiveElementParserRegistration.h"
#include "AdaptiveTable.h"
#include "AdaptiveTableRenderer.h"

using namespace ABI::AdaptiveCards::Rendering::Uwp;
using namespace ABI::Windows::UI::Xaml;
using namespace ABI::Windows::Data::Json;
using namespace Microsoft::WRL;
using namespace ABI::Windows::Foundation::Collections;

namespace AdaptiveCards::Rendering::Uwp
{
    HRESULT AdaptiveTableRenderer::RuntimeClassInitialize() noexcept
    try
    {
        return S_OK;
    }
    CATCH_RETURN;

    HRESULT AdaptiveTableRenderer::Render(_In_ IAdaptiveCardElement* /*adaptiveCardElement*/,
                                          _In_ IAdaptiveRenderContext* /*renderContext*/,
                                          _In_ IAdaptiveRenderArgs* /*renderArgs*/,
                                          _COM_Outptr_ IUIElement** /*columnSetControl*/) noexcept
    try
    {
        return S_OK;
    }
    CATCH_RETURN;

    HRESULT AdaptiveTableRenderer::FromJson(
        _In_ IJsonObject* jsonObject,
        _In_ IAdaptiveElementParserRegistration* elementParserRegistration,
        _In_ IAdaptiveActionParserRegistration* actionParserRegistration,
        _In_ IVector<AdaptiveWarning*>* adaptiveWarnings,
        _COM_Outptr_ IAdaptiveCardElement** element) noexcept
    try
    {
        return AdaptiveCards::Rendering::Uwp::FromJson<AdaptiveCards::Rendering::Uwp::AdaptiveTable, AdaptiveCards::Table, AdaptiveCards::TableParser>(
            jsonObject, elementParserRegistration, actionParserRegistration, adaptiveWarnings, element);
    }
    CATCH_RETURN;
}
