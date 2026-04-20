#include "QImPlotLegendNode.h"
#include "implot.h"
#include "QtImGuiUtils.h"
#include <QDebug>

namespace QIM
{

class QImPlotLegendNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotLegendNode)

public:
    PrivateData(QImPlotLegendNode* p);

    // 位置相关
    ImPlotLocation location { ImPlotLocation_NorthEast };
    // ImPlotLegendFlags（存储原始标志位）
    ImPlotLegendFlags flags { ImPlotLegendFlags_NoMenus };
};

QImPlotLegendNode::PrivateData::PrivateData(QImPlotLegendNode* p) : q_ptr(p)
{
}


// ----------------------------------------------------
// QImPlotLegendNode 实现
// ----------------------------------------------------
/**
 * \if ENGLISH
 * @brief Constructs a QImPlotLegendNode with optional parent
 * @param[in] parent Parent QObject
 * @details Initializes legend node with default location NorthEast and NoMenus flag.
 *          Auto-ID is disabled as legend nodes are managed internally by QImPlotNode.
 * \endif
 *
 * \if CHINESE
 * @brief 构造 QImPlotLegendNode，可选指定父对象
 * @param[in] parent 父 QObject
 * @details 初始化图例节点，默认位置为东北角，设置 NoMenus 标志。
 *          禁用自动 ID，因为图例节点由 QImPlotNode 内部管理。
 * \endif
 */
QImPlotLegendNode::QImPlotLegendNode(QObject* parent) : QImAbstractNode(parent), QIM_PIMPL_CONSTRUCT
{
    setObjectName(QStringLiteral("PlotLegendNode"));
    setAutoIdEnabled(false);
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotLegendNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotLegendNode 的析构函数
 * \endif
 */
QImPlotLegendNode::~QImPlotLegendNode()
{
}

// ----------------------------------------------------
// 位置相关方法
// ----------------------------------------------------
/**
 * \if ENGLISH
 * @brief Gets the legend location within the plot area
 * @return Current legend location as QImPlotLegendLocation enum value
 * \endif
 *
 * \if CHINESE
 * @brief 获取图例在绘图区域内的位置
 * @return 当前图例位置，以 QImPlotLegendLocation 枚举值返回
 * \endif
 */
QImPlotLegendLocation QImPlotLegendNode::location() const
{
    QIM_DC(d);
    return toQImPlotLegendLocation(d->location);
}

/**
 * \if ENGLISH
 * @brief Sets the legend location within the plot area
 * @param[in] v New legend location as QImPlotLegendLocation enum value
 * @details Emits locationChanged() signal only when value changes.
 * \endif
 *
 * \if CHINESE
 * @brief 设置图例在绘图区域内的位置
 * @param[in] v 新图例位置，以 QImPlotLegendLocation 枚举值指定
 * @details 仅在值改变时触发 locationChanged() 信号。
 * \endif
 */
void QImPlotLegendNode::setLocation(QImPlotLegendLocation v)
{
    QIM_D(d);
    ImPlotLocation iml = toImPlotLocation(v);
    if (d->location != iml) {
        d->location = iml;
        Q_EMIT locationChanged(v);
    }
}

/**
 * \if ENGLISH
 * @brief Gets the legend location as Qt alignment flags
 * @return Qt::Alignment corresponding to current legend location
 * @details Converts ImPlotLocation to Qt alignment for compatibility with Qt layouts.
 * \endif
 *
 * \if CHINESE
 * @brief 获取图例位置对应的 Qt 对齐标志
 * @return 对应当前图例位置的 Qt::Alignment
 * @details 将 ImPlotLocation 转换为 Qt 对齐标志，以便与 Qt 布局兼容。
 * \endif
 */
Qt::Alignment QImPlotLegendNode::alignment() const
{
    QIM_DC(d);
    return toQAlignment(d->location);
}

/**
 * \if ENGLISH
 * @brief Sets the legend location using Qt alignment flags
 * @param[in] a Qt::Alignment value (e.g., Qt::AlignLeft, Qt::AlignTop)
 * @details Converts Qt alignment to ImPlotLocation. Emits locationChanged() signal on change.
 * \endif
 *
 * \if CHINESE
 * @brief 使用 Qt 对齐标志设置图例位置
 * @param[in] a Qt::Alignment 值（例如 Qt::AlignLeft, Qt::AlignTop）
 * @details 将 Qt 对齐转换为 ImPlotLocation。值改变时触发 locationChanged() 信号。
 * \endif
 */
void QImPlotLegendNode::setAlignment(Qt::Alignment a)
{
    QIM_D(d);
    ImPlotLocation iml = toImPlotLocation(a);
    if (d->location != iml) {
        d->location = iml;
        Q_EMIT locationChanged(toQImPlotLegendLocation(iml));
    }
}

// ----------------------------------------------------
// 方向相关方法
// ----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Sets the legend layout orientation (horizontal or vertical)
 * @param[in] vh Qt::Horizontal for horizontal layout, Qt::Vertical for vertical layout
 * @details Controls ImPlotLegendFlags_Horizontal flag. Emits legendFlagChanged() signal on change.
 * \endif
 *
 * \if CHINESE
 * @brief 设置图例布局方向（水平或垂直）
 * @param[in] vh Qt::Horizontal 为水平布局，Qt::Vertical 为垂直布局
 * @details 控制 ImPlotLegendFlags_Horizontal 标志。值改变时触发 legendFlagChanged() 信号。
 * \endif
 */
void QImPlotLegendNode::setOrientation(Qt::Orientation vh)
{
    QIM_D(d);
    const bool isCurrentHorizontal = (d->flags & ImPlotLegendFlags_Horizontal) != 0;
    const bool isTargetHorizontal  = (vh == Qt::Horizontal);

    // 方向一致则直接返回
    if (isCurrentHorizontal == isTargetHorizontal) {
        return;
    }

    // 方向不一致时，更新标志位
    if (isTargetHorizontal) {
        d->flags |= ImPlotLegendFlags_Horizontal;  // 设置水平标志
    } else {
        d->flags &= (~ImPlotLegendFlags_Horizontal);  // 清除水平标志
    }
    Q_EMIT legendFlagChanged();
}

/**
 * \if ENGLISH
 * @brief Gets the legend layout orientation
 * @return Qt::Horizontal for horizontal layout, Qt::Vertical for vertical layout
 * \endif
 *
 * \if CHINESE
 * @brief 获取图例布局方向
 * @return Qt::Horizontal 为水平布局，Qt::Vertical 为垂直布局
 * \endif
 */
Qt::Orientation QImPlotLegendNode::orientation() const
{
    QIM_DC(d);
    return (d->flags & ImPlotLegendFlags_Horizontal) ? Qt::Horizontal : Qt::Vertical;
}

// clang-format off

// ----------------------------------------------------
// 外观相关方法
// ----------------------------------------------------
QIMPLOT_FLAG_ACCESSOR(QImPlotLegendNode, Outside, ImPlotLegendFlags_Outside, legendFlagChanged)

// ImPlotLegendFlags_NoButtons
QIMPLOT_FLAG_ENABLED_ACCESSOR(QImPlotLegendNode, ButtonEnabled, ImPlotLegendFlags_NoButtons, legendFlagChanged)
// ImPlotLegendFlags_NoHighlightItem
QIMPLOT_FLAG_ENABLED_ACCESSOR(QImPlotLegendNode, HighlightItemEnabled, ImPlotLegendFlags_NoHighlightItem, legendFlagChanged)
// ImPlotLegendFlags_NoButtons
QIMPLOT_FLAG_ENABLED_ACCESSOR(QImPlotLegendNode, HighlightAxisEnabled, ImPlotLegendFlags_NoHighlightAxis, legendFlagChanged)
//ImPlotLegendFlags_Sort
QIMPLOT_FLAG_ACCESSOR(QImPlotLegendNode, Sort, ImPlotLegendFlags_Sort, legendFlagChanged)
//ImPlotLegendFlags_Reverse
QIMPLOT_FLAG_ACCESSOR(QImPlotLegendNode, Reverse, ImPlotLegendFlags_Reverse, legendFlagChanged)

// clang-format on
//===============================================================
// name
//===============================================================
/**
 * \if ENGLISH
 * @brief Gets the raw ImPlotLegendFlags bitmask value
 * @return Current ImPlotLegendFlags as int
 * \endif
 *
 * \if CHINESE
 * @brief 获取原始 ImPlotLegendFlags 位掩码值
 * @return 当前 ImPlotLegendFlags 的 int 值
 * \endif
 */
int QImPlotLegendNode::legendFlags() const
{
    QIM_DC(d);
    return d->flags;
}

/**
 * \if ENGLISH
 * @brief Sets the raw ImPlotLegendFlags bitmask value
 * @param[in] flags New ImPlotLegendFlags bitmask value
 * @details Directly replaces internal flag storage. Emits legendFlagChanged() signal on change.
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始 ImPlotLegendFlags 位掩码值
 * @param[in] flags 新的 ImPlotLegendFlags 位掩码值
 * @details 直接替换内部标志存储。值改变时触发 legendFlagChanged() 信号。
 * \endif
 */
void QImPlotLegendNode::setLegendFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast< ImPlotLegendFlags >(flags);
        Q_EMIT legendFlagChanged();
    }
}


// ----------------------------------------------------
// 渲染方法
// ----------------------------------------------------
/**
 * \if ENGLISH
 * @brief Renders the legend by calling ImPlot::SetupLegend()
 * @return false (legend does not require endDraw)
 * @details Applies current location and flags to ImPlot legend setup during the plot's beginDraw phase.
 * \endif
 *
 * \if CHINESE
 * @brief 通过调用 ImPlot::SetupLegend() 渲染图例
 * @return false（图例不需要 endDraw）
 * @details 在绘图的 beginDraw 阶段将当前位置和标志应用到 ImPlot 图例设置。
 * \endif
 */
bool QImPlotLegendNode::beginDraw()
{
    QIM_D(d);
    // 在渲染时应用图例设置
    ImPlot::SetupLegend(d->location, d->flags);
    return false;  // 不需要特别的渲染上下文
}


}  // namespace QIM
