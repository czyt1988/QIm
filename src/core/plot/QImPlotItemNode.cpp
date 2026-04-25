#include "QImPlotItemNode.h"
#include <algorithm>
#include "implot.h"
#include "implot_internal.h"
#include "QtImGuiUtils.h"
#include "QImPlotNode.h"

namespace QIM
{

class QImPlotItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlotItemNode)
public:
    PrivateData(QImPlotItemNode* p);

public:
    QByteArray utf8Label;
    ImAxis xAxisId { ImAxis_X1 };
    ImAxis yAxisId { ImAxis_Y1 };
    ImPlotItem* plotItem { nullptr };
    bool userVisible { true };  ///< 用户设置的可见性状态（首次渲染前有效）
};

QImPlotItemNode::PrivateData::PrivateData(QImPlotItemNode* p) : q_ptr(p)
{
}
//----------------------------------------------------
// QImPlotItemNode
//----------------------------------------------------
/**
 * \if ENGLISH
 * @brief Constructs a QImPlotItemNode with optional parent
 * @param[in] par Parent QObject (typically a QImPlotNode)
 * @details Initializes the item node with auto-ID disabled and RenderIgnoreVisible set to true,
 *          because item visibility is managed by ImPlotItem::Show for legend interaction support.
 * \endif
 *
 * \if CHINESE
 * @brief 构造 QImPlotItemNode，可选指定父对象
 * @param[in] par 父 QObject（通常为 QImPlotNode）
 * @details 初始化项节点，禁用自动 ID，设置 RenderIgnoreVisible 为 true，
 *          因为项的可见性由 ImPlotItem::Show 管理，以支持图例交互。
 * \endif
 */
QImPlotItemNode::QImPlotItemNode(QObject* par) : QImAbstractNode(par), QIM_PIMPL_CONSTRUCT
{
    setAutoIdEnabled(false);
    setRenderOption(RenderIgnoreVisible, true);  // line的visible由ImPlotItem::Show决定，这样才可以和legend联动
}

/**
 * \if ENGLISH
 * @brief Destructor for QImPlotItemNode
 * \endif
 *
 * \if CHINESE
 * @brief QImPlotItemNode 的析构函数
 * \endif
 */
QImPlotItemNode::~QImPlotItemNode()
{
}

/**
 * \if ENGLISH
 * @brief Sets the item label (legend entry name)
 * @param[in] name Label string displayed in legend
 * @details Stores as UTF-8 internally. Emits labelChanged() signal only when value changes.
 * \endif
 *
 * \if CHINESE
 * @brief 设置项标签（图例条目名称）
 * @param[in] name 显示在图例中的标签字符串
 * @details 内部存储为 UTF-8。仅在值改变时触发 labelChanged() 信号。
 * \endif
 */
void QImPlotItemNode::setLabel(const QString& name)
{
    QIM_D(d);
    QByteArray nameUtf = name.toUtf8();
    if (nameUtf != d->utf8Label) {
        d->utf8Label = nameUtf;
        Q_EMIT labelChanged(name);
    }
}

/**
 * \if ENGLISH
 * @brief Gets the item label text
 * @return Current label string decoded from UTF-8 storage
 * \endif
 *
 * \if CHINESE
 * @brief 获取项标签文本
 * @return 从 UTF-8 存储解码的当前标签字符串
 * \endif
 */
QString QImPlotItemNode::label() const
{
    QIM_DC(d);
    return QString::fromUtf8(d->utf8Label);
}

/**
 * \if ENGLISH
 * @brief Gets the raw UTF-8 label data pointer for ImPlot API
 * @return const char* pointer to UTF-8 label data, or "##plotItem" if empty
 * @details Used internally for passing label to ImPlot::PlotXXX() functions.
 *          Returns a fallback "##plotItem" for empty labels (ImGui hidden ID convention).
 * \endif
 *
 * \if CHINESE
 * @brief 获取原始 UTF-8 标签数据指针，用于 ImPlot API
 * @return UTF-8 标签数据的 const char* 指针，空时返回 "##plotItem"
 * @details 内部用于将标签传递给 ImPlot::PlotXXX() 函数。
 *          空标签时返回回退值 "##plotItem"（ImGui 隐藏 ID 规范）。
 * \endif
 */
const char* QImPlotItemNode::labelConstData() const
{
    QIM_DC(d);
    return (d->utf8Label.isEmpty() ? "##plotItem" : d->utf8Label.constData());
}

/**
 * \if ENGLISH
 * @brief Binds the item to specific X and Y axes
 * @param[in] x QImPlotAxisId for the X axis
 * @param[in] y QImPlotAxisId for the Y axis
 * @details Converts QImPlotAxisId to ImAxis and stores internally. Only valid axis IDs are accepted.
 * \endif
 *
 * \if CHINESE
 * @brief 将项绑定到指定的 X 轴和 Y 轴
 * @param[in] x X 轴的 QImPlotAxisId
 * @param[in] y Y 轴的 QImPlotAxisId
 * @details 将 QImPlotAxisId 转换为 ImAxis 并内部存储。仅接受有效的轴 ID。
 * \endif
 */
void QImPlotItemNode::bindAxis(QImPlotAxisId x, QImPlotAxisId y)
{
    QIM_D(d);
    if (isXAxisId(x) && isYAxisId(y)) {
        d->xAxisId = toImAxis(x);
        d->yAxisId = toImAxis(y);
    }
}

/**
 * \if ENGLISH
 * @brief Converts screen pixel coordinates to plot coordinates using item's bound axes
 * @param[in] screenX X coordinate in screen pixels
 * @param[in] screenY Y coordinate in screen pixels
 * @return QPointF containing plot data coordinates
 * \endif
 *
 * \if CHINESE
 * @brief 使用项绑定的坐标轴将屏幕像素坐标转换为绘图坐标
 * @param[in] screenX 屏幕像素中的 X 坐标
 * @param[in] screenY 屏幕像素中的 Y 坐标
 * @return 包含绘图数据坐标的 QPointF
 * \endif
 */
QPointF QImPlotItemNode::pixelsToPlot(const float& screenX, const float& screenY)
{
    QIM_D(d);
    return toQPointF(ImPlot::PixelsToPlot(screenX, screenY, d->xAxisId, d->yAxisId));
}

/**
 * \if ENGLISH
 * @brief Converts plot data coordinates to screen pixel coordinates using item's bound axes
 * @param[in] doubleX X coordinate in plot data space
 * @param[in] doubleY Y coordinate in plot data space
 * @return QPointF containing screen pixel coordinates
 * \endif
 *
 * \if CHINESE
 * @brief 使用项绑定的坐标轴将绘图数据坐标转换为屏幕像素坐标
 * @param[in] doubleX 绘图数据空间中的 X 坐标
 * @param[in] doubleY 绘图数据空间中的 Y 坐标
 * @return 包含屏幕像素坐标的 QPointF
 * \endif
 */
QPointF QImPlotItemNode::plotToPixels(const double& doubleX, const double& doubleY)
{
    QIM_D(d);
    return toQPointF(ImPlot::PlotToPixels(doubleX, doubleY, d->xAxisId, d->yAxisId));
}


/**
 * \if ENGLISH
 * @brief Gets the bound X axis identifier
 * @return QImPlotAxisId for the currently bound X axis
 * \endif
 *
 * \if CHINESE
 * @brief 获取绑定的 X 轴标识符
 * @return 当前绑定的 X 轴的 QImPlotAxisId
 * \endif
 */
QImPlotAxisId QImPlotItemNode::xAxisId() const
{
    QIM_DC(d);
    return toQImPlotAxisId(d->xAxisId);
}

/**
 * \if ENGLISH
 * @brief Gets the bound Y axis identifier
 * @return QImPlotAxisId for the currently bound Y axis
 * \endif
 *
 * \if CHINESE
 * @brief 获取绑定的 Y 轴标识符
 * @return 当前绑定的 Y 轴的 QImPlotAxisId
 * \endif
 */
QImPlotAxisId QImPlotItemNode::yAxisId() const
{
    QIM_DC(d);
    return toQImPlotAxisId(d->yAxisId);
}

/**
 * \if ENGLISH
 * @brief Gets the parent plot node by traversing up the object tree
 * @return Pointer to QImPlotNode if found, nullptr otherwise
 * @details Searches ancestors for the first QImPlotNode type. Used to access plot-level properties.
 * \endif
 *
 * \if CHINESE
 * @brief 通过遍历对象树获取父绘图节点
 * @return 找到时返回 QImPlotNode 指针，否则返回 nullptr
 * @details 在祖先中搜索第一个 QImPlotNode 类型。用于访问绘图级别的属性。
 * \endif
 */
QImPlotNode* QImPlotItemNode::plotNode() const
{
    QImAbstractNode* parent = parentNode();

    while (parent != nullptr) {
        if (auto* plotNode = qobject_cast< QImPlotNode* >(parent)) {
            return plotNode;
        }
        parent = parent->parentNode();
    }

    return nullptr;
}

/**
 * \if ENGLISH
 * @brief Gets the current ImPlot-assigned item color
 * @return QColor of the item, or invalid QColor() if not yet rendered
 * @details Returns the color assigned by ImPlot during rendering. Only valid after beginDraw().
 * \endif
 *
 * \if CHINESE
 * @brief 获取当前 ImPlot 分配的项颜色
 * @return 项的 QColor，未渲染时返回无效的 QColor()
 * @details 返回 ImPlot 在渲染期间分配的颜色。仅在 beginDraw() 之后有效。
 * \endif
 */
QColor QImPlotItemNode::itemColor() const
{
    QIM_DC(d);
    if (d->plotItem) {
        return toQColor(d->plotItem->Color);
    }
    return QColor();
}

/**
 * \if ENGLISH
 * @brief Checks if the legend entry for this item is hovered
 * @return true if legend entry is currently hovered by mouse
 * @details Only valid after rendering. Returns false if ImPlotItem pointer is null.
 * \endif
 *
 * \if CHINESE
 * @brief 检查此项的图例条目是否被悬停
 * @return true 表示图例条目当前被鼠标悬停
 * @details 仅在渲染后有效。ImPlotItem 指针为空时返回 false。
 * \endif
 */
bool QImPlotItemNode::isLegendHovered() const
{
    QIM_DC(d);
    if (d->plotItem) {
        return d->plotItem->LegendHovered;
    }
    return false;
}

/**
 * \if ENGLISH
 * @brief Checks if the item is visible in the plot
 * @return true if item is visible (ImPlotItem::Show is true), false if hidden or not yet rendered
 * @details After rendering, returns the ImPlot internal visibility state.
 *          Before rendering, returns false. Visibility can be toggled via legend click.
 * \endif
 *
 * \if CHINESE
 * @brief 检查项是否在绘图中可见
 * @return true 表示项可见（ImPlotItem::Show 为 true），false 表示隐藏或尚未渲染
 * @details 渲染后返回 ImPlot 内部的可见性状态。
 *          渲染前返回 false。可见性可通过图例点击切换。
 * \endif
 */
bool QImPlotItemNode::isVisible() const
{
    QIM_DC(d);
    if (d->plotItem) {
        // 渲染后返回 ImPlot 的实际状态
        return d->plotItem->Show;
    }
    return false;
}

/**
 * \if ENGLISH
 * @brief Sets item visibility and synchronizes with ImPlot
 * @param[in] visible true to show the item, false to hide
 * @details After rendering, synchronizes visibility to ImPlotItem::Show.
 *          Also calls QImAbstractNode::setVisible() to update node tree and emit signals.
 * \endif
 *
 * \if CHINESE
 * @brief 设置项可见性并同步到 ImPlot
 * @param[in] visible true 显示项，false 隐藏
 * @details 渲染后将可见性同步到 ImPlotItem::Show。
 *          同时调用 QImAbstractNode::setVisible() 更新节点树并触发信号。
 * \endif
 */
void QImPlotItemNode::setVisible(bool visible)
{
    QIM_D(d);
    if (d->plotItem) {
        // 渲染后同步到 ImPlotItem
        d->plotItem->Show = visible;
        // 此函数同步根节点的可见性状态，同时会触发信号
        QImAbstractNode::setVisible(visible);
    }
}

/**
 * \if ENGLISH
 * @brief Calculate optimal target points from plot pixel width
 * @param pixelWidth Plot width in pixels (from ImPlot::GetPlotSize().x)
 * @return Clamped target point count: max(100, min(pixelWidth * kPixelToPointRatio, 10000))
 * \endif
 *
 * \if CHINESE
 * @brief 根据绘图像素宽度计算最佳降采样目标点数
 * @param pixelWidth 以像素为单位的绘图宽度（来自 ImPlot::GetPlotSize().x）
 * @return 钳位后的目标点数：max(100, min(pixelWidth * kPixelToPointRatio, 10000))
 * \endif
 */
int QImPlotItemNode::pixelAwareTargetPoints(int pixelWidth)
{
    return std::clamp(static_cast<int>(pixelWidth * kPixelToPointRatio), 100, 10000);
}

void QImPlotItemNode::endDraw()
{
}

/**
 * \if ENGLISH
 * @brief Gets the underlying ImPlotItem pointer
 * @return ImPlotItem pointer if available, nullptr otherwise
 * @details Provides direct access to ImPlot's internal item representation for advanced usage.
 * \endif
 *
 * \if CHINESE
 * @brief 获取底层 ImPlotItem 指针
 * @return ImPlotItem 指针（可用时），否则返回 nullptr
 * @details 提供 ImPlot 内部项表示的直接访问，供高级用户使用。
 * \endif
 */
ImPlotItem* QImPlotItemNode::imPlotItem() const
{
    QIM_DC(d);
    return d->plotItem;
}

/**
 * \if ENGLISH
 * @brief Records the ImPlotItem pointer after rendering
 * @param[in] item ImPlotItem pointer obtained from ImPlot context after PlotXXX() call
 * @details Stores the ImPlot item pointer for visibility synchronization and color retrieval.
 *          Skips if the pointer is unchanged.
 * \endif
 *
 * \if CHINESE
 * @brief 渲染后记录 ImPlotItem 指针
 * @param[in] item PlotXXX() 调用后从 ImPlot 上下文获取的 ImPlotItem 指针
 * @details 存储 ImPlot 项指针，用于可见性同步和颜色获取。
 *          指针未改变时跳过。
 * \endif
 */
void QImPlotItemNode::setImPlotItem(ImPlotItem* item)
{
    QIM_D(d);
    if (d->plotItem == item) {
        return;
    }
    d->plotItem = item;
}

}  // end namespace QIM
