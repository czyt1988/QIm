#include "QImPlot3DMeshItemNode.h"
#include "QImAPI.h"
#include "QImPlot3DTypes.h"
#include "QtImGuiUtils.h"
#include "implot3d.h"
#include "implot3d_internal.h"
#include <QVector>

namespace QIM
{

// ===============================================================
// PrivateData Implementation
// ===============================================================

class QImPlot3DMeshItemNode::PrivateData
{
    QIM_DECLARE_PUBLIC(QImPlot3DMeshItemNode)

public:
    explicit PrivateData(QImPlot3DMeshItemNode* q) : q_ptr(q) {}

    std::vector<QImPlot3DPoint> vertices;
    std::vector<unsigned int> indices;
    int flags{0};
    int markerShape{ImPlot3DMarker_None};
    float markerSize{4.0f};
    float markerWeight{1.0f};
    QImOptional3DColor fillColor;
    QImOptional3DColor lineColor;
    QImOptional3DColor markerFillColor;
    QImOptional3DColor markerOutlineColor;
    float lineWidth{1.0f};
    float fillAlpha{-1.0f};  // IMPLOT3D_AUTO
    // Per-triangle color storage (ImVec4 for SetNextFillStyle/SetNextLineStyle)
    QVector<ImVec4> m_fillColorsVec4;
    QVector<ImVec4> m_lineColorsVec4;
    // Pre-resolved colors (cycled/truncated) to exactly m_triCount entries
    QVector<ImVec4> m_resolvedFillColors;
    QVector<ImVec4> m_resolvedLineColors;
    // Per-triangle hidden labels (_##tri_N suffix, hidden from legend)
    QVector<QByteArray> m_triLabels;
    // Flat array: 3 ImPlot3DPoint per triangle (tri0: [0,1,2], tri1: [3,4,5])
    QVector<ImPlot3DPoint> m_triVertices;
    int m_triCount{0};
    bool m_perTriangleFillColorEnabled{false};
    bool m_perTriangleLineColorEnabled{false};
    bool m_subMeshDataDirty{true};

    // Pre-compute all sub-mesh data (labels, vertices, resolved colors)
    void recomputeSubMeshData();
};

// ===============================================================
// PrivateData::recomputeSubMeshData
// ===============================================================

void QImPlot3DMeshItemNode::PrivateData::recomputeSubMeshData()
{
    QIM_Q(QImPlot3DMeshItemNode, q);
    m_triCount = static_cast<int>(indices.size() / 3);
    m_triLabels.resize(m_triCount);
    m_triVertices.resize(m_triCount * 3);
    m_resolvedFillColors.resize(m_triCount);
    m_resolvedLineColors.resize(m_triCount);

    const QByteArray baseLabel = q->labelConstData();
    for (int i = 0; i < m_triCount; ++i) {
        // Per-triangle hidden label: base_##tri_N (ImGui ## convention hides from legend)
        m_triLabels[i] = baseLabel + "_##tri_" + QByteArray::number(i);
        // Extract 3 vertices for this triangle using original indices
        m_triVertices[i * 3 + 0] = ImPlot3DPoint(vertices[indices[i * 3 + 0]].x,
                                                   vertices[indices[i * 3 + 0]].y,
                                                   vertices[indices[i * 3 + 0]].z);
        m_triVertices[i * 3 + 1] = ImPlot3DPoint(vertices[indices[i * 3 + 1]].x,
                                                   vertices[indices[i * 3 + 1]].y,
                                                   vertices[indices[i * 3 + 1]].z);
        m_triVertices[i * 3 + 2] = ImPlot3DPoint(vertices[indices[i * 3 + 2]].x,
                                                   vertices[indices[i * 3 + 2]].y,
                                                   vertices[indices[i * 3 + 2]].z);
        // Resolve fill color: cycle colors when count < triangle count
        if (!m_fillColorsVec4.isEmpty()) {
            m_resolvedFillColors[i] = m_fillColorsVec4[i % m_fillColorsVec4.size()];
            // Apply fillAlpha if explicitly set (not auto)
            if (fillAlpha >= 0.0f) {
                m_resolvedFillColors[i].w = fillAlpha;
            }
        } else {
            m_resolvedFillColors[i] = IMPLOT3D_AUTO_COL;
        }
        // Resolve line color: cycle colors when count < triangle count
        if (!m_lineColorsVec4.isEmpty()) {
            m_resolvedLineColors[i] = m_lineColorsVec4[i % m_lineColorsVec4.size()];
        } else {
            m_resolvedLineColors[i] = IMPLOT3D_AUTO_COL;
        }
    }
    m_subMeshDataDirty = false;
}

// ===============================================================
// QImPlot3DMeshItemNode Implementation
// ===============================================================

QImPlot3DMeshItemNode::QImPlot3DMeshItemNode(QObject* parent)
    : QImPlot3DItemNode(parent)
    , QIM_PIMPL_CONSTRUCT
{
}

QImPlot3DMeshItemNode::~QImPlot3DMeshItemNode()
{
}

void QImPlot3DMeshItemNode::setMeshData(const std::vector<QImPlot3DPoint>& vertices, const std::vector<unsigned int>& indices)
{
    QIM_D(d);
    d->vertices = vertices;
    d->indices = indices;
    d->m_subMeshDataDirty = true;
    Q_EMIT dataChanged();
}

const std::vector<QImPlot3DPoint>& QImPlot3DMeshItemNode::vertices() const
{
    QIM_DC(d);
    return d->vertices;
}

const std::vector<unsigned int>& QImPlot3DMeshItemNode::indices() const
{
    QIM_DC(d);
    return d->indices;
}

QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DMeshItemNode, LinesVisible, ImPlot3DMeshFlags_NoLines, meshFlagChanged)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DMeshItemNode, FillVisible, ImPlot3DMeshFlags_NoFill, meshFlagChanged)
QIMPLOT3D_FLAG_ENABLED_ACCESSOR(QImPlot3DMeshItemNode, MarkersVisible, ImPlot3DMeshFlags_NoMarkers, meshFlagChanged)

int QImPlot3DMeshItemNode::markerShape() const
{
    QIM_DC(d);
    return d->markerShape;
}

void QImPlot3DMeshItemNode::setMarkerShape(int shape)
{
    QIM_D(d);
    if (d->markerShape != shape) {
        d->markerShape = shape;
        Q_EMIT markerShapeChanged(shape);
    }
}

float QImPlot3DMeshItemNode::markerSize() const
{
    QIM_DC(d);
    return d->markerSize;
}

void QImPlot3DMeshItemNode::setMarkerSize(float size)
{
    QIM_D(d);
    if (!qFuzzyCompare(d->markerSize, size)) {
        d->markerSize = size;
        Q_EMIT markerStyleChanged();
    }
}

float QImPlot3DMeshItemNode::markerWeight() const
{
    QIM_DC(d);
    return d->markerWeight;
}

void QImPlot3DMeshItemNode::setMarkerWeight(float weight)
{
    QIM_D(d);
    if (!qFuzzyCompare(d->markerWeight, weight)) {
        d->markerWeight = weight;
        Q_EMIT markerStyleChanged();
    }
}

/**
 * \if ENGLISH
 * @brief Returns the fill color, or invalid QColor if not set
 * @details When no fill color has been explicitly set, returns an invalid QColor().
 *          After the first render, unset colors capture the ImPlot3D default.
 * \endif
 *
 * \if CHINESE
 * @brief 返回填充颜色，若未设置则返回无效QColor
 * @details 当未显式设置填充颜色时，返回无效的QColor()。
 *          首次渲染后，未设置的颜色会捕获ImPlot3D默认值。
 * \endif
 */
QColor QImPlot3DMeshItemNode::fillColor() const
{
    QIM_DC(d);
    return (d->fillColor.has_value()) ? toQColor(d->fillColor->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Sets the fill color and emits fillColorChanged
 * @param[in] color The new fill color
 * @details Stores the color as QImOptional3DColor for lazy initialization.
 *          Always emits fillColorChanged to satisfy Q_PROPERTY NOTIFY.
 * \endif
 *
 * \if CHINESE
 * @brief 设置填充颜色并发射fillColorChanged信号
 * @param[in] color 新的填充颜色
 * @details 将颜色存储为QImOptional3DColor以支持延迟初始化。
 *          总是发射fillColorChanged以满足Q_PROPERTY NOTIFY。
 * \endif
 */
void QImPlot3DMeshItemNode::setFillColor(const QColor& color)
{
    QIM_D(d);
    d->fillColor = toImVec4(color);
    Q_EMIT fillColorChanged(color);
}

/**
 * \if ENGLISH
 * @brief Returns the line color, or invalid QColor if not set
 * @details When no line color has been explicitly set, returns an invalid QColor().
 *          After the first render, unset colors capture the ImPlot3D default.
 * \endif
 *
 * \if CHINESE
 * @brief 返回线条颜色，若未设置则返回无效QColor
 * @details 当未显式设置线条颜色时，返回无效的QColor()。
 *          首次渲染后，未设置的颜色会捕获ImPlot3D默认值。
 * \endif
 */
QColor QImPlot3DMeshItemNode::lineColor() const
{
    QIM_DC(d);
    return (d->lineColor.has_value()) ? toQColor(d->lineColor->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Sets the line color and emits lineColorChanged
 * @param[in] color The new line color
 * @details Stores the color as QImOptional3DColor for lazy initialization.
 *          Always emits lineColorChanged to satisfy Q_PROPERTY NOTIFY.
 * \endif
 *
 * \if CHINESE
 * @brief 设置线条颜色并发射lineColorChanged信号
 * @param[in] color 新的线条颜色
 * @details 将颜色存储为QImOptional3DColor以支持延迟初始化。
 *          总是发射lineColorChanged以满足Q_PROPERTY NOTIFY。
 * \endif
 */
void QImPlot3DMeshItemNode::setLineColor(const QColor& color)
{
    QIM_D(d);
    d->lineColor = toImVec4(color);
    Q_EMIT lineColorChanged(color);
}

/**
 * \if ENGLISH
 * @brief Returns the marker fill color, or invalid QColor if not set
 * @details When no marker fill color has been explicitly set, returns an invalid QColor().
 *          After the first render, unset colors capture the ImPlot3D default.
 * \endif
 *
 * \if CHINESE
 * @brief 返回标记填充颜色，若未设置则返回无效QColor
 * @details 当未显式设置标记填充颜色时，返回无效的QColor()。
 *          首次渲染后，未设置的颜色会捕获ImPlot3D默认值。
 * \endif
 */
QColor QImPlot3DMeshItemNode::markerFillColor() const
{
    QIM_DC(d);
    return (d->markerFillColor.has_value()) ? toQColor(d->markerFillColor->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Sets the marker fill color and emits markerFillColorChanged
 * @param[in] color The new marker fill color
 * @details Stores the color as QImOptional3DColor for lazy initialization.
 *          Always emits markerFillColorChanged to satisfy Q_PROPERTY NOTIFY.
 * \endif
 *
 * \if CHINESE
 * @brief 设置标记填充颜色并发射markerFillColorChanged信号
 * @param[in] color 新的标记填充颜色
 * @details 将颜色存储为QImOptional3DColor以支持延迟初始化。
 *          总是发射markerFillColorChanged以满足Q_PROPERTY NOTIFY。
 * \endif
 */
void QImPlot3DMeshItemNode::setMarkerFillColor(const QColor& color)
{
    QIM_D(d);
    d->markerFillColor = toImVec4(color);
    Q_EMIT markerFillColorChanged(color);
}

/**
 * \if ENGLISH
 * @brief Returns the marker outline color, or invalid QColor if not set
 * @details When no marker outline color has been explicitly set, returns an invalid QColor().
 *          After the first render, unset colors capture the ImPlot3D default.
 * \endif
 *
 * \if CHINESE
 * @brief 返回标记轮廓颜色，若未设置则返回无效QColor
 * @details 当未显式设置标记轮廓颜色时，返回无效的QColor()。
 *          首次渲染后，未设置的颜色会捕获ImPlot3D默认值。
 * \endif
 */
QColor QImPlot3DMeshItemNode::markerOutlineColor() const
{
    QIM_DC(d);
    return (d->markerOutlineColor.has_value()) ? toQColor(d->markerOutlineColor->value()) : QColor();
}

/**
 * \if ENGLISH
 * @brief Sets the marker outline color and emits markerOutlineColorChanged
 * @param[in] color The new marker outline color
 * @details Stores the color as QImOptional3DColor for lazy initialization.
 *          Always emits markerOutlineColorChanged to satisfy Q_PROPERTY NOTIFY.
 * \endif
 *
 * \if CHINESE
 * @brief 设置标记轮廓颜色并发射markerOutlineColorChanged信号
 * @param[in] color 新的标记轮廓颜色
 * @details 将颜色存储为QImOptional3DColor以支持延迟初始化。
 *          总是发射markerOutlineColorChanged以满足Q_PROPERTY NOTIFY。
 * \endif
 */
void QImPlot3DMeshItemNode::setMarkerOutlineColor(const QColor& color)
{
    QIM_D(d);
    d->markerOutlineColor = toImVec4(color);
    Q_EMIT markerOutlineColorChanged(color);
}

float QImPlot3DMeshItemNode::lineWidth() const
{
    QIM_DC(d);
    return d->lineWidth;
}

void QImPlot3DMeshItemNode::setLineWidth(float width)
{
    QIM_D(d);
    if (!qFuzzyCompare(d->lineWidth, width)) {
        d->lineWidth = width;
        Q_EMIT lineWidthChanged(width);
    }
}

/**
 * \if ENGLISH
 * @brief Returns the current fill alpha value
 * @return Fill alpha value (0.0 to 1.0, or -1.0 for auto)
 * @details Returns the fill transparency value. A value of -1.0 indicates
 *          IMPLOT3D_AUTO, meaning the default style value should be used.
 * \endif
 *
 * \if CHINESE
 * @brief 返回当前填充透明度值
 * @return 填充透明度值（0.0到1.0，或-1.0表示自动）
 * @details 返回填充透明度值。-1.0表示IMPLOT3D_AUTO，应使用默认样式值。
 * \endif
 */
float QImPlot3DMeshItemNode::fillAlpha() const
{
    QIM_DC(d);
    return d->fillAlpha;
}

/**
 * \if ENGLISH
 * @brief Sets the fill alpha value
 * @param[in] alpha Fill alpha value (0.0 to 1.0, or -1.0 for auto)
 * @details Sets the fill transparency value. A value of -1.0 indicates
 *          IMPLOT3D_AUTO, meaning the default style value should be used.
 *          Values outside the valid range are clamped to [0.0, 1.0].
 * \endif
 *
 * \if CHINESE
 * @brief 设置填充透明度值
 * @param[in] alpha 填充透明度值（0.0到1.0，或-1.0表示自动）
 * @details 设置填充透明度值。-1.0表示IMPLOT3D_AUTO，应使用默认样式值。
 *          超出有效范围的值将被限制在[0.0, 1.0]范围内。
 * \endif
 */
void QImPlot3DMeshItemNode::setFillAlpha(float alpha)
{
    QIM_D(d);
    // Clamp to valid range, but allow -1.0 for IMPLOT3D_AUTO
    float clampedAlpha = alpha;
    if (alpha >= 0.0f) {
        // Only clamp non-negative values (auto is -1.0)
        clampedAlpha = qBound(0.0f, alpha, 1.0f);
    }
    
    if (!qFuzzyCompare(d->fillAlpha, clampedAlpha)) {
        d->fillAlpha = clampedAlpha;
        d->m_subMeshDataDirty = true;
        Q_EMIT fillAlphaChanged(clampedAlpha);
    }
}

/**
 * \if ENGLISH
 * @brief Checks if per-triangle fill coloring is enabled
 * @return True if per-triangle fill coloring is enabled
 * \endif
 *
 * \if CHINESE
 * @brief 检查逐三角面填充颜色是否启用
 * @return 逐三角面填充颜色启用时返回true
 * \endif
 */
bool QImPlot3DMeshItemNode::isPerTriangleFillColorEnabled() const
{
    QIM_DC(d);
    return d->m_perTriangleFillColorEnabled;
}

/**
 * \if ENGLISH
 * @brief Enables or disables per-triangle fill coloring
 * @param[in] enabled True to enable per-triangle fill coloring
 * @details When enabled, the mesh uses per-triangle fill colors from fillColors()
 *          instead of the single fillColor property.
 * \endif
 *
 * \if CHINESE
 * @brief 启用或禁用逐三角面填充颜色
 * @param[in] enabled true启用逐三角面填充颜色
 * @details 启用时，网格使用fillColors()提供的逐面填充颜色，
 *          而非单一的fillColor属性。
 * \endif
 */
void QImPlot3DMeshItemNode::setPerTriangleFillColorEnabled(bool enabled)
{
    QIM_D(d);
    if (d->m_perTriangleFillColorEnabled != enabled) {
        d->m_perTriangleFillColorEnabled = enabled;
        d->m_subMeshDataDirty = true;
        Q_EMIT perTriangleFillColorEnabledChanged(enabled);
    }
}

/**
 * \if ENGLISH
 * @brief Checks if per-triangle line coloring is enabled
 * @return True if per-triangle line coloring is enabled
 * \endif
 *
 * \if CHINESE
 * @brief 检查逐三角面线条颜色是否启用
 * @return 逐三角面线条颜色启用时返回true
 * \endif
 */
bool QImPlot3DMeshItemNode::isPerTriangleLineColorEnabled() const
{
    QIM_DC(d);
    return d->m_perTriangleLineColorEnabled;
}

/**
 * \if ENGLISH
 * @brief Enables or disables per-triangle line coloring
 * @param[in] enabled True to enable per-triangle line coloring
 * @details When enabled, the mesh uses per-triangle line colors from lineColors()
 *          instead of the single lineColor property.
 * \endif
 *
 * \if CHINESE
 * @brief 启用或禁用逐三角面线条颜色
 * @param[in] enabled true启用逐三角面线条颜色
 * @details 启用时，网格使用lineColors()提供的逐面线条颜色，
 *          而非单一的lineColor属性。
 * \endif
 */
void QImPlot3DMeshItemNode::setPerTriangleLineColorEnabled(bool enabled)
{
    QIM_D(d);
    if (d->m_perTriangleLineColorEnabled != enabled) {
        d->m_perTriangleLineColorEnabled = enabled;
        d->m_subMeshDataDirty = true;
        Q_EMIT perTriangleLineColorEnabledChanged(enabled);
    }
}

/**
 * \if ENGLISH
 * @brief Sets per-triangle fill colors from a list of QColor
 * @param[in] colors List of colors, one per triangle (should match idx_count/3)
 * @details Converts each QColor to ImVec4 internally.
 *          The number of colors should match the number of triangles.
 * \endif
 *
 * \if CHINESE
 * @brief 从QColor列表设置逐三角面填充颜色
 * @param[in] colors 颜色列表，每个三角面一个（应与idx_count/3匹配）
 * @details 内部将每个QColor转换为ImVec4。
 *          颜色数量应与三角面数量匹配。
 * \endif
 */
void QImPlot3DMeshItemNode::setFillColors(const QList<QColor>& colors)
{
    QIM_D(d);
    d->m_fillColorsVec4.clear();
    d->m_fillColorsVec4.reserve(colors.size());
    for (const QColor& c : colors) {
        d->m_fillColorsVec4.append(toImVec4(c));
    }
    d->m_subMeshDataDirty = true;
}

/**
 * \if ENGLISH
 * @brief Returns per-triangle fill colors as a list of QColor
 * @return List of fill colors converted from internal ImVec4 storage
 * \endif
 *
 * \if CHINESE
 * @brief 返回逐三角面填充颜色为QColor列表
 * @return 从内部ImVec4存储转换的填充颜色列表
 * \endif
 */
QList<QColor> QImPlot3DMeshItemNode::fillColors() const
{
    QIM_DC(d);
    QList<QColor> result;
    result.reserve(d->m_fillColorsVec4.size());
    for (const ImVec4& col : d->m_fillColorsVec4) {
        result.append(toQColor(col));
    }
    return result;
}

/**
 * \if ENGLISH
 * @brief Sets per-triangle line colors from a list of QColor
 * @param[in] colors List of colors, one per triangle (should match idx_count/3)
 * @details Converts each QColor to ImVec4 internally.
 *          The number of colors should match the number of triangles.
 * \endif
 *
 * \if CHINESE
 * @brief 从QColor列表设置逐三角面线条颜色
 * @param[in] colors 颜色列表，每个三角面一个（应与idx_count/3匹配）
 * @details 内部将每个QColor转换为ImVec4。
 *          颜色数量应与三角面数量匹配。
 * \endif
 */
void QImPlot3DMeshItemNode::setLineColors(const QList<QColor>& colors)
{
    QIM_D(d);
    d->m_lineColorsVec4.clear();
    d->m_lineColorsVec4.reserve(colors.size());
    for (const QColor& c : colors) {
        d->m_lineColorsVec4.append(toImVec4(c));
    }
    d->m_subMeshDataDirty = true;
}

/**
 * \if ENGLISH
 * @brief Returns per-triangle line colors as a list of QColor
 * @return List of line colors converted from internal ImVec4 storage
 * \endif
 *
 * \if CHINESE
 * @brief 返回逐三角面线条颜色为QColor列表
 * @return 从内部ImVec4存储转换的线条颜色列表
 * \endif
 */
QList<QColor> QImPlot3DMeshItemNode::lineColors() const
{
    QIM_DC(d);
    QList<QColor> result;
    result.reserve(d->m_lineColorsVec4.size());
    for (const ImVec4& col : d->m_lineColorsVec4) {
        result.append(toQColor(col));
    }
    return result;
}

int QImPlot3DMeshItemNode::meshFlags() const
{
    QIM_DC(d);
    return d->flags;
}

void QImPlot3DMeshItemNode::setMeshFlags(int flags)
{
    QIM_D(d);
    if (d->flags != flags) {
        d->flags = flags;
        Q_EMIT meshFlagChanged();
    }
}

bool QImPlot3DMeshItemNode::beginDraw()
{
    QIM_D(d);
    if (d->vertices.empty() || d->indices.size() < 3 || (d->indices.size() % 3) != 0) {
        return false;
    }

    const bool perTriFill = d->m_perTriangleFillColorEnabled && !d->m_fillColorsVec4.isEmpty();
    const bool perTriLine = d->m_perTriangleLineColorEnabled && !d->m_lineColorsVec4.isEmpty();

    if (perTriFill || perTriLine) {
        // ── Split path: per-triangle coloring via multiple PlotMesh calls ──
        if (d->m_subMeshDataDirty) {
            d->recomputeSubMeshData();
        }

        static const unsigned int triIdx[3] = {0, 1, 2};

        for (int i = 0; i < d->m_triCount; ++i) {
            // Set per-triangle fill style
            if (perTriFill) {
                ImPlot3D::SetNextFillStyle(d->m_resolvedFillColors[i]);
            } else {
                // Single fill color for all triangles
                if (d->fillColor.has_value()) {
                    ImVec4 fillCol = d->fillColor->value();
                    if (d->fillAlpha >= 0.0f) {
                        fillCol.w = d->fillAlpha;
                    }
                    ImPlot3D::SetNextFillStyle(fillCol);
                }
            }

            // Set per-triangle line style
            if (perTriLine) {
                ImPlot3D::SetNextLineStyle(d->m_resolvedLineColors[i], d->lineWidth);
            } else {
                // Single line color for all triangles
                if (d->lineColor.has_value()) {
                    ImPlot3D::SetNextLineStyle(d->lineColor->value(), d->lineWidth);
                } else {
                    ImPlot3D::SetNextLineStyle(IMPLOT3D_AUTO_COL, d->lineWidth);
                }
            }

            // Skip markers in per-triangle mode (visually wrong on 3-vertex sub-triangles)

            ImPlot3D::PlotMesh(
                d->m_triLabels[i].constData(),
                &d->m_triVertices[i * 3],
                triIdx,
                3,
                3,
                 static_cast<ImPlot3DMeshFlags>(d->flags | ImPlot3DMeshFlags_NoMarkers)
            );
        }
    } else {
        // ── Single-call path: no per-triangle coloring ──
        if (d->fillColor.has_value()) {
            ImVec4 fillColor = d->fillColor->value();
            // Apply fill alpha if explicitly set (not auto)
            if (d->fillAlpha >= 0.0f) {
                fillColor.w = d->fillAlpha;
            }
            ImPlot3D::SetNextFillStyle(fillColor);
        }
        if (d->lineColor.has_value()) {
            ImPlot3D::SetNextLineStyle(d->lineColor->value(), d->lineWidth);
        } else {
            ImPlot3D::SetNextLineStyle(IMPLOT3D_AUTO_COL, d->lineWidth);
        }
        if (d->markerShape != ImPlot3DMarker_None) {
            const ImVec4 fill = d->markerFillColor.has_value() ? d->markerFillColor->value() : IMPLOT3D_AUTO_COL;
            const ImVec4 outline = d->markerOutlineColor.has_value() ? d->markerOutlineColor->value() : IMPLOT3D_AUTO_COL;
            ImPlot3D::SetNextMarkerStyle(static_cast<ImPlot3DMarker>(d->markerShape), d->markerSize, fill, d->markerWeight, outline);
        }

        // Convert QImPlot3DPoint vertices to ImPlot3DPoint for ImPlot3D API
        std::vector<ImPlot3DPoint> impVertices;
        impVertices.reserve(d->vertices.size());
        for (const auto& v : d->vertices) {
            impVertices.push_back(ImPlot3DPoint(v.x, v.y, v.z));
        }

        ImPlot3D::PlotMesh(
            labelConstData(),
            impVertices.data(),
            d->indices.data(),
            static_cast<int>(impVertices.size()),
            static_cast<int>(d->indices.size()),
            static_cast<ImPlot3DMeshFlags>(d->flags)
        );
    }

    // Capture defaults for unset colors
    if (!d->fillColor.has_value()) {
        d->fillColor = captureItemColor();
    }
    if (!d->lineColor.has_value()) {
        d->lineColor = captureItemColor();
    }
    if (!d->markerFillColor.has_value()) {
        d->markerFillColor = captureItemColor();
    }
    if (!d->markerOutlineColor.has_value()) {
        d->markerOutlineColor = captureItemColor();
    }

    return false;
}

// ===============================================================
// Static mesh data accessors
// ===============================================================

/**
 * \if ENGLISH
 * @brief Returns cube vertices as QList<QImPlot3DPoint>
 * @details Converts ImPlot3D::cube_vtx array to QList<QImPlot3DPoint>.
 *          Each ImPlot3DPoint (float) is converted to QImPlot3DPoint (double).
 * \endif
 *
 * \if CHINESE
 * @brief 返回立方体顶点为 QList<QImPlot3DPoint>
 * @details 将 ImPlot3D::cube_vtx 数组转换为 QList<QImPlot3DPoint>。
 *          每个 ImPlot3DPoint (float) 转换为 QImPlot3DPoint (double)。
 * \endif
 */
QList<QImPlot3DPoint> QImPlot3DMeshItemNode::cubeVertices()
{
    QList<QImPlot3DPoint> result;
    result.reserve(ImPlot3D::CUBE_VTX_COUNT);
    for (int i = 0; i < ImPlot3D::CUBE_VTX_COUNT; ++i) {
        result.append(toQImPlot3DPoint(ImPlot3D::cube_vtx[i]));
    }
    return result;
}

/**
 * \if ENGLISH
 * @brief Returns cube indices as QList<unsigned int>
 * @details Converts ImPlot3D::cube_idx array to QList<unsigned int>.
 * \endif
 *
 * \if CHINESE
 * @brief 返回立方体索引为 QList<unsigned int>
 * @details 将 ImPlot3D::cube_idx 数组转换为 QList<unsigned int>。
 * \endif
 */
QList<unsigned int> QImPlot3DMeshItemNode::cubeIndices()
{
    QList<unsigned int> result;
    result.reserve(ImPlot3D::CUBE_IDX_COUNT);
    for (int i = 0; i < ImPlot3D::CUBE_IDX_COUNT; ++i) {
        result.append(ImPlot3D::cube_idx[i]);
    }
    return result;
}

/**
 * \if ENGLISH
 * @brief Returns sphere vertices as QList<QImPlot3DPoint>
 * @details Converts ImPlot3D::sphere_vtx array to QList<QImPlot3DPoint>.
 *          Each ImPlot3DPoint (float) is converted to QImPlot3DPoint (double).
 * \endif
 *
 * \if CHINESE
 * @brief 返回球体顶点为 QList<QImPlot3DPoint>
 * @details 将 ImPlot3D::sphere_vtx 数组转换为 QList<QImPlot3DPoint>。
 *          每个 ImPlot3DPoint (float) 转换为 QImPlot3DPoint (double)。
 * \endif
 */
QList<QImPlot3DPoint> QImPlot3DMeshItemNode::sphereVertices()
{
    QList<QImPlot3DPoint> result;
    result.reserve(ImPlot3D::SPHERE_VTX_COUNT);
    for (int i = 0; i < ImPlot3D::SPHERE_VTX_COUNT; ++i) {
        result.append(toQImPlot3DPoint(ImPlot3D::sphere_vtx[i]));
    }
    return result;
}

/**
 * \if ENGLISH
 * @brief Returns sphere indices as QList<unsigned int>
 * @details Converts ImPlot3D::sphere_idx array to QList<unsigned int>.
 * \endif
 *
 * \if CHINESE
 * @brief 返回球体索引为 QList<unsigned int>
 * @details 将 ImPlot3D::sphere_idx 数组转换为 QList<unsigned int>。
 * \endif
 */
QList<unsigned int> QImPlot3DMeshItemNode::sphereIndices()
{
    QList<unsigned int> result;
    result.reserve(ImPlot3D::SPHERE_IDX_COUNT);
    for (int i = 0; i < ImPlot3D::SPHERE_IDX_COUNT; ++i) {
        result.append(ImPlot3D::sphere_idx[i]);
    }
    return result;
}

/**
 * \if ENGLISH
 * @brief Returns duck vertices as QList<QImPlot3DPoint>
 * @details Converts ImPlot3D::duck_vtx array to QList<QImPlot3DPoint>.
 *          Each ImPlot3DPoint (float) is converted to QImPlot3DPoint (double).
 * \endif
 *
 * \if CHINESE
 * @brief 返回鸭子模型顶点为 QList<QImPlot3DPoint>
 * @details 将 ImPlot3D::duck_vtx 数组转换为 QList<QImPlot3DPoint>。
 *          每个 ImPlot3DPoint (float) 转换为 QImPlot3DPoint (double)。
 * \endif
 */
QList<QImPlot3DPoint> QImPlot3DMeshItemNode::duckVertices()
{
    QList<QImPlot3DPoint> result;
    result.reserve(ImPlot3D::DUCK_VTX_COUNT);
    for (int i = 0; i < ImPlot3D::DUCK_VTX_COUNT; ++i) {
        result.append(toQImPlot3DPoint(ImPlot3D::duck_vtx[i]));
    }
    return result;
}

/**
 * \if ENGLISH
 * @brief Returns duck indices as QList<unsigned int>
 * @details Converts ImPlot3D::duck_idx array to QList<unsigned int>.
 * \endif
 *
 * \if CHINESE
 * @brief 返回鸭子模型索引为 QList<unsigned int>
 * @details 将 ImPlot3D::duck_idx 数组转换为 QList<unsigned int>。
 * \endif
 */
QList<unsigned int> QImPlot3DMeshItemNode::duckIndices()
{
    QList<unsigned int> result;
    result.reserve(ImPlot3D::DUCK_IDX_COUNT);
    for (int i = 0; i < ImPlot3D::DUCK_IDX_COUNT; ++i) {
        result.append(ImPlot3D::duck_idx[i]);
    }
    return result;
}

}  // namespace QIM
