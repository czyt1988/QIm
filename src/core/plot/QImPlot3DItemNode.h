#ifndef QIMPLOT3DITEMNODE_H
#define QIMPLOT3DITEMNODE_H

#include "QImAbstractNode.h"

namespace QIM
{
class QImPlot3DNode;

class QIM_CORE_API QImPlot3DItemNode : public QImAbstractNode
{
    Q_OBJECT

    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)

public:
    enum TypeValue
    {
        InnerType = 200,
        UserType  = 10000
    };

    explicit QImPlot3DItemNode(QObject* parent = nullptr);
    ~QImPlot3DItemNode() override;

    virtual int type() const = 0;

    void setLabel(const QString& label);
    QString label() const;
    const char* labelConstData() const;

    QImPlot3DNode* plotNode() const;

Q_SIGNALS:
    void labelChanged(const QString& label);

protected:
    void endDraw() override;

private:
    QByteArray m_utf8Label;
};
}  // namespace QIM

#endif  // QIMPLOT3DITEMNODE_H
