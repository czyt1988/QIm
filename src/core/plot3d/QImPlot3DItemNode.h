#ifndef QIMPLOT3DITEMNODE_H
#define QIMPLOT3DITEMNODE_H

#include "QImAbstractNode.h"
#include "QImPlot3D.h"
#include <optional>
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"
#include "../plot/QImPlotItemNode.h"

namespace QIM
{

// ============================================================
// Type alias definitions - simplifies 3D Plot Item color property declarations
// ============================================================

/**
 * @brief Optional tracked color value for 3D plot items
 * @details Reuses 2D QImOptionalColor infrastructure (zero-cost alias).
 *          - std::nullopt: user hasn't set a color, use ImPlot3D default
 *          - has value: user set a color, or captured ImPlot3D default
 *          Compatible with deferred initialization pattern in beginDraw().
 */
using QImOptional3DColor = QImOptionalColor;

class QImPlot3DNode;

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
 * @see QImPlot3DNode, QImAbstractNode
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
 * @see QImPlot3DNode, QImAbstractNode
 * \endif
 */
class QIM_CORE_API QImPlot3DItemNode : public QImAbstractNode
{
    Q_OBJECT
    QIM_DECLARE_PRIVATE(QImPlot3DItemNode)

    // Label property
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

    // Constructor
    explicit QImPlot3DItemNode(QObject* par = nullptr);

    // Destructor
    ~QImPlot3DItemNode() override;

    // Returns the item type for fast identification
    virtual int type() const = 0;

    //----------------------------------------------------
    // Label property
    //----------------------------------------------------

    // Sets the item label displayed in legend
    void setLabel(const QString& name);

    // Returns the item label as QString
    QString label() const;

    // Returns direct UTF8 pointer for rendering
    const char* labelConstData() const;

    //----------------------------------------------------
    // Parent plot node
    //----------------------------------------------------

    // Returns the parent QImPlot3DNode
    QImPlot3DNode* plot3DNode() const;

    //----------------------------------------------------
    // Item flags (positive semantics)
    //----------------------------------------------------

    // Checks if this item shows in legend
    bool isLegendEntryEnabled() const;

    // Sets whether this item shows in legend
    void setLegendEntryEnabled(bool on);

    // Checks if this item participates in axis fitting
    bool isFitEnabled() const;

    // Sets whether this item participates in axis fitting
    void setFitEnabled(bool on);

    //----------------------------------------------------
    // Visibility
    //----------------------------------------------------

    // Checks if this item is visible
    virtual bool isVisible() const override;

    // Sets item visibility
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
    // Captures the default color assigned by ImPlot3D for this item
    ImVec4 captureItemColor() const;

    // Called after beginDraw() completes
    virtual void endDraw() override;
};

}  // end namespace QIM

#endif  // QIMPLOT3DITEMNODE_H