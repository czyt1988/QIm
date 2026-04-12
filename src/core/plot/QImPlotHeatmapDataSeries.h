#ifndef QIMPLOTHEATMAPDATASERIES_H
#define QIMPLOTHEATMAPDATASERIES_H

#include "QImPlotDataSeries.h"
#include <QByteArray>
#include <vector>
#include <limits>
#include <QtGlobal>
#include <type_traits>

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Abstract base class for heatmap data series
 *
 * @class QImAbstractHeatmapDataSeries
 * @ingroup plot_data
 *
 * @details Provides a unified interface for data series that represent heatmap charts.
 *          Stores a 2D matrix of values with rows and columns dimensions.
 *          Supports zero-copy rendering when data is contiguous in memory.
 *          By default, values are assumed to be in row-major order.
 *
 * @see QImAbstractPlotDataSeries
 * @see QImVectorHeatmapDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 热力图数据系列的抽象基类
 *
 * @class QImAbstractHeatmapDataSeries
 * @ingroup plot_data
 *
 * @details 为表示热力图的数据系列提供统一接口。
 *          存储具有行和列维度的二维值矩阵。
 *          当数据在内存中连续时支持零拷贝渲染。
 *          默认情况下，值假定为行主序。
 *
 * @see QImAbstractPlotDataSeries
 * @see QImVectorHeatmapDataSeries
 * \endif
 */
class QIM_CORE_API QImAbstractHeatmapDataSeries : public QImAbstractPlotDataSeries
{
public:
    /**
     * \if ENGLISH
     * @brief Heatmap data type enumeration
     * \endif
     *
     * \if CHINESE
     * @brief 热力图数据类型枚举
     * \endif
     */
    enum HeatmapDataType
    {
        HeatmapData = XYData + 2   ///< \if ENGLISH Heatmap data type \endif \if CHINESE 热力图数据类型 \endif
    };

    QImAbstractHeatmapDataSeries() : QImAbstractPlotDataSeries() {}
    virtual ~QImAbstractHeatmapDataSeries() = default;

    /**
     * \if ENGLISH
     * @brief Get the data type
     * @return HeatmapDataType indicating heatmap data
     * \endif
     *
     * \if CHINESE
     * @brief 获取数据类型
     * @return HeatmapDataType 指示热力图数据
     * \endif
     */
    virtual int type() const override { return HeatmapData; }

    /**
     * \if ENGLISH
     * @brief Get number of rows
     * @return Row count
     * \endif
     *
     * \if CHINESE
     * @brief 获取行数
     * @return 行数
     * \endif
     */
    virtual int rows() const = 0;

    /**
     * \if ENGLISH
     * @brief Get number of columns
     * @return Column count
     * \endif
     *
     * \if CHINESE
     * @brief 获取列数
     * @return 列数
     * \endif
     */
    virtual int cols() const = 0;

    /**
     * \if ENGLISH
     * @brief Get raw pointer to values matrix (row-major by default)
     * @return Pointer to contiguous double array, or nullptr if not contiguous
     * \endif
     *
     * \if CHINESE
     * @brief 获取值矩阵的原始指针（默认行主序）
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
     * @brief Check if data is stored in column-major order
     * @return true if column-major, false if row-major (default)
     * \endif
     *
     * \if CHINESE
     * @brief 检查数据是否按列主序存储
     * @return true 表示列主序，false 表示行主序（默认）
     * \endif
     */
    virtual bool isColMajor() const = 0;

    /**
     * \if ENGLISH
     * @brief Get value at specified row and column indices
     * @param row Row index (0-based)
     * @param col Column index (0-based)
     * @return Value at position
     * \endif
     *
     * \if CHINESE
     * @brief 获取指定行和列索引处的值
     * @param row 行索引（从0开始）
     * @param col 列索引（从0开始）
     * @return 位置处的值
     * \endif
     */
    virtual double value(int row, int col) const = 0;
};

/**
 * \if ENGLISH
 * @brief Template class for heatmap data series with arbitrary containers
 *
 * @class QImVectorHeatmapDataSeries
 * @ingroup plot_data
 *
 * @details Provides a concrete implementation of QImAbstractHeatmapDataSeries
 *          using arbitrary container types for values.
 *          Supports std::vector, QVector, or any container with:
 *          - value_type typedef
 *          - data() method returning pointer
 *          - size() method
 *          - operator[]
 *
 * @tparam ContainerValues Type for values container (must store double)
 *
 * @see QImAbstractHeatmapDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 支持任意容器的热力图数据系列模板类
 *
 * @class QImVectorHeatmapDataSeries
 * @ingroup plot_data
 *
 * @details 使用任意容器类型为值提供QImAbstractHeatmapDataSeries的具体实现。
 *          支持std::vector、QVector或任何具有以下特性的容器：
 *          - value_type 类型定义
 *          - 返回指针的data()方法
 *          - size()方法
 *          - operator[]
 *
 * @tparam ContainerValues 值容器类型（必须存储double）
 *
 * @see QImAbstractHeatmapDataSeries
 * \endif
 */
template<typename ContainerValues>
class QImVectorHeatmapDataSeries : public QImAbstractHeatmapDataSeries
{
public:
    static_assert(std::is_same_v<typename std::remove_reference_t<ContainerValues>::value_type, double>, 
                  "ContainerValues must store double");
    
    using StoredType = std::remove_cv_t<std::remove_reference_t<ContainerValues>>;

    /**
     * \if ENGLISH
     * @brief Construct with values matrix (perfect forwarding)
     * @param values Values container in row-major order (size must equal rows * cols)
     * @param rows Number of rows
     * @param cols Number of columns
     * @param colMajor Whether values are stored in column-major order (default false)
     * \endif
     *
     * \if CHINESE
     * @brief 使用值矩阵构造（完美转发）
     * @param values 行主序的值容器（大小必须等于 rows * cols）
     * @param rows 行数
     * @param cols 列数
     * @param colMajor 值是否按列主序存储（默认 false）
     * \endif
     */
    template<typename T = ContainerValues>
    QImVectorHeatmapDataSeries(T&& values,
                               int rows, int cols,
                               bool colMajor = false)
        : m_values(std::forward<T>(values))
        , m_rows(rows)
        , m_cols(cols)
        , m_colMajor(colMajor)
    {
        Q_ASSERT(static_cast<int>(m_values.size()) == m_rows * m_cols);
    }

    virtual ~QImVectorHeatmapDataSeries() = default;

    // QImAbstractPlotDataSeries interface
    int size() const override
    {
        return m_rows * m_cols;
    }

    // QImAbstractHeatmapDataSeries interface
    int rows() const override
    {
        return m_rows;
    }

    int cols() const override
    {
        return m_cols;
    }

    const double* valuesRawData() const override
    {
        return m_values.data();
    }

    bool isContiguous() const override
    {
        return true;
    }

    bool isColMajor() const override
    {
        return m_colMajor;
    }

    double value(int row, int col) const override
    {
        if (row < 0 || row >= m_rows || col < 0 || col >= m_cols) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        if (m_colMajor) {
            // column-major: index = col * rows + row
            return m_values[col * m_rows + row];
        } else {
            // row-major: index = row * cols + col
            return m_values[row * m_cols + col];
        }
    }

private:
    StoredType m_values;
    int m_rows;
    int m_cols;
    bool m_colMajor;
};

} // namespace QIM

#endif // QIMPLOTHEATMAPDATASERIES_H