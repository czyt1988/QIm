#ifndef LEGENDFUNCTION_H
#define LEGENDFUNCTION_H

#include "../TestFunction.h"
#include <QObject>

namespace QIM {
class QImFigureWidget;
class QImPlotNode;
class QImPlotLegendNode;
class QImPlotLineItemNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating legend configuration
 * 
 * LegendFunction creates a plot with a sine wave line item and exposes
 * all 8 QImPlotLegendNode properties for interactive editing:
 * - location (ComboBox with manual enum mapping for bitwise flags)
 * - orientation (EnumComboBox for Qt::Orientation)
 * - outside, buttonEnabled, highlightItemEnabled, highlightAxisEnabled,
 *   sort, reverse (CheckBox for boolean flags)
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new LegendFunction(this);
 * func->createPlot(figureWidget);
 * // Adjust legend location, orientation and flags
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotLegendNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示图例配置的测试函数
 * 
 * LegendFunction 创建一个包含正弦波线项的图表，并暴露所有 8 个
 * QImPlotLegendNode 属性用于交互式编辑：
 * - location（ComboBox + 手动枚举映射，位标志枚举）
 * - orientation（EnumComboBox，Qt::Orientation）
 * - outside、buttonEnabled、highlightItemEnabled、highlightAxisEnabled、
 *   sort、reverse（CheckBox，布尔标志）
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new LegendFunction(this);
 * func->createPlot(figureWidget);
 * // 调整图例位置、方向和标志
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlotLegendNode
 * \endif
 */
class LegendFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY locationChanged)
    Q_PROPERTY(int orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
    Q_PROPERTY(bool outside READ outside WRITE setOutside NOTIFY outsideChanged)
    Q_PROPERTY(bool buttonEnabled READ buttonEnabled WRITE setButtonEnabled NOTIFY buttonEnabledChanged)
    Q_PROPERTY(bool highlightItemEnabled READ highlightItemEnabled WRITE setHighlightItemEnabled NOTIFY highlightItemEnabledChanged)
    Q_PROPERTY(bool highlightAxisEnabled READ highlightAxisEnabled WRITE setHighlightAxisEnabled NOTIFY highlightAxisEnabledChanged)
    Q_PROPERTY(bool sort READ sort WRITE setSort NOTIFY sortChanged)
    Q_PROPERTY(bool reverse READ reverse WRITE setReverse NOTIFY reverseChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for LegendFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief LegendFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit LegendFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for LegendFunction
     * \endif
     * 
     * \if CHINESE
     * @brief LegendFunction 析构函数
     * \endif
     */
    ~LegendFunction() override;
    
    /**
     * \if ENGLISH
     * @brief Create plot nodes in the given figure widget
     * @param figure Pointer to the figure widget where plots will be created
     * \endif
     * 
     * \if CHINESE
     * @brief 在指定的图表控件中创建绘图节点
     * @param figure 将要创建绘图的图表控件指针
     * \endif
     */
    void createPlot(QIM::QImFigureWidget* figure) override;
    
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
    void cleanupPlot() override;
    
    // Location property accessors
    QString location() const { return m_location; }
    void setLocation(const QString& location);
    
    // Orientation property accessors
    int orientation() const { return m_orientation; }
    void setOrientation(int orientation);
    
    // Outside property accessors
    bool outside() const { return m_outside; }
    void setOutside(bool enabled);
    
    // Button enabled property accessors
    bool buttonEnabled() const { return m_buttonEnabled; }
    void setButtonEnabled(bool enabled);
    
    // Highlight item enabled property accessors
    bool highlightItemEnabled() const { return m_highlightItemEnabled; }
    void setHighlightItemEnabled(bool enabled);
    
    // Highlight axis enabled property accessors
    bool highlightAxisEnabled() const { return m_highlightAxisEnabled; }
    void setHighlightAxisEnabled(bool enabled);
    
    // Sort property accessors
    bool sort() const { return m_sort; }
    void setSort(bool enabled);
    
    // Reverse property accessors
    bool reverse() const { return m_reverse; }
    void setReverse(bool enabled);
    
Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Signal emitted when location changes
     * @param location New location text value
     * \endif
     * 
     * \if CHINESE
     * @brief 位置改变时发出的信号
     * @param location 新的位置文本值
     * \endif
     */
    void locationChanged(const QString& location);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when orientation changes
     * @param orientation New orientation value (0=Horizontal, 1=Vertical)
     * \endif
     * 
     * \if CHINESE
     * @brief 方向改变时发出的信号
     * @param orientation 新的方向值（0=水平，1=垂直）
     * \endif
     */
    void orientationChanged(int orientation);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when outside setting changes
     * @param enabled New setting value
     * \endif
     * 
     * \if CHINESE
     * @brief 外部放置设置改变时发出的信号
     * @param enabled 新的设置值
     * \endif
     */
    void outsideChanged(bool enabled);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when button enabled setting changes
     * @param enabled New setting value
     * \endif
     * 
     * \if CHINESE
     * @brief 按钮启用设置改变时发出的信号
     * @param enabled 新的设置值
     * \endif
     */
    void buttonEnabledChanged(bool enabled);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when highlight item enabled setting changes
     * @param enabled New setting value
     * \endif
     * 
     * \if CHINESE
     * @brief 高亮项启用设置改变时发出的信号
     * @param enabled 新的设置值
     * \endif
     */
    void highlightItemEnabledChanged(bool enabled);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when highlight axis enabled setting changes
     * @param enabled New setting value
     * \endif
     * 
     * \if CHINESE
     * @brief 高亮轴启用设置改变时发出的信号
     * @param enabled 新的设置值
     * \endif
     */
    void highlightAxisEnabledChanged(bool enabled);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when sort setting changes
     * @param enabled New setting value
     * \endif
     * 
     * \if CHINESE
     * @brief 排序设置改变时发出的信号
     * @param enabled 新的设置值
     * \endif
     */
    void sortChanged(bool enabled);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when reverse setting changes
     * @param enabled New setting value
     * \endif
     * 
     * \if CHINESE
     * @brief 反序设置改变时发出的信号
     * @param enabled 新的设置值
     * \endif
     */
    void reverseChanged(bool enabled);
    
private:
    QString m_location = QStringLiteral("North");
    int m_orientation = 0;  // Qt::Horizontal
    bool m_outside = false;
    bool m_buttonEnabled = true;
    bool m_highlightItemEnabled = true;
    bool m_highlightAxisEnabled = true;
    bool m_sort = false;
    bool m_reverse = false;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    QIM::QImPlotLegendNode* m_legendNode = nullptr;
    QIM::QImPlotLineItemNode* m_lineNode = nullptr;
};

#endif // LEGENDFUNCTION_H