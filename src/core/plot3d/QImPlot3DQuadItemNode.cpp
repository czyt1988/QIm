#include "QImPlot3DQuadItemNode.h"
#include "QImAPI.h"
#include "QtImGuiUtils.h"
#include "implot3d.h"
#include "implot3d_internal.h"
#include "QImPlot3DDataSeries.h"

namespace QIM
{

// ===============================================================
// PrivateData Implementation
// ===============================================================

class QImPlot3DQuadItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlot3DQuadItemNode)

public:
    explicit PrivateData(QImPlot3DQuadItemNode* q) : q_ptr(q) {}

    int flags{0};
    int markerShape{ImPlot3DMarker_None};
    float markerSize{4.0f};
    float markerWeight{1.0f};
    QImOptional3DColor fillColor;
    QImOptional3DColor lineColor;
    QImOptional3DColor markerFillColor;
    QImOptional3DColor markerOutlineColor;
    float lineWidth{1.0f};
    QImAbstractXYZDataSeries* dataSeries = nullptr;
};

// ===============================================================
// QImPlot3DQuadItemNode Implementation
// ===============================================================

/**
 * \if ENGLISH
 * @brief Constructs a QImPlot3DQuadItemNode
 * @param parent Parent QObject
 * @details Initializes the quad item node with default properties:
 *          - linesVisible: true
 *          - fillVisible: true
 *          - markersVisible: false
 *          - markerShape: ImPlot3DMarker_None
 *          - markerSize: 4.0f
 *          - markerWeight: 1.0f
 *          - lineWidth: 1.0f
 * \endif
 *
 * \if CHINESE
 * @brief 构造 QImPlot3DQuadItemNode
 * @param parent 父 QObject
 * @details 初始化四边形项节点，默认属性：
 *          - linesVisible: true
 *          - fillVisible: true
 *          - markersVisible: false
 *          - markerShape: ImPlot3DMarker_None
 *          - markerSize: 4.0f
 *          - markerWeight: 1.0f
 *          - lineWidth: 1.0f
 * \endif
 */
QImPlot3DQuadItemNode::QImPlot3DQuadItemNode(QObject* parent)
    : QImPlot3DItemNode(parent)
    , QIM_PIMPL_CONSTRUCT
{
}

/**
 * \if ENGLISH
 * @brief Destroys the QImPlot3DQuadItemNode
 * @details Cleans up the data series if owned.
 * \endif
 *
 * \if CHINESE
 * @brief 析构 QImPlot3DQuadItemNode
 * @details 清理数据系列（如果拥有所有权）。
 * \endif
 */
QImPlot3DQuadItemNode::~QImPlot3DQuadItemNode()
{
    QIM_D(d);
    delete d->dataSeries;
}

/**
 * \if ENGLISH
 * @brief Sets the data series for this quad item
 * @param series The XYZ data series (takes ownership)
 * @details Deletes the previous data series and adopts the new one.
 * \endif
 *
 * \if CHINESE
 * @brief 设置此四边形项的数据系列
 * @param series XYZ 数据系列（取得所有权）
 * @details 删除之前的数据系列并采用新的数据系列。
 * \endif
 */
void QImPlot3DQuadItemNode::setData(QImAbstractXYZDataSeries* series)
{
    QIM_D(d);
    delete d->dataSeries;
    d->dataSeries = series;
}

/**
 * \if ENGLISH
 * @brief Returns the current data series
 * @return Pointer to the data series (does not transfer ownership)
 * \endif
 *
 * \if CHINESE
 * @brief 返回当前数据系列
 * @return 数据系列指针（不转移所有权）
 * \endif
 */
QImAbstractXYZDataSeries* QImPlot3DQuadItemNode::dataSeries() const
{
    QIM_DC(d);
    return d->dataSeries;
}

QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DQuadItemNode, LinesVisible, ImPlot3DQuadFlags_NoLines, quadFlagChanged)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DQuadItemNode, FillVisible, ImPlot3DQuadFlags_NoFill, quadFlagChanged)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DQuadItemNode, MarkersVisible, ImPlot3DQuadFlags_NoMarkers, quadFlagChanged)

/**
 * \if ENGLISH
 * @brief Returns the marker shape
 * @return ImPlot3DMarker enum value
 * \endif
 *
 * \if CHINESE
 * @brief 返回标记形状
 * @return ImPlot3DMarker 枚举值
 * \endif
 */
int QImPlot3DQuadItemNode::markerShape() const
{
    QIM_DC(d);
    return d->markerShape;
}

/**
 * \if ENGLISH
 * @brief Sets the marker shape
 * @param shape The marker shape as ImPlot3DMarker enum value
 * \endif
 *
 * \if CHINESE
 * @brief 设置标记形状
 * @param shape 标记形状（ImPlot3DMarker 枚举值）
 * \endif
 */
void QImPlot3DQuadItemNode::setMarkerShape(int shape)
{
    QIM_D(d);
    if (d->markerShape != shape) {
        d->markerShape = shape;
        Q_EMIT markerShapeChanged(shape);
    }
}

/**
 * \if ENGLISH
 * @brief Returns the marker size
 * @return Marker size in pixels
 * \endif
 *
 * \if CHINESE
 * @brief 返回标记大小
 * @return 标记大小（像素）
 * \endif
 */
float QImPlot3DQuadItemNode::markerSize() const
{
    QIM_DC(d);
    return d->markerSize;
}

/**
 * \if ENGLISH
 * @brief Sets the marker size
 * @param size Marker size in pixels
 * \endif
 *
 * \if CHINESE
 * @brief 设置标记大小
 * @param size 标记大小（像素）
 * \endif
 */
void QImPlot3DQuadItemNode::setMarkerSize(float size)
{
    QIM_D(d);
    if (!qFuzzyCompare(d->markerSize, size)) {
        d->markerSize = size;
        Q_EMIT markerStyleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the marker outline weight
 * @return Marker outline weight in pixels
 * \endif
 *
 * \if CHINESE
 * @brief 返回标记轮廓粗细
 * @return 标记轮廓粗细（像素）
 * \endif
 */
float QImPlot3DQuadItemNode::markerWeight() const
{
    QIM_DC(d);
    return d->markerWeight;
}

/**
 * \if ENGLISH
 * @brief Sets the marker outline weight
 * @param weight Marker outline weight in pixels
 * \endif
 *
 * \if CHINESE
 * @brief 设置标记轮廓粗细
 * @param weight 标记轮廓粗细（像素）
 * \endif
 */
void QImPlot3DQuadItemNode::setMarkerWeight(float weight)
{
    QIM_D(d);
    if (!qFuzzyCompare(d->markerWeight, weight)) {
        d->markerWeight = weight;
        Q_EMIT markerStyleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the fill color
 * @return Current fill color, or invalid QColor if not set
 * \endif
 *
 * \if CHINESE
 * @brief 返回填充颜色
 * @return 当前填充颜色，如果未设置则返回无效的 QColor
 * \endif
 */
QColor QImPlot3DQuadItemNode::fillColor() const
{
    QIM_DC(d);
    return (d->fillColor.has_value()) ? toQColor(d->fillColor->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Sets the fill color
 * @param color The fill color
 * \endif
 *
 * \if CHINESE
 * @brief 设置填充颜色
 * @param color 填充颜色
 * \endif
 */
void QImPlot3DQuadItemNode::setFillColor(const QColor& color)
{
    QIM_D(d);
    d->fillColor = toImVec4(color);
    Q_EMIT fillColorChanged(color);
}

/**
 * \if ENGLISH
 * @brief Returns the line color
 * @return Current line color, or invalid QColor if not set
 * \endif
 *
 * \if CHINESE
 * @brief 返回线条颜色
 * @return 当前线条颜色，如果未设置则返回无效的 QColor
 * \endif
 */
QColor QImPlot3DQuadItemNode::lineColor() const
{
    QIM_DC(d);
    return (d->lineColor.has_value()) ? toQColor(d->lineColor->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Sets the line color
 * @param color The line color
 * \endif
 *
 * \if CHINESE
 * @brief 设置线条颜色
 * @param color 线条颜色
 * \endif
 */
void QImPlot3DQuadItemNode::setLineColor(const QColor& color)
{
    QIM_D(d);
    d->lineColor = toImVec4(color);
    Q_EMIT lineColorChanged(color);
}

/**
 * \if ENGLISH
 * @brief Returns the marker fill color
 * @return Current marker fill color, or invalid QColor if not set
 * \endif
 *
 * \if CHINESE
 * @brief 返回标记填充颜色
 * @return 当前标记填充颜色，如果未设置则返回无效的 QColor
 * \endif
 */
QColor QImPlot3DQuadItemNode::markerFillColor() const
{
    QIM_DC(d);
    return (d->markerFillColor.has_value()) ? toQColor(d->markerFillColor->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Sets the marker fill color
 * @param color The marker fill color
 * \endif
 *
 * \if CHINESE
 * @brief 设置标记填充颜色
 * @param color 标记填充颜色
 * \endif
 */
void QImPlot3DQuadItemNode::setMarkerFillColor(const QColor& color)
{
    QIM_D(d);
    d->markerFillColor = toImVec4(color);
    Q_EMIT markerFillColorChanged(color);
}

/**
 * \if ENGLISH
 * @brief Returns the marker outline color
 * @return Current marker outline color, or invalid QColor if not set
 * \endif
 *
 * \if CHINESE
 * @brief 返回标记轮廓颜色
 * @return 当前标记轮廓颜色，如果未设置则返回无效的 QColor
 * \endif
 */
QColor QImPlot3DQuadItemNode::markerOutlineColor() const
{
    QIM_DC(d);
    return (d->markerOutlineColor.has_value()) ? toQColor(d->markerOutlineColor->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Sets the marker outline color
 * @param color The marker outline color
 * \endif
 *
 * \if CHINESE
 * @brief 设置标记轮廓颜色
 * @param color 标记轮廓颜色
 * \endif
 */
void QImPlot3DQuadItemNode::setMarkerOutlineColor(const QColor& color)
{
    QIM_D(d);
    d->markerOutlineColor = toImVec4(color);
    Q_EMIT markerOutlineColorChanged(color);
}

/**
 * \if ENGLISH
 * @brief Returns the line width
 * @return Line width in pixels
 * \endif
 *
 * \if CHINESE
 * @brief 返回线宽
 * @return 线宽（像素）
 * \endif
 */
float QImPlot3DQuadItemNode::lineWidth() const
{
    QIM_DC(d);
    return d->lineWidth;
}

/**
 * \if ENGLISH
 * @brief Sets the line width
 * @param width Line width in pixels
 * \endif
 *
 * \if CHINESE
 * @brief 设置线宽
 * @param width 线宽（像素）
 * \endif
 */
void QImPlot3DQuadItemNode::setLineWidth(float width)
{
    QIM_D(d);
    if (!qFuzzyCompare(d->lineWidth, width)) {
        d->lineWidth = width;
        Q_EMIT lineWidthChanged(width);
    }
}

/**
 * \if ENGLISH
 * @brief Returns the quad flags
 * @return Current ImPlot3DQuadFlags value
 * \endif
 *
 * \if CHINESE
 * @brief 返回四边形标志
 * @return 当前 ImPlot3DQuadFlags 值
 * \endif
 */
int QImPlot3DQuadItemNode::quadFlags() const
{
    QIM_DC(d);
    return d->flags;
}

/**
 * \if ENGLISH
 * @brief Sets the quad flags
 * @param flags The ImPlot3DQuadFlags value
 * \endif
 *
 * \if CHINESE
 * @brief 设置四边形标志
 * @param flags ImPlot3DQuadFlags 值
 * \endif
 */
void QImPlot3DQuadItemNode::setQuadFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = flags;
        Q_EMIT quadFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Renders the quad item using ImPlot3D
 * @return false (3D items don't need endDraw)
 * @details This method:
 *          1. Validates the data series
 *          2. Checks minimum point count (4 for quads)
 *          3. Sets up line, fill, and marker styles
 *          4. Calls ImPlot3D::PlotQuad
 *          5. Captures default colors for unset properties
 * \endif
 *
 * \if CHINESE
 * @brief 使用 ImPlot3D 渲染四边形项
 * @return false（3D 项不需要 endDraw）
 * @details 此方法：
 *          1. 验证数据系列
 *          2. 检查最小点数（四边形需要 4 个点）
 *          3. 设置线条、填充和标记样式
 *          4. 调用 ImPlot3D::PlotQuad
 *          5. 捕获未设置属性的默认颜色
 * \endif
 */
bool QImPlot3DQuadItemNode::beginDraw()
{
    QIM_D(d);
    if (!d->dataSeries || !d->dataSeries->isValid()) {
        return false;
    }
    const int count = d->dataSeries->size();
    if (count < 4) {
        return false;
    }

    if (d->fillColor.has_value()) {
        ImPlot3D::SetNextFillStyle(d->fillColor->value());
    }
    if (d->lineColor.has_value()) {
        ImPlot3D::SetNextLineStyle(d->lineColor->value(), d->lineWidth);
    } else {
        ImPlot3D::SetNextLineStyle(IMPLOT3D_AUTO_COL, d->lineWidth);
    }
    if (d->markerShape != ImPlot3DMarker_None) {
        const ImVec4 fill = d->markerFillColor.has_value() ? d->markerFillColor->value() : IMPLOT3D_AUTO_COL;
        const ImVec4 outline = d->markerOutlineColor.has_value() ? d->markerOutlineColor->value() : IMPLOT3D_AUTO_COL;
        ImPlot3D::SetNextMarkerStyle(static_cast<ImPlot3DMarker>(d->markerShape), d->markerSize, fill, d->markerWeight, outline);
    }

    ImPlot3D::PlotQuad(
        labelConstData(),
        d->dataSeries->xRawData(),
        d->dataSeries->yRawData(),
        d->dataSeries->zRawData(),
        count,
        static_cast<ImPlot3DQuadFlags>(d->flags)
    );

    // Capture defaults for unset colors
    if (!d->fillColor.has_value()) {
        d->fillColor = captureItemColor();
    }
    if (!d->lineColor.has_value()) {
        d->lineColor = captureItemColor();
    }
    if (!d->markerFillColor.has_value()) {
        d->markerFillColor = captureItemColor();
    }
    if (!d->markerOutlineColor.has_value()) {
        d->markerOutlineColor = captureItemColor();
    }

    return false;
}

}  // namespace QIM
