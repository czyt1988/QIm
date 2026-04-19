#ifndef PLOT3DSUBPLOTSFUNCTION_H
#define PLOT3DSUBPLOTSFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
class QImPlot3DNode;
class QImPlot3DLineItemNode;
class QImPlot3DScatterItemNode;
class QImPlot3DSurfaceItemNode;
class QImPlot3DDummyItemNode;
}

/**
 * \if ENGLISH
 * @brief Test function demonstrating 2x2 3D subplot grid with multiple plot types
 * 
 * Plot3DSubplotsFunction creates a 2x2 grid of 3D subplots with 4 different plot types:
 * - 3D Line (helix spiral)
 * - 3D Scatter
 * - 3D Surface (sin(x)*cos(y))
 * - 3D Legend with dummy items
 * 
 * It registers properties for each subplot title under "Subplots"/"Layout" category.
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * auto func = new Plot3DSubplotsFunction(this);
 * func->createPlot(figureWidget);
 * // Properties will be available via getRegisteredProperties()
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlot3DNode
 * \endif
 * 
 * \if CHINESE
 * @brief 演示 2x2 3D子图网格的测试函数，包含多种绘图类型
 * 
 * Plot3DSubplotsFunction 创建一个 2x2 网格的 3D 子图，包含4种不同的绘图类型：
 * - 3D 线图（螺旋线）
 * - 3D 散点图
 * - 3D 曲面图（sin(x)*cos(y)）
 * - 包含虚拟项的3D图例
 * 
 * 它在"Subplots"/"Layout"分类下为每个子图标题注册属性。
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * auto func = new Plot3DSubplotsFunction(this);
 * func->createPlot(figureWidget);
 * // 属性可通过 getRegisteredProperties() 获取
 * \endcode
 * 
 * @see TestFunction
 * @see QImPlot3DNode
 * \endif
 */
class Plot3DSubplotsFunction : public TestFunction {
    Q_OBJECT
    
    Q_PROPERTY(QString title1 READ title1 WRITE setTitle1 NOTIFY title1Changed)
    Q_PROPERTY(QString title2 READ title2 WRITE setTitle2 NOTIFY title2Changed)
    Q_PROPERTY(QString title3 READ title3 WRITE setTitle3 NOTIFY title3Changed)
    Q_PROPERTY(QString title4 READ title4 WRITE setTitle4 NOTIFY title4Changed)
    
public:
    /**
     * \if ENGLISH
     * @brief Constructor for Plot3DSubplotsFunction
     * @param parent Parent object
     * \endif
     * 
     * \if CHINESE
     * @brief Plot3DSubplotsFunction 构造函数
     * @param parent 父对象
     * \endif
     */
    explicit Plot3DSubplotsFunction(QObject* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for Plot3DSubplotsFunction
     * \endif
     * 
     * \if CHINESE
     * @brief Plot3DSubplotsFunction 析构函数
     * \endif
     */
    ~Plot3DSubplotsFunction() override;
    
    /**
     * \if ENGLISH
     * @brief Create 2x2 3D subplot grid in the given figure widget
     * @param figure Pointer to the figure widget where plots will be created
     * \endif
     * 
     * \if CHINESE
     * @brief 在指定的图表控件中创建 2x2 3D子图网格
     * @param figure 将要创建绘图的图表控件指针
     * \endif
     */
    void createPlot(QIM::QImFigureWidget* figure) override;
    
    // Title property accessors
    QString title1() const { return m_title1; }
    void setTitle1(const QString& title);
    
    QString title2() const { return m_title2; }
    void setTitle2(const QString& title);
    
    QString title3() const { return m_title3; }
    void setTitle3(const QString& title);
    
    QString title4() const { return m_title4; }
    void setTitle4(const QString& title);
    
Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Signal emitted when title 1 changes
     * @param title New title value
     * \endif
     * 
     * \if CHINESE
     * @brief 标题1改变时发出的信号
     * @param title 新的标题值
     * \endif
     */
    void title1Changed(const QString& title);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when title 2 changes
     * @param title New title value
     * \endif
     * 
     * \if CHINESE
     * @brief 标题2改变时发出的信号
     * @param title 新的标题值
     * \endif
     */
    void title2Changed(const QString& title);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when title 3 changes
     * @param title New title value
     * \endif
     * 
     * \if CHINESE
     * @brief 标题3改变时发出的信号
     * @param title 新的标题值
     * \endif
     */
    void title3Changed(const QString& title);
    
    /**
     * \if ENGLISH
     * @brief Signal emitted when title 4 changes
     * @param title New title value
     * \endif
     * 
     * \if CHINESE
     * @brief 标题4改变时发出的信号
     * @param title 新的标题值
     * \endif
     */
    void title4Changed(const QString& title);
    
private:
    QString m_title1 = QStringLiteral("3D Line");
    QString m_title2 = QStringLiteral("3D Scatter");
    QString m_title3 = QStringLiteral("3D Surface");
    QString m_title4 = QStringLiteral("3D Legend");
    
    QIM::QImPlot3DNode* m_plot3DNode1 = nullptr;
    QIM::QImPlot3DNode* m_plot3DNode2 = nullptr;
    QIM::QImPlot3DNode* m_plot3DNode3 = nullptr;
    QIM::QImPlot3DNode* m_plot3DNode4 = nullptr;
    
    QIM::QImPlot3DLineItemNode* m_lineItem = nullptr;
    QIM::QImPlot3DScatterItemNode* m_scatterItem = nullptr;
    QIM::QImPlot3DSurfaceItemNode* m_surfaceItem = nullptr;
    QIM::QImPlot3DDummyItemNode* m_dummyItem1 = nullptr;
    QIM::QImPlot3DDummyItemNode* m_dummyItem2 = nullptr;
    QIM::QImPlot3DDummyItemNode* m_dummyItem3 = nullptr;
};

#endif // PLOT3DSUBPLOTSFUNCTION_H