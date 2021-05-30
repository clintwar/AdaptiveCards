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
        ComPtr<IGridStatics> gridStatics;
        RETURN_IF_FAILED(GetActivationFactory(HStringReference(RuntimeClass_Windows_UI_Xaml_Controls_Grid).Get(), &gridStatics));

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

        ComPtr<IVector<RowDefinition*>> xamlRowDefinitions;
        RETURN_IF_FAILED(xamlGrid->get_RowDefinitions(&xamlRowDefinitions));

        UINT rowNumber = 0;
        IterateOverVectorWithFailure<AdaptiveTableRow, IAdaptiveTableRow>(rows.Get(), false /*BECKYTODO*/, [&](IAdaptiveTableRow* row) {
            ComPtr<IRowDefinition> xamlRowDefinition = XamlHelpers::CreateXamlClass<IRowDefinition>(
                HStringReference(RuntimeClass_Windows_UI_Xaml_Controls_RowDefinition));

            RETURN_IF_FAILED(xamlRowDefinitions->Append(xamlRowDefinition.Get()));

            ComPtr<ABI::Windows::Foundation::IReference<HAlignment>> rowHorizontalAlignment;
            RETURN_IF_FAILED(row->get_HorizontalCellContentAlignment(&rowHorizontalAlignment));

            ComPtr<ABI::Windows::Foundation::IReference<ABI::AdaptiveCards::Rendering::Uwp::VerticalContentAlignment>> rowVerticalAlignment;
            RETURN_IF_FAILED(row->get_VerticalCellContentAlignment(&rowVerticalAlignment));

            // Create the cells
            ComPtr<IVector<AdaptiveTableCell*>> cells;
            RETURN_IF_FAILED(row->get_Cells(&cells));

            UINT cellNumber = 0;
            IterateOverVectorWithFailure<AdaptiveTableCell, IAdaptiveTableCell>(cells.Get(), false /*BECKYTODO*/, [&](IAdaptiveTableCell* cell) {
                ComPtr<IAdaptiveElementRendererRegistration> rendererRegistration;
                renderContext->get_ElementRenderers(&rendererRegistration);

                ComPtr<IAdaptiveElementRenderer> containerRenderer;
                HString containerTypeString;
                RETURN_IF_FAILED(containerTypeString.Set(L"Container"));
                RETURN_IF_FAILED(rendererRegistration->Get(containerTypeString.Get(), &containerRenderer));

                ComPtr<IAdaptiveTableCell> tableCell(cell);
                ComPtr<IAdaptiveContainer> tableCellAsContainer;
                tableCell.As(&tableCellAsContainer);

                ComPtr<ABI::Windows::Foundation::IReference<ABI::AdaptiveCards::Rendering::Uwp::VerticalContentAlignment>> cellVerticalAlignment;
                RETURN_IF_FAILED(tableCellAsContainer->get_VerticalContentAlignment(&cellVerticalAlignment));

                ComPtr<IAdaptiveCardElement> tableCellAsCardElement;
                RETURN_IF_FAILED(tableCell.As(&tableCellAsCardElement));

                // Render the cell as a container
                ComPtr<IUIElement> renderedCell;
                RETURN_IF_FAILED(containerRenderer->Render(tableCellAsCardElement.Get(), renderContext, renderArgs, &renderedCell));

                ComPtr<IFrameworkElement> cellFrameworkElement;

                ComPtr<IAdaptiveHostConfig> hostConfig;
                RETURN_IF_FAILED(renderContext->get_HostConfig(&hostConfig));

                if (showGridLines)
                {
                    // If we're showing grid lines put the cell in a border
                    ComPtr<IBorder> cellBorder =
                        XamlHelpers::CreateXamlClass<IBorder>(HStringReference(RuntimeClass_Windows_UI_Xaml_Controls_Border));

                    ABI::Windows::UI::Color attentionColor;
                    RETURN_IF_FAILED(GetColorFromAdaptiveColor(
                        hostConfig.Get(), ForegroundColor_Default, ContainerStyle_Default, false, false, &attentionColor));

                    RETURN_IF_FAILED(cellBorder->put_BorderBrush(XamlHelpers::GetSolidColorBrush(attentionColor).Get()));

                    // Create a border around the cell. Only set the top or left borders if we're in the top or leftmost
                    // cells respectively in order to avoid double-thickness borders
                    Thickness borderThickness = {0, 0, 1, 1};
                    if (cellNumber == 0)
                    {
                        borderThickness.Left = 1;
                    }
                    if (rowNumber == 0)
                    {
                        borderThickness.Top = 1;
                    }

                    cellBorder->put_BorderThickness(borderThickness);

                    cellBorder->put_Child(renderedCell.Get());

                    cellBorder.As(&cellFrameworkElement);
                }
                else
                {
                    // If we're not showing gridlines, use the cell as-is as the frameworkElement, and add the cell spacing
                    RETURN_IF_FAILED(renderedCell.As(&cellFrameworkElement));

                    ComPtr<IAdaptiveTableConfig> tableConfig;
                    RETURN_IF_FAILED(hostConfig->get_Table(&tableConfig));

                    UINT32 cellSpacing;
                    RETURN_IF_FAILED(tableConfig->get_CellSpacing(&cellSpacing));
                    DOUBLE cellSpacingDouble = static_cast<DOUBLE>(cellSpacing);

                    // Set left and top margin for each cell (to avoid double margins). Don't set the margin on topmost
                    // or leftmost cells to avoid creating margin outside the table.
                    Thickness marginThickness = {cellSpacingDouble, cellSpacingDouble, 0, 0};
                    if (cellNumber == 0)
                    {
                        marginThickness.Left = 0;
                    }
                    if (rowNumber == 0)
                    {
                        marginThickness.Top = 0;
                    }

                    RETURN_IF_FAILED(cellFrameworkElement->put_Margin(marginThickness));
                }

                RETURN_IF_FAILED(gridStatics->SetColumn(cellFrameworkElement.Get(), cellNumber));
                RETURN_IF_FAILED(gridStatics->SetRow(cellFrameworkElement.Get(), rowNumber));

                ComPtr<IPanel> xamlGridAsPanel;
                RETURN_IF_FAILED(xamlGrid.As(&xamlGridAsPanel));

                XamlHelpers::AppendXamlElementToPanel(cellFrameworkElement.Get(), xamlGridAsPanel.Get());

                cellNumber++;
                return S_OK;
            });

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
