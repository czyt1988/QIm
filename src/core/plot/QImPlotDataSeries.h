#ifndef QIMPLOTDATASERIES_H
#define QIMPLOTDATASERIES_H
#include "QImAPI.h"
#include <algorithm>
#include <QtGlobal>
namespace QIM
{
// 通用数据访问接口类
class QIM_CORE_API QImAbstractPlotDataSeries
{
public:
    /**
     * @brief 把内置的数据类型枚举出来，避免dynamic_cast
     */
    enum DataType
    {
        XYData
    };

public:
    QImAbstractPlotDataSeries()
    {
    }
    virtual ~QImAbstractPlotDataSeries() = default;
    /**
     * @brief 用于快速区分类型，避免dynamic_cast
     * @return
     */
    virtual int type() const = 0;

    /**
     * @brief 数据尺寸
     * @return
     */
    virtual int size() const = 0;
};

/**
 * @brief 针对XY数据的数据获取器
 */
class QIM_CORE_API QImAbstractXYDataSeries : public QImAbstractPlotDataSeries
{
public:
    QImAbstractXYDataSeries() : QImAbstractPlotDataSeries()
    {
    }
    virtual ~QImAbstractXYDataSeries() = default;

    // 识别类型，xydata将使用PlotLine
    virtual int type() const override
    {
        return XYData;
    }
    // 是否使用连续内存（可走 PlotLine 快速路径）
    virtual bool isContiguous() const = 0;

    // 步幅（stride），连续内存模式下有效
    virtual int stride() const
    {
        return sizeof(double);
    }

    // X数据指针：如果不是Y-only模式，返回nullptr表示使用索引计算
    // 如果返回非nullptr，ImPlot直接使用该指针（零拷贝）
    virtual const double* xRawData() const
    {
        return nullptr;
    }

    // Y数据指针：必须有效（Y-only模式也返回此指针）
    virtual const double* yRawData() const = 0;

    // Y-only模式参数（当xRawData返回nullptr时使用）
    virtual double xScale() const
    {
        return 1.0;
    }
    virtual double xStart() const
    {
        return 0.0;
    }
    virtual int offset() const
    {
        return 0;
    }

    virtual double xValue(int index) const = 0;
    virtual double yValue(int index) const = 0;
    /**
     * @brief 二分查找：给定X值，返回最接近的Y值
     *
     * 智能处理两种模式：
     * - 完整XY模式 (xRawData() != nullptr)：在X数组中二分查找
     * - Y-only模式 (xRawData() == nullptr)：通过公式计算索引
     *
     * @param x 目标X值
     * @param[out] index 可选：返回匹配的索引位置
     * @param[out] exact 可选：是否精确匹配（X值完全相等）
     * @return 对应的Y值；若数据无效返回NaN
     *
     * @note 要求X数据单调递增（时间序列/数值序列均满足）
     * @note 内部自动处理边界情况（x超出范围时返回首/尾点）
     */
    virtual double yValueAtX(double x, int* index = nullptr, bool* exact = nullptr) const
    {
        const int n = size();
        if (n <= 0) {
            if (index)
                *index = -1;
            if (exact)
                *exact = false;
            return std::numeric_limits< double >::quiet_NaN();
        }

        // Y-only 模式：通过公式计算索引
        if (!xRawData()) {
            double idx = offset() + (x - xStart()) / xScale();
            int i      = static_cast< int >(std::round(idx));
            i          = qBound(0, i, n - 1);
            if (index)
                *index = i;
            if (exact)
                *exact = (std::abs(idx - i) < 1e-6);
            return yRawData()[ i ];
        }

        // 完整XY模式：二分查找
        const double* xs = xRawData();
        int lo = 0, hi = n - 1;

        // 边界快速处理
        if (x <= xs[ 0 ]) {
            if (index)
                *index = 0;
            if (exact)
                *exact = (x == xs[ 0 ]);
            return yRawData()[ 0 ];
        }
        if (x >= xs[ n - 1 ]) {
            if (index)
                *index = n - 1;
            if (exact)
                *exact = (x == xs[ n - 1 ]);
            return yRawData()[ n - 1 ];
        }

        // 标准二分查找
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (std::abs(xs[ mid ] - x) < 1e-10) {  // 精确匹配
                if (index)
                    *index = mid;
                if (exact)
                    *exact = true;
                return yRawData()[ mid ];
            }
            if (xs[ mid ] < x) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }

        // 未精确匹配：返回最近邻
        int closest = (std::abs(xs[ lo ] - x) < std::abs(xs[ hi ] - x)) ? lo : hi;
        if (index)
            *index = closest;
        if (exact)
            *exact = false;
        return yRawData()[ closest ];
    }
};

/**
 * @brief 标准连续 double 容器封装
 *
 * 支持：std::vector<double>, QVector<double>, std::deque<double>（只要内存连续）
 *
 */
template< typename ContainerX, typename ContainerY >
class QImVectorXYDataSeries : public QImAbstractXYDataSeries
{
public:
    // 静态检查：确保容器存储的是 double（或可以按 double 解释的类型）
    static_assert(std::is_same_v< typename ContainerX::value_type, double >, "ContainerX must store double");
    static_assert(std::is_same_v< typename ContainerY::value_type, double >, "ContainerY must store double");
    explicit QImVectorXYDataSeries(ContainerX&& xs, ContainerY&& ys)
        : QImAbstractXYDataSeries(), m_xs(std::move(xs)), m_ys(std::move(ys)), m_yOnly(false)
    {
    }
    explicit QImVectorXYDataSeries(const ContainerX& xs, const ContainerY& ys)
        : QImAbstractXYDataSeries(), m_xs(xs), m_ys(ys), m_yOnly(false)
    {
    }
    virtual ~QImVectorXYDataSeries() = default;
    int size() const override
    {
        return std::min(m_xs.size(), m_ys.size());
    }

    bool isContiguous() const override
    {
        return true;
    }

    // 标准连续内存，步幅就是 sizeof(double)
    int stride() const override
    {
        return sizeof(double);
    }

    const double* xRawData() const override
    {
        // XY模式：交替存储 x,y,x,y...
        return (m_yOnly ? nullptr : m_xs.data());
    }

    const double* yRawData() const override
    {
        return m_ys.data();
    }

    double xScale() const override
    {
        return m_xScale;
    }
    double xStart() const override
    {
        return m_xStart;
    }
    virtual double xValue(int index) const
    {
        if (m_yOnly) {
            return m_xStart + (m_xScale * index);
        }
        return m_xs[ index ];
    }
    virtual double yValue(int index) const
    {
        return m_ys[ index ];
    }
    void setYOnly(bool on, double xStart = 0.0, double xScale = 1.0)
    {
        m_yOnly = on;
        if (on) {
            m_xStart = xStart;
            m_xScale = xScale;
        } else {
            // 非Y-only模式时重置参数，避免污染
            m_xStart = 0.0;
            m_xScale = 1.0;
        }
    }
    bool empty() const
    {
        return m_ys.empty();
    }
    typename ContainerX::value_type xmin() const
    {
        return xValue(0);
    }
    typename ContainerX::value_type xman() const
    {
        return xValue(size() - 1);
    }
    typename ContainerX::value_type ymin() const
    {
        return yValue(0);
    }
    typename ContainerX::value_type yman() const
    {
        return yValue(size() - 1);
    }

protected:
    ContainerX m_xs;
    ContainerY m_ys;
    bool m_yOnly    = false;
    double m_xScale = 1.0;
    double m_xStart = 0.0;
};
}
#endif  // QIMPLOTDATASERIES_H
