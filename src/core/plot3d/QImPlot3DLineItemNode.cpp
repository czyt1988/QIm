#include "QImPlot3DLineItemNode.h"
#include <QColor>
#include "implot3d.h"

namespace QIM
{

class QImPlot3DLineItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlot3DLineItemNode)

public:
    PrivateData(QImPlot3DLineItemNode* p);

public:
    std::unique_ptr<QImAbstractXYZDataSeries> dataSeries;            ///< XYZ data series
    ImVec4 colorVec4 { IMPLOT3D_AUTO_COL };                          ///< Pre-converted color for beginDraw
    float lineWeightVal { IMPLOT3D_AUTO };                           ///< Line weight value
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
    return d_ptr->dataSeries.get();
}

//----------------------------------------------------
// Style properties
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Sets the line color with beginDraw minimization
 * @param c New color (converted to ImVec4 in setter, not in beginDraw)
 * @details Pre-converts QColor to ImVec4 for efficient rendering.
 *          Uses alpha=-1 as auto color indicator.
 * \endif
 *
 * \if CHINESE
 * @brief 设置线条颜色（最小化 beginDraw）
 * @param c 新颜色（在 setter 中转换为 ImVec4，不在 beginDraw 中）
 * @details 预转换 QColor 为 ImVec4 以实现高效渲染。
 *          使用 alpha=-1 作为自动颜色指示器。
 * \endif
 */
void QImPlot3DLineItemNode::setColor(const QColor& c)
{
    QIM_D(d);
    d->colorVec4 = ImVec4(c.redF(), c.greenF(), c.blueF(), c.alphaF());
    Q_EMIT colorChanged(c);
}

QColor QImPlot3DLineItemNode::color() const
{
    const ImVec4& v = d_ptr->colorVec4;
    // Check if auto color (alpha == -1)
    if (v.w < 0) {
        return QColor();
    }
    return QColor::fromRgbF(v.x, v.y, v.z, v.w);
}

void QImPlot3DLineItemNode::setLineWeight(float weight)
{
    QIM_D(d);
    if (d->lineWeightVal != weight) {
        d->lineWeightVal = weight;
        Q_EMIT lineWeightChanged(weight);
    }
}

float QImPlot3DLineItemNode::lineWeight() const
{
    return d_ptr->lineWeightVal;
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
 * @brief Renders the 3D line plot with minimal conversion overhead
 * @return false (no endDraw needed)
 * @details Calls SetNextLineStyle with pre-converted color/weight,
 *          then PlotLine with XYZ data and flags.
 *          All data conversion happens in setters, not here.
 * \endif
 *
 * \if CHINESE
 * @brief 渲染 3D 线图（最小转换开销）
 * @return false（无需 endDraw）
 * @details 使用预转换的颜色/线宽调用 SetNextLineStyle，
 *          然后使用 XYZ 数据和标志调用 PlotLine。
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

    // Set line style (pre-converted, no conversion in beginDraw)
    // IMPLOT3D_AUTO_COL has alpha=-1, IMPLOT3D_AUTO=-1 for weight
    if (d->colorVec4.w >= 0 || d->lineWeightVal >= 0) {
        ImPlot3D::SetNextLineStyle(d->colorVec4, d->lineWeightVal);
    }

    // Plot the 3D line
    ImPlot3D::PlotLine(labelConstData(),
                       d->dataSeries->xRawData(),
                       d->dataSeries->yRawData(),
                       d->dataSeries->zRawData(),
                       d->dataSeries->size(),
                       d->flags);

    return false;
}

}  // end namespace QIM