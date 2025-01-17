// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#pragma once

#include "TableCell.h"
#include "AdaptiveBackgroundImage.h"

namespace AdaptiveCards::Rendering::Uwp
{
    class DECLSPEC_UUID("8670BAA9-7DAC-4714-81F3-8D1553CD0E4E") AdaptiveTableCell
        : public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::RuntimeClassType::WinRtClassicComMix>,
                                              ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveTableCell,
                                              ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveContainerBase,
                                              ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveCardElement,
                                              Microsoft::WRL::CloakedIid<ITypePeek>,
                                              Microsoft::WRL::CloakedIid<AdaptiveCards::Rendering::Uwp::AdaptiveCardElementBase>>
    {
        AdaptiveRuntime(AdaptiveTableCell);

    public:
        AdaptiveTableCell();
        HRESULT RuntimeClassInitialize() noexcept;
        HRESULT RuntimeClassInitialize(const std::shared_ptr<AdaptiveCards::TableCell>& sharedTableCell);

        // IAdaptiveTableCell
        IFACEMETHODIMP get_Items(
            _COM_Outptr_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveCardElement*>** items) override;

        IFACEMETHODIMP get_VerticalContentAlignment(
            _COM_Outptr_ ABI::Windows::Foundation::IReference<ABI::AdaptiveCards::Rendering::Uwp::VerticalContentAlignment>** verticalContentAlignment) override;
        IFACEMETHODIMP put_VerticalContentAlignment(
            _In_ ABI::Windows::Foundation::IReference<ABI::AdaptiveCards::Rendering::Uwp::VerticalContentAlignment>* verticalContentAlignment) override;

        IFACEMETHODIMP get_BackgroundImage(_Outptr_ ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveBackgroundImage** backgroundImage) override;
        IFACEMETHODIMP put_BackgroundImage(_In_ ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveBackgroundImage* backgroundImage) override;

        IFACEMETHODIMP get_Rtl(_COM_Outptr_ ABI::Windows::Foundation::IReference<bool>** value) override;
        IFACEMETHODIMP put_Rtl(_In_ ABI::Windows::Foundation::IReference<bool>* value) override;

        // IAdaptiveContainerBase
        IFACEMETHODIMP get_Style(_Out_ ABI::AdaptiveCards::Rendering::Uwp::ContainerStyle* style) override;
        IFACEMETHODIMP put_Style(ABI::AdaptiveCards::Rendering::Uwp::ContainerStyle style) override;

        IFACEMETHODIMP get_SelectAction(_COM_Outptr_ ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveActionElement** action) override;
        IFACEMETHODIMP put_SelectAction(_In_ ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveActionElement* action) override;

        IFACEMETHODIMP get_Bleed(_Out_ boolean* bleed) override;
        IFACEMETHODIMP put_Bleed(boolean bleed) override;

        IFACEMETHODIMP get_BleedDirection(_Out_ ABI::AdaptiveCards::Rendering::Uwp::BleedDirection* bleedDirection) override;

        IFACEMETHODIMP get_MinHeight(_Out_ UINT32* minHeight) override;
        IFACEMETHODIMP put_MinHeight(UINT32 minHeight) override;

        // IAdaptiveCardElement
        IFACEMETHODIMP get_ElementType(_Out_ ABI::AdaptiveCards::Rendering::Uwp::ElementType* elementType) override;

        IFACEMETHODIMP get_Spacing(_Out_ ABI::AdaptiveCards::Rendering::Uwp::Spacing* spacing) override
        {
            return AdaptiveCardElementBase::get_Spacing(spacing);
        }
        IFACEMETHODIMP put_Spacing(ABI::AdaptiveCards::Rendering::Uwp::Spacing spacing) override
        {
            return AdaptiveCardElementBase::put_Spacing(spacing);
        }

        IFACEMETHODIMP get_Separator(_Out_ boolean* separator) override
        {
            return AdaptiveCardElementBase::get_Separator(separator);
        }
        IFACEMETHODIMP put_Separator(boolean separator) override
        {
            return AdaptiveCardElementBase::put_Separator(separator);
        }

        IFACEMETHODIMP get_IsVisible(_Out_ boolean* isVisible) override
        {
            return AdaptiveCardElementBase::get_IsVisible(isVisible);
        }
        IFACEMETHODIMP put_IsVisible(boolean isVisible) override
        {
            return AdaptiveCardElementBase::put_IsVisible(isVisible);
        }

        IFACEMETHODIMP get_Id(_Outptr_ HSTRING* id) override { return AdaptiveCardElementBase::get_Id(id); }
        IFACEMETHODIMP put_Id(_In_ HSTRING id) override { return AdaptiveCardElementBase::put_Id(id); }

        IFACEMETHODIMP get_FallbackType(_Out_ ABI::AdaptiveCards::Rendering::Uwp::FallbackType* fallback) override
        {
            return AdaptiveCardElementBase::get_FallbackType(fallback);
        }
        IFACEMETHODIMP get_FallbackContent(_COM_Outptr_ ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveCardElement** content) override
        {
            return AdaptiveCardElementBase::get_FallbackContent(content);
        }

        IFACEMETHODIMP put_FallbackType(ABI::AdaptiveCards::Rendering::Uwp::FallbackType fallback) override
        {
            return AdaptiveCardElementBase::put_FallbackType(fallback);
        }

        IFACEMETHODIMP put_FallbackContent(_In_ ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveCardElement* content) override
        {
            return AdaptiveCardElementBase::put_FallbackContent(content);
        }

        IFACEMETHODIMP get_ElementTypeString(_Outptr_ HSTRING* value) override
        {
            return AdaptiveCardElementBase::get_ElementTypeString(value);
        }

        IFACEMETHODIMP get_AdditionalProperties(_COM_Outptr_ ABI::Windows::Data::Json::IJsonObject** result) override
        {
            return AdaptiveCardElementBase::get_AdditionalProperties(result);
        }
        IFACEMETHODIMP put_AdditionalProperties(_In_ ABI::Windows::Data::Json::IJsonObject* value) override
        {
            return AdaptiveCardElementBase::put_AdditionalProperties(value);
        }

        IFACEMETHODIMP get_Requirements(
            _COM_Outptr_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::AdaptiveRequirement*>** requirements) override
        {
            return AdaptiveCardElementBase::get_Requirements(requirements);
        }

        IFACEMETHODIMP ToJson(_COM_Outptr_ ABI::Windows::Data::Json::IJsonObject** result) override
        {
            return AdaptiveCardElementBase::ToJson(result);
        }

        virtual HRESULT GetSharedModel(std::shared_ptr<AdaptiveCards::BaseCardElement>& sharedModel) override;

        IFACEMETHODIMP get_Height(_Out_ ABI::AdaptiveCards::Rendering::Uwp::HeightType* height) override
        {
            return AdaptiveCardElementBase::get_Height(height);
        }
        IFACEMETHODIMP put_Height(ABI::AdaptiveCards::Rendering::Uwp::HeightType height) override
        {
            return AdaptiveCardElementBase::put_Height(height);
        }

        // ITypePeek method
        void* PeekAt(REFIID riid) override { return PeekHelper(riid, this); }

    private:
        Microsoft::WRL::ComPtr<ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveCardElement*>> m_items;
        Microsoft::WRL::ComPtr<ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveActionElement> m_selectAction;
        ABI::AdaptiveCards::Rendering::Uwp::ContainerStyle m_style;
        Microsoft::WRL::ComPtr<ABI::Windows::Foundation::IReference<ABI::AdaptiveCards::Rendering::Uwp::VerticalContentAlignment>> m_verticalContentAlignment;
        Microsoft::WRL::ComPtr<ABI::AdaptiveCards::Rendering::Uwp::IAdaptiveBackgroundImage> m_backgroundImage;
        UINT32 m_minHeight;
        boolean m_bleed;
        ABI::AdaptiveCards::Rendering::Uwp::BleedDirection m_bleedDirection;
        Microsoft::WRL::ComPtr<ABI::Windows::Foundation::IReference<bool>> m_rtl;
    };

    ActivatableClass(AdaptiveTableCell);
}
