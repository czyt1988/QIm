#include "MainWindow.h"
#include "widgets/FunctionTreeWidget.h"
#include "widgets/PropertyPanelWidget.h"
#include "functions/TestFunctionManager.h"
#include "functions/TestFunction.h"
#include <QImFigureWidget.h>
#include <QDockWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QStringList>
#include <QDebug>

/**
 * \if ENGLISH
 * @brief Initialize UI components
 * 
 * Creates dock widgets, central figure widget, and connects signals.
 * Also registers all test functions and selects the first one by default.
 * \endif
 * 
 * \if CHINESE
 * @brief 初始化 UI 组件
 * 
 * 创建停靠控件、中央图表控件并连接信号。
 * 同时注册所有测试函数并默认选择第一个。
 * \endif
 */
void MainWindow::initializeUi()
{
    // Create function manager and register all functions
    m_functionManager = new TestFunctionManager(this);
    m_functionManager->registerAllFunctions();
    
    // Create dock widgets and central widget
    createDockWidgets();
    createCentralWidget();
    
    // Connect signals between components
    connectSignals();
    
    // Initialize tree widget
    m_treeWidget->initializeTree();
    
    // Select first function by default
    QStringList allIds = m_functionManager->getAllFunctionIds();
    if (!allIds.isEmpty()) {
        m_currentFunctionId = allIds.first();
        onFunctionSelected(m_currentFunctionId);
    }
}

/**
 * \if ENGLISH
 * @brief Create dock widgets for left and right panels
 * 
 * Creates left dock with FunctionTreeWidget and right dock with PropertyPanelWidget.
 * Both docks are configured with appropriate allowed areas and object names.
 * \endif
 * 
 * \if CHINESE
 * @brief 创建左右面板的停靠控件
 * 
 * 创建包含 FunctionTreeWidget 的左停靠控件和包含 PropertyPanelWidget 的右停靠控件。
 * 两个停靠控件都配置了适当的允许区域和对象名称。
 * \endif
 */
void MainWindow::createDockWidgets()
{
    // Create left dock for function tree
    m_leftDock = new QDockWidget(tr("Functions"), this);
    m_leftDock->setObjectName(QStringLiteral("leftDock"));
    m_leftDock->setAllowedAreas(Qt::LeftDockWidgetArea);
    m_leftDock->setWidget(m_treeWidget);  // Set FunctionTreeWidget as dock content
    
    // Create right dock for property panel
    m_rightDock = new QDockWidget(tr("Properties"), this);
    m_rightDock->setObjectName(QStringLiteral("rightDock"));
    m_rightDock->setAllowedAreas(Qt::RightDockWidgetArea);
    m_rightDock->setWidget(m_propertyPanel);  // Set PropertyPanelWidget as dock content
    
    addDockWidget(Qt::LeftDockWidgetArea, m_leftDock);
    addDockWidget(Qt::RightDockWidgetArea, m_rightDock);
}

/**
 * \if ENGLISH
 * @brief Create central figure widget
 * 
 * Creates QImFigureWidget as the central widget with render mode set to OnDemand.
 * \endif
 * 
 * \if CHINESE
 * @brief 创建中央图表控件
 * 
 * 创建 QImFigureWidget 作为中央控件，渲染模式设置为 OnDemand。
 * \endif
 */
void MainWindow::createCentralWidget()
{
    m_figureWidget = new QIM::QImFigureWidget(this);
    m_figureWidget->setRenderMode(QIM::QImWidget::RenderOnDemand);
    setCentralWidget(m_figureWidget);
}

/**
 * \if ENGLISH
 * @brief Connect signals between components
 * 
 * Connects FunctionTreeWidget's functionSelected signal to onFunctionSelected slot.
 * Also connects PropertyPanelWidget's propertyChanged signal to request render.
 * \endif
 * 
 * \if CHINESE
 * @brief 连接组件之间的信号
 * 
 * 将 FunctionTreeWidget 的 functionSelected 信号连接到 onFunctionSelected 槽。
 * 同时连接 PropertyPanelWidget 的 propertyChanged 信号以请求重新渲染。
 * \endif
 */
void MainWindow::connectSignals()
{
    connect(m_treeWidget, &FunctionTreeWidget::functionSelected,
            this, &MainWindow::onFunctionSelected);
    
    // Connect property panel changes to request render
    connect(m_propertyPanel, &PropertyPanelWidget::propertyChanged,
            this, [this](QObject* target, const QString& propertyName, const QVariant& newValue) {
        qDebug() << "[MainWindow] propertyChanged received: target=" << target
                 << ", propertyName=" << propertyName << ", newValue=" << newValue;
        m_figureWidget->requestRender();
        qDebug() << "[MainWindow] requestRender called";
    });
    
    qDebug() << "[MainWindow] connectSignals: property panel connected";
}

/**
 * \if ENGLISH
 * @brief Handle function selection from tree widget
 * @param functionId The ID of the selected function
 * 
 * This slot is called when a user selects a function from the tree widget.
 * It performs the following steps:
 * 1. Cleanup the previous function's plot
 * 2. Clear the figure widget
 * 3. Create new plot for the selected function
 * 4. Update property panel with the function's properties
 * \endif
 * 
 * \if CHINESE
 * @brief 处理树控件中的函数选择
 * @param functionId 选中函数的 ID
 * 
 * 当用户从树控件中选择函数时调用此槽。
 * 执行以下步骤：
 * 1. 清理前一个函数的绘图
 * 2. 清除图表控件
 * 3. 为选中的函数创建新绘图
 * 4. 用函数的属性更新属性面板
 * \endif
 */
void MainWindow::onFunctionSelected(const QString& functionId)
{
    // Get function from manager
    TestFunction* function = m_functionManager->getFunction(functionId);
    if (!function) {
        return;
    }
    
    // Cleanup old function's plot
    if (m_currentFunction && m_currentFunction != function) {
        m_currentFunction->cleanupPlot();
    }
    
    // Clear figure widget by removing all plot nodes
    auto plotNodes = m_figureWidget->plotNodes();
    for (auto* plot : plotNodes) {
        m_figureWidget->removePlotNode(plot);
    }
    
    // Create new plot for the selected function
    function->createPlot(m_figureWidget);
    
    // Request render to update display immediately
    m_figureWidget->requestRender();
    
    // Update property panel with function's properties
    m_propertyPanel->setProperties(function->getRegisteredProperties());
    
    // Update current function
    m_currentFunctionId = functionId;
    m_currentFunction = function;
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_leftDock(nullptr)
    , m_rightDock(nullptr)
    , m_figureWidget(nullptr)
    , m_treeWidget(new FunctionTreeWidget(this))
    , m_propertyPanel(new PropertyPanelWidget(this))
    , m_functionManager(nullptr)
    , m_currentFunction(nullptr)
{
    // Set window title
    setWindowTitle(tr("QIm Figure Test"));
    
    // Initialize UI components
    initializeUi();
    
    // Show maximized on start
    showMaximized();
}

MainWindow::~MainWindow()
{
    // Cleanup current function if exists
    if (m_currentFunction) {
        m_currentFunction->cleanupPlot();
    }
}
