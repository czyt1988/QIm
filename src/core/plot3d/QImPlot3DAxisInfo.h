#ifndef QIMPLOT3DAXISINFO_H
#define QIMPLOT3DAXISINFO_H

#include <QObject>
#include "QImAPI.h"
#include "QImPlot3D.h"

namespace QIM
{

class QImPlot3DExtNode;

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

    QIM_DECLARE_PRIVATE(QImPlot3DAxisInfo)
    Q_DISABLE_COPY(QImPlot3DAxisInfo)

public:
    /**
     * \if ENGLISH
     * @brief Constructor for QImPlot3DAxisInfo
     * @param axis The ImPlot3D axis identifier (X1, Y1, or Z1)
     * @param plot Optional QImPlot3DExtNode parent for memory management
     * \endif
     *
     * \if CHINESE
     * @brief QImPlot3DAxisInfo 构造函数
     * @param axis ImPlot3D 轴标识符（X1、Y1 或 Z1）
     * @param plot 可选的 QImPlot3DExtNode 父对象，用于内存管理
     * \endif
     */
    explicit QImPlot3DAxisInfo(QImPlot3DAxisId axis, QImPlot3DExtNode* plot = nullptr);
    ~QImPlot3DAxisInfo();

    /**
     * \if ENGLISH
     * @brief Returns the QImPlot3DAxisId axis identifier
     * @return QImPlot3DAxisId enum value (X1, Y1, or Z1)
     * \endif
     *
     * \if CHINESE
     * @brief 返回 QImPlot3DAxisId 轴标识符
     * @return QImPlot3DAxisId 枚举值（X1、Y1 或 Z1）
     * \endif
     */
    QImPlot3DAxisId axisId() const;

    /**
     * \if ENGLISH
     * @brief Returns the raw ImAxis3D value for direct ImPlot3D API usage
     * @return Integer matching ImAxis3D_ enumeration (ImAxis3D_X=0, ImAxis3D_Y=1, ImAxis3D_Z=2)
     * \endif
     *
     * \if CHINESE
     * @brief 返回原始 ImAxis3D 值，用于直接调用 ImPlot3D API
     * @return 匹配 ImAxis3D_ 枚举的整数（ImAxis3D_X=0, ImAxis3D_Y=1, ImAxis3D_Z=2）
     * \endif
     */
    int imAxis3D() const;

    /**
     * \if ENGLISH
     * @brief Returns the parent QImPlot3DExtNode
     * @return Pointer to the parent plot node, or nullptr if not attached
     * \endif
     *
     * \if CHINESE
     * @brief 返回父级 QImPlot3DExtNode
     * @return 父级绘图节点指针，如未关联则返回 nullptr
     * \endif
     */
    QImPlot3DExtNode* plotNode() const;

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

    /**
     * \if ENGLISH
     * @brief Applies all stored configuration to ImPlot3D via Setup API calls
     * @details Should be called inside BeginPlot/EndPlot block before plotting items.
     *          Calls ImPlot3D::SetupAxis(), SetupAxisLimits(), SetupAxisScale() as needed.
     * \endif
     *
     * \if CHINESE
     * @brief 通过 Setup API 将所有存储的配置应用到 ImPlot3D
     * @details 应在 BeginPlot/EndPlot 块内调用，在绘制项目之前。
     *          根据需要调用 ImPlot3D::SetupAxis()、SetupAxisLimits()、SetupAxisScale()。
     * \endif
     */
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

private:
    QImPlot3DAxisId m_axisId;
};

}  // namespace QIM

#endif  // QIMPLOT3DAXISINFO_H

