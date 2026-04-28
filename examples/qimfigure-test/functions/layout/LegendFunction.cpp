#include "LegendFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotLegendNode.h"
#include "plot/QImPlotLineItemNode.h"
#include "plot/QImPlot.h"
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * \if ENGLISH
 * @brief Constructor for LegendFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * Properties are organized in categories:
 * - "Legend"/"Position"/"Location" - Legend location (ComboBox with bitwise enum mapping)
 * - "Legend"/"Position"/"Orientation" - Legend orientation (Horizontal/Vertical)
 * - "Legend"/"Position"/"Outside" - Place legend outside plot area
 * - "Legend"/"Interaction"/"Button Enabled" - Show legend toggle buttons
 * - "Legend"/"Interaction"/"Highlight Item" - Highlight item on legend hover
 * - "Legend"/"Interaction"/"Highlight Axis" - Highlight axis on legend hover
 * - "Legend"/"Display"/"Sort" - Sort legend entries alphabetically
 * - "Legend"/"Display"/"Reverse" - Reverse legend entry order
 * \endif
 * 
 * \if CHINESE
 * @brief LegendFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * 属性按类别组织：
 * - "Legend"/"Position"/"Location" - 图例位置（ComboBox + 位标志枚举映射）
 * - "Legend"/"Position"/"Orientation" - 图例方向（水平/垂直）
 * - "Legend"/"Position"/"Outside" - 将图例放置在绘图区域外
 * - "Legend"/"Interaction"/"Button Enabled" - 显示图例切换按钮
 * - "Legend"/"Interaction"/"Highlight Item" - 鼠标悬停时高亮数据项
 * - "Legend"/"Interaction"/"Highlight Axis" - 鼠标悬停时高亮坐标轴
 * - "Legend"/"Display"/"Sort" - 按字母顺序排序图例条目
 * - "Legend"/"Display"/"Reverse" - 反序排列图例条目
 * \endif
 */
LegendFunction::LegendFunction(QObject* parent)
    : TestFunction(parent)
{
    // Register location property (ComboBox - bitwise enum, index≠value)
    PropertyRegistration locationReg;
    locationReg.category = tr("Legend");
    locationReg.subcategory = tr("Position");
    locationReg.displayName = tr("Location");
    locationReg.briefDesc = tr("Legend location");
    locationReg.detailDesc = tr("Sets the legend position (North, South, West, East, corners, Center)");
    locationReg.editorType = EditorType::ComboBox;
    locationReg.comboBoxOptions = QStringList{
        tr("North"), tr("South"), tr("West"), tr("East"),
        tr("NorthWest"), tr("NorthEast"), tr("SouthWest"), tr("SouthEast"),
        tr("Center")
    };
    locationReg.defaultValue = m_location;
    locationReg.propertyName = "location";
    locationReg.target = this;
    registerProperty(locationReg);
    
    // Register orientation property (EnumComboBox - Qt::Orientation: Horizontal=0, Vertical=1)
    PropertyRegistration orientationReg;
    orientationReg.category = tr("Legend");
    orientationReg.subcategory = tr("Position");
    orientationReg.displayName = tr("Orientation");
    orientationReg.briefDesc = tr("Legend orientation");
    orientationReg.detailDesc = tr("Sets the legend orientation (Horizontal or Vertical)");
    orientationReg.editorType = EditorType::EnumComboBox;
    orientationReg.comboBoxOptions = QStringList{tr("Horizontal"), tr("Vertical")};
    orientationReg.defaultValue = m_orientation;
    orientationReg.propertyName = "orientation";
    orientationReg.target = this;
    registerProperty(orientationReg);
    
    // Register outside property
    PropertyRegistration outsideReg;
    outsideReg.category = tr("Legend");
    outsideReg.subcategory = tr("Position");
    outsideReg.displayName = tr("Outside");
    outsideReg.briefDesc = tr("Place legend outside");
    outsideReg.detailDesc = tr("When enabled, the legend is placed outside the plot area");
    outsideReg.editorType = EditorType::CheckBox;
    outsideReg.defaultValue = m_outside;
    outsideReg.propertyName = "outside";
    outsideReg.target = this;
    registerProperty(outsideReg);
    
    // Register buttonEnabled property
    PropertyRegistration buttonReg;
    buttonReg.category = tr("Legend");
    buttonReg.subcategory = tr("Interaction");
    buttonReg.displayName = tr("Button Enabled");
    buttonReg.briefDesc = tr("Show legend buttons");
    buttonReg.detailDesc = tr("When enabled, legend entries have toggle buttons");
    buttonReg.editorType = EditorType::CheckBox;
    buttonReg.defaultValue = m_buttonEnabled;
    buttonReg.propertyName = "buttonEnabled";
    buttonReg.target = this;
    registerProperty(buttonReg);
    
    // Register highlightItemEnabled property
    PropertyRegistration highlightItemReg;
    highlightItemReg.category = tr("Legend");
    highlightItemReg.subcategory = tr("Interaction");
    highlightItemReg.displayName = tr("Highlight Item");
    highlightItemReg.briefDesc = tr("Highlight item on hover");
    highlightItemReg.detailDesc = tr("When enabled, hovering over a legend entry highlights the corresponding plot item");
    highlightItemReg.editorType = EditorType::CheckBox;
    highlightItemReg.defaultValue = m_highlightItemEnabled;
    highlightItemReg.propertyName = "highlightItemEnabled";
    highlightItemReg.target = this;
    registerProperty(highlightItemReg);
    
    // Register highlightAxisEnabled property
    PropertyRegistration highlightAxisReg;
    highlightAxisReg.category = tr("Legend");
    highlightAxisReg.subcategory = tr("Interaction");
    highlightAxisReg.displayName = tr("Highlight Axis");
    highlightAxisReg.briefDesc = tr("Highlight axis on hover");
    highlightAxisReg.detailDesc = tr("When enabled, hovering over a legend entry highlights the corresponding axis");
    highlightAxisReg.editorType = EditorType::CheckBox;
    highlightAxisReg.defaultValue = m_highlightAxisEnabled;
    highlightAxisReg.propertyName = "highlightAxisEnabled";
    highlightAxisReg.target = this;
    registerProperty(highlightAxisReg);
    
    // Register sort property
    PropertyRegistration sortReg;
    sortReg.category = tr("Legend");
    sortReg.subcategory = tr("Display");
    sortReg.displayName = tr("Sort");
    sortReg.briefDesc = tr("Sort entries alphabetically");
    sortReg.detailDesc = tr("When enabled, legend entries are sorted alphabetically");
    sortReg.editorType = EditorType::CheckBox;
    sortReg.defaultValue = m_sort;
    sortReg.propertyName = "sort";
    sortReg.target = this;
    registerProperty(sortReg);
    
    // Register reverse property
    PropertyRegistration reverseReg;
    reverseReg.category = tr("Legend");
    reverseReg.subcategory = tr("Display");
    reverseReg.displayName = tr("Reverse");
    reverseReg.briefDesc = tr("Reverse entry order");
    reverseReg.detailDesc = tr("When enabled, legend entries are displayed in reverse order");
    reverseReg.editorType = EditorType::CheckBox;
    reverseReg.defaultValue = m_reverse;
    reverseReg.propertyName = "reverse";
    reverseReg.target = this;
    registerProperty(reverseReg);
}

/**
 * \if ENGLISH
 * @brief Destructor for LegendFunction
 * \endif
 * 
 * \if CHINESE
 * @brief LegendFunction 析构函数
 * \endif
 */
LegendFunction::~LegendFunction()
{
}

/**
 * \if ENGLISH
 * @brief Cleanup plot nodes
 * @details Clears node references. Nodes are managed by the figure widget.
 * \endif
 * 
 * \if CHINESE
 * @brief 清理绘图节点
 * @details 清空节点引用。节点由图表控件管理。
 * \endif
 */
void LegendFunction::cleanupPlot()
{
    m_plotNode = nullptr;
    m_legendNode = nullptr;
    m_lineNode = nullptr;
}

/**
 * \if ENGLISH
 * @brief Create plot nodes in the given figure widget
 * @param figure Pointer to the figure widget where plots will be created
 * @details Creates a QImPlotNode with a sine wave line item to provide
 * legend content, then retrieves the built-in legend node via
 * plot->legendNode() and applies all registered properties.
 * \endif
 * 
 * \if CHINESE
 * @brief 在指定的图表控件中创建绘图节点
 * @param figure 将要创建绘图的图表控件指针
 * @details 创建一个包含正弦波线项的 QImPlotNode 以提供图例内容，
 * 然后通过 plot->legendNode() 获取内置图例节点，并应用所有已注册属性。
 * \endif
 */
void LegendFunction::createPlot(QIM::QImFigureWidget* figure)
{
    if (!figure) {
        return;
    }
    
    // Create plot node
    m_plotNode = figure->createPlotNode();
    if (!m_plotNode) {
        return;
    }
    
    m_plotNode->setTitle(tr("Legend Demo"));
    m_plotNode->setLegendEnabled(true);
    
    // Generate sine wave data (100 points) for legend content
    const int numPoints = 100;
    std::vector<double> xData(numPoints);
    std::vector<double> yData(numPoints);
    
    for (int i = 0; i < numPoints; ++i) {
        xData[i] = i * 2.0 * M_PI / (numPoints - 1);
        yData[i] = std::sin(xData[i]);
    }
    
    // Create line item node with sample data
    m_lineNode = new QIM::QImPlotLineItemNode(m_plotNode);
    m_lineNode->setData(xData, yData);
    m_lineNode->setLabel(tr("sin(x)"));
    m_plotNode->addChildNode(m_lineNode);
    
    // Get built-in legend node (NOT create new one)
    m_legendNode = m_plotNode->legendNode();
    if (!m_legendNode) {
        return;
    }
    
    // Apply all properties to legend node
    // Location: convert text to QImPlotLegendLocation enum
    setLocation(m_location);
    setOrientation(m_orientation);
    setOutside(m_outside);
    setButtonEnabled(m_buttonEnabled);
    setHighlightItemEnabled(m_highlightItemEnabled);
    setHighlightAxisEnabled(m_highlightAxisEnabled);
    setSort(m_sort);
    setReverse(m_reverse);
}

void LegendFunction::setLocation(const QString& location)
{
    if (m_location != location) {
        m_location = location;
        Q_EMIT locationChanged(location);
        if (m_legendNode) {
            // Manual mapping: text → bitwise flag enum value
            QIM::QImPlotLegendLocation locValue = QIM::QImPlotLegendLocation::North;
            if (location == tr("North") || location == "North") {
                locValue = QIM::QImPlotLegendLocation::North;
            } else if (location == tr("South") || location == "South") {
                locValue = QIM::QImPlotLegendLocation::South;
            } else if (location == tr("West") || location == "West") {
                locValue = QIM::QImPlotLegendLocation::West;
            } else if (location == tr("East") || location == "East") {
                locValue = QIM::QImPlotLegendLocation::East;
            } else if (location == tr("NorthWest") || location == "NorthWest") {
                locValue = QIM::QImPlotLegendLocation::NorthWest;
            } else if (location == tr("NorthEast") || location == "NorthEast") {
                locValue = QIM::QImPlotLegendLocation::NorthEast;
            } else if (location == tr("SouthWest") || location == "SouthWest") {
                locValue = QIM::QImPlotLegendLocation::SouthWest;
            } else if (location == tr("SouthEast") || location == "SouthEast") {
                locValue = QIM::QImPlotLegendLocation::SouthEast;
            } else if (location == tr("Center") || location == "Center") {
                locValue = QIM::QImPlotLegendLocation::Center;
            }
            m_legendNode->setLocation(locValue);
        }
    }
}

void LegendFunction::setOrientation(int orientation)
{
    if (m_orientation != orientation) {
        m_orientation = orientation;
        Q_EMIT orientationChanged(orientation);
        if (m_legendNode) {
            m_legendNode->setOrientation(static_cast<Qt::Orientation>(orientation));
        }
    }
}

void LegendFunction::setOutside(bool enabled)
{
    if (m_outside != enabled) {
        m_outside = enabled;
        Q_EMIT outsideChanged(enabled);
        if (m_legendNode) {
            m_legendNode->setOutside(enabled);
        }
    }
}

void LegendFunction::setButtonEnabled(bool enabled)
{
    if (m_buttonEnabled != enabled) {
        m_buttonEnabled = enabled;
        Q_EMIT buttonEnabledChanged(enabled);
        if (m_legendNode) {
            m_legendNode->setButtonEnabled(enabled);
        }
    }
}

void LegendFunction::setHighlightItemEnabled(bool enabled)
{
    if (m_highlightItemEnabled != enabled) {
        m_highlightItemEnabled = enabled;
        Q_EMIT highlightItemEnabledChanged(enabled);
        if (m_legendNode) {
            m_legendNode->setHighlightItemEnabled(enabled);
        }
    }
}

void LegendFunction::setHighlightAxisEnabled(bool enabled)
{
    if (m_highlightAxisEnabled != enabled) {
        m_highlightAxisEnabled = enabled;
        Q_EMIT highlightAxisEnabledChanged(enabled);
        if (m_legendNode) {
            m_legendNode->setHighlightAxisEnabled(enabled);
        }
    }
}

void LegendFunction::setSort(bool enabled)
{
    if (m_sort != enabled) {
        m_sort = enabled;
        Q_EMIT sortChanged(enabled);
        if (m_legendNode) {
            m_legendNode->setSort(enabled);
        }
    }
}

void LegendFunction::setReverse(bool enabled)
{
    if (m_reverse != enabled) {
        m_reverse = enabled;
        Q_EMIT reverseChanged(enabled);
        if (m_legendNode) {
            m_legendNode->setReverse(enabled);
        }
    }
}