#ifndef QIMPLOTDUMMYITEMNODE_H
#define QIMPLOTDUMMYITEMNODE_H
#include "QImPlotItemNode.h"
#include <QColor>

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot dummy item visualization
 *
 * @class QImPlotDummyItemNode
 * @ingroup plot_items
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot dummy items.
 *          Dummy items create placeholder entries in the legend with a color icon,
 *          useful for custom legend entries or annotations that don't correspond
 *          to actual plot data.
 *          Inherits from QImPlotItemNode and follows the same PIMPL design pattern
 *          as other plot item nodes for consistency.
 *
 * @note Dummy items only appear in the legend and do not render any visible
 *       graphics in the plot area. They are typically used to add custom
 *       legend entries for manual annotations or grouped data representations.
 *
 * @param[in] parent Parent QObject (optional)
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot 虚拟项的 Qt 风格封装
 *
 * @class QImPlotDummyItemNode
 * @ingroup plot_items
 *
 * @details 为 ImPlot 虚拟项提供 Qt 风格的保留模式封装。
 *          虚拟项在图例中创建带有颜色图标的占位符条目，
 *          适用于不与实际绘图数据对应的自定义图例条目或注释。
 *          继承自 QImPlotItemNode，并遵循与其他绘图项节点相同的 PIMPL 设计模式以保持一致性。
 *
 * @note 虚拟项仅出现在图例中，不在绘图区域渲染任何可见图形。
 *       它们通常用于添加自定义图例条目，用于手动注释或分组数据表示。
 *
 * @param[in] parent 父 QObject 对象（可选）
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotDummyItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotDummyItemNode)

    /**
     * \if ENGLISH
     * @property QImPlotDummyItemNode::color
     * @brief Color of the dummy item legend icon
     *
     * @details Defines the color for the dummy item's legend icon.
     *          When not set, uses ImPlot's default color sequence.
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     *
     * \if CHINESE
     * @property QImPlotDummyItemNode::color
     * @brief 虚拟项图例图标的颜色
     *
     * @details 定义虚拟项图例图标的颜色。
     *          未设置时，使用 ImPlot 的默认颜色序列。
     * @accessors READ color WRITE setColor NOTIFY colorChanged
     * \endif
     */
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    // Unique type identifier for QImPlotDummyItemNode
    enum
    {
        Type = InnerType + 9
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotDummyItemNode with optional parent
    QImPlotDummyItemNode(QObject* parent = nullptr);

    // Destroys the QImPlotDummyItemNode
    ~QImPlotDummyItemNode();

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    // Gets the dummy item color
    QColor color() const;

    // Sets the dummy item color
    void setColor(const QColor& color);

    // Gets the raw ImPlotDummyFlags
    int dummyFlags() const;

    // Sets the raw ImPlotDummyFlags
    void setDummyFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when dummy item color changes
     * @param color New dummy item color
     * @details Triggered by setColor() when value actually changes.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 虚拟项颜色更改时触发
     * @param color 新虚拟项颜色
     * @details 当值实际更改时由 setColor() 触发。
     *          连接到更新 UI 元素或执行相关操作。
     * \endif
     */
    void colorChanged(const QColor& color);

    /**
     * \if ENGLISH
     * @brief Emitted when dummy item flags change
     * @details Triggered by setDummyFlags() when flags change.
     *          Connect to update UI elements or perform related actions.
     * \endif
     *
     * \if CHINESE
     * @brief 虚拟项标志更改时触发
     * @details 当标志更改时由 setDummyFlags() 触发。
     *          连接到更新 UI 元素或执行相关操作。
     * \endif
     */
    void dummyFlagsChanged();

protected:
    // Begins drawing the dummy item
    virtual bool beginDraw() override;
};

}  // namespace QIM

#endif  // QIMPLOTDUMMYITEMNODE_H
