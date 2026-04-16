#include "TestFunctionManager.h"
#include "line/Line10KFunction.h"
#include "line/Line1MFunction.h"
#include "datapoints/ScatterFunction.h"
#include "datapoints/StairsFunction.h"
#include "datapoints/BarsFunction.h"
#include "datapoints/BarGroupsFunction.h"
#include "datapoints/HistogramFunction.h"
#include "datapoints/Histogram2DFunction.h"
#include "shaded/ShadedFunction.h"
#include "error/ErrorBarsFunction.h"
#include "other/StemsFunction.h"
#include "other/InfLinesFunction.h"
#include "other/PieChartFunction.h"
#include "other/HeatmapFunction.h"
#include "other/DigitalFunction.h"
#include "other/ImageFunction.h"
#include "other/TextFunction.h"
#include "other/DummyFunction.h"
#include "tools/DragPointFunction.h"
#include "tools/DragLinesFunction.h"
#include "tools/DragRectFunction.h"
#include "tools/AnnotationFunction.h"
#include "tools/TagsFunction.h"
#include "layout/SubplotsFunction.h"
#include "3d/Plot3DScatterFunction.h"
#include "3d/Plot3DLineFunction.h"

/**
 * \if ENGLISH
 * @brief Constructor for TestFunctionManager
 * @param parent Parent object
 * \endif
 * 
 * \if CHINESE
 * @brief TestFunctionManager 构造函数
 * @param parent 父对象
 * \endif
 */
TestFunctionManager::TestFunctionManager(QObject* parent)
    : QObject(parent)
{
}

/**
 * \if ENGLISH
 * @brief Destructor for TestFunctionManager
 * 
 * Cleans up all registered test function instances.
 * \endif
 * 
 * \if CHINESE
 * @brief TestFunctionManager 析构函数
 * 
 * 清理所有已注册的测试函数实例。
 * \endif
 */
TestFunctionManager::~TestFunctionManager()
{
    qDeleteAll(m_functions);
}

/**
 * \if ENGLISH
 * @brief Register all test functions
 * 
 * Creates and registers all 9 test functions with their metadata.
 * Functions are organized by category and subcategory for tree view display.
 * \endif
 * 
 * \if CHINESE
 * @brief 注册所有测试函数
 * 
 * 创建并注册所有 9 个测试函数及其元数据。
 * 函数按类别和子类别组织，用于树形视图显示。
 * \endif
 */
void TestFunctionManager::registerAllFunctions()
{
    create2DFunctions();
    create3DFunctions();
}

/**
 * \if ENGLISH
 * @brief Get test function by ID
 * @param functionId Unique function identifier
 * @return Pointer to TestFunction instance, or nullptr if not found
 * \endif
 * 
 * \if CHINESE
 * @brief 通过 ID 获取测试函数
 * @param functionId 唯一函数标识符
 * @return 指向 TestFunction 实例的指针，如果未找到则返回 nullptr
 * \endif
 */
TestFunction* TestFunctionManager::getFunction(const QString& functionId) const
{
    return m_functions.value(functionId, nullptr);
}

/**
 * \if ENGLISH
 * @brief Get list of all registered function IDs
 * @return QStringList containing all function IDs
 * \endif
 * 
 * \if CHINESE
 * @brief 获取所有已注册函数 ID 的列表
 * @return 包含所有函数 ID 的 QStringList
 * \endif
 */
QStringList TestFunctionManager::getAllFunctionIds() const
{
    return m_functions.keys();
}

/**
 * \if ENGLISH
 * @brief Get list of function metadata for tree building
 * @return QList<FunctionMetadata> containing metadata for all registered functions
 * \endif
 * 
 * \if CHINESE
 * @brief 获取用于树构建的函数元数据列表
 * @return 包含所有已注册函数元数据的 QList<FunctionMetadata>
 * \endif
 */
QList<FunctionMetadata> TestFunctionManager::getFunctionMetadataList() const
{
    return m_metadataList;
}

/**
 * \if ENGLISH
 * @brief Create and register all 2D test functions
 * 
 * Registers multiple test functions across different categories:
 * - Line: 10K Points, 1M Points
 * - Data Points: Scatter, Stairs, Bars, Bar Groups, Histogram
 * - Shaded: Shaded Plot
 * - Error: Error Bars
 * - Other: Stems, Infinite Lines, Pie Chart, Heatmap
 * \endif
 * 
 * \if CHINESE
 * @brief 创建并注册所有 2D 测试函数
 * 
 * 注册多个不同类别的测试函数：
 * - 线：10K 点、1M 点
 * - 数据点：散点、阶梯、柱状、组柱状、直方图
 * - 填充：填充图
 * - 误差：误差棒
 * - 其他：茎叶图、无限线、饼图、热力图
 * \endif
 */
void TestFunctionManager::create2DFunctions()
{
    // Line category
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Line");
        meta.displayName = tr("10K Points");
        meta.functionId = "2d_line_10k";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new Line10KFunction(this);
    }
    
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Line");
        meta.displayName = tr("1M Points");
        meta.functionId = "2d_line_1m";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new Line1MFunction(this);
    }
    
    // Data Points category
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Data Points");
        meta.displayName = tr("Scatter");
        meta.functionId = "2d_datapoints_scatter";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new ScatterFunction(this);
    }
    
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Data Points");
        meta.displayName = tr("Stairs");
        meta.functionId = "2d_datapoints_stairs";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new StairsFunction(this);
    }
    
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Data Points");
        meta.displayName = tr("Bars");
        meta.functionId = "2d_datapoints_bars";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new BarsFunction(this);
    }
    
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Data Points");
        meta.displayName = tr("Bar Groups");
        meta.functionId = "2d_datapoints_bargroups";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new BarGroupsFunction(this);
    }
    
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Data Points");
        meta.displayName = tr("Histogram");
        meta.functionId = "2d_datapoints_histogram";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new HistogramFunction(this);
    }
    
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Data Points");
        meta.displayName = tr("2D Histogram");
        meta.functionId = "2d_datapoints_histogram2d";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new Histogram2DFunction(this);
    }
    
    // Shaded category
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Shaded");
        meta.displayName = tr("Shaded Plot");
        meta.functionId = "2d_shaded_shaded";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new ShadedFunction(this);
    }
    
    // Error category
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Error");
        meta.displayName = tr("Error Bars");
        meta.functionId = "2d_errorbars";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new ErrorBarsFunction(this);
    }
    
    // Other category
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Other");
        meta.displayName = tr("Stems");
        meta.functionId = "2d_stems";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new StemsFunction(this);
    }
    
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Other");
        meta.displayName = tr("Infinite Lines");
        meta.functionId = "2d_inflines";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new InfLinesFunction(this);
    }
    
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Other");
        meta.displayName = tr("Pie Chart");
        meta.functionId = "2d_other_piechart";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new PieChartFunction(this);
    }
    
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Other");
        meta.displayName = tr("Heatmap");
        meta.functionId = "2d_other_heatmap";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new HeatmapFunction(this);
    }
    
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Other");
        meta.displayName = tr("Digital");
        meta.functionId = "2d_other_digital";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new DigitalFunction(this);
    }
    
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Other");
        meta.displayName = tr("Image");
        meta.functionId = "2d_other_image";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new ImageFunction(this);
    }
    
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Other");
        meta.displayName = tr("Text");
        meta.functionId = "2d_other_text";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new TextFunction(this);
    }
    
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Other");
        meta.displayName = tr("Dummy");
        meta.functionId = "2d_other_dummy";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new DummyFunction(this);
    }
    
    // Tools category - Interactive drag tools
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Tools");
        meta.displayName = tr("Drag Point");
        meta.functionId = "2d_tools_dragpoint";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new DragPointFunction(this);
    }
    
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Tools");
        meta.displayName = tr("Drag Lines");
        meta.functionId = "2d_tools_draglines";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new DragLinesFunction(this);
    }
    
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Tools");
        meta.displayName = tr("Drag Rect");
        meta.functionId = "2d_tools_dragrect";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new DragRectFunction(this);
    }
    
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Tools");
        meta.displayName = tr("Annotation");
        meta.functionId = "2d_tools_annotation";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new AnnotationFunction(this);
    }
    
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Tools");
        meta.displayName = tr("Tags");
        meta.functionId = "2d_tools_tags";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new TagsFunction(this);
    }
    
    // Layout category - Subplots
    {
        FunctionMetadata meta;
        meta.category = tr("2D");
        meta.subcategory = tr("Layout");
        meta.displayName = tr("Subplots");
        meta.functionId = "2d_layout_subplots";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new SubplotsFunction(this);
    }
}

/**
 * \if ENGLISH
 * @brief Create and register all 3D test functions
 * 
 * Currently empty, reserved for future 3D function implementations.
 * \endif
 * 
 * \if CHINESE
 * @brief 创建并注册所有 3D 测试函数
 * 
 * 当前为空，保留用于未来的 3D 函数实现。
 * \endif
 */
void TestFunctionManager::create3DFunctions()
{
    // 3D Scatter
    {
        FunctionMetadata meta;
        meta.category = tr("3D");
        meta.subcategory = tr("Data Points");
        meta.displayName = tr("Scatter");
        meta.functionId = "3d_datapoints_scatter";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new Plot3DScatterFunction(this);
    }
    
    // 3D Line
    {
        FunctionMetadata meta;
        meta.category = tr("3D");
        meta.subcategory = tr("Line");
        meta.displayName = tr("Spiral");
        meta.functionId = "3d_line_spiral";
        m_metadataList.append(meta);
        m_functions[meta.functionId] = new Plot3DLineFunction(this);
    }
}
