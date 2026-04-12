#ifndef QIMPLOTHISTOGRAMITEMNODE_H
#define QIMPLOTHISTOGRAMITEMNODE_H

#include "QImPlotItemNode.h"
#include "implot.h"
#include <QColor>
#include <vector>

namespace QIM
{
class QIM_CORE_API QImPlotHistogramItemNode : public QImPlotItemNode
{
    Q_OBJECT

    Q_PROPERTY(int binCount READ binCount WRITE setBinCount NOTIFY histogramConfigChanged)
    Q_PROPERTY(double barScale READ barScale WRITE setBarScale NOTIFY histogramConfigChanged)
    Q_PROPERTY(bool horizontal READ isHorizontal WRITE setHorizontal NOTIFY histogramConfigChanged)
    Q_PROPERTY(bool cumulative READ isCumulative WRITE setCumulative NOTIFY histogramConfigChanged)
    Q_PROPERTY(bool density READ isDensity WRITE setDensity NOTIFY histogramConfigChanged)
    Q_PROPERTY(bool noOutliers READ isNoOutliers WRITE setNoOutliers NOTIFY histogramConfigChanged)
    Q_PROPERTY(bool rangeEnabled READ isRangeEnabled WRITE setRangeEnabled NOTIFY histogramConfigChanged)
    Q_PROPERTY(double rangeMin READ rangeMin WRITE setRangeMin NOTIFY histogramConfigChanged)
    Q_PROPERTY(double rangeMax READ rangeMax WRITE setRangeMax NOTIFY histogramConfigChanged)
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)

public:
    enum
    {
        Type = InnerType + 9
    };

    explicit QImPlotHistogramItemNode(QObject* parent = nullptr);
    ~QImPlotHistogramItemNode() override;

    int type() const override
    {
        return Type;
    }

    template< typename Container >
    void setData(const Container& values)
    {
        m_values.assign(values.begin(), values.end());
        Q_EMIT dataChanged();
    }

    const std::vector< double >& data() const;

    int binCount() const;
    void setBinCount(int bins);

    double barScale() const;
    void setBarScale(double scale);

    bool isHorizontal() const;
    void setHorizontal(bool on);

    bool isCumulative() const;
    void setCumulative(bool on);

    bool isDensity() const;
    void setDensity(bool on);

    bool isNoOutliers() const;
    void setNoOutliers(bool on);

    bool isRangeEnabled() const;
    void setRangeEnabled(bool on);

    double rangeMin() const;
    void setRangeMin(double value);

    double rangeMax() const;
    void setRangeMax(double value);

    QColor fillColor() const;
    void setFillColor(const QColor& color);

    int histogramFlags() const;
    void setHistogramFlags(int flags);

Q_SIGNALS:
    void dataChanged();
    void histogramConfigChanged();
    void fillColorChanged(const QColor& color);

protected:
    bool beginDraw() override;

private:
    std::vector< double > m_values;
    int m_binCount { ImPlotBin_Sturges };
    double m_barScale { 1.0 };
    int m_histogramFlags { ImPlotHistogramFlags_None };
    bool m_rangeEnabled { false };
    double m_rangeMin { 0.0 };
    double m_rangeMax { 0.0 };
    QColor m_fillColor;
};
}  // namespace QIM

#endif  // QIMPLOTHISTOGRAMITEMNODE_H
