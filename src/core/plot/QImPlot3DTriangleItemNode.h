#ifndef QIMPLOT3DTRIANGLEITEMNODE_H
#define QIMPLOT3DTRIANGLEITEMNODE_H

#include "QImPlot3DItemNode.h"
#include "implot3d.h"
#include <QColor>
#include <vector>

namespace QIM
{
class QIM_CORE_API QImPlot3DTriangleItemNode : public QImPlot3DItemNode
{
    Q_OBJECT

    Q_PROPERTY(bool linesVisible READ isLinesVisible WRITE setLinesVisible NOTIFY triangleFlagChanged)
    Q_PROPERTY(bool fillVisible READ isFillVisible WRITE setFillVisible NOTIFY triangleFlagChanged)
    Q_PROPERTY(bool markersVisible READ isMarkersVisible WRITE setMarkersVisible NOTIFY triangleFlagChanged)
    Q_PROPERTY(int markerShape READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged)
    Q_PROPERTY(float markerSize READ markerSize WRITE setMarkerSize NOTIFY markerStyleChanged)
    Q_PROPERTY(float markerWeight READ markerWeight WRITE setMarkerWeight NOTIFY markerStyleChanged)
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged)
    Q_PROPERTY(QColor markerFillColor READ markerFillColor WRITE setMarkerFillColor NOTIFY markerFillColorChanged)
    Q_PROPERTY(QColor markerOutlineColor READ markerOutlineColor WRITE setMarkerOutlineColor NOTIFY markerOutlineColorChanged)
    Q_PROPERTY(float lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)

public:
    enum
    {
        Type = InnerType + 5
    };

    explicit QImPlot3DTriangleItemNode(QObject* parent = nullptr);
    ~QImPlot3DTriangleItemNode() override;

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

    bool isLinesVisible() const;
    void setLinesVisible(bool visible);

    bool isFillVisible() const;
    void setFillVisible(bool visible);

    bool isMarkersVisible() const;
    void setMarkersVisible(bool visible);

    int markerShape() const;
    void setMarkerShape(int shape);

    float markerSize() const;
    void setMarkerSize(float size);

    float markerWeight() const;
    void setMarkerWeight(float weight);

    QColor fillColor() const;
    void setFillColor(const QColor& color);

    QColor lineColor() const;
    void setLineColor(const QColor& color);

    QColor markerFillColor() const;
    void setMarkerFillColor(const QColor& color);

    QColor markerOutlineColor() const;
    void setMarkerOutlineColor(const QColor& color);

    float lineWidth() const;
    void setLineWidth(float width);

    int triangleFlags() const;
    void setTriangleFlags(int flags);

Q_SIGNALS:
    void dataChanged();
    void triangleFlagChanged();
    void markerShapeChanged(int shape);
    void markerStyleChanged();
    void fillColorChanged(const QColor& color);
    void lineColorChanged(const QColor& color);
    void markerFillColorChanged(const QColor& color);
    void markerOutlineColorChanged(const QColor& color);
    void lineWidthChanged(float width);

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
    int m_triangleFlags { 0 };
    int m_markerShape { ImPlot3DMarker_None };
    float m_markerSize { 4.0f };
    float m_markerWeight { 1.0f };
    QColor m_fillColor;
    QColor m_lineColor;
    QColor m_markerFillColor;
    QColor m_markerOutlineColor;
    float m_lineWidth { 1.0f };
};
}  // namespace QIM

#endif  // QIMPLOT3DTRIANGLEITEMNODE_H
