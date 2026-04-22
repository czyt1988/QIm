#ifndef QIMPLOT3DDATASERIES_H
#define QIMPLOT3DDATASERIES_H
#include "QImAPI.h"
#include <algorithm>
#include <QtGlobal>

#include <cmath>

namespace QIM
{

/**
 * @brief Abstract base class for XYZ data series in 3D plots
 *
 * \if ENGLISH
 * Provides a unified interface for accessing XYZ coordinate data in 3D visualizations.
 * Supports zero-copy access to raw double arrays for efficient rendering.
 * 
 * Derived classes must implement:
 * - size(): Number of data points
 * - xRawData(), yRawData(), zRawData(): Raw data pointers
 * - xValue(), yValue(), zValue(): Indexed access to individual coordinates
 * \endif
 * 
 * \if CHINESE
 * 为3D绘图提供统一的XYZ坐标数据访问接口。
 * 支持零拷贝访问原始double数组，实现高效渲染。
 * 
 * 派生类必须实现：
 * - size(): 数据点数量
 * - xRawData(), yRawData(), zRawData(): 原始数据指针
 * - xValue(), yValue(), zValue(): 按索引访问单个坐标值
 * \endif
 */
class QIM_CORE_API QImAbstractXYZDataSeries
{
public:
    // Data type enumeration for type identification without dynamic_cast
    enum DataType
    {
        XYZData
    };

public:
    QImAbstractXYZDataSeries()
    {
    }
    
    virtual ~QImAbstractXYZDataSeries() = default;

    // Get the data type for quick type identification
    virtual int type() const
    {
        return XYZData;
    }

    // Get the number of data points
    virtual int size() const = 0;

    // Get stride in bytes for contiguous memory access
    virtual int stride() const
    {
        return sizeof(double);
    }

    // Get raw pointer to X coordinate data
    virtual const double* xRawData() const = 0;

    // Get raw pointer to Y coordinate data
    virtual const double* yRawData() const = 0;

    // Get raw pointer to Z coordinate data
    virtual const double* zRawData() const = 0;

    // Get X coordinate at specified index
    virtual double xValue(int index) const = 0;

    // Get Y coordinate at specified index
    virtual double yValue(int index) const = 0;

    // Get Z coordinate at specified index
    virtual double zValue(int index) const = 0;

    // Check if the data series is valid for rendering
    virtual bool isValid() const
    {
        return size() > 0 && xRawData() != nullptr && yRawData() != nullptr && zRawData() != nullptr;
    }
};

/**
 * @brief Template class for XYZ data series using contiguous containers (std::vector, QVector)
 *
 * \if ENGLISH
 * Provides a concrete implementation of QImAbstractXYZDataSeries for standard C++ containers
 * that store double values in contiguous memory.
 * 
 * Supported container types:
 * - std::vector<double>
 * - QVector<double>
 * - std::deque<double> (if contiguous)
 * 
 * Features:
 * - Zero-copy access via data() pointers
 * - Move semantics for efficient data transfer
 * - Automatic size calculation (minimum of all three arrays)
 * \endif
 * 
 * \if CHINESE
 * 为使用连续容器（std::vector、QVector）存储XYZ数据提供具体实现。
 * 
 * 支持的容器类型：
 * - std::vector<double>
 * - QVector<double>
 * - std::deque<double>（如果内存连续）
 * 
 * 特性：
 * - 通过data()指针实现零拷贝访问
 * - 支持移动语义，高效数据传递
 * - 自动计算有效大小（取三个数组的最小值）
 * \endif
 * 
 * @tparam ContainerX Container type for X coordinates (must store double)
 * @tparam ContainerY Container type for Y coordinates (must store double)
 * @tparam ContainerZ Container type for Z coordinates (must store double)
 */
template<typename ContainerX, typename ContainerY, typename ContainerZ>
class QImVectorXYZDataSeries : public QImAbstractXYZDataSeries
{
public:
    // Static assertions: ensure containers store double
    static_assert(std::is_same_v<typename ContainerX::value_type, double>, "ContainerX must store double");
    static_assert(std::is_same_v<typename ContainerY::value_type, double>, "ContainerY must store double");
    static_assert(std::is_same_v<typename ContainerZ::value_type, double>, "ContainerZ must store double");

    // Construct with move semantics
    explicit QImVectorXYZDataSeries(ContainerX&& xs, ContainerY&& ys, ContainerZ&& zs)
        : QImAbstractXYZDataSeries()
        , m_xs(std::move(xs))
        , m_ys(std::move(ys))
        , m_zs(std::move(zs))
    {
    }

    // Construct with copy semantics
    explicit QImVectorXYZDataSeries(const ContainerX& xs, const ContainerY& ys, const ContainerZ& zs)
        : QImAbstractXYZDataSeries()
        , m_xs(xs)
        , m_ys(ys)
        , m_zs(zs)
    {
    }

    virtual ~QImVectorXYZDataSeries() = default;

    // Get the effective data size
    int size() const override
    {
        return std::min({static_cast<int>(m_xs.size()), static_cast<int>(m_ys.size()), static_cast<int>(m_zs.size())});
    }

    // Get raw pointer to X data
    const double* xRawData() const override
    {
        return m_xs.data();
    }

    // Get raw pointer to Y data
    const double* yRawData() const override
    {
        return m_ys.data();
    }

    // Get raw pointer to Z data
    const double* zRawData() const override
    {
        return m_zs.data();
    }

    // Get X coordinate at index
    double xValue(int index) const override
    {
        const int valid_size = size();
        if (index < 0 || index >= valid_size)
        {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return m_xs[static_cast<typename ContainerX::size_type>(index)];
    }

    // Get Y coordinate at index
    double yValue(int index) const override
    {
        const int valid_size = size();
        if (index < 0 || index >= valid_size)
        {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return m_ys[static_cast<typename ContainerY::size_type>(index)];
    }

    // Get Z coordinate at index
    double zValue(int index) const override
    {
        const int valid_size = size();
        if (index < 0 || index >= valid_size)
        {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return m_zs[static_cast<typename ContainerZ::size_type>(index)];
    }

    // Check if any container is empty
    bool empty() const
    {
        return m_xs.empty() || m_ys.empty() || m_zs.empty();
    }

    // Get minimum X coordinate
    typename ContainerX::value_type xmin() const
    {
        return xValue(0);
    }

    // Get maximum X coordinate
    typename ContainerX::value_type xmax() const
    {
        return xValue(size() - 1);
    }

    // Get minimum Y coordinate
    typename ContainerY::value_type ymin() const
    {
        return yValue(0);
    }

    // Get maximum Y coordinate
    typename ContainerY::value_type ymax() const
    {
        return yValue(size() - 1);
    }

    // Get minimum Z coordinate
    typename ContainerZ::value_type zmin() const
    {
        return zValue(0);
    }

    // Get maximum Z coordinate
    typename ContainerZ::value_type zmax() const
    {
        return zValue(size() - 1);
    }

protected:
    ContainerX m_xs;  ///< X coordinate storage
    ContainerY m_ys;  ///< Y coordinate storage
    ContainerZ m_zs;  ///< Z coordinate storage
};

}  // namespace QIM

#endif  // QIMPLOT3DDATASERIES_H