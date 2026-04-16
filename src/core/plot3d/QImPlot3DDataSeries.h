#ifndef QIMPLOT3DDATASERIES_H
#define QIMPLOT3DDATASERIES_H
#include "QImAPI.h"
#include <algorithm>
#include <QtGlobal>

#include <cmath>

namespace QIM
{

/**
 * @class QImAbstractXYZDataSeries
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
    /**
     * @brief Data type enumeration for type identification without dynamic_cast
     * 
     * \if ENGLISH
     * Identifies the data series type to avoid runtime type checking overhead.
     * \endif
     * 
     * \if CHINESE
     * 数据类型枚举，用于避免dynamic_cast的类型识别。
     * \endif
     */
    enum DataType
    {
        XYZData
    };

public:
    QImAbstractXYZDataSeries()
    {
    }
    
    virtual ~QImAbstractXYZDataSeries() = default;

    /**
     * @brief Get the data type for quick type identification
     * @return DataType::XYZData
     * 
     * \if ENGLISH
     * Used for fast type discrimination without dynamic_cast.
     * \endif
     * 
     * \if CHINESE
     * 用于快速区分类型，避免dynamic_cast。
     * \endif
     */
    virtual int type() const
    {
        return XYZData;
    }

    /**
     * @brief Get the number of data points
     * @return Number of XYZ points in the series
     * 
     * \if ENGLISH
     * Returns the total count of XYZ coordinate triplets.
     * \endif
     * 
     * \if CHINESE
     * 返回XYZ坐标三元组的总数量。
     * \endif
     */
    virtual int size() const = 0;

    /**
     * @brief Get stride in bytes for contiguous memory access
     * @return sizeof(double) by default
     * 
     * \if ENGLISH
     * Stride between consecutive data points in memory.
     * Default is sizeof(double) for standard contiguous arrays.
     * \endif
     * 
     * \if CHINESE
     * 内存中连续数据点之间的步幅（字节）。
     * 默认为sizeof(double)，适用于标准连续数组。
     * \endif
     */
    virtual int stride() const
    {
        return sizeof(double);
    }

    /**
     * @brief Get raw pointer to X coordinate data
     * @return Pointer to X data array, must be valid (non-null)
     * 
     * \if ENGLISH
     * Returns direct access to X coordinate array for zero-copy rendering.
     * Must return a valid pointer (unlike 2D which allows nullptr for Y-only mode).
     * \endif
     * 
     * \if CHINESE
     * 返回X坐标数组的直接访问指针，用于零拷贝渲染。
     * 必须返回有效指针（与2D不同，3D不支持Y-only模式）。
     * \endif
     */
    virtual const double* xRawData() const = 0;

    /**
     * @brief Get raw pointer to Y coordinate data
     * @return Pointer to Y data array, must be valid (non-null)
     * 
     * \if ENGLISH
     * Returns direct access to Y coordinate array for zero-copy rendering.
     * \endif
     * 
     * \if CHINESE
     * 返回Y坐标数组的直接访问指针，用于零拷贝渲染。
     * \endif
     */
    virtual const double* yRawData() const = 0;

    /**
     * @brief Get raw pointer to Z coordinate data
     * @return Pointer to Z data array, must be valid (non-null)
     * 
     * \if ENGLISH
     * Returns direct access to Z coordinate array for zero-copy rendering.
     * This is the key difference from 2D XY data series.
     * \endif
     * 
     * \if CHINESE
     * 返回Z坐标数组的直接访问指针，用于零拷贝渲染。
     * 这是与2D XY数据系列的关键区别。
     * \endif
     */
    virtual const double* zRawData() const = 0;

    /**
     * @brief Get X coordinate at specified index
     * @param index Data point index
     * @return X coordinate value, or NaN if index is out of range
     * 
     * \if ENGLISH
     * Returns the X coordinate at the given index.
     * \endif
     * 
     * \if CHINESE
     * 返回指定索引处的X坐标值。
     * \endif
     */
    virtual double xValue(int index) const = 0;

    /**
     * @brief Get Y coordinate at specified index
     * @param index Data point index
     * @return Y coordinate value, or NaN if index is out of range
     * 
     * \if ENGLISH
     * Returns the Y coordinate at the given index.
     * \endif
     * 
     * \if CHINESE
     * 返回指定索引处的Y坐标值。
     * \endif
     */
    virtual double yValue(int index) const = 0;

    /**
     * @brief Get Z coordinate at specified index
     * @param index Data point index
     * @return Z coordinate value, or NaN if index is out of range
     * 
     * \if ENGLISH
     * Returns the Z coordinate at the given index.
     * \endif
     * 
     * \if CHINESE
     * 返回指定索引处的Z坐标值。
     * \endif
     */
    virtual double zValue(int index) const = 0;

    /**
     * @brief Check if the data series is valid for rendering
     * @return true if size > 0 and all raw data pointers are non-null
     * 
     * \if ENGLISH
     * Validates that the data series has data and all coordinate arrays are accessible.
     * \endif
     * 
     * \if CHINESE
     * 验证数据系列是否包含数据且所有坐标数组都可访问。
     * \endif
     */
    virtual bool isValid() const
    {
        return size() > 0 && xRawData() != nullptr && yRawData() != nullptr && zRawData() != nullptr;
    }
};

/**
 * @class QImVectorXYZDataSeries
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

    /**
     * @brief Construct with move semantics
     * @param xs X coordinate container (moved)
     * @param ys Y coordinate container (moved)
     * @param zs Z coordinate container (moved)
     * 
     * \if ENGLISH
     * Efficient constructor that moves containers into internal storage.
     * \endif
     * 
     * \if CHINESE
     * 高效构造函数，将容器移动到内部存储。
     * \endif
     */
    explicit QImVectorXYZDataSeries(ContainerX&& xs, ContainerY&& ys, ContainerZ&& zs)
        : QImAbstractXYZDataSeries()
        , m_xs(std::move(xs))
        , m_ys(std::move(ys))
        , m_zs(std::move(zs))
    {
    }

    /**
     * @brief Construct with copy semantics
     * @param xs X coordinate container (copied)
     * @param ys Y coordinate container (copied)
     * @param zs Z coordinate container (copied)
     * 
     * \if ENGLISH
     * Constructor that copies containers into internal storage.
     * \endif
     * 
     * \if CHINESE
     * 构造函数，将容器复制到内部存储。
     * \endif
     */
    explicit QImVectorXYZDataSeries(const ContainerX& xs, const ContainerY& ys, const ContainerZ& zs)
        : QImAbstractXYZDataSeries()
        , m_xs(xs)
        , m_ys(ys)
        , m_zs(zs)
    {
    }

    virtual ~QImVectorXYZDataSeries() = default;

    /**
     * @brief Get the effective data size
     * @return Minimum of X, Y, Z container sizes
     * 
     * \if ENGLISH
     * Returns the minimum size among all three coordinate arrays to ensure valid indexing.
     * \endif
     * 
     * \if CHINESE
     * 返回三个坐标数组的最小大小，确保索引有效。
     * \endif
     */
    int size() const override
    {
        return std::min({static_cast<int>(m_xs.size()), static_cast<int>(m_ys.size()), static_cast<int>(m_zs.size())});
    }

    /**
     * @brief Get raw pointer to X data
     * @return Pointer to internal X array
     * 
     * \if ENGLISH
     * Direct access to underlying X coordinate storage.
     * \endif
     * 
     * \if CHINESE
     * 直接访问底层X坐标存储。
     * \endif
     */
    const double* xRawData() const override
    {
        return m_xs.data();
    }

    /**
     * @brief Get raw pointer to Y data
     * @return Pointer to internal Y array
     * 
     * \if ENGLISH
     * Direct access to underlying Y coordinate storage.
     * \endif
     * 
     * \if CHINESE
     * 直接访问底层Y坐标存储。
     * \endif
     */
    const double* yRawData() const override
    {
        return m_ys.data();
    }

    /**
     * @brief Get raw pointer to Z data
     * @return Pointer to internal Z array
     * 
     * \if ENGLISH
     * Direct access to underlying Z coordinate storage.
     * \endif
     * 
     * \if CHINESE
     * 直接访问底层Z坐标存储。
     * \endif
     */
    const double* zRawData() const override
    {
        return m_zs.data();
    }

    /**
     * @brief Get X coordinate at index
     * @param index Data point index
     * @return X value or NaN if out of range
     */
    double xValue(int index) const override
    {
        const int valid_size = size();
        if (index < 0 || index >= valid_size)
        {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return m_xs[static_cast<typename ContainerX::size_type>(index)];
    }

    /**
     * @brief Get Y coordinate at index
     * @param index Data point index
     * @return Y value or NaN if out of range
     */
    double yValue(int index) const override
    {
        const int valid_size = size();
        if (index < 0 || index >= valid_size)
        {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return m_ys[static_cast<typename ContainerY::size_type>(index)];
    }

    /**
     * @brief Get Z coordinate at index
     * @param index Data point index
     * @return Z value or NaN if out of range
     */
    double zValue(int index) const override
    {
        const int valid_size = size();
        if (index < 0 || index >= valid_size)
        {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return m_zs[static_cast<typename ContainerZ::size_type>(index)];
    }

    /**
     * @brief Check if any container is empty
     * @return true if X, Y, or Z container is empty
     * 
     * \if ENGLISH
     * Quick check for empty data without computing minimum size.
     * \endif
     * 
     * \if CHINESE
     * 快速检查是否有空数据，无需计算最小大小。
     * \endif
     */
    bool empty() const
    {
        return m_xs.empty() || m_ys.empty() || m_zs.empty();
    }

    /**
     * @brief Get minimum X coordinate
     * @return First X value or NaN if empty
     */
    typename ContainerX::value_type xmin() const
    {
        return xValue(0);
    }

    /**
     * @brief Get maximum X coordinate
     * @return Last X value or NaN if empty
     */
    typename ContainerX::value_type xmax() const
    {
        return xValue(size() - 1);
    }

    /**
     * @brief Get minimum Y coordinate
     * @return First Y value or NaN if empty
     */
    typename ContainerY::value_type ymin() const
    {
        return yValue(0);
    }

    /**
     * @brief Get maximum Y coordinate
     * @return Last Y value or NaN if empty
     */
    typename ContainerY::value_type ymax() const
    {
        return yValue(size() - 1);
    }

    /**
     * @brief Get minimum Z coordinate
     * @return First Z value or NaN if empty
     */
    typename ContainerZ::value_type zmin() const
    {
        return zValue(0);
    }

    /**
     * @brief Get maximum Z coordinate
     * @return Last Z value or NaN if empty
     */
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