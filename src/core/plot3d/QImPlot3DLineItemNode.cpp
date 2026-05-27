#include "QImPlot3DLineItemNode.h"
#include <QColor>
#include "implot3d.h"
#include "implot3d_internal.h"
#include "QImOptionalColor.h"
namespace QIM
{

class QImPlot3DLineItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlot3DLineItemNode)

public:
    PrivateData(QImPlot3DLineItemNode* p);

public:
    std::unique_ptr<QImAbstractXYZDataSeries> dataSeries;            ///< XYZ data series
    QImOptionalColor color;                                         ///< 颜色（延迟初始化：首次渲染时捕获ImPlot3D默认颜色）
    QImTrackedValue< float > lineWeight { 1.0f };                    ///< 线宽
    ImPlot3DLineFlags flags { ImPlot3DLineFlags_None };              ///< Line flags (must be named 'flags' for macros)
};

QImPlot3DLineItemNode::PrivateData::PrivateData(QImPlot3DLineItemNode* p)
    : q_ptr(p)
{
}

//----------------------------------------------------
// QImPlot3DLineItemNode
//----------------------------------------------------

QImPlot3DLineItemNode::QImPlot3DLineItemNode(QObject* par)
    : QImPlot3DItemNode(par)
    , QIM_PIMPL_CONSTRUCT
{
}

QImPlot3DLineItemNode::~QImPlot3DLineItemNode()
{
}

//----------------------------------------------------
// Data setting
//----------------------------------------------------

void QImPlot3DLineItemNode::setData(QImAbstractXYZDataSeries* series)
{
    QIM_D(d);
    d->dataSeries.reset(series);
}

QImAbstractXYZDataSeries* QImPlot3DLineItemNode::data() const
{
    QIM_DC(d);
    return d->dataSeries.get();
}

//----------------------------------------------------
// Style properties
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Sets the line color
 * @param c New color value as QColor
 * @details Stores the color in an optional QImTrackedValue. The dirty flag is automatically
 *          set when the value changes. In immediate mode rendering, each frame needs to
 *          call SetNextLineStyle if a color is set.
 * \endif
 *
 * \if CHINESE
 * @brief 设置线条颜色
 * @param c 新的颜色值（QColor 类型）
 * @details 将颜色存储在 optional QImTrackedValue 中。当值变化时自动设置 dirty 标记。
 *          在即时模式渲染中，如果设置了颜色，每帧都需要调用 SetNextLineStyle。
 * \endif
 */
void QImPlot3DLineItemNode::setColor(const QColor& c)
{
    QIM_D(d);
    d->color = toImVec4(c);
    Q_EMIT colorChanged(c);
}

/**
 * \if ENGLISH
 * @brief Gets the line color
 * @return Current color as QColor, or invalid QColor() if using ImPlot3D default
 * @details Returns the explicit color if set, otherwise returns default QColor().
 *          On first render without explicit color, ImPlot3D's default color is captured.
 * \endif
 *
 * \if CHINESE
 * @brief 获取线条颜色
 * @return 当前颜色（QColor），使用 ImPlot3D 默认颜色时返回无效 QColor()
 * @details 如果设置了显式颜色则返回该颜色，否则返回默认 QColor()。
 *          首次渲染时未设置显式颜色，会捕获 ImPlot3D 的默认颜色。
 * \endif
 */
QColor QImPlot3DLineItemNode::color() const
{
    QIM_DC(d);
    return (d->color.has_value()) ? toQColor(d->color->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Sets the line weight (thickness in pixels)
 * @param weight New line weight value
 * @details Assigns the value via QImTrackedValue which tracks dirty state automatically.
 * \endif
 *
 * \if CHINESE
 * @brief 设置线宽（像素厚度）
 * @param weight 新的线宽值
 * @details 通过 QImTrackedValue 赋值，自动跟踪 dirty 状态。
 * \endif
 */
void QImPlot3DLineItemNode::setLineWeight(float weight)
{
    QIM_D(d);
    d->lineWeight = weight;
    Q_EMIT lineWeightChanged(weight);
}

/**
 * \if ENGLISH
 * @brief Gets the line weight (thickness in pixels)
 * @return Current line weight value
 * \endif
 *
 * \if CHINESE
 * @brief 获取线宽（像素厚度）
 * @return 当前线宽值
 * \endif
 */
float QImPlot3DLineItemNode::lineWeight() const
{
    QIM_DC(d);
    return d->lineWeight.value();
}

//----------------------------------------------------
// Line flags (positive semantics)
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Returns raw ImPlot3DLineFlags value
 * @return Current combined line flags
 * \endif
 *
 * \if CHINESE
 * @brief 返回原始 ImPlot3DLineFlags 值
 * @return 当前组合的线条标志
 * \endif
 */
int QImPlot3DLineItemNode::lineFlags() const
{
    QIM_DC(d);
    return d->flags;
}

/**
 * \if ENGLISH
 * @brief Sets raw ImPlot3DLineFlags value
 * @param flags New combined flags value
 * @details Emits lineFlagChanged if flags actually change.
 * \endif
 *
 * \if CHINESE
 * @brief 设置原始 ImPlot3DLineFlags 值
 * @param flags 新的组合标志值
 * @details 如果标志实际改变则发射 lineFlagChanged。
 * \endif
 */
void QImPlot3DLineItemNode::setLineFlags(int flags)
{
    QIM_D(d);
    if (d->flags != static_cast<ImPlot3DLineFlags>(flags)) {
        d->flags = static_cast<ImPlot3DLineFlags>(flags);
        Q_EMIT lineFlagChanged();
    }
}

// Generate flag accessors using QIMPLOT3D_FLAG_ACCESSOR
// All ImPlot3DLineFlags_Segments, Loop, SkipNaN are Positive→Positive (NOT NoXxx)
QIMPLOT3D_FLAG_ACCESSOR(QImPlot3DLineItemNode, SegmentsEnabled, ImPlot3DLineFlags_Segments, lineFlagChanged)
QIMPLOT3D_FLAG_ACCESSOR(QImPlot3DLineItemNode, LoopEnabled, ImPlot3DLineFlags_Loop, lineFlagChanged)
QIMPLOT3D_FLAG_ACCESSOR(QImPlot3DLineItemNode, SkipNaNEnabled, ImPlot3DLineFlags_SkipNaN, lineFlagChanged)

//----------------------------------------------------
// Rendering
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Renders the 3D line plot with pre-converted data
 * @return false (no endDraw needed)
 * @details Calls SetNextLineStyle with color/weight if set, then PlotLine with XYZ data.
 *          After rendering, captures ImPlot3D default color if no explicit color was set.
 *          All data conversion happens in setters, not here.
 * \endif
 *
 * \if CHINESE
 * @brief 使用预转换数据渲染 3D 线图
 * @return false（无需 endDraw）
 * @details 如果设置了颜色/线宽则调用 SetNextLineStyle，然后使用 XYZ 数据调用 PlotLine。
 *          渲染后，如果未设置显式颜色，则捕获 ImPlot3D 默认颜色。
 *          所有数据转换在 setter 中完成，不在此处。
 * \endif
 */
bool QImPlot3DLineItemNode::beginDraw()
{
    QIM_D(d);

    // Validate data series
    if (!d->dataSeries || !d->dataSeries->isValid()) {
        return false;
    }

    // ImPlot3D is immediate mode rendering, SetNextLineStyle only affects the next PlotLine call
    // Therefore if a color is set, every frame must call SetNextLineStyle
    // Otherwise it inherits the previous line's color state (causing same-color issue for multiple lines)
    if (d->color) {
        ImPlot3D::SetNextLineStyle(d->color->value(), d->lineWeight.value());
    } else {
        ImPlot3D::SetNextLineStyle(IMPLOT3D_AUTO_COL, d->lineWeight.value());
    }

    // Plot the 3D line
    ImPlot3D::PlotLine(labelConstData(),
                       d->dataSeries->xRawData(),
                       d->dataSeries->yRawData(),
                       d->dataSeries->zRawData(),
                       d->dataSeries->size(),
                       d->flags);

    // Capture default color on first render when no explicit color was set
    if (!d->color) {
        d->color = captureItemColor();
    }

    return false;
}

}  // end namespace QIM
