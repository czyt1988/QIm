#include "TestFunctionManager.h"
#include "line/Line10KFunction.h"
#include "line/Line1MFunction.h"
#include "datapoints/ScatterFunction.h"
#include "datapoints/StairsFunction.h"
#include "datapoints/BarsFunction.h"
#include "shaded/ShadedFunction.h"
#include "error/ErrorBarsFunction.h"
#include "other/StemsFunction.h"
#include "other/InfLinesFunction.h"

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
 * Registers 9 test functions across different categories:
 * - Line: 10K Points, 1M Points
 * - Data Points: Scatter, Stairs, Bars
 * - Shaded: Shaded Plot
 * - Error: Error Bars
 * - Other: Stems, Infinite Lines
 * \endif
 * 
 * \if CHINESE
 * @brief 创建并注册所有 2D 测试函数
 * 
 * 注册 9 个不同类别的测试函数：
 * - 线：10K 点、1M 点
 * - 数据点：散点、阶梯、柱状
 * - 填充：填充图
 * - 误差：误差棒
 * - 其他：茎叶图、无限线
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
    // Reserved for future 3D functions
}
