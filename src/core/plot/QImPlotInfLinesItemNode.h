#ifndef QIMPLOTINFLINESITEMNODE_H
#define QIMPLOTINFLINESITEMNODE_H
#include <QColor>
#include <initializer_list>
#include <vector>
#include "QImPlotItemNode.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot infinite lines visualization
 *
 * @class QImPlotInfLinesItemNode
 * @ingroup plot_items
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot infinite lines.
 *          Infinite lines are vertical or horizontal lines that extend infinitely
 *          across the plot area, useful for drawing reference lines, asymptotes,
 *          or threshold markers.
 *          Each value in the data array represents one infinite line position:
 *          - Vertical mode (default): values are X coordinates for vertical lines
 *          - Horizontal mode: values are Y coordinates for horizontal lines
 *          Supports vertical (default) and horizontal orientations.
 *          Inherits from QImPlotItemNode and follows the same PIMPL design pattern
 *          as other plot item nodes for consistency.
 *
 * @note Infinite lines are typically used to mark important values like
 *       thresholds, reference values, or asymptotes. Multiple lines can be
 *       drawn with a single node by providing an array of values.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot无限线的Qt风格封装
 *
 * @class QImPlotInfLinesItemNode
 * @ingroup plot_items
 *
 * @details 为ImPlot无限线提供Qt风格的保留模式封装。
 *          无限线是垂直或水平的线条，在绘图区域内无限延伸，
 *          适用于绘制参考线、渐近线或阈值标记。
 *          数据数组中的每个值代表一条无限线的位置：
 *          - 垂直模式（默认）：值为垂直线的X坐标
 *          - 水平模式：值为水平线的Y坐标
 *          支持垂直（默认）和水平方向。
 *          继承自QImPlotItemNode，并遵循与其他绘图项节点相同的PIMPL设计模式以保持一致性。
 *
 * @note 无限线通常用于标记重要值，如阈值、参考值或渐近线。
 *       通过提供值数组，单个节点可以绘制多条无限线。
 *
 * @param[in] parent 父QObject对象（可选）
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotInfLinesItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotInfLinesItemNode)
    Q_PROPERTY(bool horizontal READ isHorizontal WRITE setHorizontal NOTIFY orientationChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
public:
    // Unique type identifier for QImPlotInfLinesItemNode
    enum
    {
        Type = InnerType + 8
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotInfLinesItemNode with optional parent
    QImPlotInfLinesItemNode(QObject* parent = nullptr);

    // Destroys the QImPlotInfLinesItemNode
    ~QImPlotInfLinesItemNode();

    //----------------------------------------------------
    // Data setting interface
    //----------------------------------------------------

    // Sets a single value for one infinite line (convenience method)
    void setValue(double value);

    // Sets infinite lines data from a container of values
    template< typename Container >
    void setValues(const Container& values);

    // Sets infinite lines data from a container of values (move semantics)
    template< typename Container >
    void setValues(Container&& values);

    // Sets infinite lines data from raw pointer and size
    void setValues(const double* values, int count);

    // Sets infinite lines data from initializer list
    void setValues(std::initializer_list< double > values);

    // Sets infinite lines data from std::vector (move semantics)
    void setValues(std::vector< double >&& values);

    // Gets the number of values (number of infinite lines)
    int count() const;

    // Gets the value at specified index
    double value(int index) const;

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    // Checks if horizontal orientation is enabled
    bool isHorizontal() const;

    // Sets horizontal orientation
    void setHorizontal(bool horizontal);

    // Gets the infinite lines color
    QColor color() const;

    // Sets the infinite lines color
    void setColor(const QColor& color);

    // Gets the raw ImPlotInfLinesFlags
    int infLinesFlags() const;

    // Sets the raw ImPlotInfLinesFlags
    void setInfLinesFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when orientation changes
     * @param[in] horizontal New horizontal orientation state
     * @details Triggered by setHorizontal() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 方向更改时触发
     * @param[in] horizontal 新水平方向状态
     * @details 当值实际更改时由setHorizontal()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void orientationChanged(bool horizontal);

    /**
     * \if ENGLISH
     * @brief Emitted when infinite lines color changes
     * @param[in] color New infinite lines color
     * @details Triggered by setColor() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 无限线颜色更改时触发
     * @param[in] color 新无限线颜色
     * @details 当值实际更改时由setColor()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void colorChanged(const QColor& color);

    /**
     * \if ENGLISH
     * @brief Emitted when data changes
     * @details Triggered by setValues() when new data is assigned.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 数据更改时触发
     * @details 当分配新数据时由setValues()触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void dataChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when infinite lines flags change
     * @details Triggered by any flag property change (horizontal, etc.).
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 无限线标志更改时触发
     * @details 任何标志属性更改（水平方向等）时触发。
     *          连接到更新UI元素或执行相关操作。
     * \endif
     */
    void infLinesFlagChanged();

protected:
    // Begins drawing the infinite lines
    virtual bool beginDraw() override;
};

template< typename Container >
inline void QImPlotInfLinesItemNode::setValues(const Container& values)
{
    setValues(values.data(), static_cast< int >(values.size()));
}

template< typename Container >
inline void QImPlotInfLinesItemNode::setValues(Container&& values)
{
    setValues(std::move(values));
}

}  // namespace QIM

#endif  // QIMPLOTINFLINESITEMNODE_H
