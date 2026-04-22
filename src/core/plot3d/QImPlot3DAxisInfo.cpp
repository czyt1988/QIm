#include "QImPlot3DAxisInfo.h"
#include "QImPlot3DAxisTransform.h"
#include "implot3d.h"
#include "QImPlot3DNode.h"

namespace QIM
{

class QImPlot3DAxisInfo::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlot3DAxisInfo)

public:
    PrivateData(QImPlot3DAxisInfo* p);

public:
    QByteArray labelUtf8;                    ///< UTF-8 label cache
    ImPlot3DAxisFlags flags { ImPlot3DAxisFlags_None };  ///< Axis flags
    double minLimit { 0.0 };
    double maxLimit { 1.0 };
    bool limitsSet { false };                ///< Whether limits have been set
    ImPlot3DCond limitsCond { ImPlot3DCond_Once };
    QImPlot3DScaleType scale { QImPlot3DScaleType::Linear };
    QImPlot3DNode* plot { nullptr };
    
    // Limits constraints
    double limitsConstraintMin { 0.0 };
    double limitsConstraintMax { 1.0 };
    bool hasLimitsConstraints { false };     ///< Whether limits constraints are set
    
    // Zoom constraints
    double zoomConstraintMin { 0.0 };
    double zoomConstraintMax { 1.0 };
    bool hasZoomConstraints { false };       ///< Whether zoom constraints are set

    // Custom axis transform (lifetime external)
    QImPlot3DAxisTransform* m_axisTransform { nullptr };

    // Tick configuration
    QList<double> tickValues;                ///< Tick position values
    QList<QByteArray> tickLabels;            ///< Tick label strings (UTF-8)
    bool keepDefaultTicks { false };         ///< Whether to keep default ticks
    bool hasTickConfig { false };            ///< Whether tick configuration is set
    bool useRangeTicks { false };            ///< Whether to use range-based tick generation
    double rangeTickMin { 0.0 };             ///< Range tick minimum value
    double rangeTickMax { 1.0 };             ///< Range tick maximum value
    int rangeTickCount { 5 };                ///< Number of ticks in range mode

    // Axis formatter (lifetime external)
    QImPlot3DAxisFormatter* m_axisFormatter { nullptr };
};

QImPlot3DAxisInfo::PrivateData::PrivateData(QImPlot3DAxisInfo* p) : q_ptr(p)
{
}

//===============================================================
// Static C Callbacks for Custom Axis Transform
//===============================================================

/**
 * \if ENGLISH
 * @brief Static C callback for forwarding axis transform values
 * @param value Data value to transform
 * @param data User data pointer (cast to QImPlot3DAxisTransform*)
 * @return Transformed (screen) value
 * \endif
 *
 * \if CHINESE
 * @brief 用于正向轴变换的静态 C 回调函数
 * @param value 待变换的数据值
 * @param data 用户数据指针（转换为 QImPlot3DAxisTransform*）
 * @return 变换后的（屏幕）值
 * \endif
 */
static double axisTransformForward(double value, void* data)
{
    QImPlot3DAxisTransform* transform = static_cast<QImPlot3DAxisTransform*>(data);
    return transform->forward(value);
}

/**
 * \if ENGLISH
 * @brief Static C callback for inverse axis transform values
 * @param value Screen value to inverse-transform
 * @param data User data pointer (cast to QImPlot3DAxisTransform*)
 * @return Inverse-transformed (data) value
 * \endif
 *
 * \if CHINESE
 * @brief 用于反向轴变换的静态 C 回调函数
 * @param value 待反向变换的屏幕值
 * @param data 用户数据指针（转换为 QImPlot3DAxisTransform*）
 * @return 反向变换后的（数据）值
 * \endif
 */
static double axisTransformInverse(double value, void* data)
{
    QImPlot3DAxisTransform* transform = static_cast<QImPlot3DAxisTransform*>(data);
    return transform->inverse(value);
}

//===============================================================
// QImPlot3DAxisInfo
//===============================================================

/**
 * \if ENGLISH
 * @brief Constructor for QImPlot3DAxisInfo
 * @param axis The ImPlot3D axis identifier (X1, Y1, or Z1)
 * @param plot Optional QImPlot3DNode parent for memory management
 * @details Initializes axis with default settings:
 *          - Label: empty string (will use default axis name)
 *          - Limits: [0.0, 1.0] range
 *          - Flags: ImPlot3DAxisFlags_None (all decorations enabled)
 * \endif
 *
 * \if CHINESE
 * @brief QImPlot3DAxisInfo 构造函数
 * @param axis ImPlot3D 轴标识符（X1、Y1 或 Z1）
 * @param plot 可选的 QImPlot3DNode 父对象，用于内存管理
 * @details 初始化轴的默认设置：
 *          - 标签：空字符串（将使用默认轴名称）
 *          - 范围：[0.0, 1.0]
 *          - 标志：ImPlot3DAxisFlags_None（启用所有装饰元素）
 * \endif
 */
QImPlot3DAxisInfo::QImPlot3DAxisInfo(QImPlot3DAxisId axis, QImPlot3DNode* plot)
    : QObject(plot), m_axisId(axis), QIM_PIMPL_CONSTRUCT
{
    QIM_D(d);
    d->plot = plot;
}

QImPlot3DAxisInfo::~QImPlot3DAxisInfo()
{
}

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
QImPlot3DAxisId QImPlot3DAxisInfo::axisId() const
{
    return m_axisId;
}

/**
 * \if ENGLISH
 * @brief Returns the raw ImAxis3D value for direct ImPlot3D API usage
 * @return Integer matching ImAxis3D_ enumeration
 * \endif
 *
 * \if CHINESE
 * @brief 返回原始 ImAxis3D 值，用于直接调用 ImPlot3D API
 * @return 匹配 ImAxis3D_ 枚举的整数
 * \endif
 */
int QImPlot3DAxisInfo::imAxis3D() const
{
    return toImAxis3D(m_axisId);
}

/**
 * \if ENGLISH
 * @brief Returns the parent QImPlot3DNode
 * @return Pointer to the parent plot node, or nullptr if not attached
 * \endif
 *
 * \if CHINESE
 * @brief 返回父级 QImPlot3DNode
 * @return 父级绘图节点指针，如未关联则返回 nullptr
 * \endif
 */
QImPlot3DNode* QImPlot3DAxisInfo::plotNode() const
{
    QIM_DC(d);
    return d->plot;
}

/**
 * \if ENGLISH
 * @brief Gets the axis label text displayed next to the axis
 * @return Current label string (empty string means default axis name is used)
 * \endif
 *
 * \if CHINESE
 * @brief 获取显示在坐标轴旁边的标签文本
 * @return 当前标签字符串（空字符串表示使用默认轴名称）
 * \endif
 */
QString QImPlot3DAxisInfo::label() const
{
    QIM_DC(d);
    return QString::fromUtf8(d->labelUtf8);
}

/**
 * \if ENGLISH
 * @brief Sets the axis label text
 * @param label New label string. Empty string restores default axis name.
 * \endif
 *
 * \if CHINESE
 * @brief 设置坐标轴标签文本
 * @param label 新标签字符串。空字符串将恢复默认轴名称。
 * \endif
 */
void QImPlot3DAxisInfo::setLabel(const QString& label)
{
    QIM_D(d);
    QByteArray newLabelUtf8 = label.toUtf8();
    if (d->labelUtf8 != newLabelUtf8) {
        d->labelUtf8 = newLabelUtf8;
        Q_EMIT labelChanged(label);
    }
}

/**
 * \if ENGLISH
 * @brief Gets the current minimum limit of the axis range
 * @return Minimum value of the axis visible range
 * \endif
 *
 * \if CHINESE
 * @brief 获取坐标轴范围的当前最小限制值
 * @return 坐标轴可见范围的最小值
 * \endif
 */
double QImPlot3DAxisInfo::minLimit() const
{
    QIM_DC(d);
    return d->minLimit;
}

/**
 * \if ENGLISH
 * @brief Sets the minimum limit of the axis range
 * @param min New minimum value for the axis
 * \endif
 *
 * \if CHINESE
 * @brief 设置坐标轴范围的最小限制值
 * @param min 坐标轴的新最小值
 * \endif
 */
void QImPlot3DAxisInfo::setMinLimit(double min)
{
    QIM_D(d);
    if (d->minLimit != min) {
        d->minLimit = min;
        d->limitsSet = true;
        Q_EMIT limitsChanged(d->minLimit, d->maxLimit);
    }
}

/**
 * \if ENGLISH
 * @brief Gets the current maximum limit of the axis range
 * @return Maximum value of the axis visible range
 * \endif
 *
 * \if CHINESE
 * @brief 获取坐标轴范围的当前最大限制值
 * @return 坐标轴可见范围的最大值
 * \endif
 */
double QImPlot3DAxisInfo::maxLimit() const
{
    QIM_DC(d);
    return d->maxLimit;
}

/**
 * \if ENGLISH
 * @brief Sets the maximum limit of the axis range
 * @param max New maximum value for the axis
 * \endif
 *
 * \if CHINESE
 * @brief 设置坐标轴范围的最大限制值
 * @param max 坐标轴的新最大值
 * \endif
 */
void QImPlot3DAxisInfo::setMaxLimit(double max)
{
    QIM_D(d);
    if (d->maxLimit != max) {
        d->maxLimit = max;
        d->limitsSet = true;
        Q_EMIT limitsChanged(d->minLimit, d->maxLimit);
    }
}

/**
 * \if ENGLISH
 * @brief Sets both minimum and maximum limits of the axis range simultaneously
 * @param min New minimum value
 * @param max New maximum value
 * @param cond Application condition (Once or Always)
 * \endif
 *
 * \if CHINESE
 * @brief 同时设置坐标轴范围的最小和最大限制值
 * @param min 新最小值
 * @param max 新最大值
 * @param cond 应用条件（Once 或 Always）
 * \endif
 */
void QImPlot3DAxisInfo::setLimits(double min, double max, QImPlot3DCondition cond)
{
    QIM_D(d);
    bool changed = false;
    if (d->minLimit != min) {
        d->minLimit = min;
        changed = true;
    }
    if (d->maxLimit != max) {
        d->maxLimit = max;
        changed = true;
    }
    if (changed) {
        d->limitsSet = true;
        d->limitsCond = static_cast<ImPlot3DCond>(toImPlot3DCond(cond));
        Q_EMIT limitsChanged(d->minLimit, d->maxLimit);
    }
}

/**
 * \if ENGLISH
 * @brief Gets the current axis scale type
 * @return Current scale type as QImPlot3DScaleType enum value
 * \endif
 *
 * \if CHINESE
 * @brief 获取当前坐标轴刻度类型
 * @return 当前刻度类型，以 QImPlot3DScaleType 枚举值返回
 * \endif
 */
QImPlot3DScaleType QImPlot3DAxisInfo::scale() const
{
    QIM_DC(d);
    return d->scale;
}

/**
 * \if ENGLISH
 * @brief Sets the axis scale type
 * @param type New scale type (Linear, Log10, or SymLog)
 * \endif
 *
 * \if CHINESE
 * @brief 设置坐标轴刻度类型
 * @param type 新刻度类型（Linear、Log10 或 SymLog）
 * \endif
 */
void QImPlot3DAxisInfo::setScale(QImPlot3DScaleType type)
{
    QIM_D(d);
    if (d->scale != type) {
        d->scale = type;
        Q_EMIT scaleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the custom axis transform object
 * @return Pointer to the current QImPlot3DAxisTransform, or nullptr if not set
 * \endif
 *
 * \if CHINESE
 * @brief 返回自定义坐标轴变换对象
 * @return 当前 QImPlot3DAxisTransform 指针，未设置时返回 nullptr
 * \endif
 */
QImPlot3DAxisTransform* QImPlot3DAxisInfo::axisTransform() const
{
    QIM_DC(d);
    return d->m_axisTransform;
}

/**
 * \if ENGLISH
 * @brief Sets a custom axis transform for scale conversion
 * @param transform Pointer to a QImPlot3DAxisTransform implementation (nullptr resets to builtin scale)
 * @note QImPlot3DAxisInfo does NOT own the transform — lifetime must be managed externally.
 *       When a custom transform is set, it overrides the builtin scale type (Linear/Log10/SymLog).
 * \endif
 *
 * \if CHINESE
 * @brief 设置自定义坐标轴变换用于刻度转换
 * @param transform QImPlot3DAxisTransform 实现的指针（nullptr 重置为内置刻度）
 * @note QImPlot3DAxisInfo 不拥有变换对象 — 生命周期必须由外部管理。
 *       设置自定义变换时，将覆盖内置刻度类型（Linear/Log10/SymLog）。
 * \endif
 */
void QImPlot3DAxisInfo::setAxisTransform(QImPlot3DAxisTransform* transform)
{
    QIM_D(d);
    if (d->m_axisTransform != transform) {
        d->m_axisTransform = transform;
        Q_EMIT axisTransformChanged(transform);
    }
}

/**
 * \if ENGLISH
 * @brief Returns the raw ImPlot3DAxisFlags value
 * @return ImPlot3DAxisFlags value as integer
 * \endif
 *
 * \if CHINESE
 * @brief 返回原始 ImPlot3DAxisFlags 值
 * @return ImPlot3DAxisFlags 值作为整数返回
 * \endif
 */
int QImPlot3DAxisInfo::axisFlags() const
{
    QIM_DC(d);
    return d->flags;
}

/**
 * \if ENGLISH
 * @brief Sets the ImPlot3DAxisFlags value directly
 * @param flags New flags value as integer
 * \endif
 *
 * \if CHINESE
 * @brief 直接设置 ImPlot3DAxisFlags 值
 * @param flags 作为整数的新标志值
 * \endif
 */
void QImPlot3DAxisInfo::setAxisFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast<ImPlot3DAxisFlags>(flags);
        Q_EMIT axisFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Checks if axis is visible (decorations enabled)
 * @return true if axis is enabled
 * \endif
 *
 * \if CHINESE
 * @brief 检查坐标轴是否可见（装饰元素启用）
 * @return true 表示坐标轴启用
 * \endif
 */
bool QImPlot3DAxisInfo::isEnabled() const
{
    return isDecorationsEnabled();
}

/**
 * \if ENGLISH
 * @brief Sets axis visibility
 * @param on true to enable axis, false to hide via NoDecorations
 * \endif
 *
 * \if CHINESE
 * @brief 设置坐标轴可见性
 * @param on true 启用坐标轴，false 通过 NoDecorations 隐藏
 * \endif
 */
void QImPlot3DAxisInfo::setEnabled(bool on)
{
    setDecorationsEnabled(on);
}

// ===== Flag Accessors using Macros =====
// Negative to Positive semantic (NoXxx -> xxxEnabled)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DAxisInfo, LabelEnabled, ImPlot3DAxisFlags_NoLabel, axisFlagChanged)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DAxisInfo, GridLinesEnabled, ImPlot3DAxisFlags_NoGridLines, axisFlagChanged)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DAxisInfo, TickMarksEnabled, ImPlot3DAxisFlags_NoTickMarks, axisFlagChanged)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DAxisInfo, TickLabelsEnabled, ImPlot3DAxisFlags_NoTickLabels, axisFlagChanged)

// Positive to Positive semantic (direct mapping)
QIMPLOT3D_FLAG_ACCESSOR(QImPlot3DAxisInfo, LockMin, ImPlot3DAxisFlags_LockMin, axisFlagChanged)
QIMPLOT3D_FLAG_ACCESSOR(QImPlot3DAxisInfo, LockMax, ImPlot3DAxisFlags_LockMax, axisFlagChanged)
QIMPLOT3D_FLAG_ACCESSOR(QImPlot3DAxisInfo, AutoFit, ImPlot3DAxisFlags_AutoFit, axisFlagChanged)
QIMPLOT3D_FLAG_ACCESSOR(QImPlot3DAxisInfo, Invert, ImPlot3DAxisFlags_Invert, axisFlagChanged)
QIMPLOT3D_FLAG_ACCESSOR(QImPlot3DAxisInfo, PanStretch, ImPlot3DAxisFlags_PanStretch, axisFlagChanged)

// ===== Combined Flags - Manual Implementation =====

/**
 * \if ENGLISH
 * @brief Checks if both min and max values are locked
 * @return true if both LockMin and LockMax flags are set
 * \endif
 *
 * \if CHINESE
 * @brief 检查最小和最大值是否同时锁定
 * @return true 表示同时设置了 LockMin 和 LockMax 标志
 * \endif
 */
bool QImPlot3DAxisInfo::isLock() const
{
    QIM_DC(d);
    return (d->flags & ImPlot3DAxisFlags_Lock) == ImPlot3DAxisFlags_Lock;
}

/**
 * \if ENGLISH
 * @brief Locks/unlocks both min and max axis values simultaneously
 * @param on true to lock both ends, false to unlock
 * \endif
 *
 * \if CHINESE
 * @brief 同时锁定/解锁最小和最大轴值
 * @param on true 锁定两端，false 解锁
 * \endif
 */
void QImPlot3DAxisInfo::setLock(bool on)
{
    QIM_D(d);
    const ImPlot3DAxisFlags oldFlags = d->flags;
    if (on) {
        d->flags |= ImPlot3DAxisFlags_Lock;
    } else {
        d->flags &= ~ImPlot3DAxisFlags_Lock;
    }
    if (d->flags != oldFlags) {
        Q_EMIT axisFlagChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Checks if all decorative elements are visible (label, grid, tick labels)
 * @return true if decorations are enabled (NoDecorations NOT set)
 * @note NoDecorations = NoLabel | NoGridLines | NoTickLabels
 * \endif
 *
 * \if CHINESE
 * @brief 检查所有装饰元素是否可见（标签、网格、刻度标签）
 * @return true 表示装饰元素启用（未设置 NoDecorations）
 * @note NoDecorations = NoLabel | NoGridLines | NoTickLabels
 * \endif
 */
bool QImPlot3DAxisInfo::isDecorationsEnabled() const
{
    QIM_DC(d);
    return (d->flags & ImPlot3DAxisFlags_NoDecorations) == 0;
}

/**
 * \if ENGLISH
 * @brief Enables/disables all decorative elements (label, grid, tick labels)
 * @param enabled true to show all decorations, false to hide
 * \endif
 *
 * \if CHINESE
 * @brief 启用/禁用所有装饰元素（标签、网格、刻度标签）
 * @param enabled true 显示所有装饰，false 隐藏
 * \endif
 */
void QImPlot3DAxisInfo::setDecorationsEnabled(bool enabled)
{
    QIM_D(d);
    const ImPlot3DAxisFlags oldFlags = d->flags;
    if (enabled) {
        d->flags &= ~ImPlot3DAxisFlags_NoDecorations;
    } else {
        d->flags |= ImPlot3DAxisFlags_NoDecorations;
    }
    if (d->flags != oldFlags) {
        Q_EMIT axisFlagChanged();
    }
}

// ===== Limits Constraint Methods =====

/**
 * \if ENGLISH
 * @brief Gets the minimum limits constraint value
 * @return Minimum constraint value for axis limits
 * \endif
 *
 * \if CHINESE
 * @brief 获取范围限制约束的最小值
 * @return 坐标轴范围限制的最小约束值
 * \endif
 */
double QImPlot3DAxisInfo::limitsConstraintMin() const
{
    QIM_DC(d);
    return d->limitsConstraintMin;
}

/**
 * \if ENGLISH
 * @brief Sets the minimum limits constraint value
 * @param min New minimum constraint value
 * \endif
 *
 * \if CHINESE
 * @brief 设置范围限制约束的最小值
 * @param min 新的最小约束值
 * \endif
 */
void QImPlot3DAxisInfo::setLimitsConstraintMin(double min)
{
    QIM_D(d);
    if (d->limitsConstraintMin != min) {
        d->limitsConstraintMin = min;
        d->hasLimitsConstraints = true;
        Q_EMIT limitsConstraintsChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Gets the maximum limits constraint value
 * @return Maximum constraint value for axis limits
 * \endif
 *
 * \if CHINESE
 * @brief 获取范围限制约束的最大值
 * @return 坐标轴范围限制的最大约束值
 * \endif
 */
double QImPlot3DAxisInfo::limitsConstraintMax() const
{
    QIM_DC(d);
    return d->limitsConstraintMax;
}

/**
 * \if ENGLISH
 * @brief Sets the maximum limits constraint value
 * @param max New maximum constraint value
 * \endif
 *
 * \if CHINESE
 * @brief 设置范围限制约束的最大值
 * @param max 新的最大约束值
 * \endif
 */
void QImPlot3DAxisInfo::setLimitsConstraintMax(double max)
{
    QIM_D(d);
    if (d->limitsConstraintMax != max) {
        d->limitsConstraintMax = max;
        d->hasLimitsConstraints = true;
        Q_EMIT limitsConstraintsChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Sets both minimum and maximum limits constraints simultaneously
 * @param min New minimum constraint value
 * @param max New maximum constraint value
 * \endif
 *
 * \if CHINESE
 * @brief 同时设置范围限制约束的最小和最大值
 * @param min 新的最小约束值
 * @param max 新的最大约束值
 * \endif
 */
void QImPlot3DAxisInfo::setLimitsConstraints(double min, double max)
{
    QIM_D(d);
    bool changed = false;
    if (d->limitsConstraintMin != min) {
        d->limitsConstraintMin = min;
        changed = true;
    }
    if (d->limitsConstraintMax != max) {
        d->limitsConstraintMax = max;
        changed = true;
    }
    if (changed) {
        d->hasLimitsConstraints = true;
        Q_EMIT limitsConstraintsChanged();
    }
}

// ===== Zoom Constraint Methods =====

/**
 * \if ENGLISH
 * @brief Gets the minimum zoom constraint value
 * @return Minimum constraint value for axis zoom (range size)
 * \endif
 *
 * \if CHINESE
 * @brief 获取缩放约束的最小值
 * @return 坐标轴缩放（范围大小）的最小约束值
 * \endif
 */
double QImPlot3DAxisInfo::zoomConstraintMin() const
{
    QIM_DC(d);
    return d->zoomConstraintMin;
}

/**
 * \if ENGLISH
 * @brief Sets the minimum zoom constraint value
 * @param min New minimum zoom constraint value
 * \endif
 *
 * \if CHINESE
 * @brief 设置缩放约束的最小值
 * @param min 新的最小缩放约束值
 * \endif
 */
void QImPlot3DAxisInfo::setZoomConstraintMin(double min)
{
    QIM_D(d);
    if (d->zoomConstraintMin != min) {
        d->zoomConstraintMin = min;
        d->hasZoomConstraints = true;
        Q_EMIT zoomConstraintsChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Gets the maximum zoom constraint value
 * @return Maximum constraint value for axis zoom (range size)
 * \endif
 *
 * \if CHINESE
 * @brief 获取缩放约束的最大值
 * @return 坐标轴缩放（范围大小）的最大约束值
 * \endif
 */
double QImPlot3DAxisInfo::zoomConstraintMax() const
{
    QIM_DC(d);
    return d->zoomConstraintMax;
}

/**
 * \if ENGLISH
 * @brief Sets the maximum zoom constraint value
 * @param max New maximum zoom constraint value
 * \endif
 *
 * \if CHINESE
 * @brief 设置缩放约束的最大值
 * @param max 新的最大缩放约束值
 * \endif
 */
void QImPlot3DAxisInfo::setZoomConstraintMax(double max)
{
    QIM_D(d);
    if (d->zoomConstraintMax != max) {
        d->zoomConstraintMax = max;
        d->hasZoomConstraints = true;
        Q_EMIT zoomConstraintsChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Sets both minimum and maximum zoom constraints simultaneously
 * @param min New minimum zoom constraint value
 * @param max New maximum zoom constraint value
 * \endif
 *
 * \if CHINESE
 * @brief 同时设置缩放约束的最小和最大值
 * @param min 新的最小缩放约束值
 * @param max 新的最大缩放约束值
 * \endif
 */
void QImPlot3DAxisInfo::setZoomConstraints(double min, double max)
{
    QIM_D(d);
    bool changed = false;
    if (d->zoomConstraintMin != min) {
        d->zoomConstraintMin = min;
        changed = true;
    }
    if (d->zoomConstraintMax != max) {
        d->zoomConstraintMax = max;
        changed = true;
    }
    if (changed) {
        d->hasZoomConstraints = true;
        Q_EMIT zoomConstraintsChanged();
    }
}

// ===== Axis Formatter Methods =====

/**
 * \if ENGLISH
 * @brief Gets the current axis formatter for custom tick label formatting
 * @return Pointer to the current QImPlot3DAxisFormatter, or nullptr if no formatter is set
 * @note The formatter is NOT owned by QImPlot3DAxisInfo. The caller must ensure
 *       the formatter remains alive during rendering.
 * \endif
 *
 * \if CHINESE
 * @brief 获取当前坐标轴格式化器，用于自定义刻度标签格式化
 * @return 当前 QImPlot3DAxisFormatter 指针，若未设置则返回 nullptr
 * @note 格式化器不由 QImPlot3DAxisInfo 管理。调用者必须确保格式化器在渲染期间保持存活。
 * \endif
 */
QImPlot3DAxisFormatter* QImPlot3DAxisInfo::axisFormatter() const
{
    QIM_DC(d);
    return d->m_axisFormatter;
}

/**
 * \if ENGLISH
 * @brief Sets the axis formatter for custom tick label formatting
 * @param formatter Pointer to a QImPlot3DAxisFormatter subclass instance.
 *        Pass nullptr to remove the formatter and revert to default formatting.
 * @note The formatter is NOT owned by QImPlot3DAxisInfo. The caller must manage
 *       the formatter's lifetime and ensure it remains valid during plot rendering.
 * @warning If the formatter is deleted before the plot renders, undefined behavior
 *          will occur because ImPlot3D uses the formatter pointer as callback data.
 * \endif
 *
 * \if CHINESE
 * @brief 设置坐标轴格式化器，用于自定义刻度标签格式化
 * @param formatter QImPlot3DAxisFormatter 子类实例指针。
 *        传入 nullptr 可移除格式化器并恢复默认格式化。
 * @note 格式化器不由 QImPlot3DAxisInfo 管理。调用者必须管理格式化器的生命周期，
 *       并确保其在绘图渲染期间保持有效。
 * @warning 若格式化器在绘图渲染之前被删除，将导致未定义行为，
 *          因为 ImPlot3D 将格式化器指针作为回调数据使用。
 * \endif
 */
void QImPlot3DAxisInfo::setAxisFormatter(QImPlot3DAxisFormatter* formatter)
{
    QIM_D(d);
    if (d->m_axisFormatter != formatter) {
        d->m_axisFormatter = formatter;
        Q_EMIT axisFormatterChanged();
    }
}

// ===== Tick Configuration Methods =====

/**
 * \if ENGLISH
 * @brief Gets the list of tick position values
 * @return List of double values specifying tick positions on the axis
 * \endif
 *
 * \if CHINESE
 * @brief 获取刻度位置值列表
 * @return 指定坐标轴上刻度位置的 double 值列表
 * \endif
 */
QList<double> QImPlot3DAxisInfo::tickValues() const
{
    QIM_DC(d);
    return d->tickValues;
}

/**
 * \if ENGLISH
 * @brief Sets the list of tick position values
 * @param values List of double values specifying tick positions on the axis
 * \endif
 *
 * \if CHINESE
 * @brief 设置刻度位置值列表
 * @param values 指定坐标轴上刻度位置的 double 值列表
 * \endif
 */
void QImPlot3DAxisInfo::setTickValues(const QList<double>& values)
{
    QIM_D(d);
    if (d->tickValues != values) {
        d->tickValues = values;
        d->useRangeTicks = false;
        d->hasTickConfig = true;
        Q_EMIT tickConfigChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Gets the list of tick label strings (UTF-8 encoded)
 * @return List of QByteArray tick labels, empty list means no custom labels
 * \endif
 *
 * \if CHINESE
 * @brief 获取刻度标签字符串列表（UTF-8 编码）
 * @return QByteArray 刻度标签列表，空列表表示无自定义标签
 * \endif
 */
QList<QByteArray> QImPlot3DAxisInfo::tickLabels() const
{
    QIM_DC(d);
    return d->tickLabels;
}

/**
 * \if ENGLISH
 * @brief Sets the list of tick label strings (UTF-8 encoded)
 * @param labels List of QByteArray tick labels, empty list means no custom labels
 * \endif
 *
 * \if CHINESE
 * @brief 设置刻度标签字符串列表（UTF-8 编码）
 * @param labels QByteArray 刻度标签列表，空列表表示无自定义标签
 * \endif
 */
void QImPlot3DAxisInfo::setTickLabels(const QList<QByteArray>& labels)
{
    QIM_D(d);
    if (d->tickLabels != labels) {
        d->tickLabels = labels;
        d->hasTickConfig = true;
        Q_EMIT tickConfigChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Checks if default ticks should be kept alongside custom ticks
 * @return true if default ticks are kept, false if custom ticks replace them
 * \endif
 *
 * \if CHINESE
 * @brief 检查是否在自定义刻度之外保留默认刻度
 * @return true 表示保留默认刻度，false 表示自定义刻度替换默认刻度
 * \endif
 */
bool QImPlot3DAxisInfo::isKeepDefaultTicks() const
{
    QIM_DC(d);
    return d->keepDefaultTicks;
}

/**
 * \if ENGLISH
 * @brief Sets whether default ticks should be kept alongside custom ticks
 * @param keep true to keep default ticks, false to replace with custom ticks only
 * \endif
 *
 * \if CHINESE
 * @brief 设置是否在自定义刻度之外保留默认刻度
 * @param keep true 保留默认刻度，false 仅使用自定义刻度
 * \endif
 */
void QImPlot3DAxisInfo::setKeepDefaultTicks(bool keep)
{
    QIM_D(d);
    if (d->keepDefaultTicks != keep) {
        d->keepDefaultTicks = keep;
        d->hasTickConfig = true;
        Q_EMIT tickConfigChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Convenience method to set custom tick values with optional labels
 * @param values List of tick position values
 * @param labels Optional list of tick labels (UTF-8), empty means no custom labels
 * @param keepDefault Whether to keep default ticks alongside custom ones
 * \endif
 *
 * \if CHINESE
 * @brief 设置自定义刻度值及可选标签的便捷方法
 * @param values 刻度位置值列表
 * @param labels 可选的刻度标签列表（UTF-8），空列表表示无自定义标签
 * @param keepDefault 是否在自定义刻度之外保留默认刻度
 * \endif
 */
void QImPlot3DAxisInfo::setAxisTicks(const QList<double>& values, const QList<QByteArray>& labels, bool keepDefault)
{
    QIM_D(d);
    d->tickValues = values;
    d->tickLabels = labels;
    d->keepDefaultTicks = keepDefault;
    d->useRangeTicks = false;
    d->hasTickConfig = true;
    Q_EMIT tickConfigChanged();
}

/**
 * \if ENGLISH
 * @brief Convenience method to set ticks using a range and count
 * @param v_min Minimum value of the tick range
 * @param v_max Maximum value of the tick range
 * @param n_ticks Number of evenly-spaced ticks in the range
 * @param labels Optional list of tick labels (UTF-8), empty means no custom labels
 * @param keepDefault Whether to keep default ticks alongside custom ones
 * \endif
 *
 * \if CHINESE
 * @brief 使用范围和数量设置刻度的便捷方法
 * @param v_min 刻度范围的最小值
 * @param v_max 刻度范围的最大值
 * @param n_ticks 范围内均匀分布的刻度数量
 * @param labels 可选的刻度标签列表（UTF-8），空列表表示无自定义标签
 * @param keepDefault 是否在自定义刻度之外保留默认刻度
 * \endif
 */
void QImPlot3DAxisInfo::setAxisTicksRange(double v_min, double v_max, int n_ticks, const QList<QByteArray>& labels, bool keepDefault)
{
    QIM_D(d);
    d->rangeTickMin = v_min;
    d->rangeTickMax = v_max;
    d->rangeTickCount = n_ticks;
    d->tickLabels = labels;
    d->keepDefaultTicks = keepDefault;
    d->useRangeTicks = true;
    d->hasTickConfig = true;
    Q_EMIT tickConfigChanged();
}

/**
 * \if ENGLISH
 * @brief Applies all stored configuration to ImPlot3D via Setup API calls
 * @details Calls SetupAxis() with label and flags, SetupAxisLimits() if limits were set,
 *          SetupAxisScale() with custom transform if set or builtin scale if non-linear,
 *          SetupAxisLimitsConstraints() / SetupAxisZoomConstraints() if constraints are configured,
 *          SetupAxisFormat() if a custom formatter is set, and SetupAxisTicks() if tick config exists.
 *          When a custom axisTransform is set, it overrides the builtin scale type.
 * \endif
 *
 * \if CHINESE
 * @brief 通过 Setup API 将所有存储的配置应用到 ImPlot3D
 * @details 使用标签和标志调用 SetupAxis()，如设置了范围则调用 SetupAxisLimits()，
 *          如设置了自定义变换或刻度非线性则调用 SetupAxisScale()，
 *          如配置了约束则调用 SetupAxisLimitsConstraints() / SetupAxisZoomConstraints()，
 *          如设置了自定义格式化器则调用 SetupAxisFormat()，
 *          如配置了刻度则调用 SetupAxisTicks()。
 *          设置自定义 axisTransform 时，将覆盖内置刻度类型。
 * \endif
 */
void QImPlot3DAxisInfo::applySetup() const
{
    QIM_DC(d);
    ImAxis3D imAxis = static_cast<ImAxis3D>(toImAxis3D(m_axisId));

    // Setup axis with label and flags
    const char* labelPtr = d->labelUtf8.isEmpty() ? nullptr : d->labelUtf8.constData();
    ImPlot3D::SetupAxis(imAxis, labelPtr, d->flags);

    // Setup limits if they were explicitly set
    if (d->limitsSet) {
        ImPlot3D::SetupAxisLimits(imAxis, d->minLimit, d->maxLimit, d->limitsCond);
    }

    // Setup scale: custom transform overrides builtin scale
    if (d->m_axisTransform != nullptr) {
        ImPlot3D::SetupAxisScale(imAxis, axisTransformForward, axisTransformInverse,
                                  static_cast<void*>(d->m_axisTransform));
    } else if (d->scale != QImPlot3DScaleType::Linear) {
        ImPlot3D::SetupAxisScale(imAxis, static_cast<ImPlot3DScale>(toImPlot3DScale(d->scale)));
    }

    // Setup limits constraints if configured
    if (d->hasLimitsConstraints) {
        ImPlot3D::SetupAxisLimitsConstraints(imAxis, d->limitsConstraintMin, d->limitsConstraintMax);
    }

    // Setup zoom constraints if configured
    if (d->hasZoomConstraints) {
        ImPlot3D::SetupAxisZoomConstraints(imAxis, d->zoomConstraintMin, d->zoomConstraintMax);
    }

    // Setup axis format if custom formatter is set
    if (d->m_axisFormatter) {
        // Bridge C callback that converts QImPlot3DAxisFormatter::format() to ImPlot3DFormatter
        // ImPlot3DFormatter signature: int (*)(double value, char* buff, int size, void* user_data)
        // Returns number of bytes written to buff (excluding null terminator)
        static const auto axisFormatterCallback =
            [](double value, char* buff, int size, void* user_data) -> int {
                QImPlot3DAxisFormatter* formatter = static_cast<QImPlot3DAxisFormatter*>(user_data);
                QByteArray result = formatter->format(value, QByteArray());
                int copyLen = qMin(size - 1, static_cast<int>(result.size()));
                if (copyLen > 0) {
                    memcpy(buff, result.constData(), copyLen);
                }
                buff[copyLen] = '\0';
                return copyLen;
            };
        ImPlot3D::SetupAxisFormat(imAxis, axisFormatterCallback, static_cast<void*>(d->m_axisFormatter));
    }

    // Setup axis ticks if configured
    if (d->hasTickConfig) {
        if (d->useRangeTicks) {
            // Range-based tick generation: v_min, v_max, n_ticks
            const char* const* labelsPtr = nullptr;
            std::vector<const char*> labelPtrs;
            if (!d->tickLabels.isEmpty()) {
                labelPtrs.reserve(d->tickLabels.size());
                for (const QByteArray& lbl : d->tickLabels) {
                    labelPtrs.push_back(lbl.constData());
                }
                labelsPtr = labelPtrs.data();
            }
            ImPlot3D::SetupAxisTicks(imAxis, d->rangeTickMin, d->rangeTickMax, d->rangeTickCount, labelsPtr, d->keepDefaultTicks);
        } else {
            // Explicit tick values
            std::vector<double> valuesVec(d->tickValues.begin(), d->tickValues.end());
            const char* const* labelsPtr = nullptr;
            std::vector<const char*> labelPtrs;
            if (!d->tickLabels.isEmpty()) {
                labelPtrs.reserve(d->tickLabels.size());
                for (const QByteArray& lbl : d->tickLabels) {
                    labelPtrs.push_back(lbl.constData());
                }
                labelsPtr = labelPtrs.data();
            }
            ImPlot3D::SetupAxisTicks(imAxis, valuesVec.data(), static_cast<int>(valuesVec.size()), labelsPtr, d->keepDefaultTicks);
        }
    }
}

}  // namespace QIM