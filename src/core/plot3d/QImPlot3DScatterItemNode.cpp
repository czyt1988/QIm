#include "QImPlot3DScatterItemNode.h"
#include "implot3d.h"
#include "implot3d_internal.h"
#include <QColor>
#include <memory>

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Private implementation data for QImPlot3DScatterItemNode
 * @details Stores all internal data following PIMPL pattern.
 *          All conversions are done in setters for minimal beginDraw overhead.
 * \endif
 *
 * \if CHINESE
 * @brief QImPlot3DScatterItemNode的私有实现数据
 * @details 按照PIMPL模式存储所有内部数据。
 *          所有转换都在setter中完成，以最小化beginDraw开销。
 * \endif
 */
class QImPlot3DScatterItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlot3DScatterItemNode)

public:
    /**
     * \if ENGLISH
     * @brief Constructor
     * @param p Pointer to the public class
     * \endif
     *
     * \if CHINESE
     * @brief 构造函数
     * @param p 公共类指针
     * \endif
     */
    PrivateData(QImPlot3DScatterItemNode* p) : q_ptr(p)
    {
    }

    std::unique_ptr<QImAbstractXYZDataSeries> dataSeries;      ///< XYZ data series (managed)
    QImOptional3DColor markerFillColor;                        ///< Marker fill color (nullopt = use ImPlot3D default)
    QImOptional3DColor markerOutlineColor;                     ///< Marker outline color (nullopt = use ImPlot3D default)
    QImTrackedValue<float> markerSize {4.0f};                  ///< Marker size in pixels (actual default, not sentinel)
    QImTrackedValue<float> markerWeight {1.0f};                ///< Marker outline weight in pixels
    int markerShape {ImPlot3DMarker_None};                     ///< Marker shape (ImPlot3DMarker enum, None=0 default)
    ImPlot3DScatterFlags flags { ImPlot3DScatterFlags_None };  ///< Scatter flags (MUST be named 'flags' for macros)
};

//----------------------------------------------------
// QImPlot3DScatterItemNode
//----------------------------------------------------

QImPlot3DScatterItemNode::QImPlot3DScatterItemNode(QObject* parent)
    : QImPlot3DItemNode(parent)
    , QIM_PIMPL_CONSTRUCT
{
}

QImPlot3DScatterItemNode::~QImPlot3DScatterItemNode()
{
}

void QImPlot3DScatterItemNode::setData(QImAbstractXYZDataSeries* series)
{
    QIM_D(d);
    d->dataSeries.reset(series);
    Q_EMIT dataChanged();
}

QImAbstractXYZDataSeries* QImPlot3DScatterItemNode::data() const
{
    QIM_DC(d);
    return d->dataSeries.get();
}

/**
 * \if ENGLISH
 * @brief Gets the marker fill color
 * @return Current fill color as QColor, or invalid QColor() if using ImPlot3D default
 * @details Returns the explicit color if set, otherwise returns default QColor().
 *          On first render without explicit color, ImPlot3D's default color is captured.
 * \endif
 *
 * \if CHINESE
 * @brief 获取标记填充颜色
 * @return 当前填充颜色（QColor），使用 ImPlot3D 默认颜色时返回无效 QColor()
 * @details 如果设置了显式颜色则返回该颜色，否则返回默认 QColor()。
 *          首次渲染时未设置显式颜色，会捕获 ImPlot3D 的默认颜色。
 * \endif
 */
QColor QImPlot3DScatterItemNode::markerFillColor() const
{
    QIM_DC(d);
    return (d->markerFillColor.has_value()) ? toQColor(d->markerFillColor->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Sets the marker fill color
 * @param[in] c New fill color
 * @details Stores color in optional QImTrackedValue. Automatically sets dirty flag on change.
 *          In immediate mode rendering, every frame calls SetNextMarkerStyle if color is set.
 * \endif
 *
 * \if CHINESE
 * @brief 设置标记填充颜色
 * @param[in] c 新的填充颜色
 * @details 将颜色存储在 optional QImTrackedValue 中。值变化时自动设置 dirty 标记。
 *          在即时模式渲染中，如果设置了颜色，每帧都需要调用 SetNextMarkerStyle。
 * \endif
 */
void QImPlot3DScatterItemNode::setMarkerFillColor(const QColor& c)
{
    QIM_D(d);
    d->markerFillColor = toImVec4(c);
    Q_EMIT markerFillColorChanged(c);
}

/**
 * \if ENGLISH
 * @brief Gets the marker outline color
 * @return Current outline color as QColor, or invalid QColor() if using ImPlot3D default
 * @details Returns the explicit color if set, otherwise returns default QColor().
 *          On first render without explicit color, ImPlot3D's default color is captured.
 * \endif
 *
 * \if CHINESE
 * @brief 获取标记描边颜色
 * @return 当前描边颜色（QColor），使用 ImPlot3D 默认颜色时返回无效 QColor()
 * @details 如果设置了显式颜色则返回该颜色，否则返回默认 QColor()。
 *          首次渲染时未设置显式颜色，会捕获 ImPlot3D 的默认颜色。
 * \endif
 */
QColor QImPlot3DScatterItemNode::markerOutlineColor() const
{
    QIM_DC(d);
    return (d->markerOutlineColor.has_value()) ? toQColor(d->markerOutlineColor->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Sets the marker outline color
 * @param[in] c New outline color
 * @details Stores color in optional QImTrackedValue. Automatically sets dirty flag on change.
 *          In immediate mode rendering, every frame calls SetNextMarkerStyle if color is set.
 * \endif
 *
 * \if CHINESE
 * @brief 设置标记描边颜色
 * @param[in] c 新的描边颜色
 * @details 将颜色存储在 optional QImTrackedValue 中。值变化时自动设置 dirty 标记。
 *          在即时模式渲染中，如果设置了颜色，每帧都需要调用 SetNextMarkerStyle。
 * \endif
 */
void QImPlot3DScatterItemNode::setMarkerOutlineColor(const QColor& c)
{
    QIM_D(d);
    d->markerOutlineColor = toImVec4(c);
    Q_EMIT markerOutlineColorChanged(c);
}

float QImPlot3DScatterItemNode::markerSize() const
{
    QIM_DC(d);
    return d->markerSize.value();
}

void QImPlot3DScatterItemNode::setMarkerSize(float size)
{
    QIM_D(d);
    d->markerSize = size;
    Q_EMIT markerSizeChanged(size);
}

float QImPlot3DScatterItemNode::markerWeight() const
{
    QIM_DC(d);
    return d->markerWeight.value();
}

void QImPlot3DScatterItemNode::setMarkerWeight(float weight)
{
    QIM_D(d);
    d->markerWeight = weight;
    Q_EMIT markerWeightChanged(weight);
}

/**
 * \if ENGLISH
 * @brief Gets the marker shape
 * @return Marker shape as QImPlot3DMarkerShape enum value (int)
 * \endif
 *
 * \if CHINESE
 * @brief 获取标记形状
 * @return 标记形状（QImPlot3DMarkerShape 枚举值，int 类型）
 * \endif
 */
int QImPlot3DScatterItemNode::markerShape() const
{
    QIM_DC(d);
    // Convert ImPlot3DMarker back to QImPlot3DMarkerShape int value
    return static_cast<int>(toQImPlot3DMarkerShape(static_cast<ImPlot3DMarker>(d->markerShape)));
}

/**
 * \if ENGLISH
 * @brief Sets the marker shape
 * @param[in] shape New marker shape (QImPlot3DMarkerShape enum value)
 * \endif
 *
 * \if CHINESE
 * @brief 设置标记形状
 * @param[in] shape 新标记形状（QImPlot3DMarkerShape 枚举值）
 * \endif
 */
void QImPlot3DScatterItemNode::setMarkerShape(int shape)
{
    QIM_D(d);
    // Convert QImPlot3DMarkerShape (int) to ImPlot3DMarker in setter
    ImPlot3DMarker newMarker = static_cast<ImPlot3DMarker>(toImPlot3DMarker(static_cast<QImPlot3DMarkerShape>(shape)));
    if (d->markerShape != static_cast<int>(newMarker))
    {
        d->markerShape = static_cast<int>(newMarker);
        Q_EMIT markerShapeChanged(shape);
    }
}

int QImPlot3DScatterItemNode::scatterFlags() const
{
    QIM_DC(d);
    return static_cast<int>(d->flags);
}

void QImPlot3DScatterItemNode::setScatterFlags(int flags)
{
    QIM_D(d);
    const auto oldFlags = d->flags;
    d->flags = static_cast<ImPlot3DScatterFlags>(flags);
    if (d->flags != oldFlags)
    {
        Q_EMIT scatterFlagChanged();
    }
}

bool QImPlot3DScatterItemNode::beginDraw()
{
    QIM_D(d);

    // Check if data series is valid
    if (!d->dataSeries || !d->dataSeries->isValid())
    {
        return false;
    }

    // Always call SetNextMarkerStyle to prevent style state bleeding in immediate mode
    // Use QImOptional3DColor: if set, use captured value; if unset, pass IMPLOT3D_AUTO_COL
    const ImVec4 fill = d->markerFillColor.has_value() ? d->markerFillColor->value() : IMPLOT3D_AUTO_COL;
    const ImVec4 outline = d->markerOutlineColor.has_value() ? d->markerOutlineColor->value() : IMPLOT3D_AUTO_COL;
    ImPlot3D::SetNextMarkerStyle(static_cast<ImPlot3DMarker>(d->markerShape), d->markerSize.value(), fill, d->markerWeight.value(), outline);

    // Plot scatter with XYZ data
    ImPlot3D::PlotScatter(labelConstData(),
                          d->dataSeries->xRawData(),
                          d->dataSeries->yRawData(),
                          d->dataSeries->zRawData(),
                          d->dataSeries->size(),
                          d->flags);

    // Capture ImPlot3D default colors on first render (lazy initialization)
    if (!d->markerFillColor)
    {
        d->markerFillColor = captureItemColor();
    }
    if (!d->markerOutlineColor)
    {
        d->markerOutlineColor = captureItemColor();
    }

    return false;
}

}  // end namespace QIM