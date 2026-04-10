#ifndef QIMPLOTDUMMYITEMNODE_H
#define QIMPLOTDUMMYITEMNODE_H
#include "QImPlotItemNode.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot dummy legend items
 *
 * @class QImPlotDummyItemNode
 * @ingroup plot_items
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot dummy items.
 *          Dummy items do not render geometry in the plot area. They are mainly used
 *          to reserve legend entries, create legend grouping, or align custom legend semantics.
 *
 * @note The inherited label property is the meaningful payload of this node.
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot虚拟图例项的Qt风格封装
 *
 * @class QImPlotDummyItemNode
 * @ingroup plot_items
 *
 * @details 为ImPlot虚拟项提供Qt风格的保留模式封装。
 *          虚拟项不会在绘图区绘制几何图元，主要用于预留图例项、构建图例分组或表达自定义图例语义。
 *
 * @note 这个节点最重要的数据就是继承自基类的 label 属性。
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotDummyItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotDummyItemNode)

public:
    enum
    {
        Type = InnerType + 11
    };

    virtual int type() const override
    {
        return Type;
    }

    // Constructs a dummy item node.
    explicit QImPlotDummyItemNode(QObject* parent = nullptr);

    // Destroys the dummy item node.
    ~QImPlotDummyItemNode();

    // Returns the raw ImPlotDummyFlags value.
    int dummyFlags() const;

    // Sets the raw ImPlotDummyFlags value.
    void setDummyFlags(int flags);

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when dummy flags change.
     * \endif
     *
     * \if CHINESE
     * @brief 当虚拟项标志位发生变化时触发。
     * \endif
     */
    void dummyFlagChanged();

protected:
    // Draws the dummy legend item.
    virtual bool beginDraw() override;
};

}  // namespace QIM

#endif  // QIMPLOTDUMMYITEMNODE_H
