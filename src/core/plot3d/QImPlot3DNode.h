#ifndef QIMPLOT3DNODE_H
#define QIMPLOT3DNODE_H

#include "QImAbstractNode.h"
#include <QSizeF>
#include "QImPlot3D.h"

namespace QIM
{

class QImPlot3DAxisInfo;
class QImPlot3DItemNode;
class QImPlot3DLineItemNode;
class QImPlot3DScatterItemNode;

/**
 * \if ENGLISH
 * @brief ImPlot3D plot container node managing BeginPlot/EndPlot scope and 3D plot configuration
 * @details This class wraps ImPlot3D::BeginPlot/EndPlot and provides:
 *          - 3 axis management (X, Y, Z) via QImPlot3DAxisInfo
 *          - 3D rotation and scale configuration
 *          - Child item management (lines, scatter, surfaces, etc.)
 *          - All ImPlot3DFlags exposed as Qt properties with positive semantics
 *          Render flow (strictly following ImPlot3D constraints):
 *          1. BeginPlot() - creates plot context
 *          2. SetupAxis/SetupAxisLimits/SetupBoxRotation/SetupBoxScale - must be after BeginPlot
 *          3. Child item rendering
 *          4. EndPlot()
 * \endif
 *
 * \if CHINESE
 * @brief ImPlot3D 绘图区域节点，管理 BeginPlot/EndPlot 作用域及 3D 绘图配置
 * @details 此类封装 ImPlot3D::BeginPlot/EndPlot，提供：
 *          - 3 坐标轴管理（X、Y、Z）通过 QImPlot3DAxisInfo
 *          - 3D 旋转和缩放配置
 *          - 子元素管理（线条、散点、曲面等）
 *          - 所有 ImPlot3DFlags 以 Qt 属性形式暴露，使用肯定语义
 *          渲染流程（严格遵循 ImPlot3D 约束）：
 *          1. BeginPlot() - 创建绘图上下文
 *          2. SetupAxis/SetupAxisLimits/SetupBoxRotation/SetupBoxScale - 必须在 BeginPlot 之后
 *          3. 子元素渲染
 *          4. EndPlot()
 * \endif
 */
class QIM_CORE_API QImPlot3DNode : public QImAbstractNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlot3DNode)

    // === Title property ===
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)

    // === Size property ===
    Q_PROPERTY(QSizeF size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(bool autoSize READ isAutoSize WRITE setAutoSize NOTIFY autoSizeChanged)

    // ImPlot3DFlags - Negative to Positive semantic (NoXxx -> xxxEnabled)
    Q_PROPERTY(bool titleEnabled READ isTitleEnabled WRITE setTitleEnabled NOTIFY plot3DFlagChanged)
    Q_PROPERTY(bool legendEnabled READ isLegendEnabled WRITE setLegendEnabled NOTIFY plot3DFlagChanged)
    Q_PROPERTY(bool mouseTextEnabled READ isMouseTextEnabled WRITE setMouseTextEnabled NOTIFY plot3DFlagChanged)
    Q_PROPERTY(bool clipEnabled READ isClipEnabled WRITE setClipEnabled NOTIFY plot3DFlagChanged)
    Q_PROPERTY(bool menusEnabled READ isMenusEnabled WRITE setMenusEnabled NOTIFY plot3DFlagChanged)
    Q_PROPERTY(bool rotateEnabled READ isRotateEnabled WRITE setRotateEnabled NOTIFY plot3DFlagChanged)
    Q_PROPERTY(bool panEnabled READ isPanEnabled WRITE setPanEnabled NOTIFY plot3DFlagChanged)
    Q_PROPERTY(bool zoomEnabled READ isZoomEnabled WRITE setZoomEnabled NOTIFY plot3DFlagChanged)
    Q_PROPERTY(bool inputsEnabled READ isInputsEnabled WRITE setInputsEnabled NOTIFY plot3DFlagChanged)

    // ImPlot3DFlags - Positive to Positive semantic (direct mapping)
    Q_PROPERTY(bool equal READ isEqual WRITE setEqual NOTIFY plot3DFlagChanged)

    // Combined flags
    Q_PROPERTY(bool canvasEnabled READ isCanvasEnabled WRITE setCanvasEnabled NOTIFY plot3DFlagChanged)

    Q_DISABLE_COPY(QImPlot3DNode)

public:
    // Constructor with optional parent
    explicit QImPlot3DNode(QObject* parent = nullptr);

    // Constructor with title and optional parent
    explicit QImPlot3DNode(const QString& title, QObject* parent = nullptr);

    // Destructor
    ~QImPlot3DNode() override;

    //----------------------------------------------------
    // Title
    //----------------------------------------------------

    // Gets the plot title text
    QString title() const;

    // Sets the plot title text
    void setTitle(const QString& title);

    //----------------------------------------------------
    // Size
    //----------------------------------------------------

    // Gets the plot frame size
    QSizeF size() const;

    // Sets the plot frame size
    void setSize(const QSizeF& size);

    // Checks if auto-size mode is enabled
    bool isAutoSize() const;

    // Enables or disables auto-size mode
    void setAutoSize(bool enabled);

    //----------------------------------------------------
    // Axis management (3 axes: X, Y, Z)
    //----------------------------------------------------

    // Gets axis info by axis ID
    QImPlot3DAxisInfo* axisInfo(QImPlot3DAxisId aid) const;

    // Gets the X axis info
    QImPlot3DAxisInfo* xAxis() const;

    // Gets the Y axis info
    QImPlot3DAxisInfo* yAxis() const;

    // Gets the Z axis info
    QImPlot3DAxisInfo* zAxis() const;

    //----------------------------------------------------
    // ImPlot3DFlags accessors
    //----------------------------------------------------

    // Negative to Positive semantic (NoXxx -> xxxEnabled)
    bool isTitleEnabled() const;
    void setTitleEnabled(bool enabled);

    bool isLegendEnabled() const;
    void setLegendEnabled(bool enabled);

    bool isMouseTextEnabled() const;
    void setMouseTextEnabled(bool enabled);

    bool isClipEnabled() const;
    void setClipEnabled(bool enabled);

    bool isMenusEnabled() const;
    void setMenusEnabled(bool enabled);

    bool isRotateEnabled() const;
    void setRotateEnabled(bool enabled);

    bool isPanEnabled() const;
    void setPanEnabled(bool enabled);

    bool isZoomEnabled() const;
    void setZoomEnabled(bool enabled);

    bool isInputsEnabled() const;
    void setInputsEnabled(bool enabled);

    // Positive to Positive semantic (direct mapping)
    bool isEqual() const;
    void setEqual(bool on);

    // Combined flags
    bool isCanvasEnabled() const;
    void setCanvasEnabled(bool enabled);

    // Raw flags access
    int imPlot3DFlags() const;
    void setImPlot3DFlags(int flags);

    //----------------------------------------------------
    // 3D rotation and scale
    //----------------------------------------------------

    // Sets the plot box rotation angles
    void setBoxRotation(double elevation, double azimuth, bool animate = false, QImPlot3DCondition cond = QImPlot3DCondition::Once);

    // Sets the plot box initial rotation (for double-click reset)
    void setBoxInitialRotation(double elevation, double azimuth);

    // Sets the plot box scale factors
    void setBoxScale(double x, double y, double z);

    //----------------------------------------------------
    // Item management
    //----------------------------------------------------

    // Adds a 3D plot item as child
    void addPlot3DItem(QImPlot3DItemNode* item);

    // Gets all 3D plot item children
    QList<QImPlot3DItemNode*> plot3DItemNodes() const;

    //----------------------------------------------------
    // Quick add template methods
    //----------------------------------------------------

    // Quick add a line plot item
    template<typename CX, typename CY, typename CZ>
    QImPlot3DLineItemNode* addLine(const CX& x, const CY& y, const CZ& z, const QString& label);

    // Quick add a scatter plot item
    template<typename CX, typename CY, typename CZ>
    QImPlot3DScatterItemNode* addScatter(const CX& x, const CY& y, const CZ& z, const QString& label);

    //----------------------------------------------------
    // Interaction query
    //----------------------------------------------------

    // Checks if plot is hovered by mouse
    bool isPlot3DHovered() const;

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when title changes
     * @param title New title text
     * \endif
     *
     * \if CHINESE
     * @brief 标题改变时发射
     * @param title 新标题文本
     * \endif
     */
    void titleChanged(const QString& title);

    /**
     * \if ENGLISH
     * @brief Emitted when size changes
     * @param size New size
     * \endif
     *
     * \if CHINESE
     * @brief 尺寸改变时发射
     * @param size 新尺寸
     * \endif
     */
    void sizeChanged(const QSizeF& size);

    /**
     * \if ENGLISH
     * @brief Emitted when auto-size mode changes
     * @param enabled New auto-size state
     * \endif
     *
     * \if CHINESE
     * @brief 自动尺寸模式改变时发射
     * @param enabled 新的自动尺寸状态
     * \endif
     */
    void autoSizeChanged(bool enabled);

    /**
     * \if ENGLISH
     * @brief Emitted when any ImPlot3D flag changes
     * \endif
     *
     * \if CHINESE
     * @brief 任意 ImPlot3D 标志变更时发射
     * \endif
     */
    void plot3DFlagChanged();

protected:
    // Begins the 3D plot rendering scope
    bool beginDraw() override;

    // Ends the 3D plot rendering scope
    void endDraw() override;
};

//===============================================================
// Template implementations
//===============================================================

template<typename CX, typename CY, typename CZ>
inline QImPlot3DLineItemNode* QImPlot3DNode::addLine(const CX& x, const CY& y, const CZ& z, const QString& label)
{
    auto* item = new QImPlot3DLineItemNode(this);
    item->setLabel(label);
    item->setData(x, y, z);
    addPlot3DItem(item);
    return item;
}

template<typename CX, typename CY, typename CZ>
inline QImPlot3DScatterItemNode* QImPlot3DNode::addScatter(const CX& x, const CY& y, const CZ& z, const QString& label)
{
    auto* item = new QImPlot3DScatterItemNode(this);
    item->setLabel(label);
    item->setData(x, y, z);
    addPlot3DItem(item);
    return item;
}

}  // namespace QIM

#endif  // QIMPLOT3DNODE_H