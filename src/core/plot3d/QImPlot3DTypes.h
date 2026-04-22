#ifndef QIMPLOT3DTYPES_H
#define QIMPLOT3DTYPES_H

#include "QImAPI.h"
#include <QtGlobal>
#include <cmath>
#include <limits>

namespace QIM
{

/**
 * \if ENGLISH
 * @brief 3D point/vector with double precision
 *
 * @details Mirrors ImPlot3DPoint API with double-precision arithmetic.
 * Provides all standard vector operations: arithmetic, dot/cross products,
 * normalization, and NaN detection.
 *
 * @see ImPlot3DPoint
 * \endif
 *
 * \if CHINESE
 * @brief 双精度三维点/向量
 *
 * @details 镜像ImPlot3DPoint API，使用双精度算术。
 * 提供所有标准向量运算：算术、点积/叉积、归一化和NaN检测。
 *
 * @see ImPlot3DPoint
 * \endif
 */
struct QImPlot3DPoint
{
    double x, y, z;

    constexpr QImPlot3DPoint() : x(0.0), y(0.0), z(0.0) {}
    constexpr QImPlot3DPoint(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

    // Accessors with bounds check
    double& operator[](size_t idx)
    {
        Q_ASSERT(idx == 0 || idx == 1 || idx == 2);
        return ((double*)(void*)(char*)this)[idx];
    }
    double operator[](size_t idx) const
    {
        Q_ASSERT(idx == 0 || idx == 1 || idx == 2);
        return ((const double*)(const void*)(const char*)this)[idx];
    }

    // Binary operators with scalar
    QIM_CORE_API QImPlot3DPoint operator*(double rhs) const;
    QIM_CORE_API QImPlot3DPoint operator/(double rhs) const;

    // Binary operators with point
    QIM_CORE_API QImPlot3DPoint operator+(const QImPlot3DPoint& rhs) const;
    QIM_CORE_API QImPlot3DPoint operator-(const QImPlot3DPoint& rhs) const;
    QIM_CORE_API QImPlot3DPoint operator*(const QImPlot3DPoint& rhs) const;
    QIM_CORE_API QImPlot3DPoint operator/(const QImPlot3DPoint& rhs) const;

    // Unary operator
    QIM_CORE_API QImPlot3DPoint operator-() const;

    // Compound assignment operators
    QIM_CORE_API QImPlot3DPoint& operator*=(double rhs);
    QIM_CORE_API QImPlot3DPoint& operator/=(double rhs);
    QIM_CORE_API QImPlot3DPoint& operator+=(const QImPlot3DPoint& rhs);
    QIM_CORE_API QImPlot3DPoint& operator-=(const QImPlot3DPoint& rhs);
    QIM_CORE_API QImPlot3DPoint& operator*=(const QImPlot3DPoint& rhs);
    QIM_CORE_API QImPlot3DPoint& operator/=(const QImPlot3DPoint& rhs);

    // Comparison operators
    QIM_CORE_API bool operator==(const QImPlot3DPoint& rhs) const;
    QIM_CORE_API bool operator!=(const QImPlot3DPoint& rhs) const;

    // Dot product
    QIM_CORE_API double Dot(const QImPlot3DPoint& rhs) const;

    // Cross product
    QIM_CORE_API QImPlot3DPoint Cross(const QImPlot3DPoint& rhs) const;

    // Get vector length
    QIM_CORE_API double Length() const;

    // Get vector squared length
    QIM_CORE_API double LengthSquared() const;

    // Normalize to unit length (in place)
    QIM_CORE_API void Normalize();

    // Return normalized vector
    QIM_CORE_API QImPlot3DPoint Normalized() const;

    // Check if any component is NaN
    QIM_CORE_API bool IsNaN() const;

    // Friend: allow scalar * point (commutative)
    QIM_CORE_API friend QImPlot3DPoint operator*(double lhs, const QImPlot3DPoint& rhs);
};

/**
 * \if ENGLISH
 * @brief Ray in 3D space defined by origin and direction
 *
 * @details Mirrors ImPlot3DRay — a simple aggregate of two QImPlot3DPoints.
 *
 * @see ImPlot3DRay
 * \endif
 *
 * \if CHINESE
 * @brief 由原点和方向定义的三维射线
 *
 * @details 镜像ImPlot3DRay——两个QImPlot3DPoint的简单聚合。
 *
 * @see ImPlot3DRay
 * \endif
 */
struct QImPlot3DRay
{
    QImPlot3DPoint Origin;
    QImPlot3DPoint Direction;
};

/**
 * \if ENGLISH
 * @brief Plane in 3D space defined by a point and normal vector
 *
 * @details Mirrors ImPlot3DPlane — a simple aggregate of two QImPlot3DPoints.
 *
 * @see ImPlot3DPlane
 * \endif
 *
 * \if CHINESE
 * @brief 由点和法向量定义的三维平面
 *
 * @details 镜像ImPlot3DPlane——两个QImPlot3DPoint的简单聚合。
 *
 * @see ImPlot3DPlane
 * \endif
 */
struct QImPlot3DPlane
{
    QImPlot3DPoint Point;
    QImPlot3DPoint Normal;
};

/**
 * \if ENGLISH
 * @brief Axis-aligned bounding box in 3D space
 *
 * @details Mirrors ImPlot3DBox with Min/Max corners and utility methods
 * Expand, Contains, and ClipLineSegment.
 *
 * @see ImPlot3DBox
 * \endif
 *
 * \if CHINESE
 * @brief 三维轴对齐包围盒
 *
 * @details 镜像ImPlot3DBox，包含Min/Max角点和Expand、Contains、ClipLineSegment工具方法。
 *
 * @see ImPlot3DBox
 * \endif
 */
struct QImPlot3DBox
{
    QImPlot3DPoint Min;
    QImPlot3DPoint Max;

    constexpr QImPlot3DBox() : Min(QImPlot3DPoint()), Max(QImPlot3DPoint()) {}
    constexpr QImPlot3DBox(const QImPlot3DPoint& min, const QImPlot3DPoint& max) : Min(min), Max(max) {}

    // Expand box to include a point
    QIM_CORE_API void Expand(const QImPlot3DPoint& point);

    // Check if point is inside the box
    QIM_CORE_API bool Contains(const QImPlot3DPoint& point) const;

    // Clip a line segment against the box
    QIM_CORE_API bool ClipLineSegment(const QImPlot3DPoint& p0, const QImPlot3DPoint& p1,
                                       QImPlot3DPoint& p0_clipped, QImPlot3DPoint& p1_clipped) const;
};

/**
 * \if ENGLISH
 * @brief 1D range with min and max values
 *
 * @details Mirrors ImPlot3DRange with Expand, Contains, and Size methods.
 *
 * @see ImPlot3DRange
 * \endif
 *
 * \if CHINESE
 * @brief 一维范围（最小值和最大值）
 *
 * @details 镜像ImPlot3DRange，包含Expand、Contains和Size方法。
 *
 * @see ImPlot3DRange
 * \endif
 */
struct QImPlot3DRange
{
    double Min;
    double Max;

    constexpr QImPlot3DRange() : Min(0.0), Max(0.0) {}
    constexpr QImPlot3DRange(double min, double max) : Min(min), Max(max) {}

    // Expand range to include value
    QIM_CORE_API void Expand(double value);

    // Check if value is within range
    QIM_CORE_API bool Contains(double value) const;

    // Get range size
    double Size() const { return Max - Min; }
};

}  // namespace QIM

#endif  // QIMPLOT3DTYPES_H