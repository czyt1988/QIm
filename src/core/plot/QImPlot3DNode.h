#ifndef QIMPLOT3DNODE_H
#define QIMPLOT3DNODE_H

#include "QImAbstractNode.h"
#include <QSizeF>

struct ImVec2;

namespace QIM
{
class QImPlot3DItemNode;

class QIM_CORE_API QImPlot3DNode : public QImAbstractNode
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QSizeF size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(bool autoSize READ isAutoSize WRITE setAutoSize NOTIFY autoSizeChanged)
    Q_PROPERTY(QString xAxisLabel READ xAxisLabel WRITE setXAxisLabel NOTIFY axisLabelChanged)
    Q_PROPERTY(QString yAxisLabel READ yAxisLabel WRITE setYAxisLabel NOTIFY axisLabelChanged)
    Q_PROPERTY(QString zAxisLabel READ zAxisLabel WRITE setZAxisLabel NOTIFY axisLabelChanged)
    Q_PROPERTY(bool legendEnabled READ isLegendEnabled WRITE setLegendEnabled NOTIFY plotFlagChanged)
    Q_PROPERTY(bool mouseTextEnabled READ isMouseTextEnabled WRITE setMouseTextEnabled NOTIFY plotFlagChanged)
    Q_PROPERTY(bool inputsEnabled READ isInputsEnabled WRITE setInputsEnabled NOTIFY plotFlagChanged)
    Q_PROPERTY(bool menusEnabled READ isMenusEnabled WRITE setMenusEnabled NOTIFY plotFlagChanged)
    Q_PROPERTY(bool clippingEnabled READ isClippingEnabled WRITE setClippingEnabled NOTIFY plotFlagChanged)
    Q_PROPERTY(bool equal READ isEqual WRITE setEqual NOTIFY plotFlagChanged)

public:
    enum Axis
    {
        AxisX = 0,
        AxisY,
        AxisZ
    };
    Q_ENUM(Axis)

    explicit QImPlot3DNode(QObject* parent = nullptr);
    explicit QImPlot3DNode(const QString& title, QObject* parent = nullptr);
    ~QImPlot3DNode() override;

    QString title() const;
    void setTitle(const QString& title);

    QSizeF size() const;
    void setSize(const QSizeF& size);

    bool isAutoSize() const;
    void setAutoSize(bool autoSize);

    QString xAxisLabel() const;
    QString yAxisLabel() const;
    QString zAxisLabel() const;
    void setXAxisLabel(const QString& label);
    void setYAxisLabel(const QString& label);
    void setZAxisLabel(const QString& label);
    void setAxisLabel(Axis axis, const QString& label);

    bool isLegendEnabled() const;
    void setLegendEnabled(bool enabled);

    bool isMouseTextEnabled() const;
    void setMouseTextEnabled(bool enabled);

    bool isInputsEnabled() const;
    void setInputsEnabled(bool enabled);

    bool isMenusEnabled() const;
    void setMenusEnabled(bool enabled);

    bool isClippingEnabled() const;
    void setClippingEnabled(bool enabled);

    bool isEqual() const;
    void setEqual(bool enabled);

    int imPlot3DFlags() const;
    void setImPlot3DFlags(int flags);

    void setAxisLimits(Axis axis, double minValue, double maxValue, bool always = false);
    void clearAxisLimits(Axis axis);

    void addPlotItem(QImPlot3DItemNode* item);
    QList< QImPlot3DItemNode* > plotItemNodes() const;

Q_SIGNALS:
    void titleChanged(const QString& title);
    void sizeChanged(const QSizeF& size);
    void autoSizeChanged(bool autoSize);
    void axisLabelChanged();
    void plotFlagChanged();

protected:
    bool beginDraw() override;
    void endDraw() override;

private:
    struct AxisLimits
    {
        bool enabled { false };
        bool always { false };
        double minValue { 0.0 };
        double maxValue { 0.0 };
    };

    ImVec2 imSize() const;
    void applySetup() const;

private:
    QString m_title;
    QSizeF m_size;
    bool m_autoSize { true };
    QString m_axisLabels[ 3 ];
    int m_plotFlags { 0 };
    AxisLimits m_axisLimits[ 3 ];
    bool m_beginPlotSuccess { false };
};
}  // namespace QIM

#endif  // QIMPLOT3DNODE_H
