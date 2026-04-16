#include "QImPlot3DNode.h"
#include "QImPlot3DAxisInfo.h"
#include "QImPlot3DItemNode.h"
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
    ImVec2 sizeVec4 {-1, 0};  ///< Pre-converted size for BeginPlot

    // Flags - MUST be named 'flags' for macro access via d_ptr->flags
    ImPlot3DFlags flags {ImPlot3DFlags_None};

    // BeginPlot success flag
    bool beginPlotSuccess {false};

    // 3 Axis info objects (X, Y, Z)
    QImPlot3DAxisInfo* xAxisInfo {nullptr};
    QImPlot3DAxisInfo* yAxisInfo {nullptr};
    QImPlot3DAxisInfo* zAxisInfo {nullptr};

    // 3D rotation parameters
    double elevation {0};              ///< Elevation angle in degrees
    double azimuth {0};                ///< Azimuth angle in degrees
    bool animateRotation {false};      ///< Whether to animate rotation
    ImPlot3DCond rotationCond {ImPlot3DCond_Once};  ///< Rotation condition
    bool initialRotationSet {false};   ///< Whether initial rotation is set
    double initialElevation {0};       ///< Initial elevation for double-click reset
    double initialAzimuth {0};         ///< Initial azimuth for double-click reset

    // Box scale parameters
    double boxScaleX {1.0};            ///< X-axis box scale
    double boxScaleY {1.0};            ///< Y-axis box scale
    double boxScaleZ {1.0};            ///< Z-axis box scale
};

QImPlot3DNode::PrivateData::PrivateData(QImPlot3DNode* q) : q_ptr(q)
{
    // Default title
    titleUtf8 = "##Plot3D";

    // Create 3 axis info objects (X1, Y1, Z1)
    xAxisInfo = new QImPlot3DAxisInfo(QImPlot3DAxisId::X1, q);
    yAxisInfo = new QImPlot3DAxisInfo(QImPlot3DAxisId::Y1, q);
    zAxisInfo = new QImPlot3DAxisInfo(QImPlot3DAxisId::Z1, q);
}

//===============================================================
// QImPlot3DNode Implementation
//===============================================================

QImPlot3DNode::QImPlot3DNode(QObject* parent)
    : QImAbstractNode(parent)
    , QIM_PIMPL_CONSTRUCT
{
    setObjectName(QStringLiteral("Plot3DNode"));
}

QImPlot3DNode::QImPlot3DNode(const QString& title, QObject* parent)
    : QImPlot3DNode(parent)
{
    setTitle(title);
}

QImPlot3DNode::~QImPlot3DNode()
{
}

//----------------------------------------------------
// Title
//----------------------------------------------------

QString QImPlot3DNode::title() const
{
    QIM_DC(d);
    return QString::fromUtf8(d->titleUtf8);
}

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

QSizeF QImPlot3DNode::size() const
{
    QIM_DC(d);
    return QSizeF(d->sizeVec4.x, d->sizeVec4.y);
}

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
// Axis management
//----------------------------------------------------

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

QImPlot3DAxisInfo* QImPlot3DNode::xAxis() const
{
    return d_ptr->xAxisInfo;
}

QImPlot3DAxisInfo* QImPlot3DNode::yAxis() const
{
    return d_ptr->yAxisInfo;
}

QImPlot3DAxisInfo* QImPlot3DNode::zAxis() const
{
    return d_ptr->zAxisInfo;
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
    return (d_ptr->flags & ImPlot3DFlags_CanvasOnly) == 0;
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
    return d_ptr->flags;
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
}

void QImPlot3DNode::setBoxInitialRotation(double elevation, double azimuth)
{
    QIM_D(d);
    d->initialElevation = elevation;
    d->initialAzimuth = azimuth;
    d->initialRotationSet = true;
}

void QImPlot3DNode::setBoxScale(double x, double y, double z)
{
    QIM_D(d);
    d->boxScaleX = x;
    d->boxScaleY = y;
    d->boxScaleZ = z;
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

//----------------------------------------------------
// beginDraw / endDraw - Core rendering logic
//----------------------------------------------------

bool QImPlot3DNode::beginDraw()
{
    QIM_D(d);

    // Call BeginPlot with pre-converted data (beginDraw minimization)
    const char* titlePtr = d->titleUtf8.isEmpty() ? nullptr : d->titleUtf8.constData();
    d->beginPlotSuccess = ImPlot3D::BeginPlot(titlePtr, d->sizeVec4, d->flags);

    if (!d->beginPlotSuccess) {
        // Return true anyway for style cleanup (similar to 2D pattern)
        return true;
    }

    // Setup calls (must be after BeginPlot, before PlotItem calls)
    d->xAxisInfo->applySetup();
    d->yAxisInfo->applySetup();
    d->zAxisInfo->applySetup();

    // 3D rotation
    ImPlot3D::SetupBoxRotation(d->elevation, d->azimuth, d->animateRotation, d->rotationCond);

    // Initial rotation (for double-click reset)
    if (d->initialRotationSet) {
        ImPlot3D::SetupBoxInitialRotation(d->initialElevation, d->initialAzimuth);
    }

    // Box scale
    ImPlot3D::SetupBoxScale(d->boxScaleX, d->boxScaleY, d->boxScaleZ);

    return true;
}

void QImPlot3DNode::endDraw()
{
    if (d_ptr->beginPlotSuccess) {
        ImPlot3D::EndPlot();
    }
}

}  // namespace QIM