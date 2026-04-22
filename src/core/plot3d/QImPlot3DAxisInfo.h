#ifndef QIMPLOT3DAXISINFO_H
#define QIMPLOT3DAXISINFO_H

#include <QObject>
#include <QList>
#include "QImAPI.h"
#include "QImPlot3D.h"
#include "QImPlot3DAxisFormatter.h"

namespace QIM
{

class QImPlot3DNode;
class QImPlot3DAxisTransform;

/**
 * \if ENGLISH
 * @brief Qt wrapper class for managing ImPlot3D axis configuration with type-safe properties
 * @details Provides a QObject-based interface to configure ImPlot3D axes without directly
 *          manipulating low-level ImPlot3DAxisFlags bitmasks. Encapsulates:
 *          - Axis label text and visibility
 *          - Range limits (min/max) with auto-fit behavior
 *          - All ImPlot3DAxisFlags_ options exposed as intuitive boolean properties
 *          - Scale type configuration (linear/log/symlog)
 *          - Bidirectional conversion between Qt enums (QImPlot3DAxisId) and ImPlot3D enums (ImAxis3D)
 * @note This class does NOT render plots itself. It stores configuration state that should be
 *       applied to ImPlot3D via SetupAxis(), SetupAxisLimits(), and SetupAxisScale() during
 *       plot setup (inside BeginPlot/EndPlot block).
 * @warning All property changes are stored locally; actual plot appearance updates only after
 *          re-rendering with updated configuration applied to ImPlot3D context.
 * @see ImPlot3D::SetupAxis(), ImPlot3D::SetupAxisLimits(), ImPlot3D::SetupAxisScale()
 * \endif
 *
 * \if CHINESE
 * @brief 用于管理 ImPlot3D 坐标轴配置的 Qt 封装类，提供类型安全的属性接口
 * @details 提供基于 QObject 的接口来配置 ImPlot3D 坐标轴，无需直接操作底层 ImPlot3DAxisFlags 位掩码。
 *          封装功能包括：
 *          - 坐标轴标签文本及可见性
 *          - 范围限制（最小/最大值）及自动适配行为
 *          - 所有 ImPlot3DAxisFlags_ 选项以直观的布尔属性形式暴露
 *          - 刻度类型配置（线性/对数/对称对数）
 *          - Qt 枚举（QImPlot3DAxisId）与 ImPlot3D 枚举（ImAxis3D）的双向转换
 * @note 此类本身不渲染绘图。它仅存储配置状态，需在绘图设置阶段（BeginPlot/EndPlot 块内）
 *       通过 SetupAxis()、SetupAxisLimits() 和 SetupAxisScale() 应用于 ImPlot3D。
 * @warning 所有属性变更仅在本地存储；实际绘图外观更新需在重新渲染时将配置应用到 ImPlot3D 上下文后生效。
 * @see ImPlot3D::SetupAxis(), ImPlot3D::SetupAxisLimits(), ImPlot3D::SetupAxisScale()
 * \endif
 */
class QIM_CORE_API QImPlot3DAxisInfo : public QObject
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlot3DAxisInfo)

    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(double minLimit READ minLimit WRITE setMinLimit NOTIFY limitsChanged)
    Q_PROPERTY(double maxLimit READ maxLimit WRITE setMaxLimit NOTIFY limitsChanged)

    // ImPlot3DAxisFlags - Negative to Positive semantic (NoXxx -> xxxEnabled)
    Q_PROPERTY(bool labelEnabled READ isLabelEnabled WRITE setLabelEnabled NOTIFY axisFlagChanged)
    Q_PROPERTY(bool gridLinesEnabled READ isGridLinesEnabled WRITE setGridLinesEnabled NOTIFY axisFlagChanged)
    Q_PROPERTY(bool tickMarksEnabled READ isTickMarksEnabled WRITE setTickMarksEnabled NOTIFY axisFlagChanged)
    Q_PROPERTY(bool tickLabelsEnabled READ isTickLabelsEnabled WRITE setTickLabelsEnabled NOTIFY axisFlagChanged)

    // ImPlot3DAxisFlags - Positive to Positive semantic (direct mapping)
    Q_PROPERTY(bool lockMin READ isLockMin WRITE setLockMin NOTIFY axisFlagChanged)
    Q_PROPERTY(bool lockMax READ isLockMax WRITE setLockMax NOTIFY axisFlagChanged)
    Q_PROPERTY(bool autoFit READ isAutoFit WRITE setAutoFit NOTIFY axisFlagChanged)
    Q_PROPERTY(bool invert READ isInvert WRITE setInvert NOTIFY axisFlagChanged)
    Q_PROPERTY(bool panStretch READ isPanStretch WRITE setPanStretch NOTIFY axisFlagChanged)

    // Combined flags
    Q_PROPERTY(bool lock READ isLock WRITE setLock NOTIFY axisFlagChanged)
    Q_PROPERTY(bool decorationsEnabled READ isDecorationsEnabled WRITE setDecorationsEnabled NOTIFY axisFlagChanged)

    // Scale type
    Q_PROPERTY(QImPlot3DScaleType scale READ scale WRITE setScale NOTIFY scaleChanged)

    // Axis constraints
    Q_PROPERTY(double limitsConstraintMin READ limitsConstraintMin WRITE setLimitsConstraintMin NOTIFY limitsConstraintsChanged)
    Q_PROPERTY(double limitsConstraintMax READ limitsConstraintMax WRITE setLimitsConstraintMax NOTIFY limitsConstraintsChanged)
    Q_PROPERTY(double zoomConstraintMin READ zoomConstraintMin WRITE setZoomConstraintMin NOTIFY zoomConstraintsChanged)
    Q_PROPERTY(double zoomConstraintMax READ zoomConstraintMax WRITE setZoomConstraintMax NOTIFY zoomConstraintsChanged)

    // Tick configuration
    Q_PROPERTY(QList<double> tickValues READ tickValues WRITE setTickValues NOTIFY tickConfigChanged)
    Q_PROPERTY(QList<QByteArray> tickLabels READ tickLabels WRITE setTickLabels NOTIFY tickConfigChanged)
    Q_PROPERTY(bool keepDefaultTicks READ isKeepDefaultTicks WRITE setKeepDefaultTicks NOTIFY tickConfigChanged)

    // Axis formatter
    Q_PROPERTY(QImPlot3DAxisFormatter* axisFormatter READ axisFormatter WRITE setAxisFormatter NOTIFY axisFormatterChanged)

   // Q_DISABLE_COPY(QImPlot3DAxisInfo)
public:
    // Constructor for QImPlot3DAxisInfo
    explicit QImPlot3DAxisInfo(QImPlot3DAxisId axis, QImPlot3DNode* plot = nullptr);
    ~QImPlot3DAxisInfo();

    // Returns the QImPlot3DAxisId axis identifier
    QImPlot3DAxisId axisId() const;

    // Returns the raw ImAxis3D value for direct ImPlot3D API usage
    int imAxis3D() const;

    // Returns the parent QImPlot3DNode
    QImPlot3DNode* plotNode() const;

    // Label property
    QString label() const;
    void setLabel(const QString& label);

    // Limits properties
    double minLimit() const;
    void setMinLimit(double min);
    double maxLimit() const;
    void setMaxLimit(double max);
    void setLimits(double min, double max, QImPlot3DCondition cond = QImPlot3DCondition::Once);

    // Scale property
    QImPlot3DScaleType scale() const;
    void setScale(QImPlot3DScaleType type);

    // Custom axis transform property
    QImPlot3DAxisTransform* axisTransform() const;
    void setAxisTransform(QImPlot3DAxisTransform* transform);

    // Raw axis flags access
    int axisFlags() const;
    void setAxisFlags(int flags);

    // Axis visibility (enabled/disabled via NoDecorations flag)
    bool isEnabled() const;
    void setEnabled(bool on);

    // ImPlot3DAxisFlags accessors - Negative to Positive semantic (NoXxx -> xxxEnabled)
    bool isLabelEnabled() const;
    void setLabelEnabled(bool enabled);
    bool isGridLinesEnabled() const;
    void setGridLinesEnabled(bool enabled);
    bool isTickMarksEnabled() const;
    void setTickMarksEnabled(bool enabled);
    bool isTickLabelsEnabled() const;
    void setTickLabelsEnabled(bool enabled);

    // ImPlot3DAxisFlags accessors - Positive to Positive semantic (direct mapping)
    bool isLockMin() const;
    void setLockMin(bool on);
    bool isLockMax() const;
    void setLockMax(bool on);
    bool isAutoFit() const;
    void setAutoFit(bool on);
    bool isInvert() const;
    void setInvert(bool on);
    bool isPanStretch() const;
    void setPanStretch(bool on);

    // Combined flags accessors
    bool isLock() const;
    void setLock(bool on);
    bool isDecorationsEnabled() const;
    void setDecorationsEnabled(bool enabled);

    // Limits constraint properties
    double limitsConstraintMin() const;
    void setLimitsConstraintMin(double min);
    double limitsConstraintMax() const;
    void setLimitsConstraintMax(double max);
    void setLimitsConstraints(double min, double max);

    // Zoom constraint properties
    double zoomConstraintMin() const;
    void setZoomConstraintMin(double min);
    double zoomConstraintMax() const;
    void setZoomConstraintMax(double max);
    void setZoomConstraints(double min, double max);

    // Axis formatter property
    QImPlot3DAxisFormatter* axisFormatter() const;
    void setAxisFormatter(QImPlot3DAxisFormatter* formatter);

    // Tick configuration properties
    QList<double> tickValues() const;
    void setTickValues(const QList<double>& values);
    QList<QByteArray> tickLabels() const;
    void setTickLabels(const QList<QByteArray>& labels);
    bool isKeepDefaultTicks() const;
    void setKeepDefaultTicks(bool keep);

    // Convenience methods for tick configuration
    void setAxisTicks(const QList<double>& values, const QList<QByteArray>& labels = {}, bool keepDefault = false);
    void setAxisTicksRange(double v_min, double v_max, int n_ticks, const QList<QByteArray>& labels = {}, bool keepDefault = false);

    // Applies all stored configuration to ImPlot3D via Setup API calls
    void applySetup() const;

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when the axis label text changes
     * @param label The new label text string
     * \endif
     *
     * \if CHINESE
     * @brief 当坐标轴标签文本改变时发射
     * @param label 新的标签文本字符串
     * \endif
     */
    void labelChanged(const QString& label);

    /**
     * \if ENGLISH
     * @brief Emitted when axis range limits change (min, max, or both)
     * @param min New minimum value of the visible axis range
     * @param max New maximum value of the visible axis range
     * \endif
     *
     * \if CHINESE
     * @brief 当坐标轴范围限制改变时发射（最小值、最大值或两者同时）
     * @param min 坐标轴可见范围的新最小值
     * @param max 坐标轴可见范围的新最大值
     * \endif
     */
    void limitsChanged(double min, double max);

    /**
     * \if ENGLISH
     * @brief Emitted when any axis flag property changes
     * \endif
     *
     * \if CHINESE
     * @brief 当任意坐标轴标志属性变更时发射
     * \endif
     */
    void axisFlagChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when the axis scale type changes
     * \endif
     *
     * \if CHINESE
     * @brief 当坐标轴刻度类型变更时发射
     * \endif
     */
    void scaleChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when axis limits constraints change
     * \endif
     *
     * \if CHINESE
     * @brief 当坐标轴范围限制约束变更时发射
     * \endif
     */
    void limitsConstraintsChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when axis zoom constraints change
     * \endif
     *
     * \if CHINESE
     * @brief 当坐标轴缩放约束变更时发射
     * \endif
     */
    void zoomConstraintsChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when the custom axis transform changes
     * @param transform New transform pointer (may be nullptr if reset)
     * \endif
     *
     * \if CHINESE
     * @brief 当自定义坐标轴变换变更时发射
     * @param transform 新的变换指针（重置时可能为 nullptr）
     * \endif
     */
    void axisTransformChanged(QImPlot3DAxisTransform* transform);

    /**
     * \if ENGLISH
     * @brief Emitted when the axis formatter changes
     * \endif
     *
     * \if CHINESE
     * @brief 当坐标轴格式化器变更时发射
     * \endif
     */
    void axisFormatterChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when tick configuration changes (values, labels, or keepDefault)
     * \endif
     *
     * \if CHINESE
     * @brief 当刻度配置变更时发射（值、标签或保留默认刻度）
     * \endif
     */
    void tickConfigChanged();

private:
    QImPlot3DAxisId m_axisId;
};

}  // namespace QIM

#endif  // QIMPLOT3DAXISINFO_H

