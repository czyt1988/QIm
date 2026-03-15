#ifndef QIMPLOTERRORDATASERIES_H
#define QIMPLOTERRORDATASERIES_H
#include "QImPlotDataSeries.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Abstract base class for error data series
 *
 * @class QImAbstractErrorDataSeries
 * @ingroup plot_data
 *
 * @details Provides a unified interface for data series that include error values.
 *          Supports both symmetric errors (same for positive and negative)
 *          and asymmetric errors (different positive and negative values).
 *          Inherits from QImAbstractXYDataSeries to provide XY data access
 *          while adding error value accessors.
 *
 * @see QImAbstractXYDataSeries
 * @see QImVectorErrorDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 误差数据系列的抽象基类
 *
 * @class QImAbstractErrorDataSeries
 * @ingroup plot_data
 *
 * @details 为包含误差值的数据系列提供统一接口。
 *          支持对称误差（正负相同）和非对称误差（正负不同）。
 *          继承自 QImAbstractXYDataSeries 以提供 XY 数据访问，
 *          同时添加误差值访问器。
 *
 * @see QImAbstractXYDataSeries
 * @see QImVectorErrorDataSeries
 * \endif
 */
class QIM_CORE_API QImAbstractErrorDataSeries : public QImAbstractXYDataSeries
{
public:
    /**
     * \if ENGLISH
     * @brief Error data type enumeration
     * \endif
     *
     * \if CHINESE
     * @brief 误差数据类型枚举
     * \endif
     */
    enum ErrorDataType
    {
        SymmetricError,   ///< Same error for positive and negative
        AsymmetricError   ///< Different errors for positive and negative
    };

    QImAbstractErrorDataSeries() : QImAbstractXYDataSeries() {}
    virtual ~QImAbstractErrorDataSeries() = default;

    /**
     * \if ENGLISH
     * @brief Get the error data type
     * @return ErrorDataType indicating symmetric or asymmetric errors
     * \endif
     *
     * \if CHINESE
     * @brief 获取误差数据类型
     * @return ErrorDataType 指示对称或非对称误差
     * \endif
     */
    virtual ErrorDataType errorType() const = 0;

    /**
     * \if ENGLISH
     * @brief Check if using symmetric errors
     * @return true if symmetric error mode
     * \endif
     *
     * \if CHINESE
     * @brief 检查是否使用对称误差
     * @return 如果是对称误差模式则返回true
     * \endif
     */
    bool isSymmetric() const { return errorType() == SymmetricError; }

    /**
     * \if ENGLISH
     * @brief Check if using asymmetric errors
     * @return true if asymmetric error mode
     * \endif
     *
     * \if CHINESE
     * @brief 检查是否使用非对称误差
     * @return 如果是非对称误差模式则返回true
     * \endif
     */
    bool isAsymmetric() const { return errorType() == AsymmetricError; }

    /**
     * \if ENGLISH
     * @brief Get positive error value at index
     * @param index Data point index
     * @return Positive error value
     * \endif
     *
     * \if CHINESE
     * @brief 获取指定索引的正误差值
     * @param index 数据点索引
     * @return 正误差值
     * \endif
     */
    virtual double posError(int index) const = 0;

    /**
     * \if ENGLISH
     * @brief Get negative error value at index
     * @param index Data point index
     * @return Negative error value
     * @note For symmetric errors, returns the same value as posError()
     * \endif
     *
     * \if CHINESE
     * @brief 获取指定索引的负误差值
     * @param index 数据点索引
     * @return 负误差值
     * @note 对于对称误差，返回与 posError() 相同的值
     * \endif
     */
    virtual double negError(int index) const = 0;

    /**
     * \if ENGLISH
     * @brief Get raw pointer to positive error data
     * @return Pointer to positive error array, or nullptr if not contiguous
     * \endif
     *
     * \if CHINESE
     * @brief 获取正误差数据的原始指针
     * @return 正误差数组指针，如果不连续则返回nullptr
     * \endif
     */
    virtual const double* posErrorRawData() const { return nullptr; }

    /**
     * \if ENGLISH
     * @brief Get raw pointer to negative error data
     * @return Pointer to negative error array, or nullptr if not contiguous
     * @note For symmetric errors, may return the same pointer as posErrorRawData()
     * \endif
     *
     * \if CHINESE
     * @brief 获取负误差数据的原始指针
     * @return 负误差数组指针，如果不连续则返回nullptr
     * @note 对于对称误差，可能返回与 posErrorRawData() 相同的指针
     * \endif
     */
    virtual const double* negErrorRawData() const { return nullptr; }
};

/**
 * \if ENGLISH
 * @brief Template class for error data series with arbitrary containers
 *
 * @class QImVectorErrorDataSeries
 * @ingroup plot_data
 *
 * @details Provides a concrete implementation of QImAbstractErrorDataSeries
 *          using arbitrary container types for X, Y, and error values.
 *          Supports std::vector, QVector, or any container with:
 *          - value_type typedef
 *          - data() method returning pointer
 *          - size() method
 *          - operator[]
 *
 * @tparam ContainerX Type for X values container
 * @tparam ContainerY Type for Y values container
 * @tparam ContainerError Type for error values container
 *
 * @see QImAbstractErrorDataSeries
 * \endif
 *
 * \if CHINESE
 * @brief 支持任意容器的误差数据系列模板类
 *
 * @class QImVectorErrorDataSeries
 * @ingroup plot_data
 *
 * @details 使用任意容器类型为 X、Y 和误差值提供 QImAbstractErrorDataSeries 的具体实现。
 *          支持 std::vector、QVector 或任何具有以下特性的容器：
 *          - value_type 类型定义
 *          - 返回指针的 data() 方法
 *          - size() 方法
 *          - operator[]
 *
 * @tparam ContainerX X值容器类型
 * @tparam ContainerY Y值容器类型
 * @tparam ContainerError 误差值容器类型
 *
 * @see QImAbstractErrorDataSeries
 * \endif
 */
template<typename ContainerX, typename ContainerY, typename ContainerError>
class QImVectorErrorDataSeries : public QImAbstractErrorDataSeries
{
public:
    static_assert(std::is_same_v<typename ContainerX::value_type, double>, "ContainerX must store double");
    static_assert(std::is_same_v<typename ContainerY::value_type, double>, "ContainerY must store double");
    static_assert(std::is_same_v<typename ContainerError::value_type, double>, "ContainerError must store double");

    /**
     * \if ENGLISH
     * @brief Construct with symmetric errors
     * @param xs X values container
     * @param ys Y values container
     * @param errors Error values container (same for positive and negative)
     * \endif
     *
     * \if CHINESE
     * @brief 使用对称误差构造
     * @param xs X值容器
     * @param ys Y值容器
     * @param errors 误差值容器（正负相同）
     * \endif
     */
    QImVectorErrorDataSeries(const ContainerX& xs, const ContainerY& ys, const ContainerError& errors)
        : m_xs(xs), m_ys(ys), m_posErrors(errors), m_negErrors(), m_asymmetric(false)
    {
    }

    QImVectorErrorDataSeries(ContainerX&& xs, ContainerY&& ys, ContainerError&& errors)
        : m_xs(std::move(xs)), m_ys(std::move(ys)), m_posErrors(std::move(errors)), m_negErrors(), m_asymmetric(false)
    {
    }

    /**
     * \if ENGLISH
     * @brief Construct with asymmetric errors
     * @param xs X values container
     * @param ys Y values container
     * @param negErrors Negative error values container
     * @param posErrors Positive error values container
     * \endif
     *
     * \if CHINESE
     * @brief 使用非对称误差构造
     * @param xs X值容器
     * @param ys Y值容器
     * @param negErrors 负误差值容器
     * @param posErrors 正误差值容器
     * \endif
     */
    QImVectorErrorDataSeries(const ContainerX& xs, const ContainerY& ys, 
                             const ContainerError& negErrors, const ContainerError& posErrors)
        : m_xs(xs), m_ys(ys), m_posErrors(posErrors), m_negErrors(negErrors), m_asymmetric(true)
    {
    }

    QImVectorErrorDataSeries(ContainerX&& xs, ContainerY&& ys, 
                             ContainerError&& negErrors, ContainerError&& posErrors)
        : m_xs(std::move(xs)), m_ys(std::move(ys)), 
          m_posErrors(std::move(posErrors)), m_negErrors(std::move(negErrors)), m_asymmetric(true)
    {
    }

    virtual ~QImVectorErrorDataSeries() = default;

    // QImAbstractPlotDataSeries interface
    int size() const override
    {
        return std::min({m_xs.size(), m_ys.size(), m_posErrors.size()});
    }

    // QImAbstractErrorDataSeries interface
    ErrorDataType errorType() const override
    {
        return m_asymmetric ? AsymmetricError : SymmetricError;
    }

    double posError(int index) const override
    {
        return m_posErrors[index];
    }

    double negError(int index) const override
    {
        return m_asymmetric ? m_negErrors[index] : m_posErrors[index];
    }

    const double* posErrorRawData() const override
    {
        return m_posErrors.data();
    }

    const double* negErrorRawData() const override
    {
        return m_asymmetric ? m_negErrors.data() : m_posErrors.data();
    }

    // QImAbstractXYDataSeries interface
    bool isContiguous() const override { return true; }
    int stride() const override { return sizeof(double); }
    const double* xRawData() const override { return m_xs.data(); }
    const double* yRawData() const override { return m_ys.data(); }
    double xValue(int index) const override { return m_xs[index]; }
    double yValue(int index) const override { return m_ys[index]; }

private:
    ContainerX m_xs;
    ContainerY m_ys;
    ContainerError m_posErrors;
    ContainerError m_negErrors;
    bool m_asymmetric;
};

} // namespace QIM

#endif // QIMPLOTERRORDATASERIES_H
