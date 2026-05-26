#ifndef QIMPLOTHISTOGRAM2DDATASERIES_H
#define QIMPLOTHISTOGRAM2DDATASERIES_H

#include "QImPlotDataSeries.h"
#include <vector>
#include <limits>
#include <QtGlobal>
#include <type_traits>

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Abstract base class for 2D histogram data series
 *
 * @class QImAbstractHistogram2DDataSeries
 * @ingroup plot_data
 *
 * @details Provides a unified interface for data series that represent 2D histogram values.
 *          Stores X and Y arrays for bivariate data.
 *          Inherits from QImAbstractXYDataSeries to reuse existing infrastructure.
 *
 * @see QImAbstractXYDataSeries
 * @see QImVectorHistogram2DDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 二维直方图数据系列的抽象基类
 *
 * @class QImAbstractHistogram2DDataSeries
 * @ingroup plot_data
 *
 * @details 为表示二维直方图值的数据系列提供统一接口。
 *          存储双变量数据的X和Y数组。
 *          继承自 QImAbstractXYDataSeries 以重用现有基础设施。
 *
 * @see QImAbstractXYDataSeries
 * @see QImVectorHistogram2DDataSeries
 * \endif
 */
class QIM_CORE_API QImAbstractHistogram2DDataSeries : public QImAbstractXYDataSeries
{
public:
    /**
     * \if ENGLISH
     * @brief 2D histogram data type enumeration
     * \endif
     *
     * \if CHINESE
     * @brief 二维直方图数据类型枚举
     * \endif
     */
    enum Histogram2DDataType
    {
        Histogram2DData = XYData + 13   ///< \if ENGLISH 2D histogram data type \endif \if CHINESE 二维直方图数据类型 \endif
    };

    QImAbstractHistogram2DDataSeries() : QImAbstractXYDataSeries() {}
    virtual ~QImAbstractHistogram2DDataSeries() = default;

    // Gets the data type
    virtual int type() const override { return Histogram2DData; }

    // Checks if data is contiguous in memory
    virtual bool isContiguous() const = 0;

    // Gets raw pointer to X values array
    virtual const double* xValuesRawData() const = 0;

    // Gets raw pointer to Y values array
    virtual const double* yValuesRawData() const = 0;

    // Gets X value at specified index
    virtual double xValueAt(int index) const = 0;

    // Gets Y value at specified index
    virtual double yValueAt(int index) const = 0;
};

/**
 * \if ENGLISH
 * @brief Template class for 2D histogram data series with arbitrary containers
 *
 * @class QImVectorHistogram2DDataSeries
 * @ingroup plot_data
 *
 * @details Provides a concrete implementation of QImAbstractHistogram2DDataSeries
 *          using arbitrary container types for X and Y values.
 *          Supports std::vector, QVector, or any container with:
 *          - value_type typedef
 *          - data() method returning pointer
 *          - size() method
 *          - operator[]
 *
 * @tparam ContainerX Type for X values container (must store double)
 * @tparam ContainerY Type for Y values container (must store double)
 *
 * @see QImAbstractHistogram2DDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 支持任意容器的二维直方图数据系列模板类
 *
 * @class QImVectorHistogram2DDataSeries
 * @ingroup plot_data
 *
 * @details 使用任意容器类型为X和Y值提供 QImAbstractHistogram2DDataSeries 的具体实现。
 *          支持 std::vector、QVector 或任何具有以下特性的容器：
 *          - value_type 类型定义
 *          - 返回指针的 data() 方法
 *          - size()方法
 *          - operator[]
 *
 * @tparam ContainerX X值容器类型（必须存储double）
 * @tparam ContainerY Y值容器类型（必须存储double）
 *
 * @see QImAbstractHistogram2DDataSeries
 * \endif
 */
template<typename ContainerX, typename ContainerY>
class QImVectorHistogram2DDataSeries : public QImAbstractHistogram2DDataSeries
{
public:
    static_assert(std::is_same_v<typename ContainerX::value_type, double>, 
                  "ContainerX must store double");
    static_assert(std::is_same_v<typename ContainerY::value_type, double>, 
                  "ContainerY must store double");

    /**
     * \if ENGLISH
     * @brief Construct with X and Y containers
     * @param xs X values container
     * @param ys Y values container
     * \endif
     *
     * \if CHINESE
     * @brief 使用X和Y容器构造
     * @param xs X值容器
     * @param ys Y值容器
     * \endif
     */
    explicit QImVectorHistogram2DDataSeries(const ContainerX& xs, const ContainerY& ys)
        : m_xs(xs), m_ys(ys)
    {
        Q_ASSERT(m_xs.size() == m_ys.size());
    }

    /**
     * \if ENGLISH
     * @brief Move-construct with X and Y containers
     * @param xs X values container
     * @param ys Y values container
     * \endif
     *
     * \if CHINESE
     * @brief 使用X和Y容器移动构造
     * @param xs X值容器
     * @param ys Y值容器
     * \endif
     */
    explicit QImVectorHistogram2DDataSeries(ContainerX&& xs, ContainerY&& ys)
        : m_xs(std::move(xs)), m_ys(std::move(ys))
    {
        Q_ASSERT(m_xs.size() == m_ys.size());
    }

    virtual ~QImVectorHistogram2DDataSeries() = default;

    // QImAbstractPlotDataSeries interface
    int size() const override
    {
        return std::min(m_xs.size(), m_ys.size());
    }

    // QImAbstractHistogram2DDataSeries interface
    bool isContiguous() const override
    {
        return true;
    }

    const double* xValuesRawData() const override
    {
        return m_xs.data();
    }

    const double* yValuesRawData() const override
    {
        return m_ys.data();
    }

    double xValueAt(int index) const override
    {
        if (index < 0 || index >= static_cast<int>(m_xs.size())) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return m_xs[index];
    }

    double yValueAt(int index) const override
    {
        if (index < 0 || index >= static_cast<int>(m_ys.size())) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return m_ys[index];
    }

    // QImAbstractXYDataSeries interface (delegated)
    const double* xRawData() const override { return xValuesRawData(); }
    const double* yRawData() const override { return yValuesRawData(); }
    double xValue(int index) const override { return xValueAt(index); }
    double yValue(int index) const override { return yValueAt(index); }

private:
    ContainerX m_xs;
    ContainerY m_ys;
};

} // namespace QIM

#endif // QIMPLOTHISTOGRAM2DDATASERIES_H