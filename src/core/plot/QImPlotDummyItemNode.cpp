#include "QImPlotDummyItemNode.h"
#include <optional>
#include "implot.h"
#include "implot_internal.h"
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"

namespace QIM
{

class QImPlotDummyItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotDummyItemNode)
public:
    PrivateData(QImPlotDummyItemNode* p);

    ImPlotDummyFlags flags { ImPlotDummyFlags_None };
    // Style tracking values
    std::optional< QImTrackedValue< ImVec4, QIM::ImVecComparator< ImVec4 > > > color;
};

QImPlotDummyItemNode::PrivateData::PrivateData(QImPlotDummyItemNode* p) : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlotDummyItemNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotDummyItemNode 的构造函数
 * @param parent 父 QObject
 * \endif
 */
QImPlotDummyItemNode::QImPlotDummyItemNode(QObject* parent) : QImPlotItemNode(parent), QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotDummyItemNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotDummyItemNode 的析构函数
 * \endif
 */
QImPlotDummyItemNode::~QImPlotDummyItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Get dummy item color
 * @return Current QColor
 * \endif
 *
 * \if CHINESE
 * @brief 获取虚拟项颜色
 * @return 当前的 QColor
 * \endif
 */
QColor QImPlotDummyItemNode::color() const
{
    return (d_ptr->color.has_value()) ? toQColor(d_ptr->color->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Set dummy item color
 * @param c QColor for the dummy item
 * \endif
 *
 * \if CHINESE
 * @brief 设置虚拟项颜色
 * @param c 虚拟项的 QColor
 * \endif
 */
void QImPlotDummyItemNode::setColor(const QColor& c)
{
    d_ptr->color = toImVec4(c);
    emit colorChanged(c);
}

/**
 * \if ENGLISH
 * @brief Get raw ImPlotDummyFlags
 * @return Current ImPlotDummyFlags
 * \endif
 *
 * \if CHINESE
 * @brief 获取原始的 ImPlotDummyFlags
 * @return 当前的 ImPlotDummyFlags
 * \endif
 */
int QImPlotDummyItemNode::dummyFlags() const
{
    QIM_DC(d);
    return d->flags;
}

/**
 * \if ENGLISH
 * @brief Set raw ImPlotDummyFlags
 * @param flags New ImPlotDummyFlags
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始的 ImPlotDummyFlags
 * @param flags 新的 ImPlotDummyFlags
 * \endif
 */
void QImPlotDummyItemNode::setDummyFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast< ImPlotDummyFlags >(flags);
        Q_EMIT dummyFlagsChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Begin drawing implementation
 * @return false to prevent endDraw from being called
 * @details Calls ImPlot::PlotDummy after setting style.
 * \endif
 *
 * \if CHINESE
 * @brief 开始绘制实现
 * @return false 以防止调用 endDraw
 * @details 设置样式后调用 ImPlot::PlotDummy。
 * \endif
 */
bool QImPlotDummyItemNode::beginDraw()
{
    QIM_D(d);

    // Apply style
    if (d->color && d->color->is_dirty()) {
        ImPlot::SetNextLineStyle(d->color->value());
    }

    // Call ImPlot API
    ImPlot::PlotDummy(labelConstData(), d->flags);

    // Update item status
    if(!imPlotItem()){
        ImPlotContext* ct    = ImPlot::GetCurrentContext();
        ImPlotItem* plotItem = ct->PreviousItem;
        setImPlotItem(plotItem);
        if (plotItem && plotItem->Show != QImAbstractNode::isVisible()) {
            QImAbstractNode::setVisible(plotItem->Show);
        }
    }
    if (!d->color) {
        // First render without explicit color, get default color from ImPlot
        d->color = ImPlot::GetLastItemColor();
    }

    return false;
}

}  // namespace QIM
