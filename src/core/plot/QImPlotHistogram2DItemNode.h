#ifndef QIMPLOTHISTOGRAM2DITEMNODE_H
#define QIMPLOTHISTOGRAM2DITEMNODE_H

#include "../../QImAPI.h"
#include <QColor>
#include <QPointF>
#include "QImAbstractXYSeriesItemNode.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot 2D histogram visualization
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot 2D histograms (bivariate).
 *          Supports customizable binning in X and Y dimensions, range filtering,
 *          density normalization, outlier exclusion, and column-major data layout.
 *          Inherits from QImAbstractXYSeriesItemNode for data management.
 *
 * @note 2D histograms visualize joint distribution of two variables as a heatmap of binned counts.
 *       Useful for correlation analysis, density estimation, and 2D data exploration.
 * @note Large bin counts (>100x100) may impact performance.
 *
 * @see QImAbstractXYSeriesItemNode
 * @see QImPlotHeatmapItemNode
 * @see QImPlotHistogramItemNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot二维直方图可视化的Qt风格封装。
 *
 * @details 为ImPlot二维直方图（双变量）提供Qt风格的保留模式封装。
 *          支持可自定义的X和Y维度装箱、范围过滤。
 *          密度归一化、异常值排除和列主序数据布局。
 *          继承自QImAbstractXYSeriesItemNode以获得数据管理。
 *
 * @note 二维直方图将两个变量的联合分布可视化为装箱计数的热力图。
 *       适用于相关性分析、密度估计和二维数据探索。
 * @note 大量箱数。100x100）可能影响性能。
 *
 * @see QImAbstractXYSeriesItemNode
 * @see QImPlotHeatmapItemNode
 * @see QImPlotHistogramItemNode
 * \endif
 */
class QIM_CORE_API QImPlotHistogram2DItemNode : public QImAbstractXYSeriesItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotHistogram2DItemNode)

    Q_PROPERTY(int xBins READ xBins WRITE setXBins NOTIFY xBinsChanged)
    Q_PROPERTY(int yBins READ yBins WRITE setYBins NOTIFY yBinsChanged)
    Q_PROPERTY(double xRangeMin READ xRangeMin WRITE setXRangeMin NOTIFY xRangeChanged)
    Q_PROPERTY(double xRangeMax READ xRangeMax WRITE setXRangeMax NOTIFY xRangeChanged)
    Q_PROPERTY(double yRangeMin READ yRangeMin WRITE setYRangeMin NOTIFY yRangeChanged)
    Q_PROPERTY(double yRangeMax READ yRangeMax WRITE setYRangeMax NOTIFY yRangeChanged)
    Q_PROPERTY(bool density READ isDensity WRITE setDensity NOTIFY densityChanged)
    Q_PROPERTY(bool noOutliers READ isNoOutliers WRITE setNoOutliers NOTIFY noOutliersChanged)
    Q_PROPERTY(bool colMajor READ isColMajor WRITE setColMajor NOTIFY colMajorChanged)

public:
    // Unique type identifier for QImPlotHistogram2DItemNode
    enum
    {
        Type = InnerType + 12
    };

    // Returns the type identifier of this plot item
    virtual int type() const override
    {
        return Type;
    }

    // Constructs a QImPlotHistogram2DItemNode with optional parent
    QImPlotHistogram2DItemNode(QObject* parent = nullptr);

    // Destroys the QImPlotHistogram2DItemNode
    ~QImPlotHistogram2DItemNode();

    //----------------------------------------------------
    // Style property accessors
    //----------------------------------------------------

    // Gets the X bin count or automatic method
    int xBins() const;
    void setXBins(int bins);
    int yBins() const;
    void setYBins(int bins);
    double xRangeMin() const;
    void setXRangeMin(double min);
    double xRangeMax() const;
    void setXRangeMax(double max);
    double yRangeMin() const;
    void setYRangeMin(double min);
    double yRangeMax() const;
    void setYRangeMax(double max);
    bool isDensity() const;
    void setDensity(bool density);
    bool isNoOutliers() const;
    void setNoOutliers(bool noOutliers);
    bool isColMajor() const;
    void setColMajor(bool colMajor);
    int histogramFlags() const;
    void setHistogramFlags(int flags);

Q_SIGNALS:
    void xBinsChanged(int bins);
    void yBinsChanged(int bins);
    void xRangeChanged();
    void yRangeChanged();
    void densityChanged(bool density);
    void noOutliersChanged(bool noOutliers);
    void colMajorChanged(bool colMajor);
    void histogramFlagChanged();

protected:
    // Begins drawing the 2D histogram
    virtual bool beginDraw() override;
};

}  // end namespace QIM

#endif  // QIMPLOTHISTOGRAM2DITEMNODE_H