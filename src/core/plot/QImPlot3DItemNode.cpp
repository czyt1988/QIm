#include "QImPlot3DItemNode.h"
#include "QImPlot3DNode.h"

namespace QIM
{
QImPlot3DItemNode::QImPlot3DItemNode(QObject* parent) : QImAbstractNode(parent)
{
    setAutoIdEnabled(false);
}

QImPlot3DItemNode::~QImPlot3DItemNode()
{
}

void QImPlot3DItemNode::setLabel(const QString& label)
{
    const QByteArray utf8 = label.toUtf8();
    if (m_utf8Label != utf8) {
        m_utf8Label = utf8;
        Q_EMIT labelChanged(label);
    }
}

QString QImPlot3DItemNode::label() const
{
    return QString::fromUtf8(m_utf8Label);
}

const char* QImPlot3DItemNode::labelConstData() const
{
    return m_utf8Label.isEmpty() ? "##plot3dItem" : m_utf8Label.constData();
}

QImPlot3DNode* QImPlot3DItemNode::plotNode() const
{
    QImAbstractNode* parent = parentNode();
    while (parent != nullptr) {
        if (QImPlot3DNode* plotNode = qobject_cast< QImPlot3DNode* >(parent)) {
            return plotNode;
        }
        parent = parent->parentNode();
    }
    return nullptr;
}

void QImPlot3DItemNode::endDraw()
{
}
}  // namespace QIM
