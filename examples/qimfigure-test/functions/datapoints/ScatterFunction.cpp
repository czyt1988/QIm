#include "ScatterFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotAxisInfo.h"
#include "plot/QImPlotScatterItemNode.h"
#include "plot/QImPlotValueTrackerNode.h"
#include <random>

/**
 * \if ENGLISH
 * @brief Constructor for ScatterFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Scatter"/"Marker"/"Size" - Marker size
 * - "Scatter"/"Marker"/"Shape" - Marker shape
 * - "Scatter"/"Marker"/"Fill" - Marker fill
 * - "Scatter"/"Marker"/"Color" - Marker color
 * \endif
 * 
 * \if CHINESE
 * @brief ScatterFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Scatter"/"Marker"/"Size" - 标记大小
 * - "Scatter"/"Marker"/"Shape" - 标记形状
 * - "Scatter"/"Marker"/"Fill" - 标记填充
 * - "Scatter"/"Marker"/"Color" - 标记颜色
 * \endif
 */
ScatterFunction::ScatterFunction(QObject* parent)
    : TestFunction(parent)
{
    // Register title property
    PropertyRegistration titleReg;
    titleReg.category = tr("Plot");
    titleReg.subcategory = tr("Basic Info");
    titleReg.displayName = tr("Title");
    titleReg.briefDesc = tr("Plot title");
    titleReg.detailDesc = tr("Sets the title text displayed at the top of the plot");
    titleReg.editorType = EditorType::LineEdit;
    titleReg.defaultValue = m_title;
    titleReg.propertyName = "title";
    titleReg.target = this;
    registerProperty(titleReg);
    
    // Register X axis label property
    PropertyRegistration xLabelReg;
    xLabelReg.category = tr("Plot");
    xLabelReg.subcategory = tr("Axis");
    xLabelReg.displayName = tr("X Label");
    xLabelReg.briefDesc = tr("X axis label");
    xLabelReg.detailDesc = tr("Sets the label text for the X axis");
    xLabelReg.editorType = EditorType::LineEdit;
    xLabelReg.defaultValue = m_xLabel;
    xLabelReg.propertyName = "xLabel";
    xLabelReg.target = this;
    registerProperty(xLabelReg);
    
    // Register Y axis label property
    PropertyRegistration yLabelReg;
    yLabelReg.category = tr("Plot");
    yLabelReg.subcategory = tr("Axis");
    yLabelReg.displayName = tr("Y Label");
    yLabelReg.briefDesc = tr("Y axis label");
    yLabelReg.detailDesc = tr("Sets the label text for the Y axis");
    yLabelReg.editorType = EditorType::LineEdit;
    yLabelReg.defaultValue = m_yLabel;
    yLabelReg.propertyName = "yLabel";
    yLabelReg.target = this;
    registerProperty(yLabelReg);
    
    // Register marker size property
    PropertyRegistration sizeReg;
    sizeReg.category = tr("Scatter");
    sizeReg.subcategory = tr("Marker");
    sizeReg.displayName = tr("Size");
    sizeReg.briefDesc = tr("Marker size");
    sizeReg.detailDesc = tr("Sets the size of scatter plot markers in pixels");
    sizeReg.editorType = EditorType::DoubleSpinBox;
    sizeReg.defaultValue = m_markerSize;
    sizeReg.minValue = 1.0;
    sizeReg.maxValue = 20.0;
    sizeReg.stepValue = 0.5;
    sizeReg.propertyName = "markerSize";
    sizeReg.target = this;
    registerProperty(sizeReg);
    
    // Register marker shape property
    PropertyRegistration shapeReg;
    shapeReg.category = tr("Scatter");
    shapeReg.subcategory = tr("Marker");
    shapeReg.displayName = tr("Shape");
    shapeReg.briefDesc = tr("Marker shape");
    shapeReg.detailDesc = tr("Sets the shape of scatter plot markers");
    shapeReg.editorType = EditorType::EnumComboBox;
    shapeReg.comboBoxOptions = QStringList{
        tr("Circle"),     // ImPlotMarker_Circle = 0
        tr("Square"),     // ImPlotMarker_Square = 1
        tr("Diamond"),    // ImPlotMarker_Diamond = 2
        tr("Up"),         // ImPlotMarker_Up = 3
        tr("Down"),       // ImPlotMarker_Down = 4
        tr("Left"),       // ImPlotMarker_Left = 5
        tr("Right"),      // ImPlotMarker_Right = 6
        tr("Cross"),      // ImPlotMarker_Cross = 7
        tr("Plus"),       // ImPlotMarker_Plus = 8
        tr("Asterisk"),   // ImPlotMarker_Asterisk = 9
    };
    shapeReg.defaultValue = m_markerShape;  // 0 = Circle by default
    shapeReg.propertyName = "markerShape";
    shapeReg.target = this;
    registerProperty(shapeReg);
    
    // Register marker fill property
    PropertyRegistration fillReg;
    fillReg.category = tr("Scatter");
    fillReg.subcategory = tr("Marker");
    fillReg.displayName = tr("Fill");
    fillReg.briefDesc = tr("Marker fill");
    fillReg.detailDesc = tr("Sets whether markers are filled or outline only");
    fillReg.editorType = EditorType::CheckBox;
    fillReg.defaultValue = m_markerFill;
    fillReg.propertyName = "markerFill";
    fillReg.target = this;
    registerProperty(fillReg);
    
    // Register marker color property
    PropertyRegistration colorReg;
    colorReg.category = tr("Scatter");
    colorReg.subcategory = tr("Marker");
    colorReg.displayName = tr("Color");
    colorReg.briefDesc = tr("Marker color");
    colorReg.detailDesc = tr("Sets the color of scatter plot markers");
    colorReg.editorType = EditorType::ColorPicker;
    colorReg.defaultValue = m_markerColor;
    colorReg.propertyName = "markerColor";
    colorReg.target = this;
    registerProperty(colorReg);
    
    // Register clipping enabled property
    PropertyRegistration clippingEnabledReg;
    clippingEnabledReg.category = tr("Scatter");
    clippingEnabledReg.subcategory = tr("Flags");
    clippingEnabledReg.displayName = tr("Clipping Enabled");
    clippingEnabledReg.briefDesc = tr("Clip markers at plot edges");
    clippingEnabledReg.detailDesc = tr("When enabled, markers at the edge of a plot will be clipped. When disabled, markers may extend beyond the plot area.");
    clippingEnabledReg.editorType = EditorType::CheckBox;
    clippingEnabledReg.defaultValue = m_clippingEnabled;
    clippingEnabledReg.propertyName = "clippingEnabled";
    clippingEnabledReg.target = this;
    registerProperty(clippingEnabledReg);
    
    // Register downsample algorithm property
    PropertyRegistration downsampleAlgoReg;
    downsampleAlgoReg.category = tr("Scatter");
    downsampleAlgoReg.subcategory = tr("Downsample");
    downsampleAlgoReg.displayName = tr("Algorithm");
    downsampleAlgoReg.briefDesc = tr("Downsampling algorithm");
    downsampleAlgoReg.detailDesc = tr("Sets the downsampling algorithm: None, LTTB, MinMaxLTTB, or Auto");
    downsampleAlgoReg.editorType = EditorType::EnumComboBox;
    downsampleAlgoReg.comboBoxOptions = QStringList{
        tr("None"),       // QImDownsampleAlgorithm::None = 0
        tr("LTTB"),       // QImDownsampleAlgorithm::LTTB = 1
        tr("MinMaxLTTB"), // QImDownsampleAlgorithm::MinMaxLTTB = 2
        tr("Auto"),       // QImDownsampleAlgorithm::Auto = 3
    };
    downsampleAlgoReg.defaultValue = static_cast<int>(m_downsampleAlgorithm);
    downsampleAlgoReg.propertyName = "downsampleAlgorithm";
    downsampleAlgoReg.target = this;
    registerProperty(downsampleAlgoReg);
    
    // Register downsample threshold property
    PropertyRegistration downsampleThresholdReg;
    downsampleThresholdReg.category = tr("Scatter");
    downsampleThresholdReg.subcategory = tr("Downsample");
    downsampleThresholdReg.displayName = tr("Threshold");
    downsampleThresholdReg.briefDesc = tr("Downsample threshold");
    downsampleThresholdReg.detailDesc = tr("Sets the data point threshold for triggering downsampling (0 means no threshold)");
    downsampleThresholdReg.editorType = EditorType::SpinBox;
    downsampleThresholdReg.defaultValue = m_downsampleThreshold;
    downsampleThresholdReg.minValue = 0;
    downsampleThresholdReg.maxValue = 100000;
    downsampleThresholdReg.stepValue = 1000;
    downsampleThresholdReg.propertyName = "downsampleThreshold";
    downsampleThresholdReg.target = this;
    registerProperty(downsampleThresholdReg);
    
    // === QImPlotNode: Size properties ===
    
    // Register plot width property
    PropertyRegistration plotWidthReg;
    plotWidthReg.category = tr("Plot");
    plotWidthReg.subcategory = tr("Size");
    plotWidthReg.displayName = tr("Plot Width");
    plotWidthReg.briefDesc = tr("Plot width");
    plotWidthReg.detailDesc = tr("Sets the width of the plot area in pixels. -1 means auto/default.");
    plotWidthReg.editorType = EditorType::DoubleSpinBox;
    plotWidthReg.defaultValue = m_plotSize.width();
    plotWidthReg.minValue = -1.0;
    plotWidthReg.maxValue = 2000.0;
    plotWidthReg.stepValue = 10.0;
    plotWidthReg.propertyName = "plotWidth";
    plotWidthReg.target = this;
    registerProperty(plotWidthReg);
    
    // Register plot height property
    PropertyRegistration plotHeightReg;
    plotHeightReg.category = tr("Plot");
    plotHeightReg.subcategory = tr("Size");
    plotHeightReg.displayName = tr("Plot Height");
    plotHeightReg.briefDesc = tr("Plot height");
    plotHeightReg.detailDesc = tr("Sets the height of the plot area in pixels. -1 means auto/default.");
    plotHeightReg.editorType = EditorType::DoubleSpinBox;
    plotHeightReg.defaultValue = m_plotSize.height();
    plotHeightReg.minValue = -1.0;
    plotHeightReg.maxValue = 2000.0;
    plotHeightReg.stepValue = 10.0;
    plotHeightReg.propertyName = "plotHeight";
    plotHeightReg.target = this;
    registerProperty(plotHeightReg);
    
    // Register auto size property
    PropertyRegistration autoSizeReg;
    autoSizeReg.category = tr("Plot");
    autoSizeReg.subcategory = tr("Size");
    autoSizeReg.displayName = tr("Auto Size");
    autoSizeReg.briefDesc = tr("Auto size plot");
    autoSizeReg.detailDesc = tr("When enabled, the plot automatically sizes to fill its container area");
    autoSizeReg.editorType = EditorType::CheckBox;
    autoSizeReg.defaultValue = m_autoSize;
    autoSizeReg.propertyName = "autoSize";
    autoSizeReg.target = this;
    registerProperty(autoSizeReg);
    
    // === QImPlotNode: ImPlotFlags properties ===
    
    // Register title enabled property
    PropertyRegistration titleEnabledReg;
    titleEnabledReg.category = tr("Plot");
    titleEnabledReg.subcategory = tr("Flags");
    titleEnabledReg.displayName = tr("Title Enabled");
    titleEnabledReg.briefDesc = tr("Show plot title");
    titleEnabledReg.detailDesc = tr("When enabled, the plot title is displayed at the top of the plot area");
    titleEnabledReg.editorType = EditorType::CheckBox;
    titleEnabledReg.defaultValue = m_titleEnabled;
    titleEnabledReg.propertyName = "titleEnabled";
    titleEnabledReg.target = this;
    registerProperty(titleEnabledReg);
    
    // Register legend enabled property
    PropertyRegistration legendEnabledReg;
    legendEnabledReg.category = tr("Plot");
    legendEnabledReg.subcategory = tr("Flags");
    legendEnabledReg.displayName = tr("Legend Enabled");
    legendEnabledReg.briefDesc = tr("Show plot legend");
    legendEnabledReg.detailDesc = tr("When enabled, the plot legend is displayed showing data series labels");
    legendEnabledReg.editorType = EditorType::CheckBox;
    legendEnabledReg.defaultValue = m_legendEnabled;
    legendEnabledReg.propertyName = "legendEnabled";
    legendEnabledReg.target = this;
    registerProperty(legendEnabledReg);
    
    // Register mouse text enabled property
    PropertyRegistration mouseTextEnabledReg;
    mouseTextEnabledReg.category = tr("Plot");
    mouseTextEnabledReg.subcategory = tr("Flags");
    mouseTextEnabledReg.displayName = tr("Mouse Text Enabled");
    mouseTextEnabledReg.briefDesc = tr("Show mouse coordinate text");
    mouseTextEnabledReg.detailDesc = tr("When enabled, the plot shows coordinate text near the mouse cursor");
    mouseTextEnabledReg.editorType = EditorType::CheckBox;
    mouseTextEnabledReg.defaultValue = m_mouseTextEnabled;
    mouseTextEnabledReg.propertyName = "mouseTextEnabled";
    mouseTextEnabledReg.target = this;
    registerProperty(mouseTextEnabledReg);
    
    // Register inputs enabled property
    PropertyRegistration inputsEnabledReg;
    inputsEnabledReg.category = tr("Plot");
    inputsEnabledReg.subcategory = tr("Flags");
    inputsEnabledReg.displayName = tr("Inputs Enabled");
    inputsEnabledReg.briefDesc = tr("Enable user interaction inputs");
    inputsEnabledReg.detailDesc = tr("When enabled, the plot responds to user mouse and keyboard interactions");
    inputsEnabledReg.editorType = EditorType::CheckBox;
    inputsEnabledReg.defaultValue = m_inputsEnabled;
    inputsEnabledReg.propertyName = "inputsEnabled";
    inputsEnabledReg.target = this;
    registerProperty(inputsEnabledReg);
    
    // Register menus enabled property
    PropertyRegistration menusEnabledReg;
    menusEnabledReg.category = tr("Plot");
    menusEnabledReg.subcategory = tr("Flags");
    menusEnabledReg.displayName = tr("Menus Enabled");
    menusEnabledReg.briefDesc = tr("Enable context menus");
    menusEnabledReg.detailDesc = tr("When enabled, right-click context menus are available in the plot area");
    menusEnabledReg.editorType = EditorType::CheckBox;
    menusEnabledReg.defaultValue = m_menusEnabled;
    menusEnabledReg.propertyName = "menusEnabled";
    menusEnabledReg.target = this;
    registerProperty(menusEnabledReg);
    
    // Register box select enabled property
    PropertyRegistration boxSelectEnabledReg;
    boxSelectEnabledReg.category = tr("Plot");
    boxSelectEnabledReg.subcategory = tr("Flags");
    boxSelectEnabledReg.displayName = tr("Box Select Enabled");
    boxSelectEnabledReg.briefDesc = tr("Enable box selection");
    boxSelectEnabledReg.detailDesc = tr("When enabled, users can drag to select a rectangular region in the plot");
    boxSelectEnabledReg.editorType = EditorType::CheckBox;
    boxSelectEnabledReg.defaultValue = m_boxSelectEnabled;
    boxSelectEnabledReg.propertyName = "boxSelectEnabled";
    boxSelectEnabledReg.target = this;
    registerProperty(boxSelectEnabledReg);
    
    // Register frame enabled property
    PropertyRegistration frameEnabledReg;
    frameEnabledReg.category = tr("Plot");
    frameEnabledReg.subcategory = tr("Flags");
    frameEnabledReg.displayName = tr("Frame Enabled");
    frameEnabledReg.briefDesc = tr("Show plot frame");
    frameEnabledReg.detailDesc = tr("When enabled, the plot renders an outer frame border around the plot area");
    frameEnabledReg.editorType = EditorType::CheckBox;
    frameEnabledReg.defaultValue = m_frameEnabled;
    frameEnabledReg.propertyName = "frameEnabled";
    frameEnabledReg.target = this;
    registerProperty(frameEnabledReg);
    
    // Register equal aspect property
    PropertyRegistration equalReg;
    equalReg.category = tr("Plot");
    equalReg.subcategory = tr("Flags");
    equalReg.displayName = tr("Equal Aspect");
    equalReg.briefDesc = tr("Equal aspect ratio");
    equalReg.detailDesc = tr("When enabled, the plot enforces equal aspect ratio between X and Y axes");
    equalReg.editorType = EditorType::CheckBox;
    equalReg.defaultValue = m_equal;
    equalReg.propertyName = "equal";
    equalReg.target = this;
    registerProperty(equalReg);
    
    // Register crosshairs property
    PropertyRegistration crosshairsReg;
    crosshairsReg.category = tr("Plot");
    crosshairsReg.subcategory = tr("Flags");
    crosshairsReg.displayName = tr("Crosshairs");
    crosshairsReg.briefDesc = tr("Show crosshairs cursor");
    crosshairsReg.detailDesc = tr("When enabled, the plot displays crosshair lines that follow the mouse cursor");
    crosshairsReg.editorType = EditorType::CheckBox;
    crosshairsReg.defaultValue = m_crosshairs;
    crosshairsReg.propertyName = "crosshairs";
    crosshairsReg.target = this;
    registerProperty(crosshairsReg);
    
    // Register canvas enabled property
    PropertyRegistration canvasEnabledReg;
    canvasEnabledReg.category = tr("Plot");
    canvasEnabledReg.subcategory = tr("Flags");
    canvasEnabledReg.displayName = tr("Canvas Enabled");
    canvasEnabledReg.briefDesc = tr("Show canvas background");
    canvasEnabledReg.detailDesc = tr("When enabled, the plot renders a colored canvas background behind the plot area");
    canvasEnabledReg.editorType = EditorType::CheckBox;
    canvasEnabledReg.defaultValue = m_canvasEnabled;
    canvasEnabledReg.propertyName = "canvasEnabled";
    canvasEnabledReg.target = this;
    registerProperty(canvasEnabledReg);
    
    // === QImPlotAxisInfo X1: Limits properties ===
    
    // Register X1 min limits property
    PropertyRegistration x1MinLimitsReg;
    x1MinLimitsReg.category = tr("Plot");
    x1MinLimitsReg.subcategory = tr("Axis Config");
    x1MinLimitsReg.displayName = tr("X Min Limits");
    x1MinLimitsReg.briefDesc = tr("X axis minimum limit");
    x1MinLimitsReg.detailDesc = tr("Sets the minimum visible value on the X1 axis");
    x1MinLimitsReg.editorType = EditorType::DoubleSpinBox;
    x1MinLimitsReg.defaultValue = m_x1MinLimits;
    x1MinLimitsReg.minValue = -1e6;
    x1MinLimitsReg.maxValue = 1e6;
    x1MinLimitsReg.stepValue = 1.0;
    x1MinLimitsReg.propertyName = "x1MinLimits";
    x1MinLimitsReg.target = this;
    registerProperty(x1MinLimitsReg);
    
    // Register X1 max limits property
    PropertyRegistration x1MaxLimitsReg;
    x1MaxLimitsReg.category = tr("Plot");
    x1MaxLimitsReg.subcategory = tr("Axis Config");
    x1MaxLimitsReg.displayName = tr("X Max Limits");
    x1MaxLimitsReg.briefDesc = tr("X axis maximum limit");
    x1MaxLimitsReg.detailDesc = tr("Sets the maximum visible value on the X1 axis");
    x1MaxLimitsReg.editorType = EditorType::DoubleSpinBox;
    x1MaxLimitsReg.defaultValue = m_x1MaxLimits;
    x1MaxLimitsReg.minValue = -1e6;
    x1MaxLimitsReg.maxValue = 1e6;
    x1MaxLimitsReg.stepValue = 1.0;
    x1MaxLimitsReg.propertyName = "x1MaxLimits";
    x1MaxLimitsReg.target = this;
    registerProperty(x1MaxLimitsReg);
    
    // === QImPlotAxisInfo X1: Flags properties ===
    
    // Register X1 auto fit property
    PropertyRegistration x1AutoFitReg;
    x1AutoFitReg.category = tr("Plot");
    x1AutoFitReg.subcategory = tr("Axis Config");
    x1AutoFitReg.displayName = tr("X Auto Fit");
    x1AutoFitReg.briefDesc = tr("X axis auto fit");
    x1AutoFitReg.detailDesc = tr("When enabled, the X1 axis automatically adjusts its range to fit all data points");
    x1AutoFitReg.editorType = EditorType::CheckBox;
    x1AutoFitReg.defaultValue = m_x1AutoFit;
    x1AutoFitReg.propertyName = "x1AutoFit";
    x1AutoFitReg.target = this;
    registerProperty(x1AutoFitReg);
    
    // Register X1 inverted property
    PropertyRegistration x1InvertedReg;
    x1InvertedReg.category = tr("Plot");
    x1InvertedReg.subcategory = tr("Axis Config");
    x1InvertedReg.displayName = tr("X Inverted");
    x1InvertedReg.briefDesc = tr("X axis inverted direction");
    x1InvertedReg.detailDesc = tr("When enabled, the X1 axis direction is inverted (values decrease left to right)");
    x1InvertedReg.editorType = EditorType::CheckBox;
    x1InvertedReg.defaultValue = m_x1Inverted;
    x1InvertedReg.propertyName = "x1Inverted";
    x1InvertedReg.target = this;
    registerProperty(x1InvertedReg);
    
    // Register X1 label enabled property
    PropertyRegistration x1LabelEnabledReg;
    x1LabelEnabledReg.category = tr("Plot");
    x1LabelEnabledReg.subcategory = tr("Axis Config");
    x1LabelEnabledReg.displayName = tr("X Label Visible");
    x1LabelEnabledReg.briefDesc = tr("X axis label visible");
    x1LabelEnabledReg.detailDesc = tr("When enabled, the X1 axis label text is displayed");
    x1LabelEnabledReg.editorType = EditorType::CheckBox;
    x1LabelEnabledReg.defaultValue = m_x1LabelEnabled;
    x1LabelEnabledReg.propertyName = "x1LabelEnabled";
    x1LabelEnabledReg.target = this;
    registerProperty(x1LabelEnabledReg);
    
    // Register X1 grid lines enabled property
    PropertyRegistration x1GridLinesEnabledReg;
    x1GridLinesEnabledReg.category = tr("Plot");
    x1GridLinesEnabledReg.subcategory = tr("Axis Config");
    x1GridLinesEnabledReg.displayName = tr("X Grid Lines");
    x1GridLinesEnabledReg.briefDesc = tr("X axis grid lines visible");
    x1GridLinesEnabledReg.detailDesc = tr("When enabled, grid lines are displayed on the X1 axis");
    x1GridLinesEnabledReg.editorType = EditorType::CheckBox;
    x1GridLinesEnabledReg.defaultValue = m_x1GridLinesEnabled;
    x1GridLinesEnabledReg.propertyName = "x1GridLinesEnabled";
    x1GridLinesEnabledReg.target = this;
    registerProperty(x1GridLinesEnabledReg);
    
    // Register X1 tick marks enabled property
    PropertyRegistration x1TickMarksEnabledReg;
    x1TickMarksEnabledReg.category = tr("Plot");
    x1TickMarksEnabledReg.subcategory = tr("Axis Config");
    x1TickMarksEnabledReg.displayName = tr("X Tick Marks");
    x1TickMarksEnabledReg.briefDesc = tr("X axis tick marks visible");
    x1TickMarksEnabledReg.detailDesc = tr("When enabled, tick marks are displayed on the X1 axis");
    x1TickMarksEnabledReg.editorType = EditorType::CheckBox;
    x1TickMarksEnabledReg.defaultValue = m_x1TickMarksEnabled;
    x1TickMarksEnabledReg.propertyName = "x1TickMarksEnabled";
    x1TickMarksEnabledReg.target = this;
    registerProperty(x1TickMarksEnabledReg);
    
    // Register X1 tick labels enabled property
    PropertyRegistration x1TickLabelsEnabledReg;
    x1TickLabelsEnabledReg.category = tr("Plot");
    x1TickLabelsEnabledReg.subcategory = tr("Axis Config");
    x1TickLabelsEnabledReg.displayName = tr("X Tick Labels");
    x1TickLabelsEnabledReg.briefDesc = tr("X axis tick labels visible");
    x1TickLabelsEnabledReg.detailDesc = tr("When enabled, tick labels (value text) are displayed on the X1 axis");
    x1TickLabelsEnabledReg.editorType = EditorType::CheckBox;
    x1TickLabelsEnabledReg.defaultValue = m_x1TickLabelsEnabled;
    x1TickLabelsEnabledReg.propertyName = "x1TickLabelsEnabled";
    x1TickLabelsEnabledReg.target = this;
    registerProperty(x1TickLabelsEnabledReg);
    
    // Register X1 lock min property
    PropertyRegistration x1LockMinReg;
    x1LockMinReg.category = tr("Plot");
    x1LockMinReg.subcategory = tr("Axis Config");
    x1LockMinReg.displayName = tr("X Lock Min");
    x1LockMinReg.briefDesc = tr("X axis lock minimum value");
    x1LockMinReg.detailDesc = tr("When enabled, the X1 axis minimum value is locked and cannot be changed by user interaction");
    x1LockMinReg.editorType = EditorType::CheckBox;
    x1LockMinReg.defaultValue = m_x1LockMin;
    x1LockMinReg.propertyName = "x1LockMin";
    x1LockMinReg.target = this;
    registerProperty(x1LockMinReg);
    
    // Register X1 lock max property
    PropertyRegistration x1LockMaxReg;
    x1LockMaxReg.category = tr("Plot");
    x1LockMaxReg.subcategory = tr("Axis Config");
    x1LockMaxReg.displayName = tr("X Lock Max");
    x1LockMaxReg.briefDesc = tr("X axis lock maximum value");
    x1LockMaxReg.detailDesc = tr("When enabled, the X1 axis maximum value is locked and cannot be changed by user interaction");
    x1LockMaxReg.editorType = EditorType::CheckBox;
    x1LockMaxReg.defaultValue = m_x1LockMax;
    x1LockMaxReg.propertyName = "x1LockMax";
    x1LockMaxReg.target = this;
    registerProperty(x1LockMaxReg);
    
    // Register X1 no decorations property
    PropertyRegistration x1NoDecorationsReg;
    x1NoDecorationsReg.category = tr("Plot");
    x1NoDecorationsReg.subcategory = tr("Axis Config");
    x1NoDecorationsReg.displayName = tr("X No Decorations");
    x1NoDecorationsReg.briefDesc = tr("X axis no decorations");
    x1NoDecorationsReg.detailDesc = tr("When enabled, all decorations (labels, ticks, grid) are hidden on the X1 axis");
    x1NoDecorationsReg.editorType = EditorType::CheckBox;
    x1NoDecorationsReg.defaultValue = m_x1NoDecorations;
    x1NoDecorationsReg.propertyName = "x1NoDecorations";
    x1NoDecorationsReg.target = this;
    registerProperty(x1NoDecorationsReg);
    
    // Register X1 scale type property
    PropertyRegistration x1ScaleTypeReg;
    x1ScaleTypeReg.category = tr("Plot");
    x1ScaleTypeReg.subcategory = tr("Axis Config");
    x1ScaleTypeReg.displayName = tr("X Scale Type");
    x1ScaleTypeReg.briefDesc = tr("X axis scale type");
    x1ScaleTypeReg.detailDesc = tr("Sets the scale transformation for the X1 axis: Linear, Time, Log10, or SymLog");
    x1ScaleTypeReg.editorType = EditorType::EnumComboBox;
    x1ScaleTypeReg.comboBoxOptions = QStringList{
        tr("Linear"),   // QImPlotScaleType::Linear = 0
        tr("Time"),     // QImPlotScaleType::Time = 1
        tr("Log10"),    // QImPlotScaleType::Log10 = 2
        tr("SymLog"),   // QImPlotScaleType::SymLog = 3
    };
    x1ScaleTypeReg.defaultValue = static_cast<int>(m_x1ScaleType);
    x1ScaleTypeReg.propertyName = "x1ScaleType";
    x1ScaleTypeReg.target = this;
    registerProperty(x1ScaleTypeReg);
    
    // === QImPlotAxisInfo Y1: Limits properties ===
    
    // Register Y1 min limits property
    PropertyRegistration y1MinLimitsReg;
    y1MinLimitsReg.category = tr("Plot");
    y1MinLimitsReg.subcategory = tr("Axis Config");
    y1MinLimitsReg.displayName = tr("Y Min Limits");
    y1MinLimitsReg.briefDesc = tr("Y axis minimum limit");
    y1MinLimitsReg.detailDesc = tr("Sets the minimum visible value on the Y1 axis");
    y1MinLimitsReg.editorType = EditorType::DoubleSpinBox;
    y1MinLimitsReg.defaultValue = m_y1MinLimits;
    y1MinLimitsReg.minValue = -1e6;
    y1MinLimitsReg.maxValue = 1e6;
    y1MinLimitsReg.stepValue = 1.0;
    y1MinLimitsReg.propertyName = "y1MinLimits";
    y1MinLimitsReg.target = this;
    registerProperty(y1MinLimitsReg);
    
    // Register Y1 max limits property
    PropertyRegistration y1MaxLimitsReg;
    y1MaxLimitsReg.category = tr("Plot");
    y1MaxLimitsReg.subcategory = tr("Axis Config");
    y1MaxLimitsReg.displayName = tr("Y Max Limits");
    y1MaxLimitsReg.briefDesc = tr("Y axis maximum limit");
    y1MaxLimitsReg.detailDesc = tr("Sets the maximum visible value on the Y1 axis");
    y1MaxLimitsReg.editorType = EditorType::DoubleSpinBox;
    y1MaxLimitsReg.defaultValue = m_y1MaxLimits;
    y1MaxLimitsReg.minValue = -1e6;
    y1MaxLimitsReg.maxValue = 1e6;
    y1MaxLimitsReg.stepValue = 1.0;
    y1MaxLimitsReg.propertyName = "y1MaxLimits";
    y1MaxLimitsReg.target = this;
    registerProperty(y1MaxLimitsReg);
    
    // === QImPlotAxisInfo Y1: Flags properties ===
    
    // Register Y1 auto fit property
    PropertyRegistration y1AutoFitReg;
    y1AutoFitReg.category = tr("Plot");
    y1AutoFitReg.subcategory = tr("Axis Config");
    y1AutoFitReg.displayName = tr("Y Auto Fit");
    y1AutoFitReg.briefDesc = tr("Y axis auto fit");
    y1AutoFitReg.detailDesc = tr("When enabled, the Y1 axis automatically adjusts its range to fit all data points");
    y1AutoFitReg.editorType = EditorType::CheckBox;
    y1AutoFitReg.defaultValue = m_y1AutoFit;
    y1AutoFitReg.propertyName = "y1AutoFit";
    y1AutoFitReg.target = this;
    registerProperty(y1AutoFitReg);
    
    // Register Y1 inverted property
    PropertyRegistration y1InvertedReg;
    y1InvertedReg.category = tr("Plot");
    y1InvertedReg.subcategory = tr("Axis Config");
    y1InvertedReg.displayName = tr("Y Inverted");
    y1InvertedReg.briefDesc = tr("Y axis inverted direction");
    y1InvertedReg.detailDesc = tr("When enabled, the Y1 axis direction is inverted (values increase bottom to top)");
    y1InvertedReg.editorType = EditorType::CheckBox;
    y1InvertedReg.defaultValue = m_y1Inverted;
    y1InvertedReg.propertyName = "y1Inverted";
    y1InvertedReg.target = this;
    registerProperty(y1InvertedReg);
    
    // Register Y1 label enabled property
    PropertyRegistration y1LabelEnabledReg;
    y1LabelEnabledReg.category = tr("Plot");
    y1LabelEnabledReg.subcategory = tr("Axis Config");
    y1LabelEnabledReg.displayName = tr("Y Label Visible");
    y1LabelEnabledReg.briefDesc = tr("Y axis label visible");
    y1LabelEnabledReg.detailDesc = tr("When enabled, the Y1 axis label text is displayed");
    y1LabelEnabledReg.editorType = EditorType::CheckBox;
    y1LabelEnabledReg.defaultValue = m_y1LabelEnabled;
    y1LabelEnabledReg.propertyName = "y1LabelEnabled";
    y1LabelEnabledReg.target = this;
    registerProperty(y1LabelEnabledReg);
    
    // Register Y1 grid lines enabled property
    PropertyRegistration y1GridLinesEnabledReg;
    y1GridLinesEnabledReg.category = tr("Plot");
    y1GridLinesEnabledReg.subcategory = tr("Axis Config");
    y1GridLinesEnabledReg.displayName = tr("Y Grid Lines");
    y1GridLinesEnabledReg.briefDesc = tr("Y axis grid lines visible");
    y1GridLinesEnabledReg.detailDesc = tr("When enabled, grid lines are displayed on the Y1 axis");
    y1GridLinesEnabledReg.editorType = EditorType::CheckBox;
    y1GridLinesEnabledReg.defaultValue = m_y1GridLinesEnabled;
    y1GridLinesEnabledReg.propertyName = "y1GridLinesEnabled";
    y1GridLinesEnabledReg.target = this;
    registerProperty(y1GridLinesEnabledReg);
    
    // Register Y1 tick marks enabled property
    PropertyRegistration y1TickMarksEnabledReg;
    y1TickMarksEnabledReg.category = tr("Plot");
    y1TickMarksEnabledReg.subcategory = tr("Axis Config");
    y1TickMarksEnabledReg.displayName = tr("Y Tick Marks");
    y1TickMarksEnabledReg.briefDesc = tr("Y axis tick marks visible");
    y1TickMarksEnabledReg.detailDesc = tr("When enabled, tick marks are displayed on the Y1 axis");
    y1TickMarksEnabledReg.editorType = EditorType::CheckBox;
    y1TickMarksEnabledReg.defaultValue = m_y1TickMarksEnabled;
    y1TickMarksEnabledReg.propertyName = "y1TickMarksEnabled";
    y1TickMarksEnabledReg.target = this;
    registerProperty(y1TickMarksEnabledReg);
    
    // Register Y1 tick labels enabled property
    PropertyRegistration y1TickLabelsEnabledReg;
    y1TickLabelsEnabledReg.category = tr("Plot");
    y1TickLabelsEnabledReg.subcategory = tr("Axis Config");
    y1TickLabelsEnabledReg.displayName = tr("Y Tick Labels");
    y1TickLabelsEnabledReg.briefDesc = tr("Y axis tick labels visible");
    y1TickLabelsEnabledReg.detailDesc = tr("When enabled, tick labels (value text) are displayed on the Y1 axis");
    y1TickLabelsEnabledReg.editorType = EditorType::CheckBox;
    y1TickLabelsEnabledReg.defaultValue = m_y1TickLabelsEnabled;
    y1TickLabelsEnabledReg.propertyName = "y1TickLabelsEnabled";
    y1TickLabelsEnabledReg.target = this;
    registerProperty(y1TickLabelsEnabledReg);
    
    // Register Y1 lock min property
    PropertyRegistration y1LockMinReg;
    y1LockMinReg.category = tr("Plot");
    y1LockMinReg.subcategory = tr("Axis Config");
    y1LockMinReg.displayName = tr("Y Lock Min");
    y1LockMinReg.briefDesc = tr("Y axis lock minimum value");
    y1LockMinReg.detailDesc = tr("When enabled, the Y1 axis minimum value is locked and cannot be changed by user interaction");
    y1LockMinReg.editorType = EditorType::CheckBox;
    y1LockMinReg.defaultValue = m_y1LockMin;
    y1LockMinReg.propertyName = "y1LockMin";
    y1LockMinReg.target = this;
    registerProperty(y1LockMinReg);
    
    // Register Y1 lock max property
    PropertyRegistration y1LockMaxReg;
    y1LockMaxReg.category = tr("Plot");
    y1LockMaxReg.subcategory = tr("Axis Config");
    y1LockMaxReg.displayName = tr("Y Lock Max");
    y1LockMaxReg.briefDesc = tr("Y axis lock maximum value");
    y1LockMaxReg.detailDesc = tr("When enabled, the Y1 axis maximum value is locked and cannot be changed by user interaction");
    y1LockMaxReg.editorType = EditorType::CheckBox;
    y1LockMaxReg.defaultValue = m_y1LockMax;
    y1LockMaxReg.propertyName = "y1LockMax";
    y1LockMaxReg.target = this;
    registerProperty(y1LockMaxReg);
    
    // Register Y1 no decorations property
    PropertyRegistration y1NoDecorationsReg;
    y1NoDecorationsReg.category = tr("Plot");
    y1NoDecorationsReg.subcategory = tr("Axis Config");
    y1NoDecorationsReg.displayName = tr("Y No Decorations");
    y1NoDecorationsReg.briefDesc = tr("Y axis no decorations");
    y1NoDecorationsReg.detailDesc = tr("When enabled, all decorations (labels, ticks, grid) are hidden on the Y1 axis");
    y1NoDecorationsReg.editorType = EditorType::CheckBox;
    y1NoDecorationsReg.defaultValue = m_y1NoDecorations;
    y1NoDecorationsReg.propertyName = "y1NoDecorations";
    y1NoDecorationsReg.target = this;
    registerProperty(y1NoDecorationsReg);
    
    // Register Y1 scale type property
    PropertyRegistration y1ScaleTypeReg;
    y1ScaleTypeReg.category = tr("Plot");
    y1ScaleTypeReg.subcategory = tr("Axis Config");
    y1ScaleTypeReg.displayName = tr("Y Scale Type");
    y1ScaleTypeReg.briefDesc = tr("Y axis scale type");
    y1ScaleTypeReg.detailDesc = tr("Sets the scale transformation for the Y1 axis: Linear, Time, Log10, or SymLog");
    y1ScaleTypeReg.editorType = EditorType::EnumComboBox;
    y1ScaleTypeReg.comboBoxOptions = QStringList{
        tr("Linear"),   // QImPlotScaleType::Linear = 0
        tr("Time"),     // QImPlotScaleType::Time = 1
        tr("Log10"),    // QImPlotScaleType::Log10 = 2
        tr("SymLog"),   // QImPlotScaleType::SymLog = 3
    };
    y1ScaleTypeReg.defaultValue = static_cast<int>(m_y1ScaleType);
    y1ScaleTypeReg.propertyName = "y1ScaleType";
    y1ScaleTypeReg.target = this;
    registerProperty(y1ScaleTypeReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for ScatterFunction
 * \endif
 * 
 * \if CHINESE
 * @brief ScatterFunction 析构函数
 * \endif
 */
ScatterFunction::~ScatterFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a plot with 1000 random scatter points, configures axes,
 * adds a scatter item with marker settings, and attaches a value tracker node.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含 1000 个随机散点的图表，配置坐标轴，
 * 添加带有标记设置的散点项，并附加一个值追踪器节点。
 * \endif
 */
void ScatterFunction::createPlot(QIM::QImFigureWidget* figure)
{
    if (!figure) {
        return;
    }
    
    // Create plot node
    m_plotNode = figure->createPlotNode();
    if (!m_plotNode) {
        return;
    }
    
    // Configure axes and title
    m_plotNode->x1Axis()->setLabel(m_xLabel);
    m_plotNode->y1Axis()->setLabel(m_yLabel);
    m_plotNode->setTitle(m_title);
    m_plotNode->setLegendEnabled(true);
    
    // Generate 1000 random scatter points
    const int numPoints = 1000;
    std::vector<double> xData(numPoints);
    std::vector<double> yData(numPoints);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> xDist(0.0, 1.0);
    std::normal_distribution<double> yDist(0.0, 1.0);
    
    for (int i = 0; i < numPoints; ++i) {
        xData[i] = xDist(gen);
        yData[i] = yDist(gen);
    }
    
    // Create scatter item node and set data
    m_scatterNode = new QIM::QImPlotScatterItemNode(m_plotNode);
    m_scatterNode->setData(xData, yData);
    m_scatterNode->setMarkerSize(m_markerSize);
    m_scatterNode->setMarkerShape(m_markerShape);
    m_scatterNode->setMarkerFill(m_markerFill);
    m_scatterNode->setColor(m_markerColor);
    m_scatterNode->setClippingEnabled(m_clippingEnabled);
    
    // Create and attach value tracker
    m_trackerNode = new QIM::QImPlotValueTrackerNode(m_plotNode);
    m_trackerNode->setGroup(nullptr);  // No group by default
    m_plotNode->addChildNode(m_trackerNode);
}

void ScatterFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        Q_EMIT titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);
        }
    }
}

void ScatterFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        Q_EMIT xLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLabel(label);
        }
    }
}

void ScatterFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        Q_EMIT yLabelChanged(label);
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLabel(label);
        }
    }
}

void ScatterFunction::setMarkerSize(float size)
{
    if (m_markerSize != size) {
        m_markerSize = size;
        Q_EMIT markerSizeChanged(size);
        if (m_scatterNode) {
            m_scatterNode->setMarkerSize(size);
        }
    }
}

void ScatterFunction::setMarkerShape(int shape)
{
    if (m_markerShape != shape) {
        m_markerShape = shape;
        Q_EMIT markerShapeChanged(shape);
        if (m_scatterNode) {
            m_scatterNode->setMarkerShape(shape);
        }
    }
}

void ScatterFunction::setMarkerFill(bool fill)
{
    if (m_markerFill != fill) {
        m_markerFill = fill;
        Q_EMIT markerFillChanged(fill);
        if (m_scatterNode) {
            m_scatterNode->setMarkerFill(fill);
        }
    }
}

void ScatterFunction::setMarkerColor(const QColor& color)
{
    if (m_markerColor != color) {
        m_markerColor = color;
        Q_EMIT markerColorChanged(color);
        if (m_scatterNode) {
            m_scatterNode->setColor(color);
        }
    }
}

void ScatterFunction::setClippingEnabled(bool enabled)
{
    if (m_clippingEnabled != enabled) {
        m_clippingEnabled = enabled;
        Q_EMIT clippingEnabledChanged(enabled);
        if (m_scatterNode) {
            m_scatterNode->setClippingEnabled(enabled);
        }
    }
}

void ScatterFunction::setDownsampleAlgorithm(int algo)
{
    if (static_cast<int>(m_downsampleAlgorithm) != algo) {
        m_downsampleAlgorithm = static_cast<QIM::QImDownsampleAlgorithm>(algo);
        Q_EMIT downsampleAlgorithmChanged(algo);
        if (m_scatterNode) {
            m_scatterNode->setDownsampleAlgorithm(static_cast<QIM::QImDownsampleAlgorithm>(algo));
        }
    }
}

void ScatterFunction::setDownsampleThreshold(int threshold)
{
    if (m_downsampleThreshold != threshold) {
        m_downsampleThreshold = threshold;
        Q_EMIT downsampleThresholdChanged(threshold);
        if (m_scatterNode) {
            m_scatterNode->setDownsampleThreshold(threshold);
        }
    }
}

// --- QImPlotNode: Size setters ---

void ScatterFunction::setPlotWidth(double width)
{
    if (m_plotSize.width() != width) {
        m_plotSize.setWidth(width);
        Q_EMIT plotSizeChanged();
        if (m_plotNode) {
            m_plotNode->setSize(m_plotSize);
        }
    }
}

void ScatterFunction::setPlotHeight(double height)
{
    if (m_plotSize.height() != height) {
        m_plotSize.setHeight(height);
        Q_EMIT plotSizeChanged();
        if (m_plotNode) {
            m_plotNode->setSize(m_plotSize);
        }
    }
}

void ScatterFunction::setAutoSize(bool autoSize)
{
    if (m_autoSize != autoSize) {
        m_autoSize = autoSize;
        Q_EMIT autoSizeChanged(autoSize);
        if (m_plotNode) {
            m_plotNode->setAutoSize(autoSize);
        }
    }
}

// --- QImPlotNode: ImPlotFlags setters ---

void ScatterFunction::setTitleEnabled(bool enabled)
{
    if (m_titleEnabled != enabled) {
        m_titleEnabled = enabled;
        Q_EMIT titleEnabledChanged(enabled);
        if (m_plotNode) {
            m_plotNode->setTitleEnabled(enabled);
        }
    }
}

void ScatterFunction::setLegendEnabled(bool enabled)
{
    if (m_legendEnabled != enabled) {
        m_legendEnabled = enabled;
        Q_EMIT legendEnabledChanged(enabled);
        if (m_plotNode) {
            m_plotNode->setLegendEnabled(enabled);
        }
    }
}

void ScatterFunction::setMouseTextEnabled(bool enabled)
{
    if (m_mouseTextEnabled != enabled) {
        m_mouseTextEnabled = enabled;
        Q_EMIT mouseTextEnabledChanged(enabled);
        if (m_plotNode) {
            m_plotNode->setMouseTextEnabled(enabled);
        }
    }
}

void ScatterFunction::setInputsEnabled(bool enabled)
{
    if (m_inputsEnabled != enabled) {
        m_inputsEnabled = enabled;
        Q_EMIT inputsEnabledChanged(enabled);
        if (m_plotNode) {
            m_plotNode->setInputsEnabled(enabled);
        }
    }
}

void ScatterFunction::setMenusEnabled(bool enabled)
{
    if (m_menusEnabled != enabled) {
        m_menusEnabled = enabled;
        Q_EMIT menusEnabledChanged(enabled);
        if (m_plotNode) {
            m_plotNode->setMenusEnabled(enabled);
        }
    }
}

void ScatterFunction::setBoxSelectEnabled(bool enabled)
{
    if (m_boxSelectEnabled != enabled) {
        m_boxSelectEnabled = enabled;
        Q_EMIT boxSelectEnabledChanged(enabled);
        if (m_plotNode) {
            m_plotNode->setBoxSelectEnabled(enabled);
        }
    }
}

void ScatterFunction::setFrameEnabled(bool enabled)
{
    if (m_frameEnabled != enabled) {
        m_frameEnabled = enabled;
        Q_EMIT frameEnabledChanged(enabled);
        if (m_plotNode) {
            m_plotNode->setFrameEnabled(enabled);
        }
    }
}

void ScatterFunction::setEqual(bool on)
{
    if (m_equal != on) {
        m_equal = on;
        Q_EMIT equalChanged(on);
        if (m_plotNode) {
            m_plotNode->setEqual(on);
        }
    }
}

void ScatterFunction::setCrosshairs(bool on)
{
    if (m_crosshairs != on) {
        m_crosshairs = on;
        Q_EMIT crosshairsChanged(on);
        if (m_plotNode) {
            m_plotNode->setCrosshairs(on);
        }
    }
}

void ScatterFunction::setCanvasEnabled(bool enabled)
{
    if (m_canvasEnabled != enabled) {
        m_canvasEnabled = enabled;
        Q_EMIT canvasEnabledChanged(enabled);
        if (m_plotNode) {
            m_plotNode->setCanvasEnabled(enabled);
        }
    }
}

// --- QImPlotAxisInfo X1: Limits setters ---

void ScatterFunction::setX1MinLimits(double min)
{
    if (m_x1MinLimits != min) {
        m_x1MinLimits = min;
        Q_EMIT x1LimitsChanged(m_x1MinLimits, m_x1MaxLimits);
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLimits(m_x1MinLimits, m_x1MaxLimits);
        }
    }
}

void ScatterFunction::setX1MaxLimits(double max)
{
    if (m_x1MaxLimits != max) {
        m_x1MaxLimits = max;
        Q_EMIT x1LimitsChanged(m_x1MinLimits, m_x1MaxLimits);
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLimits(m_x1MinLimits, m_x1MaxLimits);
        }
    }
}

// --- QImPlotAxisInfo X1: Flags setters ---

void ScatterFunction::setX1AutoFit(bool on)
{
    if (m_x1AutoFit != on) {
        m_x1AutoFit = on;
        Q_EMIT x1AxisFlagChanged();
        if (m_plotNode) {
            m_plotNode->x1Axis()->setAutoFit(on);
        }
    }
}

void ScatterFunction::setX1Inverted(bool on)
{
    if (m_x1Inverted != on) {
        m_x1Inverted = on;
        Q_EMIT x1AxisFlagChanged();
        if (m_plotNode) {
            m_plotNode->x1Axis()->setInverted(on);
        }
    }
}

void ScatterFunction::setX1LabelEnabled(bool enabled)
{
    if (m_x1LabelEnabled != enabled) {
        m_x1LabelEnabled = enabled;
        Q_EMIT x1AxisFlagChanged();
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLabelEnabled(enabled);
        }
    }
}

void ScatterFunction::setX1GridLinesEnabled(bool enabled)
{
    if (m_x1GridLinesEnabled != enabled) {
        m_x1GridLinesEnabled = enabled;
        Q_EMIT x1AxisFlagChanged();
        if (m_plotNode) {
            m_plotNode->x1Axis()->setGridLinesEnabled(enabled);
        }
    }
}

void ScatterFunction::setX1TickMarksEnabled(bool enabled)
{
    if (m_x1TickMarksEnabled != enabled) {
        m_x1TickMarksEnabled = enabled;
        Q_EMIT x1AxisFlagChanged();
        if (m_plotNode) {
            m_plotNode->x1Axis()->setTickMarksEnabled(enabled);
        }
    }
}

void ScatterFunction::setX1TickLabelsEnabled(bool enabled)
{
    if (m_x1TickLabelsEnabled != enabled) {
        m_x1TickLabelsEnabled = enabled;
        Q_EMIT x1AxisFlagChanged();
        if (m_plotNode) {
            m_plotNode->x1Axis()->setTickLabelsEnabled(enabled);
        }
    }
}

void ScatterFunction::setX1LockMin(bool on)
{
    if (m_x1LockMin != on) {
        m_x1LockMin = on;
        Q_EMIT x1AxisFlagChanged();
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLockMin(on);
        }
    }
}

void ScatterFunction::setX1LockMax(bool on)
{
    if (m_x1LockMax != on) {
        m_x1LockMax = on;
        Q_EMIT x1AxisFlagChanged();
        if (m_plotNode) {
            m_plotNode->x1Axis()->setLockMax(on);
        }
    }
}

void ScatterFunction::setX1NoDecorations(bool enabled)
{
    if (m_x1NoDecorations != enabled) {
        m_x1NoDecorations = enabled;
        Q_EMIT x1AxisFlagChanged();
        if (m_plotNode) {
            m_plotNode->x1Axis()->setNoDecorations(enabled);
        }
    }
}

void ScatterFunction::setX1ScaleType(int type)
{
    auto scaleType = static_cast<QIM::QImPlotScaleType>(type);
    if (m_x1ScaleType != scaleType) {
        m_x1ScaleType = scaleType;
        Q_EMIT x1ScaleTypeChanged();
        if (m_plotNode) {
            m_plotNode->x1Axis()->setScaleType(scaleType);
        }
    }
}

// --- QImPlotAxisInfo Y1: Limits setters ---

void ScatterFunction::setY1MinLimits(double min)
{
    if (m_y1MinLimits != min) {
        m_y1MinLimits = min;
        Q_EMIT y1LimitsChanged(m_y1MinLimits, m_y1MaxLimits);
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLimits(m_y1MinLimits, m_y1MaxLimits);
        }
    }
}

void ScatterFunction::setY1MaxLimits(double max)
{
    if (m_y1MaxLimits != max) {
        m_y1MaxLimits = max;
        Q_EMIT y1LimitsChanged(m_y1MinLimits, m_y1MaxLimits);
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLimits(m_y1MinLimits, m_y1MaxLimits);
        }
    }
}

// --- QImPlotAxisInfo Y1: Flags setters ---

void ScatterFunction::setY1AutoFit(bool on)
{
    if (m_y1AutoFit != on) {
        m_y1AutoFit = on;
        Q_EMIT y1AxisFlagChanged();
        if (m_plotNode) {
            m_plotNode->y1Axis()->setAutoFit(on);
        }
    }
}

void ScatterFunction::setY1Inverted(bool on)
{
    if (m_y1Inverted != on) {
        m_y1Inverted = on;
        Q_EMIT y1AxisFlagChanged();
        if (m_plotNode) {
            m_plotNode->y1Axis()->setInverted(on);
        }
    }
}

void ScatterFunction::setY1LabelEnabled(bool enabled)
{
    if (m_y1LabelEnabled != enabled) {
        m_y1LabelEnabled = enabled;
        Q_EMIT y1AxisFlagChanged();
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLabelEnabled(enabled);
        }
    }
}

void ScatterFunction::setY1GridLinesEnabled(bool enabled)
{
    if (m_y1GridLinesEnabled != enabled) {
        m_y1GridLinesEnabled = enabled;
        Q_EMIT y1AxisFlagChanged();
        if (m_plotNode) {
            m_plotNode->y1Axis()->setGridLinesEnabled(enabled);
        }
    }
}

void ScatterFunction::setY1TickMarksEnabled(bool enabled)
{
    if (m_y1TickMarksEnabled != enabled) {
        m_y1TickMarksEnabled = enabled;
        Q_EMIT y1AxisFlagChanged();
        if (m_plotNode) {
            m_plotNode->y1Axis()->setTickMarksEnabled(enabled);
        }
    }
}

void ScatterFunction::setY1TickLabelsEnabled(bool enabled)
{
    if (m_y1TickLabelsEnabled != enabled) {
        m_y1TickLabelsEnabled = enabled;
        Q_EMIT y1AxisFlagChanged();
        if (m_plotNode) {
            m_plotNode->y1Axis()->setTickLabelsEnabled(enabled);
        }
    }
}

void ScatterFunction::setY1LockMin(bool on)
{
    if (m_y1LockMin != on) {
        m_y1LockMin = on;
        Q_EMIT y1AxisFlagChanged();
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLockMin(on);
        }
    }
}

void ScatterFunction::setY1LockMax(bool on)
{
    if (m_y1LockMax != on) {
        m_y1LockMax = on;
        Q_EMIT y1AxisFlagChanged();
        if (m_plotNode) {
            m_plotNode->y1Axis()->setLockMax(on);
        }
    }
}

void ScatterFunction::setY1NoDecorations(bool enabled)
{
    if (m_y1NoDecorations != enabled) {
        m_y1NoDecorations = enabled;
        Q_EMIT y1AxisFlagChanged();
        if (m_plotNode) {
            m_plotNode->y1Axis()->setNoDecorations(enabled);
        }
    }
}

void ScatterFunction::setY1ScaleType(int type)
{
    auto scaleType = static_cast<QIM::QImPlotScaleType>(type);
    if (m_y1ScaleType != scaleType) {
        m_y1ScaleType = scaleType;
        Q_EMIT y1ScaleTypeChanged();
        if (m_plotNode) {
            m_plotNode->y1Axis()->setScaleType(scaleType);
        }
    }
}
