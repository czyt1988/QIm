#include "QImPlotItemNode.h"
#include "implot.h"
#include "QtImGuiUtils.h"
namespace QIM
{

class QImPlotItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotItemNode)
public:
    PrivateData(QImPlotItemNode* p);

public:
    QByteArray utf8Label;
    ImAxis xAxisId { ImAxis_X1 };
    ImAxis yAxisId { ImAxis_Y1 };
};

QImPlotItemNode::PrivateData::PrivateData(QImPlotItemNode* p) : q_ptr(p)
{
}
//----------------------------------------------------
// QImPlotItemNode
//----------------------------------------------------
QImPlotItemNode::QImPlotItemNode(QObject* par) : QImAbstractNode(par), QIM_PIMPL_CONSTRUCT
{
    setAutoIdEnabled(false);
}

QImPlotItemNode::~QImPlotItemNode()
{
}

void QImPlotItemNode::setLabel(const QString& name)
{
    QIM_D(d);
    QByteArray nameUtf = name.toUtf8();
    if (nameUtf != d->utf8Label) {
        d->utf8Label = nameUtf;
        Q_EMIT labelChanged(name);
    }
}

QString QImPlotItemNode::label() const
{
    return QString::fromUtf8(d_ptr->utf8Label);
}

const char* QImPlotItemNode::labelConstData() const
{
    return (d_ptr->utf8Label.isEmpty() ? "##plotItem" : d_ptr->utf8Label.constData());
}

void QImPlotItemNode::bindAxis(QImPlotAxisId x, QImPlotAxisId y)
{
    if (isXAxisId(x) && isYAxisId(y)) {
        d_ptr->xAxisId = toImAxis(x);
        d_ptr->yAxisId = toImAxis(y);
    }
}

QPointF QImPlotItemNode::pixelsToPlot(const float& screenX, const float& screenY)
{
    return toQPointF(ImPlot::PixelsToPlot(screenX, screenY, d_ptr->xAxisId, d_ptr->yAxisId));
}

QPointF QImPlotItemNode::plotToPixels(const double& doubleX, const double& doubleY)
{
    return toQPointF(ImPlot::PlotToPixels(doubleX, doubleY, d_ptr->xAxisId, d_ptr->yAxisId));
}

QImPlotAxisId QImPlotItemNode::xAxisId() const
{
    return toQImPlotAxisId(d_ptr->xAxisId);
}

QImPlotAxisId QImPlotItemNode::yAxisId() const
{
    return toQImPlotAxisId(d_ptr->yAxisId);
}

}  // end namespace QIM
