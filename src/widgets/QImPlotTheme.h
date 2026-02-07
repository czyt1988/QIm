#ifndef QIMPLOTTHEME_H
#define QIMPLOTTHEME_H
#include <QColor>
#include "QImAPI.h"
struct ImPlotStyle;
namespace QIM
{
/**
 * \if ENGLISH
 * @brief Theme manager for ImPlot visual styles with Qt integration
 *
 * Provides a Qt-friendly interface to customize non-auto colors in ImPlot themes.
 * Internally stores colors as ImVec4 but exposes QColor API for seamless Qt integration.
 * Supports applying custom themes or built-in ImPlot themes (Auto/Classic/Dark/Light).
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot 可视化样式的 Qt 集成主题管理器
 *
 * 提供 Qt 友好的接口来自定义 ImPlot 主题中的非自动颜色。内部以 ImVec4 存储颜色，
 * 但通过 QColor API 暴露接口以实现与 Qt 的无缝集成。支持应用自定义主题或内置
 * ImPlot 主题（自动/经典/暗色/亮色）。
 * \endif
 */
class QIM_WIDGETS_API QImPlotTheme
{
    QIM_DECLARE_PRIVATE(QImPlotTheme)
public:
    /**
     * \if ENGLISH
     * @brief Built-in ImPlot theme presets
     * \endif
     *
     * \if CHINESE
     * @brief ImPlot 内置主题预设
     * \endif
     */
    enum StyleColorTheme
    {
        StyleColorsAuto    = 0, /**< \if ENGLISH Auto color scheme \else 自动配色方案 \endif */
        StyleColorsClassic = 1, /**< \if ENGLISH Classic ImGui style \else 经典 ImGui 样式 \endif */
        StyleColorsDark    = 2, /**< \if ENGLISH Dark theme \else 暗色主题 \endif */
        StyleColorsLight   = 3  /**< \if ENGLISH Light theme (default) \else 亮色主题（默认） \endif */

    };

public:
    explicit QImPlotTheme();
    QImPlotTheme(const QImPlotTheme& other);
    QImPlotTheme(QImPlotTheme&& other) noexcept;
    ~QImPlotTheme();
    // Frame
    void setFrameBackgroundColor(const QColor& c);
    QColor frameBackgroundColor() const;

    // Plot area
    void setPlotBackgroundColor(const QColor& c);
    QColor plotBackgroundColor() const;

    void setPlotBorderColor(const QColor& c);
    QColor plotBorderColor() const;

    // Legend
    void setLegendBackgroundColor(const QColor& c);
    QColor legendBackgroundColor() const;

    void setLegendBorderColor(const QColor& c);
    QColor legendBorderColor() const;

    void setLegendTextColor(const QColor& c);
    QColor legendTextColor() const;

    // Text
    void setTitleTextColor(const QColor& c);
    QColor titleTextColor() const;

    void setInlayTextColor(const QColor& c);
    QColor inlayTextColor() const;

    void setAxisTextColor(const QColor& c);
    QColor axisTextColor() const;

    // Grid & ticks
    void setAxisGridColor(const QColor& c);
    QColor axisGridColor() const;

    void setAxisTickColor(const QColor& c);
    QColor axisTickColor() const;

    // Interactive elements
    void setSelectionColor(const QColor& c);
    QColor selectionColor() const;

    void setCrosshairsColor(const QColor& c);
    QColor crosshairsColor() const;

    // operator
    bool operator==(const QImPlotTheme& other) const;
    bool operator!=(const QImPlotTheme& other) const;
    QImPlotTheme& operator=(const QImPlotTheme& other);
    QImPlotTheme& operator=(QImPlotTheme&& other) noexcept;
    // Apply theme to ImPlotStyle
    void apply(ImPlotStyle* style) const;
    // 应用默认主题
    static void apply(ImPlotStyle* style, StyleColorTheme theme);
};
}  // end namespace QIM
#endif  // QIMPLOTTHEME_H
