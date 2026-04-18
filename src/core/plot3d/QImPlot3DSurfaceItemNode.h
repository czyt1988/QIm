#ifndef QIMPLOT3DSURFACEITEMNODE_H
#define QIMPLOT3DSURFACEITEMNODE_H

#include "QImPlot3DItemNode.h"
#include <QColor>
#include <vector>

namespace QIM
{
class QIM_CORE_API QImPlot3DSurfaceItemNode : public QImPlot3DItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlot3DSurfaceItemNode)

    // Surface grid dimensions
    Q_PROPERTY(int xCount READ xCount WRITE setXCount NOTIFY gridShapeChanged)
    Q_PROPERTY(int yCount READ yCount WRITE setYCount NOTIFY gridShapeChanged)
    // Surface visibility flags
    Q_PROPERTY(bool linesVisible READ isLinesVisible WRITE setLinesVisible NOTIFY surfaceFlagChanged)
    Q_PROPERTY(bool fillVisible READ isFillVisible WRITE setFillVisible NOTIFY surfaceFlagChanged)
    Q_PROPERTY(bool markersVisible READ isMarkersVisible WRITE setMarkersVisible NOTIFY surfaceFlagChanged)
    // Marker style
    Q_PROPERTY(int markerShape READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged)
    Q_PROPERTY(float markerSize READ markerSize WRITE setMarkerSize NOTIFY markerStyleChanged)
    Q_PROPERTY(float markerWeight READ markerWeight WRITE setMarkerWeight NOTIFY markerStyleChanged)
    // Colors
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged)
    Q_PROPERTY(QColor markerFillColor READ markerFillColor WRITE setMarkerFillColor NOTIFY markerFillColorChanged)
    Q_PROPERTY(QColor markerOutlineColor READ markerOutlineColor WRITE setMarkerOutlineColor NOTIFY markerOutlineColorChanged)
    // Line width
    Q_PROPERTY(float lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    // Colormap
    Q_PROPERTY(bool colormapEnabled READ isColormapEnabled WRITE setColormapEnabled NOTIFY colormapChanged)
    Q_PROPERTY(int colormap READ colormap WRITE setColormap NOTIFY colormapChanged)

public:
    // Surface item type = InnerType3D + 3
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

    // setData template - inline to access header-level members (PrivateData is incomplete in header)
    template< typename ContainerX, typename ContainerY, typename ContainerZ >
    void setData(const ContainerX& x, const ContainerY& y, const ContainerZ& z, int xCount, int yCount)
    {
        xData_vec.assign(x.begin(), x.end());
        yData_vec.assign(y.begin(), y.end());
        zData_vec.assign(z.begin(), z.end());
        xCount_val = xCount;
        yCount_val = yCount;
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
    void trimDataToGrid();

    // Template setData needs to access these directly because PrivateData is incomplete in header.
    // The vectors are renamed without m_ prefix and kept in .h with public access to the class.
    std::vector< double > xData_vec;
    std::vector< double > yData_vec;
    std::vector< double > zData_vec;
    int xCount_val { 0 };
    int yCount_val { 0 };
};
}  // namespace QIM

#endif  // QIMPLOT3DSURFACEITEMNODE_H
