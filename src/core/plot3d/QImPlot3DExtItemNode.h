#ifndef QIMPLOT3DEXTITEMNODE_H
#define QIMPLOT3DEXTITEMNODE_H

#include "QImAbstractNode.h"
#include "QImPlot3D.h"

namespace QIM
{

class QImPlot3DExtNode;

/**
 * \if ENGLISH
 * @brief Base class for 3D plot items (Scatter, Line, Triangle, Quad, Surface, Mesh, etc.)
 * @details This class provides common functionality for all 3D plot items:
 *          - Label property with UTF8-only storage
 *          - Item flags (NoLegend/NoFit) mapped to positive semantics
 *          - Visibility control
 *          - Parent plot node reference
 *          All 3D plot items should inherit from this class and implement the type() method.
 * @note The type() method uses InnerType3D=1000 offset to distinguish from 2D item types.
 * @see QImPlot3DExtNode, QImAbstractNode
 * \endif
 *
 * \if CHINESE
 * @brief 3D 绘图元素的基类（散点、线条、三角形、四边形、曲面、网格等）
 * @details 此类为所有 3D 绘图元素提供通用功能：
 *          - 标签属性，仅使用 UTF8 存储
 *          - 元素标志（NoLegend/NoFit）映射为肯定语义
 *          - 可见性控制
 *          - 父绘图节点引用
 *          所有 3D 绘图元素都应继承此类并实现 type() 方法。
 * @note type() 方法使用 InnerType3D=1000 偏移量以区分 2D 元素类型。
 * @see QImPlot3DExtNode, QImAbstractNode
 * \endif
 */
class QIM_CORE_API QImPlot3DExtItemNode : public QImAbstractNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlot3DExtItemNode)

    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)

public:
    /**
     * \if ENGLISH
     * @brief Type values for 3D plot items
     * @details InnerType3D=1000 offset distinguishes 3D items from 2D items (InnerType=10).
     *          UserType3D=20000 allows users to define custom 3D item types.
     * \endif
     *
     * \if CHINESE
     * @brief 3D 绘图元素的类型值
     * @details InnerType3D=1000 偏移量用于区分 3D 元素和 2D 元素（InnerType=10）。
     *          UserType3D=20000 允许用户定义自定义 3D 元素类型。
     * \endif
     */
    enum TypeValue
    {
        InnerType3D = 1000,   ///< 3D item type offset (2D uses 10)
        UserType3D  = 20000   ///< Base for user-defined 3D item types
    };

    /**
     * \if ENGLISH
     * @brief Constructor
     * @param par Parent object (typically a QImPlot3DExtNode)
     * \endif
     *
     * \if CHINESE
     * @brief 构造函数
     * @param par 父对象（通常是 QImPlot3DExtNode）
     * \endif
     */
    explicit QImPlot3DExtItemNode(QObject* par = nullptr);

    /**
     * \if ENGLISH
     * @brief Destructor
     * \endif
     *
     * \if CHINESE
     * @brief 析构函数
     * \endif
     */
    ~QImPlot3DExtItemNode() override;

    /**
     * \if ENGLISH
     * @brief Returns the item type for fast identification
     * @return Type value (InnerType3D + specific type, or UserType3D + custom type)
     * @details This pure virtual method must be implemented by all derived classes.
     *          It allows avoiding multiple qobject_cast/dynamic_cast calls.
     * \endif
     *
     * \if CHINESE
     * @brief 返回元素类型用于快速识别
     * @return 类型值（InnerType3D + 特定类型，或 UserType3D + 自定义类型）
     * @details 此纯虚方法必须由所有派生类实现。
     *          它可以避免多次 qobject_cast/dynamic_cast 调用。
     * \endif
     */
    virtual int type() const = 0;

    //----------------------------------------------------
    // Label property
    //----------------------------------------------------

    /**
     * \if ENGLISH
     * @brief Sets the item label displayed in legend
     * @param name Label text (will be converted to UTF8 internally)
     * \endif
     *
     * \if CHINESE
     * @brief 设置在图例中显示的元素标签
     * @param name 标签文本（内部将转换为 UTF8）
     * \endif
     */
    void setLabel(const QString& name);

    /**
     * \if ENGLISH
     * @brief Returns the item label as QString
     * @return Label text (converted from internal UTF8 storage)
     * \endif
     *
     * \if CHINESE
     * @brief 返回元素标签（QString 格式）
     * @return 标签文本（从内部 UTF8 存储转换）
     * \endif
     */
    QString label() const;

    /**
     * \if ENGLISH
     * @brief Returns direct UTF8 pointer for rendering
     * @return Pointer to UTF8 label data, or default "##plot3dItem" if empty
     * @details This method provides direct access to UTF8 data for beginDraw().
     *          No QString conversion overhead during rendering.
     * \endif
     *
     * \if CHINESE
     * @brief 返回用于渲染的直接 UTF8 指针
     * @return UTF8 标签数据指针，若为空则返回默认 "##plot3dItem"
     * @details 此方法为 beginDraw() 提供直接的 UTF8 数据访问。
     *          渲染期间无 QString 转换开销。
     * \endif
     */
    const char* labelConstData() const;

    //----------------------------------------------------
    // Parent plot node
    //----------------------------------------------------

    /**
     * \if ENGLISH
     * @brief Returns the parent QImPlot3DExtNode
     * @return Pointer to parent 3D plot node, or nullptr if not found
     * @details Searches up the parent hierarchy for the first QImPlot3DExtNode.
     * \endif
     *
     * \if CHINESE
     * @brief 返回父 QImPlot3DExtNode
     * @return 父 3D 绘图节点指针，若未找到则返回 nullptr
     * @details 向上搜索父级层次结构以查找第一个 QImPlot3DExtNode。
     * \endif
     */
    QImPlot3DExtNode* plot3DNode() const;

    //----------------------------------------------------
    // Item flags (positive semantics)
    //----------------------------------------------------

    /**
     * \if ENGLISH
     * @brief Checks if this item shows in legend
     * @return true if legend entry is enabled, false if NoLegend flag is set
     * @details Maps ImPlot3DItemFlags_NoLegend to positive semantics.
     * \endif
     *
     * \if CHINESE
     * @brief 检查此元素是否显示在图例中
     * @return true 表示图例条目已启用，false 表示设置了 NoLegend 标志
     * @details 将 ImPlot3DItemFlags_NoLegend 映射为肯定语义。
     * \endif
     */
    bool isLegendEntryEnabled() const;

    /**
     * \if ENGLISH
     * @brief Sets whether this item shows in legend
     * @param on true to enable legend entry, false to hide (sets NoLegend flag)
     * \endif
     *
     * \if CHINESE
     * @brief 设置此元素是否显示在图例中
     * @param on true 表示启用图例条目，false 表示隐藏（设置 NoLegend 标志）
     * \endif
     */
    void setLegendEntryEnabled(bool on);

    /**
     * \if ENGLISH
     * @brief Checks if this item participates in axis fitting
     * @return true if fitting is enabled, false if NoFit flag is set
     * @details Maps ImPlot3DItemFlags_NoFit to positive semantics.
     * \endif
     *
     * \if CHINESE
     * @brief 检查此元素是否参与轴自适应
     * @return true 表示自适应已启用，false 表示设置了 NoFit 标志
     * @details 将 ImPlot3DItemFlags_NoFit 映射为肯定语义。
     * \endif
     */
    bool isFitEnabled() const;

    /**
     * \if ENGLISH
     * @brief Sets whether this item participates in axis fitting
     * @param on true to enable fitting, false to exclude (sets NoFit flag)
     * \endif
     *
     * \if CHINESE
     * @brief 设置此元素是否参与轴自适应
     * @param on true 表示启用自适应，false 表示排除（设置 NoFit 标志）
     * \endif
     */
    void setFitEnabled(bool on);

    //----------------------------------------------------
    // Visibility
    //----------------------------------------------------

    /**
     * \if ENGLISH
     * @brief Checks if this item is visible
     * @return true if visible, false otherwise
     * @details Also checks parent's visibility.
     * \endif
     *
     * \if CHINESE
     * @brief 检查此元素是否可见
     * @return true 表示可见，false 表示不可见
     * @details 同时检查父级的可见性。
     * \endif
     */
    virtual bool isVisible() const override;

    /**
     * \if ENGLISH
     * @brief Sets item visibility
     * @param visible true to show, false to hide
     * \endif
     *
     * \if CHINESE
     * @brief 设置元素可见性
     * @param visible true 表示显示，false 表示隐藏
     * \endif
     */
    virtual void setVisible(bool visible) override;

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when the label changes
     * @param name New label text
     * \endif
     *
     * \if CHINESE
     * @brief 标签改变时发射
     * @param name 新的标签文本
     * \endif
     */
    void labelChanged(const QString& name);

    /**
     * \if ENGLISH
     * @brief Emitted when legend entry enabled state changes
     * \endif
     *
     * \if CHINESE
     * @brief 图例条目启用状态改变时发射
     * \endif
     */
    void legendEntryEnabledChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when fit enabled state changes
     * \endif
     *
     * \if CHINESE
     * @brief 自适应启用状态改变时发射
     * \endif
     */
    void fitEnabledChanged();

protected:
    /**
     * \if ENGLISH
     * @brief Called after beginDraw() completes
     * @details Override in derived classes for post-render cleanup.
     * \endif
     *
     * \if CHINESE
     * @brief beginDraw() 完成后调用
     * @details 在派生类中重写以进行渲染后清理。
     * \endif
     */
    virtual void endDraw() override;
};

}  // end namespace QIM

#endif  // QIMPLOT3DEXTITEMNODE_H

