#ifndef QIMPLOTBARGROUPSDATASERIES_H
#define QIMPLOTBARGROUPSDATASERIES_H

#include "QImPlotDataSeries.h"
#include <QStringList>
#include <QByteArray>
#include <vector>
#include <limits>
#include <QtGlobal>
#include <type_traits>

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Abstract base class for bar groups data series
 *
 * @class QImAbstractBarGroupsDataSeries
 * @ingroup plot_data
 *
 * @details Provides a unified interface for data series that represent grouped bar charts.
 *          Stores item labels and a row-major matrix of values (item_count rows, group_count columns).
 *          Supports zero-copy rendering when data is contiguous in memory.
 *
 * @see QImAbstractPlotDataSeries
 * @see QImVectorBarGroupsDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 分组柱状图数据系列的抽象基类
 *
 * @class QImAbstractBarGroupsDataSeries
 * @ingroup plot_data
 *
 * @details 为表示分组柱状图的数据系列提供统一接口。
 *          存储项目标签和行主序的值矩阵（item_count 行，group_count 列）。
 *          当数据在内存中连续时支持零拷贝渲染。
 *
 * @see QImAbstractPlotDataSeries
 * @see QImVectorBarGroupsDataSeries
 * \endif
 */
class QIM_CORE_API QImAbstractBarGroupsDataSeries : public QImAbstractPlotDataSeries
{
public:
    /**
     * \if ENGLISH
     * @brief Bar groups data type enumeration
     * \endif
     *
     * \if CHINESE
     * @brief 分组柱状图数据类型枚举
     * \endif
     */
    enum BarGroupsDataType
    {
        BarGroupsData = XYData + 1   ///< \if ENGLISH Bar groups data type \endif \if CHINESE 分组柱状图数据类型 \endif
    };

    QImAbstractBarGroupsDataSeries() : QImAbstractPlotDataSeries() {}
    virtual ~QImAbstractBarGroupsDataSeries() = default;

    /**
     * \if ENGLISH
     * @brief Get the data type
     * @return BarGroupsDataType indicating bar groups data
     * \endif
     *
     * \if CHINESE
     * @brief 获取数据类型
     * @return BarGroupsDataType 指示分组柱状图数据
     * \endif
     */
    virtual int type() const override { return BarGroupsData; }

    /**
     * \if ENGLISH
     * @brief Get number of items (rows)
     * @return Item count
     * \endif
     *
     * \if CHINESE
     * @brief 获取项目数量（行数）
     * @return 项目数量
     * \endif
     */
    virtual int itemCount() const = 0;

    /**
     * \if ENGLISH
     * @brief Get number of groups (columns)
     * @return Group count
     * \endif
     *
     * \if CHINESE
     * @brief 获取组数量（列数）
     * @return 组数量
     * \endif
     */
    virtual int groupCount() const = 0;

    /**
     * \if ENGLISH
     * @brief Get item labels
     * @return QStringList of item labels
     * \endif
     *
     * \if CHINESE
     * @brief 获取项目标签
     * @return 项目标签的QStringList
     * \endif
     */
    virtual QStringList labels() const = 0;

    /**
     * \if ENGLISH
     * @brief Get raw pointer to values matrix (row-major)
     * @return Pointer to contiguous double array, or nullptr if not contiguous
     * \endif
     *
     * \if CHINESE
     * @brief 获取值矩阵的原始指针（行主序）
     * @return 连续double数组指针，如果不连续则返回nullptr
     * \endif
     */
    virtual const double* valuesRawData() const = 0;

    /**
     * \if ENGLISH
     * @brief Check if data is contiguous in memory
     * @return true if values are stored contiguously
     * \endif
     *
     * \if CHINESE
     * @brief 检查数据在内存中是否连续
     * @return 如果值连续存储则返回true
     * \endif
     */
    virtual bool isContiguous() const = 0;

    /**
     * \if ENGLISH
     * @brief Get value at specified item and group indices
     * @param itemIndex Item index (row)
     * @param groupIndex Group index (column)
     * @return Value at position
     * \endif
     *
     * \if CHINESE
     * @brief 获取指定项目和组索引处的值
     * @param itemIndex 项目索引（行）
     * @param groupIndex 组索引（列）
     * @return 位置处的值
     * \endif
     */
    virtual double value(int itemIndex, int groupIndex) const = 0;

    /**
     * \if ENGLISH
     * @brief Get array of C-string labels for ImPlot
     * @return Vector of const char* pointers, valid as long as the data series exists
     * \endif
     *
     * \if CHINESE
     * @brief 获取用于ImPlot的C字符串标签数组
     * @return const char* 指针的向量，只要数据系列存在即有效
     * \endif
     */
    virtual std::vector<const char*> labelPtrs() const = 0;
};

/**
 * \if ENGLISH
 * @brief Template class for bar groups data series with arbitrary containers
 *
 * @class QImVectorBarGroupsDataSeries
 * @ingroup plot_data
 *
 * @details Provides a concrete implementation of QImAbstractBarGroupsDataSeries
 *          using arbitrary container types for values and QStringList for labels.
 *          Supports std::vector, QVector, or any container with:
 *          - value_type typedef
 *          - data() method returning pointer
 *          - size() method
 *          - operator[]
 *
 * @tparam ContainerValues Type for values container (must store double)
 *
 * @see QImAbstractBarGroupsDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 支持任意容器的分组柱状图数据系列模板类
 *
 * @class QImVectorBarGroupsDataSeries
 * @ingroup plot_data
 *
 * @details 使用任意容器类型为值和QStringList为标签提供QImAbstractBarGroupsDataSeries的具体实现。
 *          支持std::vector、QVector或任何具有以下特性的容器：
 *          - value_type 类型定义
 *          - 返回指针的data()方法
 *          - size()方法
 *          - operator[]
 *
 * @tparam ContainerValues 值容器类型（必须存储double）
 *
 * @see QImAbstractBarGroupsDataSeries
 * \endif
 */
template<typename ContainerValues>
class QImVectorBarGroupsDataSeries : public QImAbstractBarGroupsDataSeries
{
public:
    static_assert(std::is_same_v<typename ContainerValues::value_type, double>, 
                  "ContainerValues must store double");

    /**
     * \if ENGLISH
     * @brief Construct with labels and values matrix
     * @param itemLabels QStringList of item labels (size must equal itemCount)
     * @param values Values container in row-major order (size must equal itemCount * groupCount)
     * @param itemCount Number of items (rows)
     * @param groupCount Number of groups (columns)
     * \endif
     *
     * \if CHINESE
     * @brief 使用标签和值矩阵构造
     * @param itemLabels 项目标签的QStringList（大小必须等于itemCount）
     * @param values 行主序的值容器（大小必须等于itemCount * groupCount）
     * @param itemCount 项目数量（行数）
     * @param groupCount 组数量（列数）
     * \endif
     */
    QImVectorBarGroupsDataSeries(const QStringList& itemLabels, const ContainerValues& values,
                                 int itemCount, int groupCount)
        : m_labels(itemLabels)
        , m_values(values)
        , m_itemCount(itemCount)
        , m_groupCount(groupCount)
    {
        // Ensure dimensions are consistent
        Q_ASSERT(m_labels.size() == m_itemCount);
        Q_ASSERT(static_cast<int>(m_values.size()) == m_itemCount * m_groupCount);
    }

    /**
     * \if ENGLISH
     * @brief Move-construct with labels and values matrix
     * @param itemLabels QStringList of item labels (size must equal itemCount)
     * @param values Values container in row-major order (size must equal itemCount * groupCount)
     * @param itemCount Number of items (rows)
     * @param groupCount Number of groups (columns)
     * \endif
     *
     * \if CHINESE
     * @brief 使用标签和值矩阵移动构造
     * @param itemLabels 项目标签的QStringList（大小必须等于itemCount）
     * @param values 行主序的值容器（大小必须等于itemCount * groupCount）
     * @param itemCount 项目数量（行数）
     * @param groupCount 组数量（列数）
     * \endif
     */
    QImVectorBarGroupsDataSeries(QStringList&& itemLabels, ContainerValues&& values,
                                 int itemCount, int groupCount)
        : m_labels(std::move(itemLabels))
        , m_values(std::move(values))
        , m_itemCount(itemCount)
        , m_groupCount(groupCount)
    {
        Q_ASSERT(m_labels.size() == m_itemCount);
        Q_ASSERT(static_cast<int>(m_values.size()) == m_itemCount * m_groupCount);
    }

    virtual ~QImVectorBarGroupsDataSeries() = default;

    // QImAbstractPlotDataSeries interface
    int size() const override
    {
        return m_itemCount * m_groupCount;
    }

    // QImAbstractBarGroupsDataSeries interface
    int itemCount() const override
    {
        return m_itemCount;
    }

    int groupCount() const override
    {
        return m_groupCount;
    }

    QStringList labels() const override
    {
        return m_labels;
    }

    const double* valuesRawData() const override
    {
        return m_values.data();
    }

    bool isContiguous() const override
    {
        return true;
    }

    double value(int itemIndex, int groupIndex) const override
    {
        if (itemIndex < 0 || itemIndex >= m_itemCount || groupIndex < 0 || groupIndex >= m_groupCount) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return m_values[itemIndex * m_groupCount + groupIndex];
    }

    std::vector<const char*> labelPtrs() const override
    {
        std::vector<const char*> ptrs;
        ptrs.reserve(m_labels.size());
        m_labelBytes.clear();
        m_labelBytes.reserve(m_labels.size());
        for (const QString& label : m_labels) {
            m_labelBytes.emplace_back(label.toUtf8());
            ptrs.push_back(m_labelBytes.back().constData());
        }
        return ptrs;
    }

private:
    QStringList m_labels;
    ContainerValues m_values;
    int m_itemCount;
    int m_groupCount;
    mutable std::vector<QByteArray> m_labelBytes; // Cache for C-string conversion
};

} // namespace QIM

#endif // QIMPLOTBARGROUPSDATASERIES_H