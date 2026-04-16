#ifndef QIMPLOT3DSCATTERITEMNODE_H
#define QIMPLOT3DSCATTERITEMNODE_H

#include "QImPlot3DItemNode.h"
#include "implot3d.h"
#include <QColor>
#include <vector>

namespace QIM
{
class QIM_CORE_API QImPlot3DScatterItemNode : public QImPlot3DItemNode
{
    Q_OBJECT

    Q_PROPERTY(int markerShape READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged)
    Q_PROPERTY(float markerSize READ markerSize WRITE setMarkerSize NOTIFY markerSizeChanged)
    Q_PROPERTY(float markerWeight READ markerWeight WRITE setMarkerWeight NOTIFY markerWeightChanged)
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)
    Q_PROPERTY(QColor outlineColor READ outlineColor WRITE setOutlineColor NOTIFY outlineColorChanged)

public:
    enum
    {
        Type = InnerType + 2
    };

    explicit QImPlot3DScatterItemNode(QObject* parent = nullptr);
    ~QImPlot3DScatterItemNode() override;

    int type() const override
    {
        return Type;
    }

    template< typename ContainerX, typename ContainerY, typename ContainerZ >
    void setData(const ContainerX& x, const ContainerY& y, const ContainerZ& z)
    {
        m_xData = toVector(x);
        m_yData = toVector(y);
        m_zData = toVector(z);
        trimDataToCommonSize();
        Q_EMIT dataChanged();
    }

    const std::vector< double >& xData() const;
    const std::vector< double >& yData() const;
    const std::vector< double >& zData() const;

    int markerShape() const;
    void setMarkerShape(int shape);

    float markerSize() const;
    void setMarkerSize(float size);

    float markerWeight() const;
    void setMarkerWeight(float weight);

    QColor fillColor() const;
    void setFillColor(const QColor& color);

    QColor outlineColor() const;
    void setOutlineColor(const QColor& color);

Q_SIGNALS:
    void dataChanged();
    void markerShapeChanged(int shape);
    void markerSizeChanged(float size);
    void markerWeightChanged(float weight);
    void fillColorChanged(const QColor& color);
    void outlineColorChanged(const QColor& color);

protected:
    bool beginDraw() override;

private:
    template< typename Container >
    static std::vector< double > toVector(const Container& container)
    {
        return std::vector< double >(container.begin(), container.end());
    }

    void trimDataToCommonSize();

private:
    std::vector< double > m_xData;
    std::vector< double > m_yData;
    std::vector< double > m_zData;
    int m_markerShape { ImPlot3DMarker_Circle };
    float m_markerSize { 4.0f };
    float m_markerWeight { 1.0f };
    QColor m_fillColor;
    QColor m_outlineColor;
};
}  // namespace QIM

#endif  // QIMPLOT3DSCATTERITEMNODE_H
