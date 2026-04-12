#ifndef QIMPLOTPIECHARTDATASERIES_H
#define QIMPLOTPIECHARTDATASERIES_H

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
 * @brief Abstract base class for pie chart data series
 *
 * @class QImAbstractPieChartDataSeries
 * @ingroup plot_data
 *
 * @details Provides a unified interface for data series that represent pie charts.
 *          Stores slice labels and values.
 *          Supports zero-copy rendering when data is contiguous in memory.
 *
 * @see QImAbstractPlotDataSeries
 * @see QImVectorPieChartDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 饼图数据系列的抽象基类
 *
 * @class QImAbstractPieChartDataSeries
 * @ingroup plot_data
 *
 * @details 为表示饼图的数据系列提供统一接口。
 *          存储切片标签和值。
 *          当数据在内存中连续时支持零拷贝渲染。
 *
 * @see QImAbstractPlotDataSeries
 * @see QImVectorPieChartDataSeries
 * \endif
 */
class QIM_CORE_API QImAbstractPieChartDataSeries : public QImAbstractPlotDataSeries
{
public:
    /**
     * \if ENGLISH
     * @brief Pie chart data type enumeration
     * \endif
     *
     * \if CHINESE
     * @brief 饼图数据类型枚举
     * \endif
     */
    enum PieChartDataType
    {
        PieChartData = XYData + 2   ///< \if ENGLISH Pie chart data type \endif \if CHINESE 饼图数据类型 \endif
    };

    QImAbstractPieChartDataSeries() : QImAbstractPlotDataSeries() {}
    virtual ~QImAbstractPieChartDataSeries() = default;

    /**
     * \if ENGLISH
     * @brief Get the data type
     * @return PieChartDataType indicating pie chart data
     * \endif
     *
     * \if CHINESE
     * @brief 获取数据类型
     * @return PieChartDataType 指示饼图数据
     * \endif
     */
    virtual int type() const override { return PieChartData; }

    /**
     * \if ENGLISH
     * @brief Get number of slices
     * @return Slice count
     * \endif
     *
     * \if CHINESE
     * @brief 获取切片数量
     * @return 切片数量
     * \endif
     */
    virtual int sliceCount() const = 0;

    /**
     * \if ENGLISH
     * @brief Get slice labels
     * @return QStringList of slice labels
     * \endif
     *
     * \if CHINESE
     * @brief 获取切片标签
     * @return 切片标签的QStringList
     * \endif
     */
    virtual QStringList labels() const = 0;

    /**
     * \if ENGLISH
     * @brief Get raw pointer to values array
     * @return Pointer to contiguous double array, or nullptr if not contiguous
     * \endif
     *
     * \if CHINESE
     * @brief 获取值数组的原始指针
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
     * @brief Get value at specified slice index
     * @param sliceIndex Slice index
     * @return Value at position
     * \endif
     *
     * \if CHINESE
     * @brief 获取指定切片索引处的值
     * @param sliceIndex 切片索引
     * @return 位置处的值
     * \endif
     */
    virtual double value(int sliceIndex) const = 0;

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
 * @brief Template class for pie chart data series with arbitrary containers
 *
 * @class QImVectorPieChartDataSeries
 * @ingroup plot_data
 *
 * @details Provides a concrete implementation of QImAbstractPieChartDataSeries
 *          using arbitrary container types for values and QStringList for labels.
 *          Supports std::vector, QVector, or any container with:
 *          - value_type typedef
 *          - data() method returning pointer
 *          - size() method
 *          - operator[]
 *
 * @tparam ContainerValues Type for values container (must store double)
 *
 * @see QImAbstractPieChartDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 支持任意容器的饼图数据系列模板类
 *
 * @class QImVectorPieChartDataSeries
 * @ingroup plot_data
 *
 * @details 使用任意容器类型为值和QStringList为标签提供QImAbstractPieChartDataSeries的具体实现。
 *          支持std::vector、QVector或任何具有以下特性的容器：
 *          - value_type 类型定义
 *          - 返回指针的data()方法
 *          - size()方法
 *          - operator[]
 *
 * @tparam ContainerValues 值容器类型（必须存储double）
 *
 * @see QImAbstractPieChartDataSeries
 * \endif
 */
template<typename ContainerValues>
class QImVectorPieChartDataSeries : public QImAbstractPieChartDataSeries
{
public:
    static_assert(std::is_same_v<typename ContainerValues::value_type, double>, 
                  "ContainerValues must store double");

    /**
     * \if ENGLISH
     * @brief Construct with labels and values
     * @param sliceLabels QStringList of slice labels (size must equal sliceCount)
     * @param values Values container (size must equal sliceCount)
     * \endif
     *
     * \if CHINESE
     * @brief 使用标签和值构造
     * @param sliceLabels 切片标签的QStringList（大小必须等于sliceCount）
     * @param values 值容器（大小必须等于sliceCount）
     * \endif
     */
    QImVectorPieChartDataSeries(const QStringList& sliceLabels, const ContainerValues& values)
        : m_labels(sliceLabels)
        , m_values(values)
    {
        // Ensure dimensions are consistent
        Q_ASSERT(m_labels.size() == static_cast<int>(m_values.size()));
    }

    /**
     * \if ENGLISH
     * @brief Move-construct with labels and values
     * @param sliceLabels QStringList of slice labels (size must equal sliceCount)
     * @param values Values container (size must equal sliceCount)
     * \endif
     *
     * \if CHINESE
     * @brief 使用标签和值移动构造
     * @param sliceLabels 切片标签的QStringList（大小必须等于sliceCount）
     * @param values 值容器（大小必须等于sliceCount）
     * \endif
     */
    QImVectorPieChartDataSeries(QStringList&& sliceLabels, ContainerValues&& values)
        : m_labels(std::move(sliceLabels))
        , m_values(std::move(values))
    {
        Q_ASSERT(m_labels.size() == static_cast<int>(m_values.size()));
    }

    virtual ~QImVectorPieChartDataSeries() = default;

    // QImAbstractPlotDataSeries interface
    int size() const override
    {
        return sliceCount();
    }

    // QImAbstractPieChartDataSeries interface
    int sliceCount() const override
    {
        return static_cast<int>(m_values.size());
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

    double value(int sliceIndex) const override
    {
        if (sliceIndex < 0 || sliceIndex >= sliceCount()) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return m_values[sliceIndex];
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
    mutable std::vector<QByteArray> m_labelBytes; // Cache for C-string conversion
};

} // namespace QIM

#endif // QIMPLOTPIECHARTDATASERIES_H