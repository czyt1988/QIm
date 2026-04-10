#ifndef QIMPLOTTEXTITEMNODE_H
#define QIMPLOTTEXTITEMNODE_H
#include <QColor>
#include <QPointF>
#include "QImPlotItemNode.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Qt-style wrapper for ImPlot text annotations
 *
 * @class QImPlotTextItemNode
 * @ingroup plot_items
 *
 * @details Provides Qt-style retained mode encapsulation for ImPlot text labels.
 *          The node manages annotation text, anchor position in plot coordinates,
 *          optional pixel offset, orientation, and inlay text color.
 *
 * @note Plot text is rendered in plot space and does not create a legend item.
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot文本标注的Qt风格封装
 *
 * @class QImPlotTextItemNode
 * @ingroup plot_items
 *
 * @details 为ImPlot文本标注提供Qt风格的保留模式封装。
 *          该节点管理标注文本、绘图坐标中的锚点、可选像素偏移、文字方向以及内嵌文本颜色。
 *
 * @note PlotText 在绘图空间中渲染文本，不会创建图例项。
 *
 * @see QImPlotItemNode
 * @see QImPlotNode
 * \endif
 */
class QIM_CORE_API QImPlotTextItemNode : public QImPlotItemNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlotTextItemNode)

    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY geometryChanged)
    Q_PROPERTY(QPointF pixelOffset READ pixelOffset WRITE setPixelOffset NOTIFY geometryChanged)
    Q_PROPERTY(bool vertical READ isVertical WRITE setVertical NOTIFY textFlagChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    enum
    {
        Type = InnerType + 10
    };

    virtual int type() const override
    {
        return Type;
    }

    // Constructs a text annotation item node.
    explicit QImPlotTextItemNode(QObject* parent = nullptr);

    // Destroys the text annotation item node.
    ~QImPlotTextItemNode();

    // Sets the displayed text.
    void setText(const QString& text);

    // Returns the displayed text.
    QString text() const;

    // Sets the anchor position in plot coordinates.
    void setPosition(const QPointF& position);

    // Returns the anchor position in plot coordinates.
    QPointF position() const;

    // Sets the pixel offset relative to the anchor position.
    void setPixelOffset(const QPointF& offset);

    // Returns the pixel offset relative to the anchor position.
    QPointF pixelOffset() const;

    // Returns true when text is rendered vertically.
    bool isVertical() const;

    // Enables or disables vertical rendering.
    void setVertical(bool on);

    // Returns the raw ImPlotTextFlags value.
    int textFlags() const;

    // Sets the raw ImPlotTextFlags value.
    void setTextFlags(int flags);

    // Sets the annotation text color.
    void setColor(const QColor& color);

    // Returns the annotation text color.
    QColor color() const;

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when the displayed text changes.
     * @param text New annotation text.
     * \endif
     *
     * \if CHINESE
     * @brief 当显示文本发生变化时触发。
     * @param text 新的标注文本。
     * \endif
     */
    void textChanged(const QString& text);

    /**
     * \if ENGLISH
     * @brief Emitted when position or pixel offset changes.
     * \endif
     *
     * \if CHINESE
     * @brief 当位置或像素偏移发生变化时触发。
     * \endif
     */
    void geometryChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when text flags change.
     * \endif
     *
     * \if CHINESE
     * @brief 当文本标志位发生变化时触发。
     * \endif
     */
    void textFlagChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when the annotation color changes.
     * @param color New annotation color.
     * \endif
     *
     * \if CHINESE
     * @brief 当标注颜色发生变化时触发。
     * @param color 新的标注颜色。
     * \endif
     */
    void colorChanged(const QColor& color);

protected:
    // Draws the text annotation item.
    virtual bool beginDraw() override;
};

}  // namespace QIM

#endif  // QIMPLOTTEXTITEMNODE_H
