#include "QImPlot3DAxisInfo.h"
#include "implot3d.h"
#include "QImPlot3DExtNode.h"

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
    QImPlot3DExtNode* plot { nullptr };
};

QImPlot3DAxisInfo::PrivateData::PrivateData(QImPlot3DAxisInfo* p) : q_ptr(p)
{
}

//===============================================================
// QImPlot3DAxisInfo
//===============================================================

/**
 * \if ENGLISH
 * @brief Constructor for QImPlot3DAxisInfo
 * @param axis The ImPlot3D axis identifier (X1, Y1, or Z1)
 * @param plot Optional QImPlot3DExtNode parent for memory management
 * @details Initializes axis with default settings:
 *          - Label: empty string (will use default axis name)
 *          - Limits: [0.0, 1.0] range
 *          - Flags: ImPlot3DAxisFlags_None (all decorations enabled)
 * \endif
 *
 * \if CHINESE
 * @brief QImPlot3DAxisInfo 构造函数
 * @param axis ImPlot3D 轴标识符（X1、Y1 或 Z1）
 * @param plot 可选的 QImPlot3DExtNode 父对象，用于内存管理
 * @details 初始化轴的默认设置：
 *          - 标签：空字符串（将使用默认轴名称）
 *          - 范围：[0.0, 1.0]
 *          - 标志：ImPlot3DAxisFlags_None（启用所有装饰元素）
 * \endif
 */
QImPlot3DAxisInfo::QImPlot3DAxisInfo(QImPlot3DAxisId axis, QImPlot3DExtNode* plot)
    : QObject(plot), m_axisId(axis), QIM_PIMPL_CONSTRUCT
{
    d_ptr->plot = plot;
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
 * @brief Returns the parent QImPlot3DExtNode
 * @return Pointer to the parent plot node, or nullptr if not attached
 * \endif
 *
 * \if CHINESE
 * @brief 返回父级 QImPlot3DExtNode
 * @return 父级绘图节点指针，如未关联则返回 nullptr
 * \endif
 */
QImPlot3DExtNode* QImPlot3DAxisInfo::plotNode() const
{
    return d_ptr->plot;
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
    return QString::fromUtf8(d_ptr->labelUtf8);
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
    return d_ptr->minLimit;
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
    return d_ptr->maxLimit;
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
    return d_ptr->scale;
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
    return d_ptr->flags;
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

/**
 * \if ENGLISH
 * @brief Applies all stored configuration to ImPlot3D via Setup API calls
 * @details Calls SetupAxis() with label and flags, SetupAxisLimits() if limits were set,
 *          and SetupAxisScale() if scale is non-linear.
 * \endif
 *
 * \if CHINESE
 * @brief 通过 Setup API 将所有存储的配置应用到 ImPlot3D
 * @details 使用标签和标志调用 SetupAxis()，如设置了范围则调用 SetupAxisLimits()，
 *          如刻度非线性则调用 SetupAxisScale()。
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

    // Setup scale if non-linear
    if (d->scale != QImPlot3DScaleType::Linear) {
        ImPlot3D::SetupAxisScale(imAxis, static_cast<ImPlot3DScale>(toImPlot3DScale(d->scale)));
    }
}

}  // namespace QIM

