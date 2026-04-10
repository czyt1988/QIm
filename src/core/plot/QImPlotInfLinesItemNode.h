#ifndef QIMPLOTINFLINESITEMNODE_H
#define QIMPLOTINFLINESITEMNODE_H
#include <QColor>
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
 *          Infinite lines are axis-aligned guide lines extending across the whole plot
 *          area. The node manages a list of coordinate values and supports vertical
 *          and horizontal orientations through Qt properties and signal-slot updates.
 *
 * @note In vertical mode the values are interpreted on the current x-axis.
 *       In horizontal mode the values are interpreted on the current y-axis.
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
 *          无限线是横贯整个绘图区的轴对齐参考线。
 *          该节点内部管理一组坐标值，并通过Qt属性和信号槽支持垂直/水平两种方向。
 *
 * @note 垂直模式下，数值作用于当前x轴。
 *       水平模式下，数值作用于当前y轴。
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotInfLinesItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotInfLinesItemNode)

    Q_PROPERTY(bool horizontal READ isHorizontal WRITE setHorizontal NOTIFY infLinesFlagChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    enum
    {
        Type = InnerType + 8
    };

    virtual int type() const override
    {
        return Type;
    }

    // Constructs an infinite lines plot item node.
    explicit QImPlotInfLinesItemNode(QObject* parent = nullptr);

    // Destroys the infinite lines plot item node.
    ~QImPlotInfLinesItemNode();

    // Sets all line coordinate values.
    void setValues(const std::vector< double >& values);

    // Sets all line coordinate values with move semantics.
    void setValues(std::vector< double >&& values);

    // Sets all line coordinate values from a generic container.
    template< typename Container >
    void setValues(const Container& values);

    // Returns the stored coordinate values.
    const std::vector< double >& values() const;

    // Returns true when lines are horizontal.
    bool isHorizontal() const;

    // Enables or disables horizontal mode.
    void setHorizontal(bool on);

    // Returns the raw ImPlotInfLinesFlags value.
    int infLinesFlags() const;

    // Sets the raw ImPlotInfLinesFlags value.
    void setInfLinesFlags(int flags);

    // Sets the line color.
    void setColor(const QColor& c);

    // Returns the line color.
    QColor color() const;

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when the stored line coordinates change.
     * \endif
     *
     * \if CHINESE
     * @brief 当无限线坐标数据发生变化时触发。
     * \endif
     */
    void valuesChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when infinite line flags change.
     * \endif
     *
     * \if CHINESE
     * @brief 当无限线标志位发生变化时触发。
     * \endif
     */
    void infLinesFlagChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when the line color changes.
     * @param color New line color.
     * \endif
     *
     * \if CHINESE
     * @brief 当线条颜色发生变化时触发。
     * @param color 新的线条颜色。
     * \endif
     */
    void colorChanged(const QColor& color);

protected:
    // Draws the infinite lines item.
    virtual bool beginDraw() override;
};

template< typename Container >
inline void QImPlotInfLinesItemNode::setValues(const Container& values)
{
    std::vector< double > copied;
    copied.reserve(static_cast< std::size_t >(values.size()));
    for (const auto& value : values) {
        copied.push_back(static_cast< double >(value));
    }
    setValues(std::move(copied));
}

}  // namespace QIM

#endif  // QIMPLOTINFLINESITEMNODE_H
