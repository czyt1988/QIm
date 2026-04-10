#include "QImPlotDummyItemNode.h"
#include "implot.h"

namespace QIM
{

class QImPlotDummyItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotDummyItemNode)
public:
    explicit PrivateData(QImPlotDummyItemNode* p) : q_ptr(p)
    {
    }

    ImPlotDummyFlags flags { ImPlotDummyFlags_None };
};

/**
 * \if ENGLISH
 * @brief Constructs a dummy item node.
 * @param[in] parent Parent QObject.
 * \endif
 *
 * \if CHINESE
 * @brief 构造一个虚拟项节点。
 * @param[in] parent 父QObject对象。
 * \endif
 */
QImPlotDummyItemNode::QImPlotDummyItemNode(QObject* parent)
    : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destroys the dummy item node.
 * \endif
 *
 * \if CHINESE
 * @brief 销毁虚拟项节点。
 * \endif
 */
QImPlotDummyItemNode::~QImPlotDummyItemNode()
{
}

int QImPlotDummyItemNode::dummyFlags() const
{
    return d_ptr->flags;
}

/**
 * \if ENGLISH
 * @brief Sets the raw ImPlotDummyFlags value.
 * @param[in] flags New ImPlotDummyFlags value.
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始ImPlotDummyFlags值。
 * @param[in] flags 新的ImPlotDummyFlags值。
 * \endif
 */
void QImPlotDummyItemNode::setDummyFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast< ImPlotDummyFlags >(flags);
        Q_EMIT dummyFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Draws the dummy legend item through ImPlot::PlotDummy.
 * @return Always returns false because the item has no child scope to recurse into.
 * \endif
 *
 * \if CHINESE
 * @brief 通过ImPlot::PlotDummy绘制虚拟图例项。
 * @return 始终返回false，因为该绘图项没有子作用域需要递归渲染。
 * \endif
 */
bool QImPlotDummyItemNode::beginDraw()
{
    ImPlot::PlotDummy(labelConstData(), static_cast< ImPlotDummyFlags >(d_ptr->flags));
    return false;
}

}  // namespace QIM
