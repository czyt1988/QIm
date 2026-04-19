#include "Plot3DDummyFunction.h"
#include "QImFigureWidget.h"
#include "plot3d/QImPlot3DNode.h"
#include "plot3d/QImPlot3DAxisInfo.h"
#include "plot3d/QImPlot3DDummyItemNode.h"
#include <QVector>

/**
 * \if ENGLISH
 * @brief Constructor for Plot3DDummyFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Plot"/"Basic Info"/"Title" - Plot title
 * - "Plot"/"Axis"/"X Label" - X axis label
 * - "Plot"/"Axis"/"Y Label" - Y axis label
 * - "Plot"/"Axis"/"Z Label" - Z axis label
 * - "Dummy"/"Item 1"/"Color" - Dummy item 1 color
 * - "Dummy"/"Item 2"/"Color" - Dummy item 2 color
 * - "Dummy"/"Item 3"/"Color" - Dummy item 3 color
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DDummyFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Plot"/"Basic Info"/"Title" - 图表标题
 * - "Plot"/"Axis"/"X Label" - X 轴标签
 * - "Plot"/"Axis"/"Y Label" - Y 轴标签
 * - "Plot"/"Axis"/"Z Label" - Z 轴标签
 * - "Dummy"/"Item 1"/"Color" - 虚拟项 1 颜色
 * - "Dummy"/"Item 2"/"Color" - 虚拟项 2 颜色
 * - "Dummy"/"Item 3"/"Color" - 虚拟项 3 颜色
 * \endif
 */
Plot3DDummyFunction::Plot3DDummyFunction(QObject* parent)
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
    
    // Register dummy 1 color property
    PropertyRegistration dummy1ColorReg;
    dummy1ColorReg.category = tr("Dummy");
    dummy1ColorReg.subcategory = tr("Item 1");
    dummy1ColorReg.displayName = tr("Color");
    dummy1ColorReg.briefDesc = tr("Dummy item 1 color");
    dummy1ColorReg.detailDesc = tr("Sets the color of the first dummy item (Sensor A)");
    dummy1ColorReg.editorType = EditorType::ColorPicker;
    dummy1ColorReg.defaultValue = m_dummy1Color;
    dummy1ColorReg.propertyName = "dummy1Color";
    dummy1ColorReg.target = this;
    registerProperty(dummy1ColorReg);
    
    // Register dummy 2 color property
    PropertyRegistration dummy2ColorReg;
    dummy2ColorReg.category = tr("Dummy");
    dummy2ColorReg.subcategory = tr("Item 2");
    dummy2ColorReg.displayName = tr("Color");
    dummy2ColorReg.briefDesc = tr("Dummy item 2 color");
    dummy2ColorReg.detailDesc = tr("Sets the color of the second dummy item (Sensor B)");
    dummy2ColorReg.editorType = EditorType::ColorPicker;
    dummy2ColorReg.defaultValue = m_dummy2Color;
    dummy2ColorReg.propertyName = "dummy2Color";
    dummy2ColorReg.target = this;
    registerProperty(dummy2ColorReg);
    
    // Register dummy 3 color property
    PropertyRegistration dummy3ColorReg;
    dummy3ColorReg.category = tr("Dummy");
    dummy3ColorReg.subcategory = tr("Item 3");
    dummy3ColorReg.displayName = tr("Color");
    dummy3ColorReg.briefDesc = tr("Dummy item 3 color");
    dummy3ColorReg.detailDesc = tr("Sets the color of the third dummy item (Sensor C)");
    dummy3ColorReg.editorType = EditorType::ColorPicker;
    dummy3ColorReg.defaultValue = m_dummy3Color;
    dummy3ColorReg.propertyName = "dummy3Color";
    dummy3ColorReg.target = this;
    registerProperty(dummy3ColorReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for Plot3DDummyFunction
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DDummyFunction 析构函数
 * \endif
 */
Plot3DDummyFunction::~Plot3DDummyFunction()
{
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a 3D plot with 3 dummy items for legend demonstration.
 * The dummy items have labels "Sensor A", "Sensor B", "Sensor C" with
 * configurable colors (default: red, green, blue).
 * 
 * @note Dummy items only appear in the legend; they do not render any geometry
 * in the 3D plot area. This is intended behavior for legend customization testing.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含 3 个虚拟项的 3D 图表用于图例演示。
 * 虚拟项的标签为"Sensor A"、"Sensor B"、"Sensor C"，
 * 颜色可配置（默认：红、绿、蓝）。
 * 
 * @note 虚拟项仅显示在图例中，不会在 3D 绘图区域渲染任何几何图形。
 * 这是用于图例自定义测试的预期行为。
 * \endif
 */
void Plot3DDummyFunction::createPlot(QIM::QImFigureWidget* figure)
{
    if (!figure) {
        return;
    }
    
    // Create 3D plot node as a top-level render node via QImFigureWidget API
    m_plot3DNode = figure->createPlot3DNode();
    
    // Configure axes and title
    m_plot3DNode->xAxis()->setLabel(m_xLabel);
    m_plot3DNode->yAxis()->setLabel(m_yLabel);
    m_plot3DNode->zAxis()->setLabel(m_zLabel);
    m_plot3DNode->setTitle(m_title);
    
    // Set default isometric view for better 3D visualization
    m_plot3DNode->setBoxRotation(-35.264, 45.0);  // Isometric view (elevation: -35.264°, azimuth: 45°)
    
    // Enable legend to show dummy items
    m_plot3DNode->setLegendEnabled(true);
    
    // Create 3 dummy item nodes with different colors
    m_dummy1Node = new QIM::QImPlot3DDummyItemNode(m_plot3DNode);
    m_dummy1Node->setLabel(QStringLiteral("Sensor A"));
    m_dummy1Node->setColor(m_dummy1Color);
    
    m_dummy2Node = new QIM::QImPlot3DDummyItemNode(m_plot3DNode);
    m_dummy2Node->setLabel(QStringLiteral("Sensor B"));
    m_dummy2Node->setColor(m_dummy2Color);
    
    m_dummy3Node = new QIM::QImPlot3DDummyItemNode(m_plot3DNode);
    m_dummy3Node->setLabel(QStringLiteral("Sensor C"));
    m_dummy3Node->setColor(m_dummy3Color);
}

void Plot3DDummyFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        Q_EMIT titleChanged(title);
        if (m_plot3DNode) {
            m_plot3DNode->setTitle(title);
        }
    }
}

void Plot3DDummyFunction::setXLabel(const QString& label)
{
    if (m_xLabel != label) {
        m_xLabel = label;
        Q_EMIT xLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->xAxis()->setLabel(label);
        }
    }
}

void Plot3DDummyFunction::setYLabel(const QString& label)
{
    if (m_yLabel != label) {
        m_yLabel = label;
        Q_EMIT yLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->yAxis()->setLabel(label);
        }
    }
}

void Plot3DDummyFunction::setZLabel(const QString& label)
{
    if (m_zLabel != label) {
        m_zLabel = label;
        Q_EMIT zLabelChanged(label);
        if (m_plot3DNode) {
            m_plot3DNode->zAxis()->setLabel(label);
        }
    }
}

void Plot3DDummyFunction::setDummy1Color(const QColor& color)
{
    if (m_dummy1Color != color) {
        m_dummy1Color = color;
        Q_EMIT dummy1ColorChanged(color);
        if (m_dummy1Node) {
            m_dummy1Node->setColor(color);
        }
    }
}

void Plot3DDummyFunction::setDummy2Color(const QColor& color)
{
    if (m_dummy2Color != color) {
        m_dummy2Color = color;
        Q_EMIT dummy2ColorChanged(color);
        if (m_dummy2Node) {
            m_dummy2Node->setColor(color);
        }
    }
}

void Plot3DDummyFunction::setDummy3Color(const QColor& color)
{
    if (m_dummy3Color != color) {
        m_dummy3Color = color;
        Q_EMIT dummy3ColorChanged(color);
        if (m_dummy3Node) {
            m_dummy3Node->setColor(color);
        }
    }
}
