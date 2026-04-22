#ifndef QIMPLOT3DSTYLENODE_H
#define QIMPLOT3DSTYLENODE_H

#include <QObject>
#include <QColor>
#include <QSizeF>
#include "QImAPI.h"
#include "QImPlot3D.h"

namespace QIM
{

class QImPlot3DNode;

/**
 * \if ENGLISH
 * @brief Persistent style node for 3D plots with Q_PROPERTY-based style management
 * @details Provides a QObject-based interface to manage all ImPlot3DStyle fields and
 *          ImPlot3DCol color values as Qt properties. Supports theme presets via
 *          styleColorsAuto/Dark/Light/Classic convenience methods.
 *          Applied as a single ImPlot3D::GetStyle() assignment in QImPlot3DNode::beginDraw()
 *          before child items render — no Push/Pop API exposed to users.
 * @note This node is created as a child of QImPlot3DNode (one style node per plot).
 * @see QImPlot3DNode::styleNode(), ImPlot3D::GetStyle(), ImPlot3D::StyleColorsAuto()
 * \endif
 *
 * \if CHINESE
 * @brief 三维绘图的持久样式节点，提供基于 Q_PROPERTY 的样式管理
 * @details 提供基于 QObject 的接口来管理所有 ImPlot3DStyle 字段和 ImPlot3DCol 颜色值，
 *          以 Qt 属性形式暴露。支持通过 styleColorsAuto/Dark/Light/Classic 便捷方法
 *          设置主题预设。在 QImPlot3DNode::beginDraw() 中作为一次性的 GetStyle() 赋值
 *          应用到子元素渲染之前 — 不向用户暴露 Push/Pop API。
 * @note 此节点作为 QImPlot3DNode 的子节点创建（每个绘图一个样式节点）。
 * @see QImPlot3DNode::styleNode(), ImPlot3D::GetStyle(), ImPlot3D::StyleColorsAuto()
 * \endif
 */
class QIM_CORE_API QImPlot3DStyleNode : public QObject
{
    Q_OBJECT

    // Style variables
    Q_PROPERTY(float lineWeight READ lineWeight WRITE setLineWeight NOTIFY styleChanged)
    Q_PROPERTY(int marker READ marker WRITE setMarker NOTIFY styleChanged)
    Q_PROPERTY(float markerSize READ markerSize WRITE setMarkerSize NOTIFY styleChanged)
    Q_PROPERTY(float markerWeight READ markerWeight WRITE setMarkerWeight NOTIFY styleChanged)
    Q_PROPERTY(float fillAlpha READ fillAlpha WRITE setFillAlpha NOTIFY styleChanged)
    Q_PROPERTY(QSizeF plotDefaultSize READ plotDefaultSize WRITE setPlotDefaultSize NOTIFY styleChanged)
    Q_PROPERTY(QSizeF plotMinSize READ plotMinSize WRITE setPlotMinSize NOTIFY styleChanged)
    Q_PROPERTY(QSizeF plotPadding READ plotPadding WRITE setPlotPadding NOTIFY styleChanged)
    Q_PROPERTY(QSizeF labelPadding READ labelPadding WRITE setLabelPadding NOTIFY styleChanged)
    Q_PROPERTY(float viewScaleFactor READ viewScaleFactor WRITE setViewScaleFactor NOTIFY styleChanged)
    Q_PROPERTY(QSizeF legendPadding READ legendPadding WRITE setLegendPadding NOTIFY styleChanged)
    Q_PROPERTY(QSizeF legendInnerPadding READ legendInnerPadding WRITE setLegendInnerPadding NOTIFY styleChanged)
    Q_PROPERTY(QSizeF legendSpacing READ legendSpacing WRITE setLegendSpacing NOTIFY styleChanged)

    // Plot colors
    Q_PROPERTY(QColor titleTextColor READ titleTextColor WRITE setTitleTextColor NOTIFY styleChanged)
    Q_PROPERTY(QColor inlayTextColor READ inlayTextColor WRITE setInlayTextColor NOTIFY styleChanged)
    Q_PROPERTY(QColor frameBgColor READ frameBgColor WRITE setFrameBgColor NOTIFY styleChanged)
    Q_PROPERTY(QColor plotBgColor READ plotBgColor WRITE setPlotBgColor NOTIFY styleChanged)
    Q_PROPERTY(QColor plotBorderColor READ plotBorderColor WRITE setPlotBorderColor NOTIFY styleChanged)

    // Legend colors
    Q_PROPERTY(QColor legendBgColor READ legendBgColor WRITE setLegendBgColor NOTIFY styleChanged)
    Q_PROPERTY(QColor legendBorderColor READ legendBorderColor WRITE setLegendBorderColor NOTIFY styleChanged)
    Q_PROPERTY(QColor legendTextColor READ legendTextColor WRITE setLegendTextColor NOTIFY styleChanged)

    // Axis colors
    Q_PROPERTY(QColor axisTextColor READ axisTextColor WRITE setAxisTextColor NOTIFY styleChanged)
    Q_PROPERTY(QColor axisGridColor READ axisGridColor WRITE setAxisGridColor NOTIFY styleChanged)
    Q_PROPERTY(QColor axisTickColor READ axisTickColor WRITE setAxisTickColor NOTIFY styleChanged)

    QIM_DECLARE_PRIVATE(QImPlot3DStyleNode)
    Q_DISABLE_COPY(QImPlot3DStyleNode)

public:
    // Constructor with QImPlot3DNode as parent
    explicit QImPlot3DStyleNode(QImPlot3DNode* parent = nullptr);

    // Destructor
    ~QImPlot3DStyleNode();

    //----------------------------------------------------
    // Style variables
    //----------------------------------------------------

    // Gets the line weight in pixels
    float lineWeight() const;
    // Sets the line weight in pixels
    void setLineWeight(float weight);

    // Gets the marker type as QImPlot3DMarkerShape enum value
    int marker() const;
    // Sets the marker type from QImPlot3DMarkerShape enum value
    void setMarker(int marker);

    // Gets the marker size in pixels
    float markerSize() const;
    // Sets the marker size in pixels
    void setMarkerSize(float size);

    // Gets the marker outline weight in pixels
    float markerWeight() const;
    // Sets the marker outline weight in pixels
    void setMarkerWeight(float weight);

    // Gets the fill alpha modifier
    float fillAlpha() const;
    // Sets the fill alpha modifier
    void setFillAlpha(float alpha);

    // Gets the default plot size
    QSizeF plotDefaultSize() const;
    // Sets the default plot size
    void setPlotDefaultSize(const QSizeF& size);

    // Gets the minimum plot size
    QSizeF plotMinSize() const;
    // Sets the minimum plot size
    void setPlotMinSize(const QSizeF& size);

    // Gets the plot padding
    QSizeF plotPadding() const;
    // Sets the plot padding
    void setPlotPadding(const QSizeF& padding);

    // Gets the label padding
    QSizeF labelPadding() const;
    // Sets the label padding
    void setLabelPadding(const QSizeF& padding);

    // Gets the 3D view scale factor
    float viewScaleFactor() const;
    // Sets the 3D view scale factor
    void setViewScaleFactor(float factor);

    // Gets the legend padding from plot edges
    QSizeF legendPadding() const;
    // Sets the legend padding from plot edges
    void setLegendPadding(const QSizeF& padding);

    // Gets the legend inner padding
    QSizeF legendInnerPadding() const;
    // Sets the legend inner padding
    void setLegendInnerPadding(const QSizeF& padding);

    // Gets the spacing between legend entries
    QSizeF legendSpacing() const;
    // Sets the spacing between legend entries
    void setLegendSpacing(const QSizeF& spacing);

    //----------------------------------------------------
    // Plot colors
    //----------------------------------------------------

    // Gets the title text color
    QColor titleTextColor() const;
    // Sets the title text color
    void setTitleTextColor(const QColor& color);

    // Gets the inlay text color
    QColor inlayTextColor() const;
    // Sets the inlay text color
    void setInlayTextColor(const QColor& color);

    // Gets the frame background color
    QColor frameBgColor() const;
    // Sets the frame background color
    void setFrameBgColor(const QColor& color);

    // Gets the plot background color
    QColor plotBgColor() const;
    // Sets the plot background color
    void setPlotBgColor(const QColor& color);

    // Gets the plot border color
    QColor plotBorderColor() const;
    // Sets the plot border color
    void setPlotBorderColor(const QColor& color);

    //----------------------------------------------------
    // Legend colors
    //----------------------------------------------------

    // Gets the legend background color
    QColor legendBgColor() const;
    // Sets the legend background color
    void setLegendBgColor(const QColor& color);

    // Gets the legend border color
    QColor legendBorderColor() const;
    // Sets the legend border color
    void setLegendBorderColor(const QColor& color);

    // Gets the legend text color
    QColor legendTextColor() const;
    // Sets the legend text color
    void setLegendTextColor(const QColor& color);

    //----------------------------------------------------
    // Axis colors
    //----------------------------------------------------

    // Gets the axis text color
    QColor axisTextColor() const;
    // Sets the axis text color
    void setAxisTextColor(const QColor& color);

    // Gets the axis grid color
    QColor axisGridColor() const;
    // Sets the axis grid color
    void setAxisGridColor(const QColor& color);

    // Gets the axis tick color
    QColor axisTickColor() const;
    // Sets the axis tick color
    void setAxisTickColor(const QColor& color);

    //----------------------------------------------------
    // Theme convenience methods
    //----------------------------------------------------

    // Applies the Auto theme (colors derived from current ImGui style)
    void styleColorsAuto();

    // Applies the Dark theme
    void styleColorsDark();

    // Applies the Light theme
    void styleColorsLight();

    // Applies the Classic theme
    void styleColorsClassic();

    //----------------------------------------------------
    // Style application
    //----------------------------------------------------

    // Applies all stored style properties to ImPlot3D::GetStyle()
    void applyStyle() const;

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when any style property changes
     * \endif
     *
     * \if CHINESE
     * @brief 当任意样式属性变更时发射
     * \endif
     */
    void styleChanged();
};

}  // namespace QIM

#endif  // QIMPLOT3DSTYLENODE_H