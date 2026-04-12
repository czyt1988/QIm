#ifndef TESTFUNCTIONMANAGER_H
#define TESTFUNCTIONMANAGER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QList>

class TestFunction;

/**
 * \if ENGLISH
 * @brief Metadata structure for test function registration and tree building
 * 
 * FunctionMetadata contains categorization information for organizing test functions
 * in a tree view, including category, subcategory, display name, and unique function ID.
 * \endif
 * 
 * \if CHINESE
 * @brief 用于测试函数注册和树形结构构建的元数据结构
 * 
 * FunctionMetadata 包含用于在树形视图中组织测试函数的分类信息，
 * 包括类别、子类别、显示名称和唯一函数 ID。
 * \endif
 */
struct FunctionMetadata {
    QString category;      ///< \if ENGLISH Main category (e.g., "2D", "3D") \endif \if CHINESE 主分类（例如"2D"、"3D"） \endif
    QString subcategory;   ///< \if ENGLISH Sub category (e.g., "Line", "Data Points") \endif \if CHINESE 子分类（例如"线"、"数据点"） \endif
    QString displayName;   ///< \if ENGLISH Display name (e.g., "10K Points") \endif \if CHINESE 显示名称（例如"10K 点"） \endif
    QString functionId;    ///< \if ENGLISH Unique function identifier \endif \if CHINESE 唯一函数标识符 \endif
};

/**
 * \if ENGLISH
 * @brief Manager class for test function instances with ID-based lookup and metadata
 * 
 * TestFunctionManager provides centralized management of all test function instances,
 * supporting registration, lookup by ID, and metadata retrieval for UI tree building.
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * TestFunctionManager* manager = new TestFunctionManager(this);
 * manager->registerAllFunctions();
 * 
 * // Get function by ID
 * TestFunction* func = manager->getFunction("2d_line_10k");
 * 
 * // Get all function IDs
 * QStringList ids = manager->getAllFunctionIds();
 * 
 * // Get metadata list for tree building
 * QList<FunctionMetadata> metadata = manager->getFunctionMetadataList();
 * \endcode
 * 
 * @see TestFunction
 * @see FunctionMetadata
 * \endif
 * 
 * \if CHINESE
 * @brief 带有 ID 查找和元数据的测试函数实例管理器类
 * 
 * TestFunctionManager 提供所有测试函数实例的集中管理，
 * 支持注册、按 ID 查找和用于 UI 树构建的元数据检索。
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * TestFunctionManager* manager = new TestFunctionManager(this);
 * manager->registerAllFunctions();
 * 
 * // 通过 ID 获取函数
 * TestFunction* func = manager->getFunction("2d_line_10k");
 * 
 * // 获取所有函数 ID
 * QStringList ids = manager->getAllFunctionIds();
 * 
 * // 获取用于树构建的元数据列表
 * QList<FunctionMetadata> metadata = manager->getFunctionMetadataList();
 * \endcode
 * 
 * @see TestFunction
 * @see FunctionMetadata
 * \endif
 */
class TestFunctionManager : public QObject {
    Q_OBJECT

public:
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
    explicit TestFunctionManager(QObject* parent = nullptr);
    
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
    ~TestFunctionManager() override;
    
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
    void registerAllFunctions();
    
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
    TestFunction* getFunction(const QString& functionId) const;
    
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
    QStringList getAllFunctionIds() const;
    
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
    QList<FunctionMetadata> getFunctionMetadataList() const;

private:
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
    void create2DFunctions();
    
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
    void create3DFunctions();
    
    QMap<QString, TestFunction*> m_functions;     ///< \if ENGLISH Map of function ID to TestFunction pointer \endif \if CHINESE 函数 ID 到 TestFunction 指针的映射 \endif
    QList<FunctionMetadata> m_metadataList;        ///< \if ENGLISH List of function metadata for tree building \endif \if CHINESE 用于树构建的函数元数据列表 \endif
};

#endif // TESTFUNCTIONMANAGER_H
