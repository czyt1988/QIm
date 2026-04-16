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
    /**
     * \if ENGLISH
     * @brief Constructor with optional parent
     * @param parent Parent QObject for memory management
     * \endif
     *
     * \if CHINESE
     * @brief 构造函数，可选父对象
     * @param parent 用于内存管理的父 QObject
     * \endif
     */
    explicit QImPlot3DNode(QObject* parent = nullptr);

    /**
     * \if ENGLISH
     * @brief Constructor with title and optional parent
     * @param title Plot title text
     * @param parent Parent QObject for memory management
     * \endif
     *
     * \if CHINESE
     * @brief 构造函数，带标题和可选父对象
     * @param title 绘图标题文本
     * @param parent 用于内存管理的父 QObject
     * \endif
     */
    explicit QImPlot3DNode(const QString& title, QObject* parent = nullptr);

    /**
     * \if ENGLISH
     * @brief Destructor
     * \endif
     *
     * \if CHINESE
     * @brief 析构函数
     * \endif
     */
    ~QImPlot3DNode() override;

    //----------------------------------------------------
    // Title
    //----------------------------------------------------

    /**
     * \if ENGLISH
     * @brief Gets the plot title text
     * @return Current title as QString
     * \endif
     *
     * \if CHINESE
     * @brief 获取绘图标题文本
     * @return 当前标题，QString 格式
     * \endif
     */
    QString title() const;

    /**
     * \if ENGLISH
     * @brief Sets the plot title text
     * @param title New title text
     * \endif
     *
     * \if CHINESE
     * @brief 设置绘图标题文本
     * @param title 新标题文本
     * \endif
     */
    void setTitle(const QString& title);

    //----------------------------------------------------
    // Size
    //----------------------------------------------------

    /**
     * \if ENGLISH
     * @brief Gets the plot frame size
     * @return Current size as QSizeF
     * \endif
     *
     * \if CHINESE
     * @brief 获取绘图框架尺寸
     * @return 当前尺寸，QSizeF 格式
     * \endif
     */
    QSizeF size() const;

    /**
     * \if ENGLISH
     * @brief Sets the plot frame size
     * @param size New size (QSizeF(-1,-1) for auto-size)
     * \endif
     *
     * \if CHINESE
     * @brief 设置绘图框架尺寸
     * @param size 新尺寸（QSizeF(-1,-1) 表示自动尺寸）
     * \endif
     */
    void setSize(const QSizeF& size);

    //----------------------------------------------------
    // Axis management (3 axes: X, Y, Z)
    //----------------------------------------------------

    /**
     * \if ENGLISH
     * @brief Gets axis info by axis ID
     * @param aid Axis identifier (X1, Y1, or Z1)
     * @return Pointer to QImPlot3DAxisInfo, nullptr if invalid
     * \endif
     *
     * \if CHINESE
     * @brief 根据轴 ID 获取坐标轴信息
     * @param aid 轴标识符（X1、Y1 或 Z1）
     * @return QImPlot3DAxisInfo 指针，无效则返回 nullptr
     * \endif
     */
    QImPlot3DAxisInfo* axisInfo(QImPlot3DAxisId aid) const;

    /**
     * \if ENGLISH
     * @brief Gets the X axis info
     * @return Pointer to X-axis QImPlot3DAxisInfo
     * \endif
     *
     * \if CHINESE
     * @brief 获取 X 轴信息
     * @return X 轴 QImPlot3DAxisInfo 指针
     * \endif
     */
    QImPlot3DAxisInfo* xAxis() const;

    /**
     * \if ENGLISH
     * @brief Gets the Y axis info
     * @return Pointer to Y-axis QImPlot3DAxisInfo
     * \endif
     *
     * \if CHINESE
     * @brief 获取 Y 轴信息
     * @return Y 轴 QImPlot3DAxisInfo 指针
     * \endif
     */
    QImPlot3DAxisInfo* yAxis() const;

    /**
     * \if ENGLISH
     * @brief Gets the Z axis info
     * @return Pointer to Z-axis QImPlot3DAxisInfo
     * \endif
     *
     * \if CHINESE
     * @brief 获取 Z 轴信息
     * @return Z 轴 QImPlot3DAxisInfo 指针
     * \endif
     */
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

    /**
     * \if ENGLISH
     * @brief Sets the plot box rotation angles
     * @param elevation Elevation angle in degrees
     * @param azimuth Azimuth angle in degrees
     * @param animate Whether to animate rotation (default false)
     * @param cond Application condition (default Once)
     * \endif
     *
     * \if CHINESE
     * @brief 设置绘图盒子旋转角度
     * @param elevation 仰角（度）
     * @param azimuth 方位角（度）
     * @param animate 是否动画旋转（默认 false）
     * @param cond 应用条件（默认 Once）
     * \endif
     */
    void setBoxRotation(double elevation, double azimuth, bool animate = false, QImPlot3DCondition cond = QImPlot3DCondition::Once);

    /**
     * \if ENGLISH
     * @brief Sets the plot box initial rotation (for double-click reset)
     * @param elevation Initial elevation angle in degrees
     * @param azimuth Initial azimuth angle in degrees
     * \endif
     *
     * \if CHINESE
     * @brief 设置绘图盒子初始旋转（用于双击重置）
     * @param elevation 初始仰角（度）
     * @param azimuth 初始方位角（度）
     * \endif
     */
    void setBoxInitialRotation(double elevation, double azimuth);

    /**
     * \if ENGLISH
     * @brief Sets the plot box scale factors
     * @param x X-axis scale factor (default 1.0)
     * @param y Y-axis scale factor (default 1.0)
     * @param z Z-axis scale factor (default 1.0)
     * \endif
     *
     * \if CHINESE
     * @brief 设置绘图盒子缩放因子
     * @param x X 轴缩放因子（默认 1.0）
     * @param y Y 轴缩放因子（默认 1.0）
     * @param z Z 轴缩放因子（默认 1.0）
     * \endif
     */
    void setBoxScale(double x, double y, double z);

    //----------------------------------------------------
    // Item management
    //----------------------------------------------------

    /**
     * \if ENGLISH
     * @brief Adds a 3D plot item as child
     * @param item Pointer to QImPlot3DItemNode to add
     * \endif
     *
     * \if CHINESE
     * @brief 添加 3D 绘图元素作为子节点
     * @param item 要添加的 QImPlot3DItemNode 指针
     * \endif
     */
    void addPlot3DItem(QImPlot3DItemNode* item);

    /**
     * \if ENGLISH
     * @brief Gets all 3D plot item children
     * @return List of QImPlot3DItemNode pointers
     * \endif
     *
     * \if CHINESE
     * @brief 获取所有 3D 绘图元素子节点
     * @return QImPlot3DItemNode 指针列表
     * \endif
     */
    QList<QImPlot3DItemNode*> plot3DItemNodes() const;

    //----------------------------------------------------
    // Quick add template methods
    //----------------------------------------------------

    /**
     * \if ENGLISH
     * @brief Quick add a line plot item
     * @tparam CX Container type for X coordinates
     * @tparam CY Container type for Y coordinates
     * @tparam CZ Container type for Z coordinates
     * @param x X coordinate data
     * @param y Y coordinate data
     * @param z Z coordinate data
     * @param label Item label for legend
     * @return Pointer to created QImPlot3DLineItemNode
     * \endif
     *
     * \if CHINESE
     * @brief 快速添加线条绘图元素
     * @tparam CX X 坐标容器类型
     * @tparam CY Y 坐标容器类型
     * @tparam CZ Z 坐标容器类型
     * @param x X 坐标数据
     * @param y Y 坐标数据
     * @param z Z 坐标数据
     * @param label 图例中的元素标签
     * @return 创建的 QImPlot3DLineItemNode 指针
     * \endif
     */
    template<typename CX, typename CY, typename CZ>
    QImPlot3DLineItemNode* addLine(const CX& x, const CY& y, const CZ& z, const QString& label);

    /**
     * \if ENGLISH
     * @brief Quick add a scatter plot item
     * @tparam CX Container type for X coordinates
     * @tparam CY Container type for Y coordinates
     * @tparam CZ Container type for Z coordinates
     * @param x X coordinate data
     * @param y Y coordinate data
     * @param z Z coordinate data
     * @param label Item label for legend
     * @return Pointer to created QImPlot3DScatterItemNode
     * \endif
     *
     * \if CHINESE
     * @brief 快速添加散点绘图元素
     * @tparam CX X 坐标容器类型
     * @tparam CY Y 坐标容器类型
     * @tparam CZ Z 坐标容器类型
     * @param x X 坐标数据
     * @param y Y 坐标数据
     * @param z Z 坐标数据
     * @param label 图例中的元素标签
     * @return 创建的 QImPlot3DScatterItemNode 指针
     * \endif
     */
    template<typename CX, typename CY, typename CZ>
    QImPlot3DScatterItemNode* addScatter(const CX& x, const CY& y, const CZ& z, const QString& label);

    //----------------------------------------------------
    // Interaction query
    //----------------------------------------------------

    /**
     * \if ENGLISH
     * @brief Checks if plot is hovered by mouse
     * @return true if mouse is over plot area
     * @note Must be called inside beginDraw/endDraw block
     * \endif
     *
     * \if CHINESE
     * @brief 检查鼠标是否悬停在绘图上
     * @return true 表示鼠标在绘图区域上方
     * @note 必须在 beginDraw/endDraw 块内调用
     * \endif
     */
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
     * @brief Emitted when any ImPlot3D flag changes
     * \endif
     *
     * \if CHINESE
     * @brief 任意 ImPlot3D 标志变更时发射
     * \endif
     */
    void plot3DFlagChanged();

protected:
    /**
     * \if ENGLISH
     * @brief Begins the 3D plot rendering scope
     * @return true if BeginPlot succeeded, false otherwise
     * @details Calls ImPlot3D::BeginPlot, then applies axis setup, rotation, and scale
     * \endif
     *
     * \if CHINESE
     * @brief 开始 3D 绘图渲染作用域
     * @return true 表示 BeginPlot 成功，false 表示失败
     * @details 调用 ImPlot3D::BeginPlot，然后应用坐标轴设置、旋转和缩放
     * \endif
     */
    bool beginDraw() override;

    /**
     * \if ENGLISH
     * @brief Ends the 3D plot rendering scope
     * @details Calls ImPlot3D::EndPlot
     * \endif
     *
     * \if CHINESE
     * @brief 结束 3D 绘图渲染作用域
     * @details 调用 ImPlot3D::EndPlot
     * \endif
     */
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
    // Note: setData method will be implemented in QImPlot3DLineItemNode
    // item->setData(x, y, z);
    addPlot3DItem(item);
    return item;
}

template<typename CX, typename CY, typename CZ>
inline QImPlot3DScatterItemNode* QImPlot3DNode::addScatter(const CX& x, const CY& y, const CZ& z, const QString& label)
{
    auto* item = new QImPlot3DScatterItemNode(this);
    item->setLabel(label);
    // Note: setData method will be implemented in QImPlot3DScatterItemNode
    // item->setData(x, y, z);
    addPlot3DItem(item);
    return item;
}

}  // namespace QIM

#endif  // QIMPLOT3DNODE_H