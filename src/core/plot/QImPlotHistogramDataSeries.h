#ifndef QIMPLOTHISTOGRAMDATASERIES_H
#define QIMPLOTHISTOGRAMDATASERIES_H

#include "QImPlotDataSeries.h"
#include <vector>
#include <limits>
#include <QtGlobal>
#include <type_traits>

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Abstract base class for histogram data series
 *
 * @class QImAbstractHistogramDataSeries
 * @ingroup plot_data
 *
 * @details Provides a unified interface for data series that represent histogram values.
 *          Stores single values (Y) only, with X generated automatically as indices.
 *          Inherits from QImAbstractXYDataSeries to reuse existing infrastructure.
 *
 * @see QImAbstractXYDataSeries
 * @see QImVectorHistogramDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 直方图数据系列的抽象基类
 *
 * @class QImAbstractHistogramDataSeries
 * @ingroup plot_data
 *
 * @details 为表示直方图值的数据系列提供统一接口。
 *          仅存储单值（Y），X自动生成为索引。
 *          继承自 QImAbstractXYDataSeries 以重用现有基础设施。
 *
 * @see QImAbstractXYDataSeries
 * @see QImVectorHistogramDataSeries
 * \endif
 */
class QIM_CORE_API QImAbstractHistogramDataSeries : public QImAbstractXYDataSeries
{
public:
    /**
     * \if ENGLISH
     * @brief Histogram data type enumeration
     * \endif
     *
     * \if CHINESE
     * @brief 直方图数据类型枚举
     * \endif
     */
    enum HistogramDataType
    {
        HistogramData = XYData + 12   ///< \if ENGLISH Histogram data type \endif \if CHINESE 直方图数据类型 \endif
    };

    QImAbstractHistogramDataSeries() : QImAbstractXYDataSeries() {}
    virtual ~QImAbstractHistogramDataSeries() = default;

    /**
     * \if ENGLISH
     * @brief Get the data type
     * @return HistogramDataType indicating histogram data
     * \endif
     *
     * \if CHINESE
     * @brief 获取数据类型
     * @return HistogramDataType 指示直方图数据
     * \endif
     */
    virtual int type() const override { return HistogramData; }

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
     * @brief Get value at specified index
     * @param index Data point index
     * @return Value at position
     * \endif
     *
     * \if CHINESE
     * @brief 获取指定索引处的值
     * @param index 数据点索引
     * @return 位置处的值
     * \endif
     */
    virtual double value(int index) const = 0;
};

/**
 * \if ENGLISH
 * @brief Template class for histogram data series with arbitrary containers
 *
 * @class QImVectorHistogramDataSeries
 * @ingroup plot_data
 *
 * @details Provides a concrete implementation of QImAbstractHistogramDataSeries
 *          using arbitrary container types for values.
 *          Supports std::vector, QVector, or any container with:
 *          - value_type typedef
 *          - data() method returning pointer
 *          - size() method
 *          - operator[]
 *
 * @tparam ContainerValues Type for values container (must store double)
 *
 * @see QImAbstractHistogramDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 支持任意容器的直方图数据系列模板类
 *
 * @class QImVectorHistogramDataSeries
 * @ingroup plot_data
 *
 * @details 使用任意容器类型为值提供 QImAbstractHistogramDataSeries 的具体实现。
 *          支持 std::vector、QVector 或任何具有以下特性的容器：
 *          - value_type 类型定义
 *          - 返回指针的 data() 方法
 *          - size() 方法
 *          - operator[]
 *
 * @tparam ContainerValues 值容器类型（必须存储double）
 *
 * @see QImAbstractHistogramDataSeries
 * \endif
 */
template<typename ContainerValues>
class QImVectorHistogramDataSeries : public QImAbstractHistogramDataSeries
{
public:
    static_assert(std::is_same_v<typename ContainerValues::value_type, double>, 
                  "ContainerValues must store double");

    /**
     * \if ENGLISH
     * @brief Construct with values container
     * @param values Values container
     * \endif
     *
     * \if CHINESE
     * @brief 使用值容器构造
     * @param values 值容器
     * \endif
     */
    explicit QImVectorHistogramDataSeries(const ContainerValues& values)
        : m_values(values)
    {
    }

    /**
     * \if ENGLISH
     * @brief Move-construct with values container
     * @param values Values container
     * \endif
     *
     * \if CHINESE
     * @brief 使用值容器移动构造
     * @param values 值容器
     * \endif
     */
    explicit QImVectorHistogramDataSeries(ContainerValues&& values)
        : m_values(std::move(values))
    {
    }

    virtual ~QImVectorHistogramDataSeries() = default;

    // QImAbstractPlotDataSeries interface
    int size() const override
    {
        return m_values.size();
    }

    // QImAbstractHistogramDataSeries interface
    bool isContiguous() const override
    {
        return true;
    }

    const double* valuesRawData() const override
    {
        return m_values.data();
    }

    double value(int index) const override
    {
        if (index < 0 || index >= static_cast<int>(m_values.size())) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return m_values[index];
    }

    // QImAbstractXYDataSeries interface
    int stride() const override { return sizeof(double); }
    const double* xRawData() const override { return nullptr; }
    const double* yRawData() const override { return m_values.data(); }
    double xValue(int index) const override { return static_cast<double>(index); }
    double yValue(int index) const override { return value(index); }
    double xScale() const override { return 1.0; }
    double xStart() const override { return 0.0; }
    int offset() const override { return 0; }

private:
    ContainerValues m_values;
};

} // namespace QIM

#endif // QIMPLOTHISTOGRAMDATASERIES_H