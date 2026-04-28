#include "Plot3DScatterFunction.h"
#include "QImFigureWidget.h"
#include "plot3d/QImPlot3DNode.h"
#include "plot3d/QImPlot3DAxisInfo.h"
#include "plot3d/QImPlot3DScatterItemNode.h"
#include "plot3d/QImPlot3DDataSeries.h"
#include <random>

/**
 * \if ENGLISH
 * @brief Constructor for Plot3DScatterFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Plot"/"Axis"/"Z Label" - Z axis label
 * - "Scatter"/"Marker"/"Size" - Marker size
 * - "Scatter"/"Marker"/"Shape" - Marker shape
 * - "Scatter"/"Marker"/"Fill Color" - Marker fill color
 * - "Scatter"/"Marker"/"Outline Color" - Marker outline color
 * - "Scatter"/"Marker"/"Weight" - Marker outline weight
 * - "3D Plot"/"Size" - Plot width, height, auto size
 * - "3D Plot"/"Flags" - Boolean flags (title, legend, mouse text, clip, menus, rotate, pan, zoom, inputs, equal, canvas)
 * - "3D Plot"/"Legend" - Legend location and flags
 * - "3D Plot"/"Axis" - X axis properties (limits, labels, grid, ticks, lock, auto-fit, invert, decorations, scale)
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DScatterFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Plot"/"Axis"/"Z Label" - Z 轴标签
 * - "Scatter"/"Marker"/"Size" - 标记大小
 * - "Scatter"/"Marker"/"Shape" - 标记形状
 * - "Scatter"/"Marker"/"Fill Color" - 标记填充颜色
 * - "Scatter"/"Marker"/"Outline Color" - 标记描边颜色
 * - "Scatter"/"Marker"/"Weight" - 标记描边粗细
 * - "3D Plot"/"Size" - 图表宽度、高度、自动尺寸
 * - "3D Plot"/"Flags" - 布尔标志（标题、图例、鼠标文本、裁剪、菜单、旋转、平移、缩放、输入、等比、画布）
 * - "3D Plot"/"Legend" - 图例位置和标志
 * - "3D Plot"/"Axis" - X 轴属性（范围限制、标签、网格、刻度、锁定、自动适配、反转、装饰、刻度类型）
 * \endif
 */
Plot3DScatterFunction::Plot3DScatterFunction(QObject* parent)
    : TestFunction(parent)
{
    // Register title property
    PropertyRegistration titleReg;
    titleReg.category = tr("Plot");
    titleReg.subcategory = tr("Basic Info");
    titleReg.displayName = tr("Title");
    titleReg.briefDesc = tr("Plot title");
    titleReg.detailDesc = tr("Sets the title text displayed at the top of the 3D plot");
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
    
    // Register Z axis label property
    PropertyRegistration zLabelReg;
    zLabelReg.category = tr("Plot");
    zLabelReg.subcategory = tr("Axis");
    zLabelReg.displayName = tr("Z Label");
    zLabelReg.briefDesc = tr("Z axis label");
    zLabelReg.detailDesc = tr("Sets the label text for the Z axis");
    zLabelReg.editorType = EditorType::LineEdit;
    zLabelReg.defaultValue = m_zLabel;
    zLabelReg.propertyName = "zLabel";
    zLabelReg.target = this;
    registerProperty(zLabelReg);
    
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
        tr("Circle"),     // QImPlot3DMarkerShape::Circle = 0
        tr("Square"),     // QImPlot3DMarkerShape::Square = 1
        tr("Diamond"),    // QImPlot3DMarkerShape::Diamond = 2
        tr("Up"),         // QImPlot3DMarkerShape::Up = 3
        tr("Down"),       // QImPlot3DMarkerShape::Down = 4
        tr("Left"),       // QImPlot3DMarkerShape::Left = 5
        tr("Right"),      // QImPlot3DMarkerShape::Right = 6
        tr("Cross"),      // QImPlot3DMarkerShape::Cross = 7
        tr("Plus"),       // QImPlot3DMarkerShape::Plus = 8
        tr("Asterisk"),   // QImPlot3DMarkerShape::Asterisk = 9
    };
    shapeReg.defaultValue = m_markerShape;  // 0 = Circle by default
    shapeReg.propertyName = "markerShape";
    shapeReg.target = this;
    registerProperty(shapeReg);
    
    // Register marker fill color property
    PropertyRegistration fillColorReg;
    fillColorReg.category = tr("Scatter");
    fillColorReg.subcategory = tr("Marker");
    fillColorReg.displayName = tr("Fill Color");
    fillColorReg.briefDesc = tr("Marker fill color");
    fillColorReg.detailDesc = tr("Sets the fill color of scatter plot markers");
    fillColorReg.editorType = EditorType::ColorPicker;
    fillColorReg.defaultValue = m_markerFillColor;
    fillColorReg.propertyName = "markerFillColor";
    fillColorReg.target = this;
    registerProperty(fillColorReg);
    
    // Register marker outline color property
    PropertyRegistration outlineColorReg;
    outlineColorReg.category = tr("Scatter");
    outlineColorReg.subcategory = tr("Marker");
    outlineColorReg.displayName = tr("Outline Color");
    outlineColorReg.briefDesc = tr("Marker outline color");
    outlineColorReg.detailDesc = tr("Sets the outline color of scatter plot markers");
    outlineColorReg.editorType = EditorType::ColorPicker;
    outlineColorReg.defaultValue = m_markerOutlineColor;
    outlineColorReg.propertyName = "markerOutlineColor";
    outlineColorReg.target = this;
    registerProperty(outlineColorReg);
    
    // Register marker weight property
    PropertyRegistration weightReg;
    weightReg.category = tr("Scatter");
    weightReg.subcategory = tr("Marker");
    weightReg.displayName = tr("Weight");
    weightReg.briefDesc = tr("Marker outline weight");
    weightReg.detailDesc = tr("Sets the outline thickness of scatter plot markers in pixels");
    weightReg.editorType = EditorType::DoubleSpinBox;
    weightReg.defaultValue = m_markerWeight;
    weightReg.minValue = 0.0;
    weightReg.maxValue = 20.0;
    weightReg.stepValue = 0.5;
    weightReg.propertyName = "markerWeight";
    weightReg.target = this;
    registerProperty(weightReg);

    // === QImPlot3DNode - Size properties ===

    // Register plot width property
    PropertyRegistration plotWidthReg;
    plotWidthReg.category = tr("3D Plot");
    plotWidthReg.subcategory = tr("Size");
    plotWidthReg.displayName = tr("Width");
    plotWidthReg.briefDesc = tr("Plot width");
    plotWidthReg.detailDesc = tr("Sets the width of the 3D plot frame");
    plotWidthReg.editorType = EditorType::DoubleSpinBox;
    plotWidthReg.defaultValue = m_plotWidth;
    plotWidthReg.minValue = 100.0;
    plotWidthReg.maxValue = 2000.0;
    plotWidthReg.stepValue = 10.0;
    plotWidthReg.propertyName = "plotWidth";
    plotWidthReg.target = this;
    registerProperty(plotWidthReg);

    // Register plot height property
    PropertyRegistration plotHeightReg;
    plotHeightReg.category = tr("3D Plot");
    plotHeightReg.subcategory = tr("Size");
    plotHeightReg.displayName = tr("Height");
    plotHeightReg.briefDesc = tr("Plot height");
    plotHeightReg.detailDesc = tr("Sets the height of the 3D plot frame");
    plotHeightReg.editorType = EditorType::DoubleSpinBox;
    plotHeightReg.defaultValue = m_plotHeight;
    plotHeightReg.minValue = 100.0;
    plotHeightReg.maxValue = 2000.0;
    plotHeightReg.stepValue = 10.0;
    plotHeightReg.propertyName = "plotHeight";
    plotHeightReg.target = this;
    registerProperty(plotHeightReg);

    // Register auto size property
    PropertyRegistration autoSizeReg;
    autoSizeReg.category = tr("3D Plot");
    autoSizeReg.subcategory = tr("Size");
    autoSizeReg.displayName = tr("Auto Size");
    autoSizeReg.briefDesc = tr("Auto size mode");
    autoSizeReg.detailDesc = tr("When enabled, the plot automatically sizes to fill available space");
    autoSizeReg.editorType = EditorType::CheckBox;
    autoSizeReg.defaultValue = m_autoSize;
    autoSizeReg.propertyName = "autoSize";
    autoSizeReg.target = this;
    registerProperty(autoSizeReg);

    // === QImPlot3DNode - Flags properties ===

    // Register title enabled property
    PropertyRegistration titleEnabledReg;
    titleEnabledReg.category = tr("3D Plot");
    titleEnabledReg.subcategory = tr("Flags");
    titleEnabledReg.displayName = tr("Title Enabled");
    titleEnabledReg.briefDesc = tr("Show plot title");
    titleEnabledReg.detailDesc = tr("When enabled, the plot title is displayed");
    titleEnabledReg.editorType = EditorType::CheckBox;
    titleEnabledReg.defaultValue = m_titleEnabled;
    titleEnabledReg.propertyName = "titleEnabled";
    titleEnabledReg.target = this;
    registerProperty(titleEnabledReg);

    // Register legend enabled property
    PropertyRegistration legendEnabledReg;
    legendEnabledReg.category = tr("3D Plot");
    legendEnabledReg.subcategory = tr("Flags");
    legendEnabledReg.displayName = tr("Legend Enabled");
    legendEnabledReg.briefDesc = tr("Show legend");
    legendEnabledReg.detailDesc = tr("When enabled, the plot legend is displayed");
    legendEnabledReg.editorType = EditorType::CheckBox;
    legendEnabledReg.defaultValue = m_legendEnabled;
    legendEnabledReg.propertyName = "legendEnabled";
    legendEnabledReg.target = this;
    registerProperty(legendEnabledReg);

    // Register mouse text enabled property
    PropertyRegistration mouseTextEnabledReg;
    mouseTextEnabledReg.category = tr("3D Plot");
    mouseTextEnabledReg.subcategory = tr("Flags");
    mouseTextEnabledReg.displayName = tr("Mouse Text Enabled");
    mouseTextEnabledReg.briefDesc = tr("Show mouse coordinates");
    mouseTextEnabledReg.detailDesc = tr("When enabled, mouse coordinate text is shown on the plot");
    mouseTextEnabledReg.editorType = EditorType::CheckBox;
    mouseTextEnabledReg.defaultValue = m_mouseTextEnabled;
    mouseTextEnabledReg.propertyName = "mouseTextEnabled";
    mouseTextEnabledReg.target = this;
    registerProperty(mouseTextEnabledReg);

    // Register clip enabled property
    PropertyRegistration clipEnabledReg;
    clipEnabledReg.category = tr("3D Plot");
    clipEnabledReg.subcategory = tr("Flags");
    clipEnabledReg.displayName = tr("Clip Enabled");
    clipEnabledReg.briefDesc = tr("Clip to plot area");
    clipEnabledReg.detailDesc = tr("When enabled, plot items are clipped to the plot frame area");
    clipEnabledReg.editorType = EditorType::CheckBox;
    clipEnabledReg.defaultValue = m_clipEnabled;
    clipEnabledReg.propertyName = "clipEnabled";
    clipEnabledReg.target = this;
    registerProperty(clipEnabledReg);

    // Register menus enabled property
    PropertyRegistration menusEnabledReg;
    menusEnabledReg.category = tr("3D Plot");
    menusEnabledReg.subcategory = tr("Flags");
    menusEnabledReg.displayName = tr("Menus Enabled");
    menusEnabledReg.briefDesc = tr("Show context menus");
    menusEnabledReg.detailDesc = tr("When enabled, right-click context menus are available");
    menusEnabledReg.editorType = EditorType::CheckBox;
    menusEnabledReg.defaultValue = m_menusEnabled;
    menusEnabledReg.propertyName = "menusEnabled";
    menusEnabledReg.target = this;
    registerProperty(menusEnabledReg);

    // Register rotate enabled property
    PropertyRegistration rotateEnabledReg;
    rotateEnabledReg.category = tr("3D Plot");
    rotateEnabledReg.subcategory = tr("Flags");
    rotateEnabledReg.displayName = tr("Rotate Enabled");
    rotateEnabledReg.briefDesc = tr("Allow rotation");
    rotateEnabledReg.detailDesc = tr("When enabled, 3D rotation interaction is allowed");
    rotateEnabledReg.editorType = EditorType::CheckBox;
    rotateEnabledReg.defaultValue = m_rotateEnabled;
    rotateEnabledReg.propertyName = "rotateEnabled";
    rotateEnabledReg.target = this;
    registerProperty(rotateEnabledReg);

    // Register pan enabled property
    PropertyRegistration panEnabledReg;
    panEnabledReg.category = tr("3D Plot");
    panEnabledReg.subcategory = tr("Flags");
    panEnabledReg.displayName = tr("Pan Enabled");
    panEnabledReg.briefDesc = tr("Allow panning");
    panEnabledReg.detailDesc = tr("When enabled, pan interaction is allowed");
    panEnabledReg.editorType = EditorType::CheckBox;
    panEnabledReg.defaultValue = m_panEnabled;
    panEnabledReg.propertyName = "panEnabled";
    panEnabledReg.target = this;
    registerProperty(panEnabledReg);

    // Register zoom enabled property
    PropertyRegistration zoomEnabledReg;
    zoomEnabledReg.category = tr("3D Plot");
    zoomEnabledReg.subcategory = tr("Flags");
    zoomEnabledReg.displayName = tr("Zoom Enabled");
    zoomEnabledReg.briefDesc = tr("Allow zoom");
    zoomEnabledReg.detailDesc = tr("When enabled, zoom interaction is allowed");
    zoomEnabledReg.editorType = EditorType::CheckBox;
    zoomEnabledReg.defaultValue = m_zoomEnabled;
    zoomEnabledReg.propertyName = "zoomEnabled";
    zoomEnabledReg.target = this;
    registerProperty(zoomEnabledReg);

    // Register inputs enabled property
    PropertyRegistration inputsEnabledReg;
    inputsEnabledReg.category = tr("3D Plot");
    inputsEnabledReg.subcategory = tr("Flags");
    inputsEnabledReg.displayName = tr("Inputs Enabled");
    inputsEnabledReg.briefDesc = tr("Allow user inputs");
    inputsEnabledReg.detailDesc = tr("When enabled, all user input interactions (rotate, pan, zoom) are allowed");
    inputsEnabledReg.editorType = EditorType::CheckBox;
    inputsEnabledReg.defaultValue = m_inputsEnabled;
    inputsEnabledReg.propertyName = "inputsEnabled";
    inputsEnabledReg.target = this;
    registerProperty(inputsEnabledReg);

    // Register equal property
    PropertyRegistration equalReg;
    equalReg.category = tr("3D Plot");
    equalReg.subcategory = tr("Flags");
    equalReg.displayName = tr("Equal Axes");
    equalReg.briefDesc = tr("Equal axis scaling");
    equalReg.detailDesc = tr("When enabled, all axes use equal scaling");
    equalReg.editorType = EditorType::CheckBox;
    equalReg.defaultValue = m_equal;
    equalReg.propertyName = "equal";
    equalReg.target = this;
    registerProperty(equalReg);

    // Register canvas enabled property
    PropertyRegistration canvasEnabledReg;
    canvasEnabledReg.category = tr("3D Plot");
    canvasEnabledReg.subcategory = tr("Flags");
    canvasEnabledReg.displayName = tr("Canvas Enabled");
    canvasEnabledReg.briefDesc = tr("Show canvas");
    canvasEnabledReg.detailDesc = tr("When enabled, the plot canvas background is displayed");
    canvasEnabledReg.editorType = EditorType::CheckBox;
    canvasEnabledReg.defaultValue = m_canvasEnabled;
    canvasEnabledReg.propertyName = "canvasEnabled";
    canvasEnabledReg.target = this;
    registerProperty(canvasEnabledReg);

    // === QImPlot3DNode - Legend configuration ===

    // Register legend location property
    PropertyRegistration legendLocationReg;
    legendLocationReg.category = tr("3D Plot");
    legendLocationReg.subcategory = tr("Legend");
    legendLocationReg.displayName = tr("Location");
    legendLocationReg.briefDesc = tr("Legend location");
    legendLocationReg.detailDesc = tr("Sets the position of the legend within the plot");
    legendLocationReg.editorType = EditorType::ComboBox;
    legendLocationReg.comboBoxOptions = QStringList{
        tr("Center"),       // QImPlot3DLocation::Center = 0
        tr("North"),        // QImPlot3DLocation::North = 1
        tr("South"),        // QImPlot3DLocation::South = 2
        tr("West"),         // QImPlot3DLocation::West = 4
        tr("East"),         // QImPlot3DLocation::East = 8
        tr("NorthWest"),    // QImPlot3DLocation::NorthWest = 5
        tr("NorthEast"),    // QImPlot3DLocation::NorthEast = 9
        tr("SouthWest"),    // QImPlot3DLocation::SouthWest = 6
        tr("SouthEast"),    // QImPlot3DLocation::SouthEast = 10
    };
    legendLocationReg.defaultValue = m_legendLocation;
    legendLocationReg.propertyName = "legendLocation";
    legendLocationReg.target = this;
    registerProperty(legendLocationReg);

    // Register legend flags property
    PropertyRegistration legendFlagsReg;
    legendFlagsReg.category = tr("3D Plot");
    legendFlagsReg.subcategory = tr("Legend");
    legendFlagsReg.displayName = tr("Flags");
    legendFlagsReg.briefDesc = tr("Legend flags");
    legendFlagsReg.detailDesc = tr("Sets the legend flags bitmask (0-255)");
    legendFlagsReg.editorType = EditorType::SpinBox;
    legendFlagsReg.defaultValue = m_legendFlags;
    legendFlagsReg.minValue = 0;
    legendFlagsReg.maxValue = 255;
    legendFlagsReg.stepValue = 1;
    legendFlagsReg.propertyName = "legendFlags";
    legendFlagsReg.target = this;
    registerProperty(legendFlagsReg);

    // === QImPlot3DAxisInfo - X axis properties ===

    // Register X axis min limit property
    PropertyRegistration xAxisMinLimitReg;
    xAxisMinLimitReg.category = tr("3D Plot");
    xAxisMinLimitReg.subcategory = tr("Axis");
    xAxisMinLimitReg.displayName = tr("X Min Limit");
    xAxisMinLimitReg.briefDesc = tr("X axis minimum limit");
    xAxisMinLimitReg.detailDesc = tr("Sets the minimum value for the X axis range");
    xAxisMinLimitReg.editorType = EditorType::DoubleSpinBox;
    xAxisMinLimitReg.defaultValue = m_xAxisMinLimit;
    xAxisMinLimitReg.minValue = -1e6;
    xAxisMinLimitReg.maxValue = 1e6;
    xAxisMinLimitReg.stepValue = 0.1;
    xAxisMinLimitReg.propertyName = "xAxisMinLimit";
    xAxisMinLimitReg.target = this;
    registerProperty(xAxisMinLimitReg);

    // Register X axis max limit property
    PropertyRegistration xAxisMaxLimitReg;
    xAxisMaxLimitReg.category = tr("3D Plot");
    xAxisMaxLimitReg.subcategory = tr("Axis");
    xAxisMaxLimitReg.displayName = tr("X Max Limit");
    xAxisMaxLimitReg.briefDesc = tr("X axis maximum limit");
    xAxisMaxLimitReg.detailDesc = tr("Sets the maximum value for the X axis range");
    xAxisMaxLimitReg.editorType = EditorType::DoubleSpinBox;
    xAxisMaxLimitReg.defaultValue = m_xAxisMaxLimit;
    xAxisMaxLimitReg.minValue = -1e6;
    xAxisMaxLimitReg.maxValue = 1e6;
    xAxisMaxLimitReg.stepValue = 0.1;
    xAxisMaxLimitReg.propertyName = "xAxisMaxLimit";
    xAxisMaxLimitReg.target = this;
    registerProperty(xAxisMaxLimitReg);

    // Register X axis label enabled property
    PropertyRegistration xAxisLabelEnabledReg;
    xAxisLabelEnabledReg.category = tr("3D Plot");
    xAxisLabelEnabledReg.subcategory = tr("Axis");
    xAxisLabelEnabledReg.displayName = tr("X Label Enabled");
    xAxisLabelEnabledReg.briefDesc = tr("Show X axis label");
    xAxisLabelEnabledReg.detailDesc = tr("When enabled, the X axis label is displayed");
    xAxisLabelEnabledReg.editorType = EditorType::CheckBox;
    xAxisLabelEnabledReg.defaultValue = m_xAxisLabelEnabled;
    xAxisLabelEnabledReg.propertyName = "xAxisLabelEnabled";
    xAxisLabelEnabledReg.target = this;
    registerProperty(xAxisLabelEnabledReg);

    // Register X axis grid lines enabled property
    PropertyRegistration xAxisGridLinesEnabledReg;
    xAxisGridLinesEnabledReg.category = tr("3D Plot");
    xAxisGridLinesEnabledReg.subcategory = tr("Axis");
    xAxisGridLinesEnabledReg.displayName = tr("X Grid Lines Enabled");
    xAxisGridLinesEnabledReg.briefDesc = tr("Show X axis grid lines");
    xAxisGridLinesEnabledReg.detailDesc = tr("When enabled, grid lines for the X axis are displayed");
    xAxisGridLinesEnabledReg.editorType = EditorType::CheckBox;
    xAxisGridLinesEnabledReg.defaultValue = m_xAxisGridLinesEnabled;
    xAxisGridLinesEnabledReg.propertyName = "xAxisGridLinesEnabled";
    xAxisGridLinesEnabledReg.target = this;
    registerProperty(xAxisGridLinesEnabledReg);

    // Register X axis tick marks enabled property
    PropertyRegistration xAxisTickMarksEnabledReg;
    xAxisTickMarksEnabledReg.category = tr("3D Plot");
    xAxisTickMarksEnabledReg.subcategory = tr("Axis");
    xAxisTickMarksEnabledReg.displayName = tr("X Tick Marks Enabled");
    xAxisTickMarksEnabledReg.briefDesc = tr("Show X axis tick marks");
    xAxisTickMarksEnabledReg.detailDesc = tr("When enabled, tick marks for the X axis are displayed");
    xAxisTickMarksEnabledReg.editorType = EditorType::CheckBox;
    xAxisTickMarksEnabledReg.defaultValue = m_xAxisTickMarksEnabled;
    xAxisTickMarksEnabledReg.propertyName = "xAxisTickMarksEnabled";
    xAxisTickMarksEnabledReg.target = this;
    registerProperty(xAxisTickMarksEnabledReg);

    // Register X axis tick labels enabled property
    PropertyRegistration xAxisTickLabelsEnabledReg;
    xAxisTickLabelsEnabledReg.category = tr("3D Plot");
    xAxisTickLabelsEnabledReg.subcategory = tr("Axis");
    xAxisTickLabelsEnabledReg.displayName = tr("X Tick Labels Enabled");
    xAxisTickLabelsEnabledReg.briefDesc = tr("Show X axis tick labels");
    xAxisTickLabelsEnabledReg.detailDesc = tr("When enabled, tick labels for the X axis are displayed");
    xAxisTickLabelsEnabledReg.editorType = EditorType::CheckBox;
    xAxisTickLabelsEnabledReg.defaultValue = m_xAxisTickLabelsEnabled;
    xAxisTickLabelsEnabledReg.propertyName = "xAxisTickLabelsEnabled";
    xAxisTickLabelsEnabledReg.target = this;
    registerProperty(xAxisTickLabelsEnabledReg);

    // Register X axis lock min property
    PropertyRegistration xAxisLockMinReg;
    xAxisLockMinReg.category = tr("3D Plot");
    xAxisLockMinReg.subcategory = tr("Axis");
    xAxisLockMinReg.displayName = tr("X Lock Min");
    xAxisLockMinReg.briefDesc = tr("Lock X axis minimum");
    xAxisLockMinReg.detailDesc = tr("When enabled, the X axis minimum limit cannot be changed by user interaction");
    xAxisLockMinReg.editorType = EditorType::CheckBox;
    xAxisLockMinReg.defaultValue = m_xAxisLockMin;
    xAxisLockMinReg.propertyName = "xAxisLockMin";
    xAxisLockMinReg.target = this;
    registerProperty(xAxisLockMinReg);

    // Register X axis lock max property
    PropertyRegistration xAxisLockMaxReg;
    xAxisLockMaxReg.category = tr("3D Plot");
    xAxisLockMaxReg.subcategory = tr("Axis");
    xAxisLockMaxReg.displayName = tr("X Lock Max");
    xAxisLockMaxReg.briefDesc = tr("Lock X axis maximum");
    xAxisLockMaxReg.detailDesc = tr("When enabled, the X axis maximum limit cannot be changed by user interaction");
    xAxisLockMaxReg.editorType = EditorType::CheckBox;
    xAxisLockMaxReg.defaultValue = m_xAxisLockMax;
    xAxisLockMaxReg.propertyName = "xAxisLockMax";
    xAxisLockMaxReg.target = this;
    registerProperty(xAxisLockMaxReg);

    // Register X axis auto fit property
    PropertyRegistration xAxisAutoFitReg;
    xAxisAutoFitReg.category = tr("3D Plot");
    xAxisAutoFitReg.subcategory = tr("Axis");
    xAxisAutoFitReg.displayName = tr("X Auto Fit");
    xAxisAutoFitReg.briefDesc = tr("Auto fit X axis");
    xAxisAutoFitReg.detailDesc = tr("When enabled, the X axis range automatically fits to the data");
    xAxisAutoFitReg.editorType = EditorType::CheckBox;
    xAxisAutoFitReg.defaultValue = m_xAxisAutoFit;
    xAxisAutoFitReg.propertyName = "xAxisAutoFit";
    xAxisAutoFitReg.target = this;
    registerProperty(xAxisAutoFitReg);

    // Register X axis invert property
    PropertyRegistration xAxisInvertReg;
    xAxisInvertReg.category = tr("3D Plot");
    xAxisInvertReg.subcategory = tr("Axis");
    xAxisInvertReg.displayName = tr("X Invert");
    xAxisInvertReg.briefDesc = tr("Invert X axis");
    xAxisInvertReg.detailDesc = tr("When enabled, the X axis direction is inverted");
    xAxisInvertReg.editorType = EditorType::CheckBox;
    xAxisInvertReg.defaultValue = m_xAxisInvert;
    xAxisInvertReg.propertyName = "xAxisInvert";
    xAxisInvertReg.target = this;
    registerProperty(xAxisInvertReg);

    // Register X axis decorations enabled property
    PropertyRegistration xAxisDecorationsEnabledReg;
    xAxisDecorationsEnabledReg.category = tr("3D Plot");
    xAxisDecorationsEnabledReg.subcategory = tr("Axis");
    xAxisDecorationsEnabledReg.displayName = tr("X Decorations Enabled");
    xAxisDecorationsEnabledReg.briefDesc = tr("Show X axis decorations");
    xAxisDecorationsEnabledReg.detailDesc = tr("When enabled, all X axis decorations (labels, ticks, grid) are displayed");
    xAxisDecorationsEnabledReg.editorType = EditorType::CheckBox;
    xAxisDecorationsEnabledReg.defaultValue = m_xAxisDecorationsEnabled;
    xAxisDecorationsEnabledReg.propertyName = "xAxisDecorationsEnabled";
    xAxisDecorationsEnabledReg.target = this;
    registerProperty(xAxisDecorationsEnabledReg);

    // Register X axis scale property
    PropertyRegistration xAxisScaleReg;
    xAxisScaleReg.category = tr("3D Plot");
    xAxisScaleReg.subcategory = tr("Axis");
    xAxisScaleReg.displayName = tr("X Scale");
    xAxisScaleReg.briefDesc = tr("X axis scale type");
    xAxisScaleReg.detailDesc = tr("Sets the scale type for the X axis (Linear, Log10, SymLog)");
    xAxisScaleReg.editorType = EditorType::EnumComboBox;
    xAxisScaleReg.comboBoxOptions = QStringList{
        tr("Linear"),   // QImPlot3DScaleType::Linear = 0
        tr("Log10"),    // QImPlot3DScaleType::Log10 = 1
        tr("SymLog"),   // QImPlot3DScaleType::SymLog = 2
    };
    xAxisScaleReg.defaultValue = m_xAxisScale;
    xAxisScaleReg.propertyName = "xAxisScale";
    xAxisScaleReg.target = this;
    registerProperty(xAxisScaleReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for Plot3DScatterFunction
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DScatterFunction 析构函数
 * \endif
 */
Plot3DScatterFunction::~Plot3DScatterFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a 3D plot with 1000 random XYZ scatter points (helix pattern),
 * configures axes, and adds a scatter item with marker settings.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含 1000 个随机 XYZ 散点（螺旋模式）的 3D 图表，
 * 配置坐标轴，并添加带有标记设置的散点项。
 * \endif
 */
void Plot3DScatterFunction::createPlot(QIM::QImFigureWidget* figure)
{
    if (!figure) {
        return;
    }
    
    // Reset to single-plot mode to clear any previous subplot configuration
    figure->setSubplot3DGrid(1, 1);
    
    // Create 3D plot node as a top-level render node via QImFigureWidget API
    m_plot3DNode = figure->createPlot3DNode();
    if (!m_plot3DNode) {
        return;
    }
    
    // Configure axes and title
    m_plot3DNode->xAxis()->setLabel(m_xLabel);
    m_plot3DNode->yAxis()->setLabel(m_yLabel);
    m_plot3DNode->zAxis()->setLabel(m_zLabel);
    m_plot3DNode->setTitle(m_title);
    
    // Apply initial QImPlot3DNode property values
    m_plot3DNode->setAutoSize(m_autoSize);
    if (!m_autoSize) {
        m_plot3DNode->setSize(QSizeF(m_plotWidth, m_plotHeight));
    }
    m_plot3DNode->setTitleEnabled(m_titleEnabled);
    m_plot3DNode->setLegendEnabled(m_legendEnabled);
    m_plot3DNode->setMouseTextEnabled(m_mouseTextEnabled);
    m_plot3DNode->setClipEnabled(m_clipEnabled);
    m_plot3DNode->setMenusEnabled(m_menusEnabled);
    m_plot3DNode->setRotateEnabled(m_rotateEnabled);
    m_plot3DNode->setPanEnabled(m_panEnabled);
    m_plot3DNode->setZoomEnabled(m_zoomEnabled);
    m_plot3DNode->setInputsEnabled(m_inputsEnabled);
    m_plot3DNode->setEqual(m_equal);
    m_plot3DNode->setCanvasEnabled(m_canvasEnabled);
    m_plot3DNode->setLegendLocation(static_cast<QIM::QImPlot3DLocation>(m_legendLocation));
    m_plot3DNode->setLegendFlags(m_legendFlags);
    
    // Apply initial X axis property values
    m_plot3DNode->xAxis()->setMinLimit(m_xAxisMinLimit);
    m_plot3DNode->xAxis()->setMaxLimit(m_xAxisMaxLimit);
    m_plot3DNode->xAxis()->setLabelEnabled(m_xAxisLabelEnabled);
    m_plot3DNode->xAxis()->setGridLinesEnabled(m_xAxisGridLinesEnabled);
    m_plot3DNode->xAxis()->setTickMarksEnabled(m_xAxisTickMarksEnabled);
    m_plot3DNode->xAxis()->setTickLabelsEnabled(m_xAxisTickLabelsEnabled);
    m_plot3DNode->xAxis()->setLockMin(m_xAxisLockMin);
    m_plot3DNode->xAxis()->setLockMax(m_xAxisLockMax);
    m_plot3DNode->xAxis()->setAutoFit(m_xAxisAutoFit);
    m_plot3DNode->xAxis()->setInvert(m_xAxisInvert);
    m_plot3DNode->xAxis()->setDecorationsEnabled(m_xAxisDecorationsEnabled);
    m_plot3DNode->xAxis()->setScale(static_cast<QIM::QImPlot3DScaleType>(m_xAxisScale));
    
    // Set default isometric view for better 3D visualization
    m_plot3DNode->setBoxRotation(35.264, 45.0);  // Isometric view (elevation: -35.264°, azimuth: 45°)
    
    // Generate 1000 random XYZ scatter points (helix/spiral pattern)
    const int numPoints = 1000;
    std::vector<double> xData(numPoints);
    std::vector<double> yData(numPoints);
    std::vector<double> zData(numPoints);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> noiseDist(0.0, 0.1);
    
    for (int i = 0; i < numPoints; ++i) {
        // Create a helix pattern with some noise for visual interest
        double t = static_cast<double>(i) / numPoints * 6.0 * 3.14159265358979323846;  // 3 full rotations
        double radius = 1.0 + noiseDist(gen) * 0.2;
        
        xData[i] = radius * std::cos(t) + noiseDist(gen) * 0.1;
        yData[i] = radius * std::sin(t) + noiseDist(gen) * 0.1;
        zData[i] = t / (6.0 * 3.14159265358979323846) * 2.0 - 1.0 + noiseDist(gen) * 0.1;  // Height from -1 to 1
    }
    
    // Create scatter item node and set data
    m_scatter3DNode = new QIM::QImPlot3DScatterItemNode(m_plot3DNode);
    m_scatter3DNode->setData(xData, yData, zData);
    m_scatter3DNode->setMarkerSize(m_markerSize);
    m_scatter3DNode->setMarkerShape(m_markerShape);
    m_scatter3DNode->setMarkerFillColor(m_markerFillColor);
    m_scatter3DNode->setMarkerOutlineColor(m_markerOutlineColor);
    m_scatter3DNode->setMarkerWeight(m_markerWeight);
}

void Plot3DScatterFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        Q_EMIT titleChanged(title);
        if (m_plot3DNode) {
            m_plot3DNode->setTitle(title);
        }
    }
}

void Plot3DScatterFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        Q_EMIT xLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->xAxis()->setLabel(label);
        }
    }
}

void Plot3DScatterFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        Q_EMIT yLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->yAxis()->setLabel(label);
        }
    }
}

void Plot3DScatterFunction::setZLabel(const QString& label)
{
    if (m_zLabel != label) {
        m_zLabel = label;
        Q_EMIT zLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->zAxis()->setLabel(label);
        }
    }
}

void Plot3DScatterFunction::setMarkerSize(float size)
{
    if (m_markerSize != size) {
        m_markerSize = size;
        Q_EMIT markerSizeChanged(size);
        if (m_scatter3DNode) {
            m_scatter3DNode->setMarkerSize(size);
        }
    }
}

void Plot3DScatterFunction::setMarkerShape(int shape)
{
    if (m_markerShape != shape) {
        m_markerShape = shape;
        Q_EMIT markerShapeChanged(shape);
        if (m_scatter3DNode) {
            m_scatter3DNode->setMarkerShape(shape);
        }
    }
}

void Plot3DScatterFunction::setMarkerFillColor(const QColor& color)
{
    if (m_markerFillColor != color) {
        m_markerFillColor = color;
        Q_EMIT markerFillColorChanged(color);
        if (m_scatter3DNode) {
            m_scatter3DNode->setMarkerFillColor(color);
        }
    }
}

void Plot3DScatterFunction::setMarkerOutlineColor(const QColor& color)
{
    if (m_markerOutlineColor != color) {
        m_markerOutlineColor = color;
        Q_EMIT markerOutlineColorChanged(color);
        if (m_scatter3DNode) {
            m_scatter3DNode->setMarkerOutlineColor(color);
        }
    }
}

void Plot3DScatterFunction::setMarkerWeight(float weight)
{
    if (m_markerWeight != weight) {
        m_markerWeight = weight;
        Q_EMIT markerWeightChanged(weight);
        if (m_scatter3DNode) {
            m_scatter3DNode->setMarkerWeight(weight);
        }
    }
}

// QImPlot3DNode - Size setters

void Plot3DScatterFunction::setPlotWidth(double width)
{
    if (m_plotWidth != width) {
        m_plotWidth = width;
        Q_EMIT plotWidthChanged(width);
        if (m_plot3DNode) {
            m_plot3DNode->setSize(QSizeF(m_plotWidth, m_plotHeight));
        }
    }
}

void Plot3DScatterFunction::setPlotHeight(double height)
{
    if (m_plotHeight != height) {
        m_plotHeight = height;
        Q_EMIT plotHeightChanged(height);
        if (m_plot3DNode) {
            m_plot3DNode->setSize(QSizeF(m_plotWidth, m_plotHeight));
        }
    }
}

void Plot3DScatterFunction::setAutoSize(bool enabled)
{
    if (m_autoSize != enabled) {
        m_autoSize = enabled;
        Q_EMIT autoSizeChanged(enabled);
        if (m_plot3DNode) {
            m_plot3DNode->setAutoSize(enabled);
        }
    }
}

// QImPlot3DNode - Flags setters

void Plot3DScatterFunction::setTitleEnabled(bool enabled)
{
    if (m_titleEnabled != enabled) {
        m_titleEnabled = enabled;
        Q_EMIT titleEnabledChanged(enabled);
        if (m_plot3DNode) {
            m_plot3DNode->setTitleEnabled(enabled);
        }
    }
}

void Plot3DScatterFunction::setLegendEnabled(bool enabled)
{
    if (m_legendEnabled != enabled) {
        m_legendEnabled = enabled;
        Q_EMIT legendEnabledChanged(enabled);
        if (m_plot3DNode) {
            m_plot3DNode->setLegendEnabled(enabled);
        }
    }
}

void Plot3DScatterFunction::setMouseTextEnabled(bool enabled)
{
    if (m_mouseTextEnabled != enabled) {
        m_mouseTextEnabled = enabled;
        Q_EMIT mouseTextEnabledChanged(enabled);
        if (m_plot3DNode) {
            m_plot3DNode->setMouseTextEnabled(enabled);
        }
    }
}

void Plot3DScatterFunction::setClipEnabled(bool enabled)
{
    if (m_clipEnabled != enabled) {
        m_clipEnabled = enabled;
        Q_EMIT clipEnabledChanged(enabled);
        if (m_plot3DNode) {
            m_plot3DNode->setClipEnabled(enabled);
        }
    }
}

void Plot3DScatterFunction::setMenusEnabled(bool enabled)
{
    if (m_menusEnabled != enabled) {
        m_menusEnabled = enabled;
        Q_EMIT menusEnabledChanged(enabled);
        if (m_plot3DNode) {
            m_plot3DNode->setMenusEnabled(enabled);
        }
    }
}

void Plot3DScatterFunction::setRotateEnabled(bool enabled)
{
    if (m_rotateEnabled != enabled) {
        m_rotateEnabled = enabled;
        Q_EMIT rotateEnabledChanged(enabled);
        if (m_plot3DNode) {
            m_plot3DNode->setRotateEnabled(enabled);
        }
    }
}

void Plot3DScatterFunction::setPanEnabled(bool enabled)
{
    if (m_panEnabled != enabled) {
        m_panEnabled = enabled;
        Q_EMIT panEnabledChanged(enabled);
        if (m_plot3DNode) {
            m_plot3DNode->setPanEnabled(enabled);
        }
    }
}

void Plot3DScatterFunction::setZoomEnabled(bool enabled)
{
    if (m_zoomEnabled != enabled) {
        m_zoomEnabled = enabled;
        Q_EMIT zoomEnabledChanged(enabled);
        if (m_plot3DNode) {
            m_plot3DNode->setZoomEnabled(enabled);
        }
    }
}

void Plot3DScatterFunction::setInputsEnabled(bool enabled)
{
    if (m_inputsEnabled != enabled) {
        m_inputsEnabled = enabled;
        Q_EMIT inputsEnabledChanged(enabled);
        if (m_plot3DNode) {
            m_plot3DNode->setInputsEnabled(enabled);
        }
    }
}

void Plot3DScatterFunction::setEqual(bool on)
{
    if (m_equal != on) {
        m_equal = on;
        Q_EMIT equalChanged(on);
        if (m_plot3DNode) {
            m_plot3DNode->setEqual(on);
        }
    }
}

void Plot3DScatterFunction::setCanvasEnabled(bool enabled)
{
    if (m_canvasEnabled != enabled) {
        m_canvasEnabled = enabled;
        Q_EMIT canvasEnabledChanged(enabled);
        if (m_plot3DNode) {
            m_plot3DNode->setCanvasEnabled(enabled);
        }
    }
}

// QImPlot3DNode - Legend configuration setters

void Plot3DScatterFunction::setLegendLocation(int location)
{
    // Map ComboBox index to QImPlot3DLocation bitwise enum value
    static const int locationMap[] = {0, 1, 2, 4, 8, 5, 9, 6, 10};
    int enumValue = locationMap[location];
    if (m_legendLocation != enumValue) {
        m_legendLocation = enumValue;
        Q_EMIT legendLocationChanged(enumValue);
        if (m_plot3DNode) {
            m_plot3DNode->setLegendLocation(static_cast<QIM::QImPlot3DLocation>(enumValue));
        }
    }
}

void Plot3DScatterFunction::setLegendFlags(int flags)
{
    if (m_legendFlags != flags) {
        m_legendFlags = flags;
        Q_EMIT legendFlagsChanged(flags);
        if (m_plot3DNode) {
            m_plot3DNode->setLegendFlags(flags);
        }
    }
}

// QImPlot3DAxisInfo - X axis setters

void Plot3DScatterFunction::setXAxisMinLimit(double min)
{
    if (m_xAxisMinLimit != min) {
        m_xAxisMinLimit = min;
        Q_EMIT xAxisMinLimitChanged(min);
        if (m_plot3DNode) {
            m_plot3DNode->xAxis()->setMinLimit(min);
        }
    }
}

void Plot3DScatterFunction::setXAxisMaxLimit(double max)
{
    if (m_xAxisMaxLimit != max) {
        m_xAxisMaxLimit = max;
        Q_EMIT xAxisMaxLimitChanged(max);
        if (m_plot3DNode) {
            m_plot3DNode->xAxis()->setMaxLimit(max);
        }
    }
}

void Plot3DScatterFunction::setXAxisLabelEnabled(bool enabled)
{
    if (m_xAxisLabelEnabled != enabled) {
        m_xAxisLabelEnabled = enabled;
        Q_EMIT xAxisLabelEnabledChanged(enabled);
        if (m_plot3DNode) {
            m_plot3DNode->xAxis()->setLabelEnabled(enabled);
        }
    }
}

void Plot3DScatterFunction::setXAxisGridLinesEnabled(bool enabled)
{
    if (m_xAxisGridLinesEnabled != enabled) {
        m_xAxisGridLinesEnabled = enabled;
        Q_EMIT xAxisGridLinesEnabledChanged(enabled);
        if (m_plot3DNode) {
            m_plot3DNode->xAxis()->setGridLinesEnabled(enabled);
        }
    }
}

void Plot3DScatterFunction::setXAxisTickMarksEnabled(bool enabled)
{
    if (m_xAxisTickMarksEnabled != enabled) {
        m_xAxisTickMarksEnabled = enabled;
        Q_EMIT xAxisTickMarksEnabledChanged(enabled);
        if (m_plot3DNode) {
            m_plot3DNode->xAxis()->setTickMarksEnabled(enabled);
        }
    }
}

void Plot3DScatterFunction::setXAxisTickLabelsEnabled(bool enabled)
{
    if (m_xAxisTickLabelsEnabled != enabled) {
        m_xAxisTickLabelsEnabled = enabled;
        Q_EMIT xAxisTickLabelsEnabledChanged(enabled);
        if (m_plot3DNode) {
            m_plot3DNode->xAxis()->setTickLabelsEnabled(enabled);
        }
    }
}

void Plot3DScatterFunction::setXAxisLockMin(bool on)
{
    if (m_xAxisLockMin != on) {
        m_xAxisLockMin = on;
        Q_EMIT xAxisLockMinChanged(on);
        if (m_plot3DNode) {
            m_plot3DNode->xAxis()->setLockMin(on);
        }
    }
}

void Plot3DScatterFunction::setXAxisLockMax(bool on)
{
    if (m_xAxisLockMax != on) {
        m_xAxisLockMax = on;
        Q_EMIT xAxisLockMaxChanged(on);
        if (m_plot3DNode) {
            m_plot3DNode->xAxis()->setLockMax(on);
        }
    }
}

void Plot3DScatterFunction::setXAxisAutoFit(bool on)
{
    if (m_xAxisAutoFit != on) {
        m_xAxisAutoFit = on;
        Q_EMIT xAxisAutoFitChanged(on);
        if (m_plot3DNode) {
            m_plot3DNode->xAxis()->setAutoFit(on);
        }
    }
}

void Plot3DScatterFunction::setXAxisInvert(bool on)
{
    if (m_xAxisInvert != on) {
        m_xAxisInvert = on;
        Q_EMIT xAxisInvertChanged(on);
        if (m_plot3DNode) {
            m_plot3DNode->xAxis()->setInvert(on);
        }
    }
}

void Plot3DScatterFunction::setXAxisDecorationsEnabled(bool enabled)
{
    if (m_xAxisDecorationsEnabled != enabled) {
        m_xAxisDecorationsEnabled = enabled;
        Q_EMIT xAxisDecorationsEnabledChanged(enabled);
        if (m_plot3DNode) {
            m_plot3DNode->xAxis()->setDecorationsEnabled(enabled);
        }
    }
}

void Plot3DScatterFunction::setXAxisScale(int scale)
{
    if (m_xAxisScale != scale) {
        m_xAxisScale = scale;
        Q_EMIT xAxisScaleChanged(scale);
        if (m_plot3DNode) {
            m_plot3DNode->xAxis()->setScale(static_cast<QIM::QImPlot3DScaleType>(scale));
        }
    }
}

void Plot3DScatterFunction::cleanupPlot()
{
    TestFunction::cleanupPlot();
    m_plot3DNode = nullptr;
    m_scatter3DNode = nullptr;
}
