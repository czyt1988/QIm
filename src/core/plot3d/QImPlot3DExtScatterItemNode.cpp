#include "QImPlot3DExtScatterItemNode.h"
#include "implot3d.h"
#include <QColor>
#include <memory>

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Private implementation data for QImPlot3DExtScatterItemNode
 * @details Stores all internal data following PIMPL pattern.
 *          All conversions are done in setters for minimal beginDraw overhead.
 * \endif
 *
 * \if CHINESE
 * @brief QImPlot3DExtScatterItemNode的私有实现数据
 * @details 按照PIMPL模式存储所有内部数据。
 *          所有转换都在setter中完成，以最小化beginDraw开销。
 * \endif
 */
class QImPlot3DExtScatterItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlot3DExtScatterItemNode)

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
    PrivateData(QImPlot3DExtScatterItemNode* p) : q_ptr(p)
    {
    }

    std::unique_ptr<QImAbstractXYZDataSeries> dataSeries;      ///< XYZ data series (managed)
    ImVec4 colorVec4 { IMPLOT3D_AUTO_COL };                    ///< Pre-converted color (from QColor in setColor)
    float markerSizeVal { IMPLOT3D_AUTO };                     ///< Marker size (IMPLOT3D_AUTO = -1 for auto)
    ImPlot3DMarker markerVal { IMPLOT3D_AUTO };                ///< Marker shape (ImPlot3DMarker enum)
    ImPlot3DScatterFlags flags { ImPlot3DScatterFlags_None };  ///< Scatter flags (MUST be named 'flags' for macros)
};

//----------------------------------------------------
// QImPlot3DExtScatterItemNode
//----------------------------------------------------

QImPlot3DExtScatterItemNode::QImPlot3DExtScatterItemNode(QObject* parent)
    : QImPlot3DExtItemNode(parent)
    , QIM_PIMPL_CONSTRUCT
{
}

QImPlot3DExtScatterItemNode::~QImPlot3DExtScatterItemNode()
{
}

void QImPlot3DExtScatterItemNode::setData(QImAbstractXYZDataSeries* series)
{
    QIM_D(d);
    d->dataSeries.reset(series);
    Q_EMIT dataChanged();
}

QImAbstractXYZDataSeries* QImPlot3DExtScatterItemNode::data() const
{
    QIM_DC(d);
    return d->dataSeries.get();
}

QColor QImPlot3DExtScatterItemNode::color() const
{
    QIM_DC(d);
    // Convert ImVec4 back to QColor
    return QColor::fromRgbF(d->colorVec4.x, d->colorVec4.y, d->colorVec4.z, d->colorVec4.w);
}

void QImPlot3DExtScatterItemNode::setColor(const QColor& c)
{
    QIM_D(d);
    // Convert QColor to ImVec4 in setter (beginDraw minimization)
    d->colorVec4 = ImVec4(c.redF(), c.greenF(), c.blueF(), c.alphaF());
    Q_EMIT colorChanged(c);
}

float QImPlot3DExtScatterItemNode::markerSize() const
{
    QIM_DC(d);
    return d->markerSizeVal;
}

void QImPlot3DExtScatterItemNode::setMarkerSize(float size)
{
    QIM_D(d);
    if (d->markerSizeVal != size)
    {
        d->markerSizeVal = size;
        Q_EMIT markerSizeChanged(size);
    }
}

int QImPlot3DExtScatterItemNode::markerShape() const
{
    QIM_DC(d);
    // Convert ImPlot3DMarker back to QImPlot3DMarkerShape int value
    return static_cast<int>(toQImPlot3DMarkerShape(d->markerVal));
}

void QImPlot3DExtScatterItemNode::setMarkerShape(int shape)
{
    QIM_D(d);
    // Convert QImPlot3DMarkerShape (int) to ImPlot3DMarker in setter
    ImPlot3DMarker newMarker = static_cast<ImPlot3DMarker>(toImPlot3DMarker(static_cast<QImPlot3DMarkerShape>(shape)));
    if (d->markerVal != newMarker)
    {
        d->markerVal = newMarker;
        Q_EMIT markerShapeChanged(shape);
    }
}

int QImPlot3DExtScatterItemNode::scatterFlags() const
{
    QIM_DC(d);
    return static_cast<int>(d->flags);
}

void QImPlot3DExtScatterItemNode::setScatterFlags(int flags)
{
    QIM_D(d);
    const auto oldFlags = d->flags;
    d->flags = static_cast<ImPlot3DScatterFlags>(flags);
    if (d->flags != oldFlags)
    {
        Q_EMIT scatterFlagChanged();
    }
}

bool QImPlot3DExtScatterItemNode::beginDraw()
{
    QIM_D(d);

    // Check if data series is valid
    if (!d->dataSeries || !d->dataSeries->isValid())
    {
        return false;
    }

    // Set marker style (all pre-converted, no conversion in beginDraw)
    // Check if any custom style is set (marker, size, or color)
    // IMPLOT3D_AUTO = -1, IMPLOT3D_AUTO_COL.w = -1 for auto color
    if (d->markerVal != IMPLOT3D_AUTO || d->markerSizeVal != IMPLOT3D_AUTO ||
        d->colorVec4.w != -1)
    {
        // SetNextMarkerStyle(marker, size, fill, weight, outline)
        // Use the same color for fill and outline
        ImPlot3D::SetNextMarkerStyle(d->markerVal, d->markerSizeVal, d->colorVec4, IMPLOT3D_AUTO, d->colorVec4);
    }

    // Plot scatter with XYZ data
    ImPlot3D::PlotScatter(labelConstData(),
                          d->dataSeries->xRawData(),
                          d->dataSeries->yRawData(),
                          d->dataSeries->zRawData(),
                          d->dataSeries->size(),
                          d->flags);

    return true;
}

}  // end namespace QIM


