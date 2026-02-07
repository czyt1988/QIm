#ifndef QIMPLOTAXISINFO_H
#define QIMPLOTAXISINFO_H
#include <QObject>
#include "QImAPI.h"
#include "QImPlot.h"
namespace QIM
{
class QImPlotNode;
/**
 * \if ENGLISH
 * @brief Qt wrapper class for managing ImPlot axis configuration with type-safe properties
 * @details Provides a QObject-based interface to configure ImPlot axes without directly
 *          manipulating low-level ImPlotAxisFlags bitmasks. Encapsulates:
 *          - Axis label text and visibility
 *          - Range limits (min/max) with auto-fit behavior
 *          - All ImPlotAxisFlags_ options exposed as intuitive boolean properties
 *          - Scale type configuration (linear/log/time/symlog)
 *          - Bidirectional conversion between Qt enums (QImPlotAxisId) and ImPlot enums (ImAxis)
 * @note This class does NOT render plots itself. It stores configuration state that should be
 *       applied to ImPlot via SetupAxis(), SetupAxisLimits(), and SetupAxisScale() during
 *       plot setup (inside BeginPlot/EndPlot block).
 * @warning All property changes are stored locally; actual plot appearance updates only after
 *          re-rendering with updated configuration applied to ImPlot context.
 * @see ImPlot::SetupAxis(), ImPlot::SetupAxisLimits(), ImPlot::SetupAxisScale()
 * \endif
 *
 * \if CHINESE
 * @brief 用于管理 ImPlot 坐标轴配置的 Qt 封装类，提供类型安全的属性接口
 * @details 提供基于 QObject 的接口来配置 ImPlot 坐标轴，无需直接操作底层 ImPlotAxisFlags 位掩码。
 *          封装功能包括：
 *          - 坐标轴标签文本及可见性
 *          - 范围限制（最小/最大值）及自动适配行为
 *          - 所有 ImPlotAxisFlags_ 选项以直观的布尔属性形式暴露
 *          - 刻度类型配置（线性/对数/时间/对称对数）
 *          - Qt 枚举（QImPlotAxisId）与 ImPlot 枚举（ImAxis）的双向转换
 * @note 此类本身不渲染绘图。它仅存储配置状态，需在绘图设置阶段（BeginPlot/EndPlot 块内）
 *       通过 SetupAxis()、SetupAxisLimits() 和 SetupAxisScale() 应用于 ImPlot。
 * @warning 所有属性变更仅在本地存储；实际绘图外观更新需在重新渲染时将配置应用到 ImPlot 上下文后生效。
 * @see ImPlot::SetupAxis(), ImPlot::SetupAxisLimits(), ImPlot::SetupAxisScale()
 * \endif
 */
class QIM_CORE_API QImPlotAxisInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(double minLimits READ minLimits WRITE setMinLimits NOTIFY limitsChanged)
    Q_PROPERTY(double maxLimits READ maxLimits WRITE setMaxLimits NOTIFY limitsChanged)
    // About ImPlotAxisFlags
    Q_PROPERTY(bool autoFit READ isAutoFit WRITE setAutoFit NOTIFY axisFlagChanged)
    Q_PROPERTY(bool inverted READ isInverted WRITE setInverted NOTIFY axisFlagChanged)
    Q_PROPERTY(bool labelEnabled READ isLabelEnabled WRITE setLabelEnabled NOTIFY axisFlagChanged)
    Q_PROPERTY(bool gridLinesEnabled READ isGridLinesEnabled WRITE setGridLinesEnabled NOTIFY axisFlagChanged)
    Q_PROPERTY(bool tickMarksEnabled READ isTickMarksEnabled WRITE setTickMarksEnabled NOTIFY axisFlagChanged)
    Q_PROPERTY(bool tickLabelsEnabled READ isTickLabelsEnabled WRITE setTickLabelsEnabled NOTIFY axisFlagChanged)
    Q_PROPERTY(bool initialFitEnabled READ isInitialFitEnabled WRITE setInitialFitEnabled NOTIFY axisFlagChanged)
    Q_PROPERTY(bool menusEnabled READ isMenusEnabled WRITE setMenusEnabled NOTIFY axisFlagChanged)
    Q_PROPERTY(bool sideSwitchEnabled READ isSideSwitchEnabled WRITE setSideSwitchEnabled NOTIFY axisFlagChanged)
    Q_PROPERTY(bool highlightEnabled READ isHighlightEnabled WRITE setHighlightEnabled NOTIFY axisFlagChanged)
    Q_PROPERTY(bool opposite READ isOpposite WRITE setOpposite NOTIFY axisFlagChanged)
    Q_PROPERTY(bool foreground READ isForeground WRITE setForeground NOTIFY axisFlagChanged)
    Q_PROPERTY(bool rangeFit READ isRangeFit WRITE setRangeFit NOTIFY axisFlagChanged)
    Q_PROPERTY(bool panStretch READ isPanStretch WRITE setPanStretch NOTIFY axisFlagChanged)
    Q_PROPERTY(bool lockMin READ isLockMin WRITE setLockMin NOTIFY axisFlagChanged)
    Q_PROPERTY(bool lockMax READ isLockMax WRITE setLockMax NOTIFY axisFlagChanged)
    Q_PROPERTY(bool lock READ isLock WRITE setLock NOTIFY axisFlagChanged)
    Q_PROPERTY(bool noDecorations READ isNoDecorations WRITE setNoDecorations NOTIFY axisFlagChanged)
    //  scaleType
    Q_PROPERTY(QImPlotScaleType scaleType READ scaleType WRITE setScaleType NOTIFY scaleTypeChanged)

    QIM_DECLARE_PRIVATE(QImPlotAxisInfo)
    Q_DISABLE_COPY(QImPlotAxisInfo)
public:
    explicit QImPlotAxisInfo(QImPlotAxisId axis, QImPlotNode* plot = nullptr);
    ~QImPlotAxisInfo();
    QImPlotAxisId axisId() const;
    int imAxis() const;
    // 标签
    QString label() const;
    void setLabel(const QString& label);

    // 范围限制
    double minLimits() const;
    void setMinLimits(double min);
    double maxLimits() const;
    void setMaxLimits(double max);

    QImPlotCondition limitsCondition() const;
    void setLimitsCondition(QImPlotCondition v);

    void setLimits(double min, double max, QImPlotCondition cond = QImPlotCondition::Once);
    // About ImPlotAxisFlags / 这里为了适应Qt的命名习惯，设置为肯定语义
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_AutoFit / cn:对应ImPlotAxisFlags::ImPlotAxisFlags_AutoFit
    bool isAutoFit() const;
    void setAutoFit(bool on);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_Invert / cn:对应ImPlotAxisFlags::ImPlotAxisFlags_Invert
    bool isInverted() const;
    void setInverted(bool on);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_NoLabel / cn:对应ImPlotAxisFlags::ImPlotAxisFlags_NoLabel
    bool isLabelEnabled() const;
    void setLabelEnabled(bool enabled);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_NoGridLines / cn:对应ImPlotAxisFlags::ImPlotAxisFlags_NoGridLines
    bool isGridLinesEnabled() const;
    void setGridLinesEnabled(bool enabled);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_NoTickMarks / cn:对应ImPlotAxisFlags::ImPlotAxisFlags_NoTickMarks
    bool isTickMarksEnabled() const;
    void setTickMarksEnabled(bool enabled);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_NoTickLabels / cn:对应ImPlotAxisFlags::ImPlotAxisFlags_NoTickLabels
    bool isTickLabelsEnabled() const;
    void setTickLabelsEnabled(bool enabled);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_NoInitialFit / cn:对应ImPlotAxisFlags::ImPlotAxisFlags_NoInitialFit
    bool isInitialFitEnabled() const;
    void setInitialFitEnabled(bool enabled);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_NoMenus / cn:对应ImPlotAxisFlags::ImPlotAxisFlags_NoMenus
    bool isMenusEnabled() const;
    void setMenusEnabled(bool enabled);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_NoSideSwitch / cn:对应ImPlotAxisFlags::ImPlotAxisFlags_NoSideSwitch
    bool isSideSwitchEnabled() const;
    void setSideSwitchEnabled(bool enabled);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_NoHighlight / cn:对应ImPlotAxisFlags::ImPlotAxisFlags_NoHighlight
    bool isHighlightEnabled() const;
    void setHighlightEnabled(bool enabled);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_Opposite / cn:对应ImPlotAxisFlags::ImPlotAxisFlags_Opposite
    bool isOpposite() const;
    void setOpposite(bool on);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_Foreground / cn:对应ImPlotAxisFlags::ImPlotAxisFlags_Foreground
    bool isForeground() const;
    void setForeground(bool on);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_RangeFit / cn:对应ImPlotAxisFlags::ImPlotAxisFlags_RangeFit
    bool isRangeFit() const;
    void setRangeFit(bool on);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_PanStretch / cn:对应ImPlotAxisFlags::ImPlotAxisFlags_PanStretch
    bool isPanStretch() const;
    void setPanStretch(bool on);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_LockMin / cn:对应ImPlotAxisFlags::ImPlotAxisFlags_LockMin
    bool isLockMin() const;
    void setLockMin(bool on);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_LockMax / cn:对应ImPlotAxisFlags::ImPlotAxisFlags_LockMax
    bool isLockMax() const;
    void setLockMax(bool on);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_Lock / cn:对应ImPlotAxisFlags::ImPlotAxisFlags_Lock
    bool isLock() const;  // LockMin && LockMax
    void setLock(bool on);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_NoDecorations / cn:对应ImPlotAxisFlags::ImPlotAxisFlags_NoDecorations
    bool isNoDecorations() const;
    void setNoDecorations(bool enabled);
    // 原始标志访问（高级用法）
    int axisFlags() const;
    void setAxisFlags(int flags);
    // Gets the current axis scale type (linear, logarithmic, time, etc.)
    QImPlotScaleType scaleType() const;
    void setScaleType(QImPlotScaleType t);
    int imPlotScale() const;
    // 标记坐标轴是否有效（隐藏或显示，对于X1和Y1隐藏或显示是通过setNoDecorations来实现）
    bool isEnabled() const;
    void setEnabled(bool on);
Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when the axis label text changes
     * @param label The new label text string (empty string indicates default axis name)
     * @details Triggered by setLabel() when the new label differs from current value.
     *          Typically used to update UI elements displaying axis names or to persist label changes.
     * @note This signal is NOT emitted when label visibility changes (controlled by labelEnabled property).
     *       Only the text content change triggers this signal.
     * @see setLabel(), isLabelEnabled()
     * \endif
     *
     * \if CHINESE
     * @brief 当坐标轴标签文本改变时发射
     * @param label 新的标签文本字符串（空字符串表示使用默认轴名称）
     * @details 当 setLabel() 设置的新标签与当前值不同时触发。
     *          通常用于更新显示轴名称的 UI 元素或持久化标签变更。
     * @note 此信号在标签可见性变更时**不会**触发（由 labelEnabled 属性控制）。
     *       仅标签文本内容变更会触发此信号。
     * @see setLabel(), isLabelEnabled()
     * \endif
     */
    void labelChanged(const QString& label);

    /**
     * \if ENGLISH
     * @brief Emitted when axis range limits change (min, max, or both)
     * @param min New minimum value of the visible axis range
     * @param max New maximum value of the visible axis range
     * @details Triggered by setMinLimits(), setMaxLimits(), or setLimits() when values differ from current limits.
     *          Note: Changing limits does NOT immediately affect the plot; must call SetupAxisLimits() during
     *          next plot render to apply changes. This signal allows UI to reflect pending limit changes.
     * @warning Values are NOT validated for min < max; invalid ranges (min >= max) may cause ImPlot rendering issues.
     * @see setLimits(), ImPlot::SetupAxisLimits()
     * \endif
     *
     * \if CHINESE
     * @brief 当坐标轴范围限制改变时发射（最小值、最大值或两者同时）
     * @param min 坐标轴可见范围的新最小值
     * @param max 坐标轴可见范围的新最大值
     * @details 当 setMinLimits()、setMaxLimits() 或 setLimits() 设置的值与当前限制不同时触发。
     *          注意：修改限制值不会立即影响绘图；必须在下次绘图渲染时调用 SetupAxisLimits() 才能应用变更。
     *          此信号允许 UI 反映待处理的限制变更。
     * @warning 值**不**验证 min < max；无效范围（min >= max）可能导致 ImPlot 渲染问题。
     * @see setLimits(), ImPlot::SetupAxisLimits()
     * \endif
     */
    void limitsChanged(double min, double max);

    /**
     * \if ENGLISH
     * @brief Emitted when any axis flag property changes (autoFit, inverted, gridLinesEnabled, etc.)
     * @details Triggered by ANY flag-modifying setter (e.g., setAutoFit(), setLock(), setDecorationsEnabled())
     *          when the underlying ImPlotAxisFlags bitmask changes. Single signal aggregates all flag changes
     *          for efficiency - connected slots should query relevant properties to determine exact change.
     * @note This signal is NOT emitted when setting flags to identical value (change detection via != comparison).
     *       Also NOT emitted by direct axisFlags() getter access (only setters trigger emission).
     * @warning Does NOT indicate which specific flag changed; slots must inspect individual properties or
     *          call axisFlags() to determine changed flags. For fine-grained change tracking, connect to
     *          individual property signals if available (e.g., onAutoFitChanged in QML).
     * @see setAxisFlags(), axisFlags()
     * \endif
     *
     * \if CHINESE
     * @brief 当任意坐标轴标志属性变更时发射（autoFit、inverted、gridLinesEnabled 等）
     * @details 当**任何**修改标志的设置器（如 setAutoFit()、setLock()、setDecorationsEnabled()）
     *          导致底层 ImPlotAxisFlags 位掩码变更时触发。单个信号聚合所有标志变更加以提高效率 —
     *          已连接的槽函数需查询相关属性以确定具体变更。
     * @note 设置标志为相同值时**不会**触发此信号（通过 != 比较进行变更检测）。
     *       直接通过 axisFlags() getter 访问**不会**触发此信号（仅设置器触发）。
     * @warning 不指示具体哪个标志发生变更；槽函数必须检查单独属性或调用 axisFlags() 以确定变更的标志。
     *          如需细粒度变更跟踪，可连接到单独的属性信号（如 QML 中的 onAutoFitChanged）。
     * @see setAxisFlags(), axisFlags()
     * \endif
     */
    void axisFlagChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when the axis scale type changes (linear → log, time → symlog, etc.)
     * @param scaleType The new scale type as QImPlotScaleType enum value
     * @details Triggered by setScaleType() when the new scale type differs from current type.
     *          Scale types control value transformation and formatting:
     *          - Linear: Standard linear mapping (default)
     *          - Time:   Unix timestamp interpretation with date/time formatting
     *          - Log10:  Base-10 logarithmic mapping (requires positive values)
     *          - SymLog: Symmetric logarithmic mapping (handles negative values near zero)
     * @note Actual scale change takes effect only after calling ImPlot::SetupAxisScale() during plot setup.
     *       This signal allows UI to update scale-type dependent controls (e.g., disable negative values for Log10).
     * @warning Changing to Log10 scale while data contains non-positive values may cause rendering artifacts.
     *         Applications should validate data compatibility before scale type changes.
     * @see setScaleType(), ImPlot::SetupAxisScale()
     * \endif
     *
     * \if CHINESE
     * @brief 当坐标轴刻度类型变更时发射（线性 → 对数、时间 → 对称对数等）
     * @param scaleType 以 QImPlotScaleType 枚举值表示的新刻度类型
     * @details 当 setScaleType() 设置的新刻度类型与当前类型不同时触发。
     *          刻度类型控制值的变换和格式化：
     *          - Linear: 标准线性映射（默认）
     *          - Time:   Unix 时间戳解释，带日期/时间格式化
     *          - Log10:  以 10 为底的对数映射（要求正值）
     *          - SymLog: 对称对数映射（在零点附近可处理负值）
     * @note 实际刻度变更仅在绘图设置阶段调用 ImPlot::SetupAxisScale() 后生效。
     *       此信号允许 UI 更新依赖刻度类型的控件（例如对 Log10 禁用负值输入）。
     * @warning 当数据包含非正值时切换到 Log10 刻度可能导致渲染异常。
     *         应用应在变更刻度类型前验证数据兼容性。
     * @see setScaleType(), ImPlot::SetupAxisScale()
     * \endif
     */
    void scaleTypeChanged();

public:
    /**
     * @brief 渲染
     */
    void render();
};
}
#endif
