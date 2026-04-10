#ifndef QIMPLOT3DLINEITEMNODE_H
#define QIMPLOT3DLINEITEMNODE_H

#include "QImPlot3DItemNode.h"
#include <QColor>
#include <vector>

namespace QIM
{
class QIM_CORE_API QImPlot3DLineItemNode : public QImPlot3DItemNode
{
    Q_OBJECT

    Q_PROPERTY(bool segments READ isSegments WRITE setSegments NOTIFY lineFlagChanged)
    Q_PROPERTY(bool loop READ isLoop WRITE setLoop NOTIFY lineFlagChanged)
    Q_PROPERTY(bool skipNaN READ isSkipNaN WRITE setSkipNaN NOTIFY lineFlagChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(float lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)

public:
    enum
    {
        Type = InnerType + 1
    };

    explicit QImPlot3DLineItemNode(QObject* parent = nullptr);
    ~QImPlot3DLineItemNode() override;

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

    bool isSegments() const;
    void setSegments(bool enabled);

    bool isLoop() const;
    void setLoop(bool enabled);

    bool isSkipNaN() const;
    void setSkipNaN(bool enabled);

    int lineFlags() const;
    void setLineFlags(int flags);

    void setColor(const QColor& color);
    QColor color() const;

    void setLineWidth(float width);
    float lineWidth() const;

Q_SIGNALS:
    void dataChanged();
    void lineFlagChanged();
    void colorChanged(const QColor& color);
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
    int m_lineFlags { 0 };
    QColor m_color;
    float m_lineWidth { 1.0f };
};
}  // namespace QIM

#endif  // QIMPLOT3DLINEITEMNODE_H
