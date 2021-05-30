// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#include "pch.h"

#include "AdaptiveElementParserRegistration.h"
#include "AdaptiveTable.h"
#include "AdaptiveTableRenderer.h"
#include "util.h"
#include "XamlHelpers.h"

using namespace AdaptiveCards::Rendering::Uwp::XamlHelpers;
using namespace ABI::AdaptiveCards::Rendering::Uwp;
using namespace ABI::Windows::Data::Json;
using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::UI::Xaml;
using namespace ABI::Windows::UI::Xaml::Controls;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;

namespace AdaptiveCards::Rendering::Uwp
{
    HRESULT AdaptiveTableRenderer::RuntimeClassInitialize() noexcept
    try
    {
        return S_OK;
    }
    CATCH_RETURN;

    HRESULT AdaptiveTableRenderer::RenderCell(_In_ IAdaptiveTableCell* cell,
                                              _In_ IAdaptiveRenderContext* renderContext,
                                              _In_ IAdaptiveRenderArgs* renderArgs,
                                              boolean showGridLines,
                                              UINT32 rowNumber,
                                              UINT32 columnNumber,
                                              _COM_Outptr_ IFrameworkElement** renderedCell)
    {
        // Render the cell as a container
        ComPtr<IAdaptiveElementRendererRegistration> rendererRegistration;
        renderContext->get_ElementRenderers(&rendererRegistration);

        ComPtr<IAdaptiveElementRenderer> containerRenderer;
        HString containerTypeString;
        RETURN_IF_FAILED(containerTypeString.Set(L"Container"));
        RETURN_IF_FAILED(rendererRegistration->Get(containerTypeString.Get(), &containerRenderer));

        ComPtr<IAdaptiveTableCell> tableCell(cell);
        ComPtr<IAdaptiveContainer> tableCellAsContainer;
        tableCell.As(&tableCellAsContainer);

        ComPtr<IAdaptiveCardElement> tableCellAsCardElement;
        RETURN_IF_FAILED(tableCell.As(&tableCellAsCardElement));

        ComPtr<IUIElement> cellRenderedAsContainer;
        RETURN_IF_FAILED(containerRenderer->Render(tableCellAsCardElement.Get(), renderContext, renderArgs, &cellRenderedAsContainer));

        ComPtr<ABI::Windows::Foundation::IReference<ABI::AdaptiveCards::Rendering::Uwp::VerticalContentAlignment>> cellVerticalAlignment;
        RETURN_IF_FAILED(tableCellAsContainer->get_VerticalContentAlignment(&cellVerticalAlignment));

        // Handle Grid Lines or Cell Spacing
        ComPtr<IAdaptiveHostConfig> hostConfig;
        RETURN_IF_FAILED(renderContext->get_HostConfig(&hostConfig));

        ComPtr<IFrameworkElement> cellFrameworkElement;
        if (showGridLines)
        {
            // If we're showing grid lines put the cell in a border
            ComPtr<IBorder> cellBorder =
                XamlHelpers::CreateXamlClass<IBorder>(HStringReference(RuntimeClass_Windows_UI_Xaml_Controls_Border));

            ABI::Windows::UI::Color attentionColor;
            RETURN_IF_FAILED(
                GetColorFromAdaptiveColor(hostConfig.Get(), ForegroundColor_Default, ContainerStyle_Default, false, false, &attentionColor));

            RETURN_IF_FAILED(cellBorder->put_BorderBrush(XamlHelpers::GetSolidColorBrush(attentionColor).Get()));

            // Create a border around the cell. Only set the top or left borders if we're in the top or leftmost
            // cells respectively in order to avoid double-thickness borders
            Thickness borderThickness = {0, 0, 1, 1};
            if (columnNumber == 0)
            {
                borderThickness.Left = 1;
            }
            if (rowNumber == 0)
            {
                borderThickness.Top = 1;
            }

            cellBorder->put_BorderThickness(borderThickness);

            cellBorder->put_Child(cellRenderedAsContainer.Get());

            cellBorder.As(&cellFrameworkElement);
        }
        else
        {
            // If we're not showing gridlines, use the cell as-is as the frameworkElement, and add the cell spacing
            RETURN_IF_FAILED(cellRenderedAsContainer.As(&cellFrameworkElement));

            ComPtr<IAdaptiveTableConfig> tableConfig;
            RETURN_IF_FAILED(hostConfig->get_Table(&tableConfig));

            UINT32 cellSpacing;
            RETURN_IF_FAILED(tableConfig->get_CellSpacing(&cellSpacing));
            DOUBLE cellSpacingDouble = static_cast<DOUBLE>(cellSpacing);

            // Set left and top margin for each cell (to avoid double margins). Don't set the margin on topmost
            // or leftmost cells to avoid creating margin outside the table.
            Thickness marginThickness = {cellSpacingDouble, cellSpacingDouble, 0, 0};
            if (columnNumber == 0)
            {
                marginThickness.Left = 0;
            }
            if (rowNumber == 0)
            {
                marginThickness.Top = 0;
            }

            RETURN_IF_FAILED(cellFrameworkElement->put_Margin(marginThickness));
        }

        RETURN_IF_FAILED(cellFrameworkElement.CopyTo(renderedCell));

        return S_OK;
    }

    HRESULT AdaptiveTableRenderer::RenderRow(_In_ IAdaptiveTableRow* row,
                                             _In_ IAdaptiveRenderContext* renderContext,
                                             _In_ IAdaptiveRenderArgs* renderArgs,
                                             boolean firstRowAsHeaders,
                                             boolean showGridLines,
                                             UINT32 rowNumber,
                                             _In_ IGrid* xamlGrid)
    {
        ComPtr<IRowDefinition> xamlRowDefinition =
            XamlHelpers::CreateXamlClass<IRowDefinition>(HStringReference(RuntimeClass_Windows_UI_Xaml_Controls_RowDefinition));

        ComPtr<IVector<RowDefinition*>> xamlRowDefinitions;
        RETURN_IF_FAILED(xamlGrid->get_RowDefinitions(&xamlRowDefinitions));
        RETURN_IF_FAILED(xamlRowDefinitions->Append(xamlRowDefinition.Get()));

        // Set the column header style if this is the first row and firstRowAsHeaders is set
        ComPtr<ABI::Windows::Foundation::IReference<ABI::AdaptiveCards::Rendering::Uwp::TextStyle>> argsTextStyle;
        if (rowNumber == 0 && firstRowAsHeaders)
        {
            // Save the old text style
            RETURN_IF_FAILED(renderArgs->get_TextStyle(&argsTextStyle));

            // Set the text style to TextStyle::ColumnHeader
            renderArgs->put_TextStyle(winrt::box_value(winrt::AdaptiveCards::Rendering::Uwp::TextStyle::ColumnHeader)
                                          .as<ABI::Windows::Foundation::IReference<ABI::AdaptiveCards::Rendering::Uwp::TextStyle>>()
                                          .get());
        }

        ComPtr<ABI::Windows::Foundation::IReference<HAlignment>> rowHorizontalAlignment;
        RETURN_IF_FAILED(row->get_HorizontalCellContentAlignment(&rowHorizontalAlignment));

        ComPtr<ABI::Windows::Foundation::IReference<ABI::AdaptiveCards::Rendering::Uwp::VerticalContentAlignment>> rowVerticalAlignment;
        RETURN_IF_FAILED(row->get_VerticalCellContentAlignment(&rowVerticalAlignment));

        ComPtr<IGridStatics> gridStatics;
        RETURN_IF_FAILED(GetActivationFactory(HStringReference(RuntimeClass_Windows_UI_Xaml_Controls_Grid).Get(), &gridStatics));

        // Create the cells
        ComPtr<IVector<AdaptiveTableCell*>> cells;
        RETURN_IF_FAILED(row->get_Cells(&cells));

        UINT columnNumber = 0;
        IterateOverVectorWithFailure<AdaptiveTableCell, IAdaptiveTableCell>(cells.Get(), false /*BECKYTODO*/, [&](IAdaptiveTableCell* cell) {
            // Render the cell
            ComPtr<IFrameworkElement> cellFrameworkElement;
            RETURN_IF_FAILED(RenderCell(cell, renderContext, renderArgs, showGridLines, rowNumber, columnNumber, &cellFrameworkElement));

            // Set the row and column of the cell
            RETURN_IF_FAILED(gridStatics->SetColumn(cellFrameworkElement.Get(), columnNumber));
            RETURN_IF_FAILED(gridStatics->SetRow(cellFrameworkElement.Get(), rowNumber));

            // Add the cell to the panel
            ComPtr<IGrid> localXamlGrid(xamlGrid);
            ComPtr<IPanel> xamlGridAsPanel;
            RETURN_IF_FAILED(localXamlGrid.As(&xamlGridAsPanel));
            XamlHelpers::AppendXamlElementToPanel(cellFrameworkElement.Get(), xamlGridAsPanel.Get());

            columnNumber++;
            return S_OK;
        });

        // If we changed the text style for row headers, change it back
        if (rowNumber == 0 && firstRowAsHeaders)
        {
            renderArgs->put_TextStyle(argsTextStyle.Get());
        }

        return S_OK;
    }

    HRESULT AdaptiveTableRenderer::Render(_In_ IAdaptiveCardElement* adaptiveCardElement,
                                          _In_ IAdaptiveRenderContext* renderContext,
                                          _In_ IAdaptiveRenderArgs* renderArgs,
                                          _COM_Outptr_ IUIElement** tableControl) noexcept
    try
    {
        ComPtr<IAdaptiveCardElement> cardElement(adaptiveCardElement);
        ComPtr<IAdaptiveTable> adaptiveTable;
        RETURN_IF_FAILED(cardElement.As(&adaptiveTable));

        // Create a grid to represent the table
        ComPtr<IGrid> xamlGrid =
            XamlHelpers::CreateXamlClass<IGrid>(HStringReference(RuntimeClass_Windows_UI_Xaml_Controls_Grid));

        ComPtr<ABI::Windows::Foundation::IReference<HAlignment>> tableHorizontalAlignment;
        RETURN_IF_FAILED(adaptiveTable->get_HorizontalCellContentAlignment(&tableHorizontalAlignment));

        ComPtr<ABI::Windows::Foundation::IReference<ABI::AdaptiveCards::Rendering::Uwp::VerticalContentAlignment>> tableVerticalAlignment;
        RETURN_IF_FAILED(adaptiveTable->get_VerticalCellContentAlignment(&tableVerticalAlignment));

        boolean showGridLines;
        RETURN_IF_FAILED(adaptiveTable->get_ShowGridLines(&showGridLines));

        // Create the column definitions
        ComPtr<IVector<AdaptiveTableColumnDefinition*>> columns;
        RETURN_IF_FAILED(adaptiveTable->get_Columns(&columns));

        ComPtr<IVector<ColumnDefinition*>> xamlColumnDefinitions;
        RETURN_IF_FAILED(xamlGrid->get_ColumnDefinitions(&xamlColumnDefinitions));

        IterateOverVectorWithFailure<AdaptiveTableColumnDefinition, IAdaptiveTableColumnDefinition>(
            columns.Get(), false /*BECKYTODO*/, [&](IAdaptiveTableColumnDefinition* column) {
                ComPtr<IColumnDefinition> xamlColumnDefinition = XamlHelpers::CreateXamlClass<IColumnDefinition>(
                    HStringReference(RuntimeClass_Windows_UI_Xaml_Controls_ColumnDefinition));

                RETURN_IF_FAILED(HandleTableColumnWidth(column, xamlColumnDefinition.Get()));

                RETURN_IF_FAILED(xamlColumnDefinitions->Append(xamlColumnDefinition.Get()));

                return S_OK;
            });

        // Create the rows
        ComPtr<IVector<AdaptiveTableRow*>> rows;
        RETURN_IF_FAILED(adaptiveTable->get_Rows(&rows));

        boolean firstRowAsHeaders;
        RETURN_IF_FAILED(adaptiveTable->get_FirstRowAsHeaders(&firstRowAsHeaders));

        UINT rowNumber = 0;
        IterateOverVectorWithFailure<AdaptiveTableRow, IAdaptiveTableRow>(rows.Get(), false /*BECKYTODO*/, [&](IAdaptiveTableRow* row) {
            RETURN_IF_FAILED(
                RenderRow(row, renderContext, renderArgs, firstRowAsHeaders, showGridLines, rowNumber, xamlGrid.Get()));
            rowNumber++;
            return S_OK;
        });

        ComPtr<IUIElement> xamlGridAsUIElement;
        xamlGrid.As(&xamlGridAsUIElement);

        return xamlGridAsUIElement.CopyTo(tableControl);
    }
    CATCH_RETURN;

    HRESULT AdaptiveTableRenderer::FromJson(_In_ IJsonObject* jsonObject,
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
