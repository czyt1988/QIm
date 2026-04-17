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
 * - "Scatter"/"Marker"/"Color" - Marker color
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
 * - "Scatter"/"Marker"/"Color" - 标记颜色
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
    m_plot3DNode->setLegendEnabled(true);
    
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
    m_scatter3DNode->setColor(m_markerColor);
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

void Plot3DScatterFunction::setMarkerColor(const QColor& color)
{
    if (m_markerColor != color) {
        m_markerColor = color;
        Q_EMIT markerColorChanged(color);
        if (m_scatter3DNode) {
            m_scatter3DNode->setColor(color);
        }
    }
}
