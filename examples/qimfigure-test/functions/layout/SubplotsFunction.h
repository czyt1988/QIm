#ifndef SUBPLOTSFUNCTION_H
#define SUBPLOTSFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
class QImSubplotsNode;
class QImPlotNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating subplots layout tool
 * 
 * SubplotsFunction creates a 2x2 subplot grid with different plot types.
 * It demonstrates:
 * - Subplot grid layout configuration
 * - Axis linking across subplots
 * - Legend sharing between plots
 * - Resizable subplot panels
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new SubplotsFunction(this);
 * func->createPlot(figureWidget);
 * // Adjust grid size and linking options
 * \endcode
 * 
 * @see TestFunction
 * @see QImSubplotsNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示子图布局工具的测试函数
 * 
 * SubplotsFunction 创建一个 2x2 子图网格，展示不同的绘图类型。
 * 它演示了：
 * - 子图网格布局配置
 * - 跨子图轴链接
 * - 图例共享
 * - 可调整大小的子图面板
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new SubplotsFunction(this);
 * func->createPlot(figureWidget);
 * // 调整网格大小和链接选项
 * \endcode
 * 
 * @see TestFunction
 * @see QImSubplotsNode
 * \endif
 */
class SubplotsFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(int rows READ rows WRITE setRows NOTIFY rowsChanged)
    Q_PROPERTY(int cols READ cols WRITE setCols NOTIFY colsChanged)
    Q_PROPERTY(bool linkAllX READ linkAllX WRITE setLinkAllX NOTIFY linkAllXChanged)
    Q_PROPERTY(bool linkAllY READ linkAllY WRITE setLinkAllY NOTIFY linkAllYChanged)
    Q_PROPERTY(bool legendEnabled READ legendEnabled WRITE setLegendEnabled NOTIFY legendEnabledChanged)
    Q_PROPERTY(bool resizable READ resizable WRITE setResizable NOTIFY resizableChanged)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for SubplotsFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief SubplotsFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit SubplotsFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for SubplotsFunction
     * \endif
     * 
     * \if CHINESE
     * @brief SubplotsFunction 析构函数
     * \endif
     */
    ~SubplotsFunction() override;
    
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
     * @details Deletes the subplots node to prevent stale rendering.
     * \endif
     * 
     * \if CHINESE
     * @brief 清理绘图节点
     * @details 删除子图节点以防止残留渲染。
     * \endif
     */
    void cleanupPlot() override;
    
    // Title property accessors
    QString title() const { return m_title; }
    void setTitle(const QString& title);
    
    // Rows property accessors
    int rows() const { return m_rows; }
    void setRows(int rows);
    
    // Columns property accessors
    int cols() const { return m_cols; }
    void setCols(int cols);
    
    // Link all X axes property accessors
    bool linkAllX() const { return m_linkAllX; }
    void setLinkAllX(bool enabled);
    
    // Link all Y axes property accessors
    bool linkAllY() const { return m_linkAllY; }
    void setLinkAllY(bool enabled);
    
    // Legend enabled property accessors
    bool legendEnabled() const { return m_legendEnabled; }
    void setLegendEnabled(bool enabled);
    
    // Resizable property accessors
    bool resizable() const { return m_resizable; }
    void setResizable(bool enabled);
    
Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Signal emitted when title changes
     * @param title New title value
     * \endif
     * 
     * \if CHINESE
     * @brief 标题改变时发出的信号
     * @param title 新的标题值
     * \endif
     */
    void titleChanged(const QString& title);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when rows count changes
     * @param rows New rows count
     * \endif
     * 
     * \if CHINESE
     * @brief 行数改变时发出的信号
     * @param rows 新的行数
     * \endif
     */
    void rowsChanged(int rows);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when columns count changes
     * @param cols New columns count
     * \endif
     * 
     * \if CHINESE
     * @brief 列数改变时发出的信号
     * @param cols 新的列数
     * \endif
     */
    void colsChanged(int cols);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when link all X axes setting changes
     * @param enabled New setting value
     * \endif
     * 
     * \if CHINESE
     * @brief 链接所有X轴设置改变时发出的信号
     * @param enabled 新的设置值
     * \endif
     */
    void linkAllXChanged(bool enabled);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when link all Y axes setting changes
     * @param enabled New setting value
     * \endif
     * 
     * \if CHINESE
     * @brief 链接所有Y轴设置改变时发出的信号
     * @param enabled 新的设置值
     * \endif
     */
    void linkAllYChanged(bool enabled);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when legend enabled setting changes
     * @param enabled New setting value
     * \endif
     * 
     * \if CHINESE
     * @brief 图例启用设置改变时发出的信号
     * @param enabled 新的设置值
     * \endif
     */
    void legendEnabledChanged(bool enabled);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when resizable setting changes
     * @param enabled New setting value
     * \endif
     * 
     * \if CHINESE
     * @brief 可调整大小设置改变时发出的信号
     * @param enabled 新的设置值
     * \endif
     */
    void resizableChanged(bool enabled);
    
private:
    QString m_title = QStringLiteral("Subplots Layout");
    int m_rows = 2;
    int m_cols = 2;
    bool m_linkAllX = true;
    bool m_linkAllY = false;
    bool m_legendEnabled = true;
    bool m_resizable = true;
    
    QIM::QImSubplotsNode* m_subplotsNode = nullptr;
    QList<QIM::QImPlotNode*> m_plotNodes;
};

#endif // SUBPLOTSFUNCTION_H