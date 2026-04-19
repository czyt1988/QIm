#include "QImPlot3DDummyItemNode.h"
#include <optional>
#include "implot3d.h"
#include "implot3d_internal.h"
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"

namespace QIM
{

class QImPlot3DDummyItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlot3DDummyItemNode)
public:
    PrivateData(QImPlot3DDummyItemNode* p);

    // Style tracking values
    QImOptional3DColor color;
};

QImPlot3DDummyItemNode::PrivateData::PrivateData(QImPlot3DDummyItemNode* p)
    : q_ptr(p)
{
}

/**
 * \if ENGLISH
 * @brief Constructor for QImPlot3DDummyItemNode
 * @param parent Parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief QImPlot3DDummyItemNode 的构造函数
 * @param parent 父 QObject
 * \endif
 */
QImPlot3DDummyItemNode::QImPlot3DDummyItemNode(QObject* parent)
    : QImPlot3DItemNode(parent)
    , QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlot3DDummyItemNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlot3DDummyItemNode 的析构函数
 * \endif
 */
QImPlot3DDummyItemNode::~QImPlot3DDummyItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Get dummy item color
 * @return Current QColor, or invalid QColor() if using ImPlot3D default
 * @details Returns the explicit color if set, otherwise returns default QColor().
 *          On first render without explicit color, ImPlot3D's default color is captured.
 * \endif
 *
 * \if CHINESE
 * @brief 获取虚拟项颜色
 * @return 当前的 QColor，使用 ImPlot3D 默认颜色时返回无效 QColor()
 * @details 如果设置了显式颜色则返回该颜色，否则返回默认 QColor()。
 *          首次渲染时未设置显式颜色，会捕获 ImPlot3D 的默认颜色。
 * \endif
 */
QColor QImPlot3DDummyItemNode::color() const
{
    QIM_DC(d);
    return (d->color.has_value()) ? toQColor(d->color->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Set dummy item color
 * @param c QColor for the dummy item
 * @details Stores the color in an optional QImTrackedValue. The dirty flag is automatically
 *          set when the value changes. In immediate mode rendering, each frame needs to
 *          call SetNextLineStyle if a color is set.
 * \endif
 *
 * \if CHINESE
 * @brief 设置虚拟项颜色
 * @param c 虚拟项的 QColor
 * @details 将颜色存储在 optional QImTrackedValue 中。当值变化时自动设置 dirty 标记。
 *          在即时模式渲染中，如果设置了颜色，每帧都需要调用 SetNextLineStyle。
 * \endif
 */
void QImPlot3DDummyItemNode::setColor(const QColor& c)
{
    QIM_D(d);
    d->color = toImVec4(c);
    Q_EMIT colorChanged(c);
}

/**
 * \if ENGLISH
 * @brief Begin drawing implementation
 * @return false to prevent endDraw from being called
 * @details Calls ImPlot3D::PlotDummy with ImPlot3DDummyFlags_None.
 *          After rendering, captures default color if not explicitly set.
 * \endif
 *
 * \if CHINESE
 * @brief 开始绘制实现
 * @return false 以防止调用 endDraw
 * @details 使用 ImPlot3DDummyFlags_None 调用 ImPlot3D::PlotDummy。
 *          渲染后，如果未设置显式颜色，则捕获默认颜色。
 * \endif
 */
bool QImPlot3DDummyItemNode::beginDraw()
{
    QIM_D(d);

    // Call ImPlot3D API with None flag (only option for dummy items)
    ImPlot3D::PlotDummy(labelConstData(), ImPlot3DDummyFlags_None);

    // Capture default color on first render when no explicit color was set
    if (!d->color) {
        d->color = captureItemColor();
    }

    return false;
}

}  // namespace QIM
