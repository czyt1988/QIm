#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
class QDockWidget;
QT_END_NAMESPACE

namespace QIM {
class QImFigureWidget;
}

class FunctionTreeWidget;
class PropertyPanelWidget;
class TestFunctionManager;
class TestFunction;

/**
 * \if ENGLISH
 * @brief Main window with classic dock layout for QIm figure testing
 * 
 * MainWindow provides a three-panel layout:
 * - Left dock: FunctionTreeWidget for selecting test functions
 * - Center: QImFigureWidget for displaying plots
 * - Right dock: PropertyPanelWidget for editing function properties
 * 
 * @section usage Usage Example
 * @code
 * MainWindow *window = new MainWindow();
 * window->showMaximized();
 * @endcode
 * 
 * @see FunctionTreeWidget
 * @see PropertyPanelWidget
 * @see QImFigureWidget
 * \endif
 * 
 * \if CHINESE
 * @brief 用于 QIm 图表测试的经典停靠布局主窗口
 * 
 * MainWindow 提供三面板布局：
 * - 左停靠：FunctionTreeWidget 用于选择测试函数
 * - 中央：QImFigureWidget 用于显示绘图
 * - 右停靠：PropertyPanelWidget 用于编辑函数属性
 * 
 * @section usage 使用示例
 * @code
 * MainWindow *window = new MainWindow();
 * window->showMaximized();
 * @endcode
 * 
 * @see FunctionTreeWidget
 * @see PropertyPanelWidget
 * @see QImFigureWidget
 * \endif
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * \if ENGLISH
     * @brief Constructor for MainWindow
     * @param parent Parent widget
     * \endif
     * 
     * \if CHINESE
     * @brief MainWindow 构造函数
     * @param parent 父控件
     * \endif
     */
    explicit MainWindow(QWidget* parent = nullptr);
    
    /**
     * \if ENGLISH
     * @brief Destructor for MainWindow
     * \endif
     * 
     * \if CHINESE
     * @brief MainWindow 析构函数
     * \endif
     */
    ~MainWindow() override;

private:
    /**
     * \if ENGLISH
     * @brief Initialize UI components
     * \endif
     * 
     * \if CHINESE
     * @brief 初始化 UI 组件
     * \endif
     */
    void initializeUi();
    
    /**
     * \if ENGLISH
     * @brief Create dock widgets for left and right panels
     * \endif
     * 
     * \if CHINESE
     * @brief 创建左右面板的停靠控件
     * \endif
     */
    void createDockWidgets();
    
    /**
     * \if ENGLISH
     * @brief Create central figure widget
     * \endif
     * 
     * \if CHINESE
     * @brief 创建中央图表控件
     * \endif
     */
    void createCentralWidget();
    
    /**
     * \if ENGLISH
     * @brief Connect signals between components
     * \endif
     * 
     * \if CHINESE
     * @brief 连接组件之间的信号
     * \endif
     */
    void connectSignals();

private Q_SLOTS:
    /**
     * \if ENGLISH
     * @brief Handle function selection from tree widget
     * @param functionId The ID of the selected function
     * \endif
     * 
     * \if CHINESE
     * @brief 处理树控件中的函数选择
     * @param functionId 选中函数的 ID
     * \endif
     */
    void onFunctionSelected(const QString& functionId);

private:
    QDockWidget* m_leftDock;              ///< \if ENGLISH Left dock widget \endif \if CHINESE 左停靠控件 \endif
    QDockWidget* m_rightDock;             ///< \if ENGLISH Right dock widget \endif \if CHINESE 右停靠控件 \endif
    QIM::QImFigureWidget* m_figureWidget; ///< \if ENGLISH Central figure widget \endif \if CHINESE 中央图表控件 \endif
    FunctionTreeWidget* m_treeWidget;     ///< \if ENGLISH Function tree widget \endif \if CHINESE 函数树控件 \endif
    PropertyPanelWidget* m_propertyPanel; ///< \if ENGLISH Property panel widget \endif \if CHINESE 属性面板控件 \endif
    TestFunctionManager* m_functionManager; ///< \if ENGLISH Test function manager \endif \if CHINESE 测试函数管理器 \endif
    QString m_currentFunctionId;          ///< \if ENGLISH Current function ID \endif \if CHINESE 当前函数 ID \endif
    TestFunction* m_currentFunction;      ///< \if ENGLISH Current test function instance \endif \if CHINESE 当前测试函数实例 \endif
};

#endif // MAINWINDOW_H
