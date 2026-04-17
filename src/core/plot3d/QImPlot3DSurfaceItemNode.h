#ifndef QIMPLOT3DSURFACEITEMNODE_H
#define QIMPLOT3DSURFACEITEMNODE_H

#include "QImPlot3DItemNode.h"
#include "implot3d.h"
#include <QColor>
#include <vector>

namespace QIM
{
class QIM_CORE_API QImPlot3DSurfaceItemNode : public QImPlot3DItemNode
{
    Q_OBJECT

    Q_PROPERTY(int xCount READ xCount WRITE setXCount NOTIFY gridShapeChanged)
    Q_PROPERTY(int yCount READ yCount WRITE setYCount NOTIFY gridShapeChanged)
    Q_PROPERTY(bool linesVisible READ isLinesVisible WRITE setLinesVisible NOTIFY surfaceFlagChanged)
    Q_PROPERTY(bool fillVisible READ isFillVisible WRITE setFillVisible NOTIFY surfaceFlagChanged)
    Q_PROPERTY(bool markersVisible READ isMarkersVisible WRITE setMarkersVisible NOTIFY surfaceFlagChanged)
    Q_PROPERTY(int markerShape READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged)
    Q_PROPERTY(float markerSize READ markerSize WRITE setMarkerSize NOTIFY markerStyleChanged)
    Q_PROPERTY(float markerWeight READ markerWeight WRITE setMarkerWeight NOTIFY markerStyleChanged)
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged)
    Q_PROPERTY(QColor markerFillColor READ markerFillColor WRITE setMarkerFillColor NOTIFY markerFillColorChanged)
    Q_PROPERTY(QColor markerOutlineColor READ markerOutlineColor WRITE setMarkerOutlineColor NOTIFY markerOutlineColorChanged)
    Q_PROPERTY(float lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    Q_PROPERTY(bool colormapEnabled READ isColormapEnabled WRITE setColormapEnabled NOTIFY colormapChanged)
    Q_PROPERTY(int colormap READ colormap WRITE setColormap NOTIFY colormapChanged)

public:
    enum
    {
        Type = InnerType3D + 3
    };

    explicit QImPlot3DSurfaceItemNode(QObject* parent = nullptr);
    ~QImPlot3DSurfaceItemNode() override;

    int type() const override
    {
        return Type;
    }

    template< typename ContainerX, typename ContainerY, typename ContainerZ >
    void setData(const ContainerX& x, const ContainerY& y, const ContainerZ& z, int xCount, int yCount)
    {
        m_xData = toVector(x);
        m_yData = toVector(y);
        m_zData = toVector(z);
        m_xCount = xCount;
        m_yCount = yCount;
        trimDataToGrid();
        Q_EMIT dataChanged();
        Q_EMIT gridShapeChanged();
    }

    const std::vector< double >& xData() const;
    const std::vector< double >& yData() const;
    const std::vector< double >& zData() const;

    int xCount() const;
    void setXCount(int count);

    int yCount() const;
    void setYCount(int count);

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

    bool isColormapEnabled() const;
    void setColormapEnabled(bool enabled);

    int colormap() const;
    void setColormap(int colormap);

    int surfaceFlags() const;
    void setSurfaceFlags(int flags);

Q_SIGNALS:
    void dataChanged();
    void gridShapeChanged();
    void surfaceFlagChanged();
    void markerShapeChanged(int shape);
    void markerStyleChanged();
    void fillColorChanged(const QColor& color);
    void lineColorChanged(const QColor& color);
    void markerFillColorChanged(const QColor& color);
    void markerOutlineColorChanged(const QColor& color);
    void lineWidthChanged(float width);
    void colormapChanged();

protected:
    bool beginDraw() override;

private:
    template< typename Container >
    static std::vector< double > toVector(const Container& container)
    {
        return std::vector< double >(container.begin(), container.end());
    }

    void trimDataToGrid();

private:
    std::vector< double > m_xData;
    std::vector< double > m_yData;
    std::vector< double > m_zData;
    int m_xCount { 0 };
    int m_yCount { 0 };
    int m_surfaceFlags { 0 };
    int m_markerShape { ImPlot3DMarker_None };
    float m_markerSize { 4.0f };
    float m_markerWeight { 1.0f };
    QColor m_fillColor;
    QColor m_lineColor;
    QColor m_markerFillColor;
    QColor m_markerOutlineColor;
    float m_lineWidth { 1.0f };
    bool m_colormapEnabled { false };
    int m_colormap { ImPlot3DColormap_Viridis };
};
}  // namespace QIM

#endif  // QIMPLOT3DSURFACEITEMNODE_H
