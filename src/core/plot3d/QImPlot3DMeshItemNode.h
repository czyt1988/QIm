#ifndef QIMPLOT3DMESHITEMNODE_H
#define QIMPLOT3DMESHITEMNODE_H

#include "QImPlot3DItemNode.h"
#include "implot3d.h"
#include <QColor>
#include <vector>

namespace QIM
{
class QIM_CORE_API QImPlot3DMeshItemNode : public QImPlot3DItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlot3DMeshItemNode)

    // Mesh visibility flags
    Q_PROPERTY(bool linesVisible READ isLinesVisible WRITE setLinesVisible NOTIFY meshFlagChanged)
    Q_PROPERTY(bool fillVisible READ isFillVisible WRITE setFillVisible NOTIFY meshFlagChanged)
    Q_PROPERTY(bool markersVisible READ isMarkersVisible WRITE setMarkersVisible NOTIFY meshFlagChanged)
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

public:
    // Mesh item type = InnerType3D + 4
    enum
    {
        Type = InnerType3D + 4
    };

    explicit QImPlot3DMeshItemNode(QObject* parent = nullptr);
    ~QImPlot3DMeshItemNode() override;

    int type() const override
    {
        return Type;
    }

    void setMeshData(const std::vector< ImPlot3DPoint >& vertices, const std::vector< unsigned int >& indices);

    const std::vector< ImPlot3DPoint >& vertices() const;
    const std::vector< unsigned int >& indices() const;

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

    int meshFlags() const;
    void setMeshFlags(int flags);

Q_SIGNALS:
    void dataChanged();
    void meshFlagChanged();
    void markerShapeChanged(int shape);
    void markerStyleChanged();
    void fillColorChanged(const QColor& color);
    void lineColorChanged(const QColor& color);
    void markerFillColorChanged(const QColor& color);
    void markerOutlineColorChanged(const QColor& color);
    void lineWidthChanged(float width);

protected:
    bool beginDraw() override;
};
}  // namespace QIM

#endif  // QIMPLOT3DMESHITEMNODE_H
