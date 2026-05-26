#include "QImPlot3DNode.h"
#include "QImPlot3DAxisInfo.h"
#include "QImPlot3DItemNode.h"
#include "QImPlot3DMeshItemNode.h"
#include "QImPlot3DStyleNode.h"
#include "QtImGuiUtils.h"
#include "implot3d.h"

namespace QIM
{

//===============================================================
// PrivateData Implementation
//===============================================================

class QImPlot3DNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlot3DNode)

public:
    PrivateData(QImPlot3DNode* q);

public:
    // UTF-8 cache (avoid conversion during rendering)
    QByteArray titleUtf8;  ///< UTF-8 encoded title (no QString storage)

    // Size (pre-converted from QSizeF, used directly in beginDraw)
    ImVec2 sizeVec4 {-1, -1};  ///< Pre-converted size for BeginPlot (-1 = auto-fill)
    bool autoSize {true};      ///< Auto-size mode flag (true = use -1 for auto-fill)

    // Flags - MUST be named 'flags' for macro access via QIM_D(d)->flags
    ImPlot3DFlags flags {ImPlot3DFlags_None};

    // BeginPlot success flag
    bool beginPlotSuccess {false};

    // 3 Axis info objects (X, Y, Z)
    QImPlot3DAxisInfo* xAxisInfo {nullptr};
    QImPlot3DAxisInfo* yAxisInfo {nullptr};
    QImPlot3DAxisInfo* zAxisInfo {nullptr};

    // 3D rotation parameters
    double elevation {35.264};              ///< Elevation angle in degrees
    double azimuth {45.0};                ///< Azimuth angle in degrees
    bool animateRotation {false};      ///< Whether to animate rotation
    ImPlot3DCond rotationCond {ImPlot3DCond_Once};  ///< Rotation condition
    bool initialRotationSet {false};   ///< Whether initial rotation is set
    double initialElevation {35.264};       ///< Initial elevation for double-click reset
    double initialAzimuth {45.0};         ///< Initial azimuth for double-click reset

    // Quaternion rotation parameters
    QQuaternion rotationQuat;               ///< Quaternion rotation (alternative to elev/azim)
    QQuaternion initialRotationQuat;        ///< Initial quaternion rotation for double-click reset
    bool useQuaternionRotation {false};     ///< Whether to use quaternion rotation instead of elev/azim
    bool initialQuaternionRotationSet {false}; ///< Whether initial quaternion rotation is set

    // Box scale parameters
    double boxScaleX {1.0};            ///< X-axis box scale
    double boxScaleY {1.0};            ///< Y-axis box scale
    double boxScaleZ {1.0};            ///< Z-axis box scale

    // Setup axes convenience method state
    bool hasSetupAxes {false};         ///< Whether setupAxes() was called
    int xAxisFlags {0};                ///< X-axis flags for SetupAxis
    int yAxisFlags {0};                ///< Y-axis flags for SetupAxis
    int zAxisFlags {0};                ///< Z-axis flags for SetupAxis
    QByteArray xAxisLabelUtf8;         ///< UTF-8 encoded X-axis label
    QByteArray yAxisLabelUtf8;         ///< UTF-8 encoded Y-axis label
    QByteArray zAxisLabelUtf8;         ///< UTF-8 encoded Z-axis label

    // Legend configuration
    QImPlot3DLocation m_legendLocation {QImPlot3DLocation::NorthWest}; ///< Legend location
    int m_legendFlags {0};             ///< Legend flags (QImPlot3DLegendFlags as int)
    bool m_hasLegendConfig {false};    ///< Whether legend configuration was explicitly set

    // Colormap push tracking
    QList<QByteArray> m_pushedColormapNames; ///< Names of pushed colormaps (for named push)
    QList<QImPlot3DColormap> m_pushedColormaps; ///< Enum values of pushed colormaps (for enum push)
    int m_pushedColormapCount {0};     ///< Total number of colormap pushes to pop

    // Style node
    QImPlot3DStyleNode* m_styleNode {nullptr}; ///< Style node for this plot
};

QImPlot3DNode::PrivateData::PrivateData(QImPlot3DNode* q) : q_ptr(q)
{
    // Default title
    titleUtf8 = "##Plot3D";

    // Create 3 axis info objects (X1, Y1, Z1)
    xAxisInfo = new QImPlot3DAxisInfo(QImPlot3DAxisId::X1, q);
    yAxisInfo = new QImPlot3DAxisInfo(QImPlot3DAxisId::Y1, q);
    zAxisInfo = new QImPlot3DAxisInfo(QImPlot3DAxisId::Z1, q);

    // Create style node
    m_styleNode = new QImPlot3DStyleNode(q);
}

//===============================================================
// QImPlot3DNode Implementation
//===============================================================

/**
 * \if ENGLISH
 * @brief Constructs a QImPlot3DNode with optional parent
 * @param[in] parent Optional parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief 构造一个带有可选父对象的 QImPlot3DNode
 * @param[in] parent 可选的父 QObject
 * \endif
 */
QImPlot3DNode::QImPlot3DNode(QObject* parent)
    : QImAbstractNode(parent)
    , QIM_PIMPL_CONSTRUCT
{
    setObjectName(QStringLiteral("Plot3DNode"));
}

/**
 * \if ENGLISH
 * @brief Constructs a QImPlot3DNode with title and optional parent
 * @param[in] title Plot title text
 * @param[in] parent Optional parent QObject
 * \endif
 *
 * \if CHINESE
 * @brief 构造一个带有标题和可选父对象的 QImPlot3DNode
 * @param[in] title 绘图标题文本
 * @param[in] parent 可选的父 QObject
 * \endif
 */
QImPlot3DNode::QImPlot3DNode(const QString& title, QObject* parent)
    : QImPlot3DNode(parent)
{
    setTitle(title);
}

/**
 * \if ENGLISH
 * @brief Destroys the QImPlot3DNode
 * \endif
 *
 * \if CHINESE
 * @brief 销毁 QImPlot3DNode
 * \endif
 */
QImPlot3DNode::~QImPlot3DNode()
{
}

//----------------------------------------------------
// Title
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Gets the plot title text
 * @return Plot title as QString
 * \endif
 *
 * \if CHINESE
 * @brief 获取绘图标题文本
 * @return 绘图标题（QString 类型）
 * \endif
 */
QString QImPlot3DNode::title() const
{
    QIM_DC(d);
    return QString::fromUtf8(d->titleUtf8);
}

/**
 * \if ENGLISH
 * @brief Sets the plot title text
 * @param[in] title New title text
 * \endif
 *
 * \if CHINESE
 * @brief 设置绘图标题文本
 * @param[in] title 新标题文本
 * \endif
 */
void QImPlot3DNode::setTitle(const QString& title)
{
    QIM_D(d);
    QByteArray newUtf8 = title.toUtf8();
    if (d->titleUtf8 != newUtf8) {
        d->titleUtf8 = newUtf8;
        Q_EMIT titleChanged(title);
    }
}

//----------------------------------------------------
// Size
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Gets the plot frame size
 * @return Plot size as QSizeF
 * \endif
 *
 * \if CHINESE
 * @brief 获取绘图框尺寸
 * @return 绘图尺寸（QSizeF 类型）
 * \endif
 */
QSizeF QImPlot3DNode::size() const
{
    QIM_DC(d);
    return QSizeF(d->sizeVec4.x, d->sizeVec4.y);
}

/**
 * \if ENGLISH
 * @brief Sets the plot frame size
 * @param[in] size New plot size (negative values for auto-size)
 * \endif
 *
 * \if CHINESE
 * @brief 设置绘图框尺寸
 * @param[in] size 新的绘图尺寸（负值表示自动尺寸）
 * \endif
 */
void QImPlot3DNode::setSize(const QSizeF& size)
{
    QIM_D(d);
    ImVec2 newSize(static_cast<float>(size.width()), static_cast<float>(size.height()));

    // Special handling: QSizeF(-1,-1) should convert to ImVec2(-1,-1)
    if (size.width() < 0) {
        newSize.x = -1.0f;
    }
    if (size.height() < 0) {
        newSize.y = -1.0f;  // -1 means auto-size for height in ImPlot3D
    }

    if (d->sizeVec4.x != newSize.x || d->sizeVec4.y != newSize.y) {
        d->sizeVec4 = newSize;
        Q_EMIT sizeChanged(size);
    }
}

//----------------------------------------------------
// Auto-size
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Checks if auto-size mode is enabled
 * @return true if auto-size is enabled
 * \endif
 *
 * \if CHINESE
 * @brief 检查自动尺寸模式是否启用
 * @return true 表示自动尺寸已启用
 * \endif
 */
bool QImPlot3DNode::isAutoSize() const
{
    QIM_DC(d);
    return d->autoSize;
}

/**
 * \if ENGLISH
 * @brief Enables or disables auto-size mode
 * @param[in] enabled true to use auto-fill size, false to use manual size
 * \endif
 *
 * \if CHINESE
 * @brief 启用或禁用自动尺寸模式
 * @param[in] enabled true 使用自动填充尺寸，false 使用手动尺寸
 * \endif
 */
void QImPlot3DNode::setAutoSize(bool enabled)
{
    QIM_D(d);
    if (d->autoSize != enabled) {
        d->autoSize = enabled;
        Q_EMIT autoSizeChanged(enabled);
    }
}

//----------------------------------------------------
// Axis management
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Gets axis info by axis ID
 * @param[in] aid Axis ID (X1, Y1, or Z1)
 * @return Pointer to QImPlot3DAxisInfo, or nullptr if invalid axis ID
 * \endif
 *
 * \if CHINESE
 * @brief 根据坐标轴 ID 获取坐标轴信息
 * @param[in] aid 坐标轴 ID（X1、Y1 或 Z1）
 * @return QImPlot3DAxisInfo 指针，无效坐标轴 ID 则返回 nullptr
 * \endif
 */
QImPlot3DAxisInfo* QImPlot3DNode::axisInfo(QImPlot3DAxisId aid) const
{
    QIM_DC(d);
    switch (aid) {
    case QImPlot3DAxisId::X1:
        return d->xAxisInfo;
    case QImPlot3DAxisId::Y1:
        return d->yAxisInfo;
    case QImPlot3DAxisId::Z1:
        return d->zAxisInfo;
    default:
        return nullptr;
    }
}

/**
 * \if ENGLISH
 * @brief Gets the X axis info
 * @return Pointer to the X-axis QImPlot3DAxisInfo
 * \endif
 *
 * \if CHINESE
 * @brief 获取 X 轴信息
 * @return X 轴 QImPlot3DAxisInfo 指针
 * \endif
 */
QImPlot3DAxisInfo* QImPlot3DNode::xAxis() const
{
    QIM_DC(d);
    return d->xAxisInfo;
}

/**
 * \if ENGLISH
 * @brief Gets the Y axis info
 * @return Pointer to the Y-axis QImPlot3DAxisInfo
 * \endif
 *
 * \if CHINESE
 * @brief 获取 Y 轴信息
 * @return Y 轴 QImPlot3DAxisInfo 指针
 * \endif
 */
QImPlot3DAxisInfo* QImPlot3DNode::yAxis() const
{
    QIM_DC(d);
    return d->yAxisInfo;
}

/**
 * \if ENGLISH
 * @brief Gets the Z axis info
 * @return Pointer to the Z-axis QImPlot3DAxisInfo
 * \endif
 *
 * \if CHINESE
 * @brief 获取 Z 轴信息
 * @return Z 轴 QImPlot3DAxisInfo 指针
 * \endif
 */
QImPlot3DAxisInfo* QImPlot3DNode::zAxis() const
{
    QIM_DC(d);
    return d->zAxisInfo;
}

//----------------------------------------------------
// Style node accessor
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Gets the style node for this plot
 * @return Pointer to QImPlot3DStyleNode
 * \endif
 *
 * \if CHINESE
 * @brief 获取此绘图的样式节点
 * @return QImPlot3DStyleNode 指针
 * \endif
 */
QImPlot3DStyleNode* QImPlot3DNode::styleNode() const
{
    QIM_DC(d);
    return d->m_styleNode;
}

//----------------------------------------------------
// ImPlot3DFlags accessors - using macros
//----------------------------------------------------

// Negative to Positive semantic (NoXxx -> xxxEnabled)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DNode, TitleEnabled, ImPlot3DFlags_NoTitle, plot3DFlagChanged)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DNode, LegendEnabled, ImPlot3DFlags_NoLegend, plot3DFlagChanged)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DNode, MouseTextEnabled, ImPlot3DFlags_NoMouseText, plot3DFlagChanged)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DNode, ClipEnabled, ImPlot3DFlags_NoClip, plot3DFlagChanged)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DNode, MenusEnabled, ImPlot3DFlags_NoMenus, plot3DFlagChanged)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DNode, RotateEnabled, ImPlot3DFlags_NoRotate, plot3DFlagChanged)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DNode, PanEnabled, ImPlot3DFlags_NoPan, plot3DFlagChanged)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DNode, ZoomEnabled, ImPlot3DFlags_NoZoom, plot3DFlagChanged)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DNode, InputsEnabled, ImPlot3DFlags_NoInputs, plot3DFlagChanged)

// Positive to Positive semantic (direct mapping)
QIMPLOT3D_FLAG_ACCESSOR(QImPlot3DNode, Equal, ImPlot3DFlags_Equal, plot3DFlagChanged)

//----------------------------------------------------
// Combined flags - Manual implementation
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Checks if plot renders in canvas-only mode (minimal UI decorations)
 * @return true if canvas mode is enabled (ImPlot3DFlags_CanvasOnly NOT set)
 * @details Canvas mode disables: Title, Legend, MouseText
 * @note ImPlot3DFlags_CanvasOnly = NoTitle | NoLegend | NoMouseText
 * \endif
 *
 * \if CHINESE
 * @brief 检查绘图是否以纯画布模式渲染（最小化 UI 装饰）
 * @return true 表示画布模式启用（未设置 ImPlot3DFlags_CanvasOnly）
 * @details 画布模式禁用：标题、图例、鼠标文本
 * @note ImPlot3DFlags_CanvasOnly = NoTitle | NoLegend | NoMouseText
 * \endif
 */
bool QImPlot3DNode::isCanvasEnabled() const
{
    QIM_DC(d);
    return (d->flags & ImPlot3DFlags_CanvasOnly) == 0;
}

/**
 * \if ENGLISH
 * @brief Enables/disables canvas-only mode
 * @param enabled true to show all decorations, false to hide title/legend/mouse-text
 * @details Convenience method that sets/clears ImPlot3DFlags_CanvasOnly component flags
 * \endif
 *
 * \if CHINESE
 * @brief 启用/禁用纯画布模式
 * @param enabled true 显示所有装饰，false 隐藏标题/图例/鼠标文本
 * @details 便捷方法，设置/清除 ImPlot3DFlags_CanvasOnly 组件标志
 * \endif
 */
void QImPlot3DNode::setCanvasEnabled(bool enabled)
{
    QIM_D(d);
    const auto oldFlags = d->flags;
    if (enabled) {
        d->flags &= ~ImPlot3DFlags_CanvasOnly;
    } else {
        d->flags |= ImPlot3DFlags_CanvasOnly;
    }
    if (d->flags != oldFlags) {
        Q_EMIT plot3DFlagChanged();
    }
}

//----------------------------------------------------
// Raw flags access
//----------------------------------------------------

int QImPlot3DNode::imPlot3DFlags() const
{
    QIM_DC(d);
    return d->flags;
}

void QImPlot3DNode::setImPlot3DFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = static_cast<ImPlot3DFlags>(flags);
        Q_EMIT plot3DFlagChanged();
    }
}

//----------------------------------------------------
// 3D rotation and scale
//----------------------------------------------------

void QImPlot3DNode::setBoxRotation(double elevation, double azimuth, bool animate, QImPlot3DCondition cond)
{
    QIM_D(d);
    d->elevation = elevation;
    d->azimuth = azimuth;
    d->animateRotation = animate;
    d->rotationCond = static_cast<ImPlot3DCond>(toImPlot3DCond(cond));
    d->useQuaternionRotation = false;
}

/**
 * \if ENGLISH
 * @brief Sets the plot box rotation using quaternion
 * @param rotation Quaternion rotation
 * @param animate Whether to animate the rotation
 * @param cond Condition for applying rotation
 * \endif
 *
 * \if CHINESE
 * @brief 使用四元数设置绘图框旋转
 * @param rotation 四元数旋转
 * @param animate 是否动画旋转
 * @param cond 应用旋转的条件
 * \endif
 */
void QImPlot3DNode::setBoxRotation(const QQuaternion& rotation, bool animate, QImPlot3DCondition cond)
{
    QIM_D(d);
    d->rotationQuat = rotation;
    d->animateRotation = animate;
    d->rotationCond = static_cast<ImPlot3DCond>(toImPlot3DCond(cond));
    d->useQuaternionRotation = true;
}

void QImPlot3DNode::setBoxInitialRotation(double elevation, double azimuth)
{
    QIM_D(d);
    d->initialElevation = elevation;
    d->initialAzimuth = azimuth;
    d->initialRotationSet = true;
    d->initialQuaternionRotationSet = false;
}

/**
 * \if ENGLISH
 * @brief Sets the plot box initial rotation using quaternion (for double-click reset)
 * @param rotation Quaternion rotation
 * \endif
 *
 * \if CHINESE
 * @brief 使用四元数设置绘图框初始旋转（用于双击重置）
 * @param rotation 四元数旋转
 * \endif
 */
void QImPlot3DNode::setBoxInitialRotation(const QQuaternion& rotation)
{
    QIM_D(d);
    d->initialRotationQuat = rotation;
    d->initialQuaternionRotationSet = true;
    d->initialRotationSet = false;
}

void QImPlot3DNode::setBoxScale(double x, double y, double z)
{
    QIM_D(d);
    d->boxScaleX = x;
    d->boxScaleY = y;
    d->boxScaleZ = z;
}

/**
 * \if ENGLISH
 * @brief Convenience wrapper for ImPlot3D::SetupAxes - sets labels and flags for all 3 axes
 * @param xLabel X-axis label text (UTF-8 encoded QByteArray)
 * @param yLabel Y-axis label text (UTF-8 encoded QByteArray)
 * @param zLabel Z-axis label text (UTF-8 encoded QByteArray)
 * @param xFlags X-axis ImPlot3DAxisFlags (default 0)
 * @param yFlags Y-axis ImPlot3DAxisFlags (default 0)
 * @param zFlags Z-axis ImPlot3DAxisFlags (default 0)
 * @details This method stores the labels and flags for later application in beginDraw().
 *          It delegates label setting to the respective axisInfo objects and stores flags
 *          for SetupAxis calls during rendering.
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot3D::SetupAxes 的便捷包装器 - 为所有 3 个坐标轴设置标签和标志
 * @param xLabel X 轴标签文本（UTF-8 编码的 QByteArray）
 * @param yLabel Y 轴标签文本（UTF-8 编码的 QByteArray）
 * @param zLabel Z 轴标签文本（UTF-8 编码的 QByteArray）
 * @param xFlags X 轴 ImPlot3DAxisFlags（默认 0）
 * @param yFlags Y 轴 ImPlot3DAxisFlags（默认 0）
 * @param zFlags Z 轴 ImPlot3DAxisFlags（默认 0）
 * @details 此方法存储标签和标志，以便稍后在 beginDraw() 中应用。
 *          它将标签设置委托给相应的 axisInfo 对象，并存储标志以供在渲染期间进行 SetupAxis 调用。
 * \endif
 */
void QImPlot3DNode::setupAxes(const QByteArray& xLabel, const QByteArray& yLabel, const QByteArray& zLabel, int xFlags, int yFlags, int zFlags)
{
    QIM_D(d);
    
    // Store labels in UTF-8 format
    d->xAxisLabelUtf8 = xLabel;
    d->yAxisLabelUtf8 = yLabel;
    d->zAxisLabelUtf8 = zLabel;
    
    // Store flags
    d->xAxisFlags = xFlags;
    d->yAxisFlags = yFlags;
    d->zAxisFlags = zFlags;
    
    // Mark that setupAxes was called
    d->hasSetupAxes = true;
    
    // Delegate label setting to axisInfo objects (convert QByteArray to QString for setLabel)
    if (d->xAxisInfo) {
        d->xAxisInfo->setLabel(QString::fromUtf8(xLabel));
    }
    if (d->yAxisInfo) {
        d->yAxisInfo->setLabel(QString::fromUtf8(yLabel));
    }
    if (d->zAxisInfo) {
        d->zAxisInfo->setLabel(QString::fromUtf8(zLabel));
    }
}

/**
 * \if ENGLISH
 * @brief Convenience wrapper for ImPlot3D::SetupAxesLimits - sets limits for all 3 axes
 * @param xMin X-axis minimum limit
 * @param xMax X-axis maximum limit
 * @param yMin Y-axis minimum limit
 * @param yMax Y-axis maximum limit
 * @param zMin Z-axis minimum limit
 * @param zMax Z-axis maximum limit
 * @param cond Condition for applying limits (default Once)
 * @details This method delegates to the respective axisInfo setLimits methods.
 *          No special beginDraw integration is needed as axis limits are already
 *          handled in applySetup().
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot3D::SetupAxesLimits 的便捷包装器 - 为所有 3 个坐标轴设置限制
 * @param xMin X 轴最小限制
 * @param xMax X 轴最大限制
 * @param yMin Y 轴最小限制
 * @param yMax Y 轴最大限制
 * @param zMin Z 轴最小限制
 * @param zMax Z 轴最大限制
 * @param cond 应用限制的条件（默认 Once）
 * @details 此方法委托给相应的 axisInfo setLimits 方法。
 *          无需特殊的 beginDraw 集成，因为坐标轴限制已在 applySetup() 中处理。
 * \endif
 */
void QImPlot3DNode::setupAxesLimits(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax, QImPlot3DCondition cond)
{
    QIM_D(d);
    
    // Delegate to axisInfo objects
    if (d->xAxisInfo) {
        d->xAxisInfo->setLimits(xMin, xMax, cond);
    }
    if (d->yAxisInfo) {
        d->yAxisInfo->setLimits(yMin, yMax, cond);
    }
    if (d->zAxisInfo) {
        d->zAxisInfo->setLimits(zMin, zMax, cond);
    }
}

//----------------------------------------------------
// Legend configuration
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Gets the legend location
 * @return Current legend location as QImPlot3DLocation
 * \endif
 *
 * \if CHINESE
 * @brief 获取图例位置
 * @return 当前图例位置，类型为 QImPlot3DLocation
 * \endif
 */
QImPlot3DLocation QImPlot3DNode::legendLocation() const
{
    QIM_DC(d);
    return d->m_legendLocation;
}

/**
 * \if ENGLISH
 * @brief Sets the legend location and marks legend as configured
 * @param location Legend location to set
 * @details When called, m_hasLegendConfig is set to true, ensuring SetupLegend
 *          will be called during beginDraw().
 * \endif
 *
 * \if CHINESE
 * @brief 设置图例位置并标记图例为已配置
 * @param location 要设置的图例位置
 * @details 调用时 m_hasLegendConfig 设为 true，确保 beginDraw() 中会调用 SetupLegend。
 * \endif
 */
void QImPlot3DNode::setLegendLocation(QImPlot3DLocation location)
{
    QIM_D(d);
    if (d->m_legendLocation != location) {
        d->m_legendLocation = location;
        d->m_hasLegendConfig = true;
        Q_EMIT legendConfigChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Gets the legend flags
 * @return Current legend flags as int (QImPlot3DLegendFlags values)
 * \endif
 *
 * \if CHINESE
 * @brief 获取图例标志
 * @return 当前图例标志，类型为 int（QImPlot3DLegendFlags 值）
 * \endif
 */
int QImPlot3DNode::legendFlags() const
{
    QIM_DC(d);
    return d->m_legendFlags;
}

/**
 * \if ENGLISH
 * @brief Sets the legend flags and marks legend as configured
 * @param flags Legend flags to set (QImPlot3DLegendFlags values cast to int)
 * @details When called, m_hasLegendConfig is set to true, ensuring SetupLegend
 *          will be called during beginDraw().
 * \endif
 *
 * \if CHINESE
 * @brief 设置图例标志并标记图例为已配置
 * @param flags 要设置的图例标志（QImPlot3DLegendFlags 值转为 int）
 * @details 调用时 m_hasLegendConfig 设为 true，确保 beginDraw() 中会调用 SetupLegend。
 * \endif
 */
void QImPlot3DNode::setLegendFlags(int flags)
{
    QIM_D(d);
    if (d->m_legendFlags != flags) {
        d->m_legendFlags = flags;
        d->m_hasLegendConfig = true;
        Q_EMIT legendConfigChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Convenience method to set legend location and flags together
 * @param location Legend location to set
 * @param flags Legend flags to set (QImPlot3DLegendFlags values, default 0)
 * @details Sets both legendLocation and legendFlags in a single call.
 *          Marks m_hasLegendConfig as true.
 * \endif
 *
 * \if CHINESE
 * @brief 便捷方法，同时设置图例位置和标志
 * @param location 要设置的图例位置
 * @param flags 要设置的图例标志（QImPlot3DLegendFlags 值，默认 0）
 * @details 在一次调用中同时设置 legendLocation 和 legendFlags。
 *          将 m_hasLegendConfig 标记为 true。
 * \endif
 */
void QImPlot3DNode::setLegend(QImPlot3DLocation location, int flags)
{
    QIM_D(d);
    d->m_legendLocation = location;
    d->m_legendFlags = flags;
    d->m_hasLegendConfig = true;
    Q_EMIT legendConfigChanged();
}

//----------------------------------------------------
// Colormap push/pop
//----------------------------------------------------

/**
 * \if ENGLISH
 * @brief Pushes a colormap onto the colormap stack (applied in beginDraw)
 * @param colormap Colormap enum value to push
 * @details Stores the colormap for later application during beginDraw().
 *          ImPlot3D::PushColormap is called in beginDraw() after setup calls,
 *          before child items render. ImPlot3D::PopColormap is called in
 *          endDraw() before EndPlot().
 * \endif
 *
 * \if CHINESE
 * @brief 将色彩映射压入色彩映射栈（在 beginDraw 中应用）
 * @param colormap 要压入的色彩映射枚举值
 * @details 存储色彩映射，以便稍后在 beginDraw() 中应用。
 *          ImPlot3D::PushColormap 在 beginDraw() 中于 setup 调用之后、
 *          子元素渲染之前调用。ImPlot3D::PopColormap 在 endDraw() 中
 *          于 EndPlot() 之前调用。
 * \endif
 */
void QImPlot3DNode::pushColormap(QImPlot3DColormap colormap)
{
    QIM_D(d);
    d->m_pushedColormaps.append(colormap);
    d->m_pushedColormapCount++;
}

/**
 * \if ENGLISH
 * @brief Pushes a colormap by name onto the colormap stack (applied in beginDraw)
 * @param name Colormap name (UTF-8 encoded)
 * @details Stores the colormap name for later application during beginDraw().
 *          ImPlot3D::PushColormap is called in beginDraw() after setup calls,
 *          before child items render.
 * \endif
 *
 * \if CHINESE
 * @brief 通过名称将色彩映射压入色彩映射栈（在 beginDraw 中应用）
 * @param name 色彩映射名称（UTF-8 编码）
 * @details 存储色彩映射名称，以便稍后在 beginDraw() 中应用。
 *          ImPlot3D::PushColormap 在 beginDraw() 中于 setup 调用之后、
 *          子元素渲染之前调用。
 * \endif
 */
void QImPlot3DNode::pushColormap(const QByteArray& name)
{
    QIM_D(d);
    d->m_pushedColormapNames.append(name);
    d->m_pushedColormapCount++;
}

/**
 * \if ENGLISH
 * @brief Pops colormaps from the stack (applied in endDraw)
 * @param count Number of colormaps to pop (default 1)
 * @details Reduces the push count so the corresponding number of
 *          ImPlot3D::PopColormap calls will be made in endDraw().
 *          If count exceeds current push count, all pushes are popped.
 * \endif
 *
 * \if CHINESE
 * @brief 从色彩映射栈弹出色彩映射（在 endDraw 中应用）
 * @param count 要弹出的色彩映射数量（默认 1）
 * @details 减少压入计数，以便在 endDraw() 中调用对应数量的
 *          ImPlot3D::PopColormap。如果 count 超过当前压入计数，
 *          则弹出所有压入的色彩映射。
 * \endif
 */
void QImPlot3DNode::popColormap(int count)
{
    QIM_D(d);
    d->m_pushedColormapCount = qMax(0, d->m_pushedColormapCount - count);
}

//----------------------------------------------------
// Item management
//----------------------------------------------------

void QImPlot3DNode::addPlot3DItem(QImPlot3DItemNode* item)
{
    addChildNode(item);
}

QList<QImPlot3DItemNode*> QImPlot3DNode::plot3DItemNodes() const
{
    return findChildrenNodes<QImPlot3DItemNode*>();
}

//----------------------------------------------------
// Interaction query
//----------------------------------------------------

bool QImPlot3DNode::isPlot3DHovered() const
{
    // Note: This requires ImPlot3D plot context to be available
    // Will be implemented when we have access to ImPlot3D plot context
    return false;  // Placeholder - actual implementation needs plot context
}

/**
 * \if ENGLISH
 * @brief Converts a 3D plot point to pixel coordinates
 * @param point 3D point in plot coordinates
 * @return Pixel coordinates as QPointF
 * @note Must be called inside BeginPlot/EndPlot scope
 * \endif
 *
 * \if CHINESE
 * @brief 将三维绘图点转换为像素坐标
 * @param point 绘图坐标系中的三维点
 * @return 像素坐标，类型为 QPointF
 * @note 必须在 BeginPlot/EndPlot 作用域内调用
 * \endif
 */
QPointF QImPlot3DNode::plotToPixels(const QImPlot3DPoint& point)
{
    ImVec2 result = ImPlot3D::PlotToPixels(toImPlot3DPoint(point));
    return toQPointF(result);
}

/**
 * \if ENGLISH
 * @brief Converts 3D plot coordinates to pixel coordinates
 * @param x X coordinate in plot space
 * @param y Y coordinate in plot space
 * @param z Z coordinate in plot space
 * @return Pixel coordinates as QPointF
 * @note Must be called inside BeginPlot/EndPlot scope
 * \endif
 *
 * \if CHINESE
 * @brief 将三维绘图坐标转换为像素坐标
 * @param x 绘图空间中的 X 坐标
 * @param y 绘图空间中的 Y 坐标
 * @param z 绘图空间中的 Z 坐标
 * @return 像素坐标，类型为 QPointF
 * @note 必须在 BeginPlot/EndPlot 作用域内调用
 * \endif
 */
QPointF QImPlot3DNode::plotToPixels(double x, double y, double z)
{
    ImVec2 result = ImPlot3D::PlotToPixels(x, y, z);
    return toQPointF(result);
}

/**
 * \if ENGLISH
 * @brief Converts pixel coordinates to a 3D ray from camera through pixel
 * @param pixel Pixel coordinates
 * @return Ray from camera origin through pixel point
 * @note Must be called inside BeginPlot/EndPlot scope
 * \endif
 *
 * \if CHINESE
 * @brief 将像素坐标转换为从相机穿过像素的三维射线
 * @param pixel 像素坐标
 * @return 从相机原点穿过像素点的射线
 * @note 必须在 BeginPlot/EndPlot 作用域内调用
 * \endif
 */
QImPlot3DRay QImPlot3DNode::pixelsToPlotRay(const QPointF& pixel)
{
    ImPlot3DRay result = ImPlot3D::PixelsToPlotRay(pixel.x(), pixel.y());
    return toQImPlot3DRay(result);
}

/**
 * \if ENGLISH
 * @brief Converts pixel coordinates to a 3D point on the specified plane
 * @param pixel Pixel coordinates
 * @param plane Target plane (YZ, XZ, or XY)
 * @param mask Whether to mask out-of-range values (default true)
 * @return 3D point on the specified plane
 * @note Must be called inside BeginPlot/EndPlot scope
 * \endif
 *
 * \if CHINESE
 * @brief 将像素坐标转换为指定平面上的三维点
 * @param pixel 像素坐标
 * @param plane 目标平面（YZ、XZ 或 XY）
 * @param mask 是否屏蔽超范围值（默认 true）
 * @return 指定平面上的三维点
 * @note 必须在 BeginPlot/EndPlot 作用域内调用
 * \endif
 */
QImPlot3DPoint QImPlot3DNode::pixelsToPlotPlane(const QPointF& pixel, QImPlane3D plane, bool mask)
{
    ImPlot3DPoint result = ImPlot3D::PixelsToPlotPlane(pixel.x(), pixel.y(), static_cast<ImPlane3D>(toImPlane3D(plane)), mask);
    return toQImPlot3DPoint(result);
}

/**
 * \if ENGLISH
 * @brief Gets the plot rectangle position in pixel coordinates
 * @return Top-left corner position of the plot rectangle
 * @note Must be called inside BeginPlot/EndPlot scope
 * \endif
 *
 * \if CHINESE
 * @brief 获取绘图矩形区域的像素坐标位置
 * @return 绘图矩形区域左上角位置
 * @note 必须在 BeginPlot/EndPlot 作用域内调用
 * \endif
 */
QPointF QImPlot3DNode::getPlotRectPos()
{
    ImVec2 result = ImPlot3D::GetPlotRectPos();
    return toQPointF(result);
}

/**
 * \if ENGLISH
 * @brief Gets the plot rectangle size in pixels
 * @return Size of the plot rectangle
 * @note Must be called inside BeginPlot/EndPlot scope
 * \endif
 *
 * \if CHINESE
 * @brief 获取绘图矩形区域的像素尺寸
 * @return 绘图矩形区域的尺寸
 * @note 必须在 BeginPlot/EndPlot 作用域内调用
 * \endif
 */
QSizeF QImPlot3DNode::getPlotRectSize()
{
    ImVec2 result = ImPlot3D::GetPlotRectSize();
    return QSizeF(result.x, result.y);
}

/**
 * \if ENGLISH
 * @brief Gets the ImDrawList for the current plot
 * @return Pointer to ImDrawList for custom drawing
 * @note Must be called inside BeginPlot/EndPlot scope
 * @details Returns the ImDrawList associated with the current plot,
 *          allowing custom drawing operations within the plot area.
 * \endif
 *
 * \if CHINESE
 * @brief 获取当前绘图的 ImDrawList
 * @return 用于自定义绘图的 ImDrawList 指针
 * @note 必须在 BeginPlot/EndPlot 作用域内调用
 * @details 返回与当前绘图关联的 ImDrawList，
 *          允许在绘图区域内执行自定义绘图操作。
 * \endif
 */
ImDrawList* QImPlot3DNode::getPlotDrawList() const
{
    return ImPlot3D::GetPlotDrawList();
}

/**
 * \if ENGLISH
 * @brief Gets the next marker shape in the cycle
 * @return Next marker shape as QImPlot3DMarkerShape
 * @note Must be called inside BeginPlot/EndPlot scope
 * @details Cycles through available marker shapes (Circle, Square, Diamond, etc.)
 *          starting from Circle and wrapping around after Asterisk.
 * \endif
 *
 * \if CHINESE
 * @brief 获取循环中的下一个标记形状
 * @return 下一个标记形状，类型为 QImPlot3DMarkerShape
 * @note 必须在 BeginPlot/EndPlot 作用域内调用
 * @details 循环遍历可用的标记形状（圆形、方形、菱形等），
 *          从圆形开始，在星号之后回绕。
 * \endif
 */
QImPlot3DMarkerShape QImPlot3DNode::nextMarker()
{
    // Static counter to cycle through markers
    static int markerCounter = 0;
    
    // Cycle through marker shapes (excluding None = -1)
    // Available markers: Circle(0) through Asterisk(9)
    int marker = markerCounter % 10;  // 10 marker shapes from 0 to 9
    
    // Increment counter for next call
    markerCounter++;
    
    // Convert to QImPlot3DMarkerShape enum
    return static_cast<QImPlot3DMarkerShape>(marker);
}

//----------------------------------------------------
// beginDraw / endDraw - Core rendering logic
//----------------------------------------------------

bool QImPlot3DNode::beginDraw()
{
    QIM_D(d);

    // Call BeginPlot with pre-converted data (beginDraw minimization)
    // When autoSize is true, use ImVec2(-1, -1) for auto-fill; otherwise use stored size
    const char* titlePtr = d->titleUtf8.isEmpty() ? nullptr : d->titleUtf8.constData();
    ImVec2 plotSize = d->autoSize ? ImVec2(-1, -1) : d->sizeVec4;
    d->beginPlotSuccess = ImPlot3D::BeginPlot(titlePtr, plotSize, d->flags);

    if (!d->beginPlotSuccess) {
        // Return true anyway for style cleanup (similar to 2D pattern)
        return true;
    }

    // Setup calls (must be after BeginPlot, before PlotItem calls)
    // Apply SetupAxis if setupAxes() was called (must be before per-axis applySetup)
    if (d->hasSetupAxes) {
        const char* xLabelPtr = d->xAxisLabelUtf8.isEmpty() ? nullptr : d->xAxisLabelUtf8.constData();
        const char* yLabelPtr = d->yAxisLabelUtf8.isEmpty() ? nullptr : d->yAxisLabelUtf8.constData();
        const char* zLabelPtr = d->zAxisLabelUtf8.isEmpty() ? nullptr : d->zAxisLabelUtf8.constData();
        
        ImPlot3D::SetupAxis(static_cast<ImAxis3D>(toImAxis3D(QImPlot3DAxisId::X1)), xLabelPtr, d->xAxisFlags);
        ImPlot3D::SetupAxis(static_cast<ImAxis3D>(toImAxis3D(QImPlot3DAxisId::Y1)), yLabelPtr, d->yAxisFlags);
        ImPlot3D::SetupAxis(static_cast<ImAxis3D>(toImAxis3D(QImPlot3DAxisId::Z1)), zLabelPtr, d->zAxisFlags);
    }
    
    d->xAxisInfo->applySetup();
    d->yAxisInfo->applySetup();
    d->zAxisInfo->applySetup();

    // 3D rotation
    if (d->useQuaternionRotation) {
        ImPlot3DQuat quat = toImPlot3DQuat(d->rotationQuat);
        ImPlot3D::SetupBoxRotation(quat, d->animateRotation, d->rotationCond);
    } else {
        ImPlot3D::SetupBoxRotation(d->elevation, d->azimuth, d->animateRotation, d->rotationCond);
    }

    // Initial rotation (for double-click reset)
    if (d->initialRotationSet) {
        ImPlot3D::SetupBoxInitialRotation(d->initialElevation, d->initialAzimuth);
    }
    if (d->initialQuaternionRotationSet) {
        ImPlot3DQuat initialQuat = toImPlot3DQuat(d->initialRotationQuat);
        ImPlot3D::SetupBoxInitialRotation(initialQuat);
    }

    // Box scale
    ImPlot3D::SetupBoxScale(d->boxScaleX, d->boxScaleY, d->boxScaleZ);

    // Legend configuration
    if (d->m_hasLegendConfig) {
        ImPlot3D::SetupLegend(toImPlot3DLocation(d->m_legendLocation), toImPlot3DLegendFlags(static_cast<QImPlot3DLegendFlags>(d->m_legendFlags)));
    }

    // Style application — after setup, before child items render
    if (d->m_styleNode) {
        d->m_styleNode->applyStyle();
    }

    // Colormap push — after setup, before child items render
    for (const QImPlot3DColormap& cmap : d->m_pushedColormaps) {
        ImPlot3D::PushColormap(static_cast<ImPlot3DColormap>(toImPlot3DColormap(cmap)));
    }
    for (const QByteArray& name : d->m_pushedColormapNames) {
        ImPlot3D::PushColormap(name.constData());
    }

    return true;
}

void QImPlot3DNode::endDraw()
{
    QIM_D(d);
    if (d->beginPlotSuccess) {
        // Colormap pop — before EndPlot, after child items render
        if (d->m_pushedColormapCount > 0) {
            ImPlot3D::PopColormap(d->m_pushedColormapCount);
        }
        ImPlot3D::EndPlot();
    }
    // Reset push tracking for next frame
    d->m_pushedColormaps.clear();
    d->m_pushedColormapNames.clear();
    d->m_pushedColormapCount = 0;
}

// ===============================================================
// Mesh factory methods
// ===============================================================

/**
 * \if ENGLISH
 * @brief Creates and adds a cube mesh item to the plot
 * @param[in] label Optional label for the mesh item
 * @return Pointer to the created mesh item node
 * @details Creates a QImPlot3DMeshItemNode with cube geometry.
 *          Uses static cube vertices and indices from QImPlot3DMeshItemNode.
 * \endif
 *
 * \if CHINESE
 * @brief 创建并添加立方体网格项到绘图
 * @param[in] label 网格项的可选标签
 * @return 指向创建的网格项节点的指针
 * @details 创建具有立方体几何形状的 QImPlot3DMeshItemNode。
 *          使用 QImPlot3DMeshItemNode 中的静态立方体顶点和索引。
 * \endif
 */
QImPlot3DMeshItemNode* QImPlot3DNode::addCube(const QString& label)
{
    auto* item = new QImPlot3DMeshItemNode(this);
    item->setLabel(label);
    
    // Convert QList to std::vector for setMeshData
    QList<QImPlot3DPoint> vertices = QImPlot3DMeshItemNode::cubeVertices();
    QList<unsigned int> indices = QImPlot3DMeshItemNode::cubeIndices();
    
    std::vector<QImPlot3DPoint> verticesVec(vertices.begin(), vertices.end());
    std::vector<unsigned int> indicesVec(indices.begin(), indices.end());
    
    item->setMeshData(verticesVec, indicesVec);
    addPlot3DItem(item);
    return item;
}

/**
 * \if ENGLISH
 * @brief Creates and adds a sphere mesh item to the plot
 * @param[in] label Optional label for the mesh item
 * @return Pointer to the created mesh item node
 * @details Creates a QImPlot3DMeshItemNode with sphere geometry.
 *          Uses static sphere vertices and indices from QImPlot3DMeshItemNode.
 * \endif
 *
 * \if CHINESE
 * @brief 创建并添加球体网格项到绘图
 * @param[in] label 网格项的可选标签
 * @return 指向创建的网格项节点的指针
 * @details 创建具有球体几何形状的 QImPlot3DMeshItemNode。
 *          使用 QImPlot3DMeshItemNode 中的静态球体顶点和索引。
 * \endif
 */
QImPlot3DMeshItemNode* QImPlot3DNode::addSphere(const QString& label)
{
    auto* item = new QImPlot3DMeshItemNode(this);
    item->setLabel(label);
    
    // Convert QList to std::vector for setMeshData
    QList<QImPlot3DPoint> vertices = QImPlot3DMeshItemNode::sphereVertices();
    QList<unsigned int> indices = QImPlot3DMeshItemNode::sphereIndices();
    
    std::vector<QImPlot3DPoint> verticesVec(vertices.begin(), vertices.end());
    std::vector<unsigned int> indicesVec(indices.begin(), indices.end());
    
    item->setMeshData(verticesVec, indicesVec);
    addPlot3DItem(item);
    return item;
}

/**
 * \if ENGLISH
 * @brief Creates and adds a duck mesh item to the plot
 * @param[in] label Optional label for the mesh item
 * @return Pointer to the created mesh item node
 * @details Creates a QImPlot3DMeshItemNode with duck geometry.
 *          Uses static duck vertices and indices from QImPlot3DMeshItemNode.
 * \endif
 *
 * \if CHINESE
 * @brief 创建并添加鸭子模型网格项到绘图
 * @param[in] label 网格项的可选标签
 * @return 指向创建的网格项节点的指针
 * @details 创建具有鸭子模型几何形状的 QImPlot3DMeshItemNode。
 *          使用 QImPlot3DMeshItemNode 中的静态鸭子模型顶点和索引。
 * \endif
 */
QImPlot3DMeshItemNode* QImPlot3DNode::addDuck(const QString& label)
{
    auto* item = new QImPlot3DMeshItemNode(this);
    item->setLabel(label);
    
    // Convert QList to std::vector for setMeshData
    QList<QImPlot3DPoint> vertices = QImPlot3DMeshItemNode::duckVertices();
    QList<unsigned int> indices = QImPlot3DMeshItemNode::duckIndices();
    
    std::vector<QImPlot3DPoint> verticesVec(vertices.begin(), vertices.end());
    std::vector<unsigned int> indicesVec(indices.begin(), indices.end());
    
    item->setMeshData(verticesVec, indicesVec);
    addPlot3DItem(item);
    return item;
}

}  // namespace QIM
