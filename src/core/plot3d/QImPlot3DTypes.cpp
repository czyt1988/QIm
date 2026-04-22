#include "QImPlot3DTypes.h"
#include <algorithm>
#include <cmath>

namespace QIM
{

// ===============================================================
// QImPlot3DPoint Implementation
// ===============================================================

/**
 * \if ENGLISH
 * @brief Multiply point by scalar
 * @details Returns a new point with each component multiplied by rhs.
 * \endif
 *
 * \if CHINESE
 * @brief 点乘标量
 * @details 返回各分量乘以rhs的新点。
 * \endif
 */
QImPlot3DPoint QImPlot3DPoint::operator*(double rhs) const
{
    return QImPlot3DPoint(x * rhs, y * rhs, z * rhs);
}

/**
 * \if ENGLISH
 * @brief Divide point by scalar
 * @details Returns a new point with each component divided by rhs.
 * \endif
 *
 * \if CHINESE
 * @brief 点除标量
 * @details 返回各分量除以rhs的新点。
 * \endif
 */
QImPlot3DPoint QImPlot3DPoint::operator/(double rhs) const
{
    return QImPlot3DPoint(x / rhs, y / rhs, z / rhs);
}

/**
 * \if ENGLISH
 * @brief Add two points
 * @details Returns a new point with component-wise addition.
 * \endif
 *
 * \if CHINESE
 * @brief 两点相加
 * @details 返回逐分量相加的新点。
 * \endif
 */
QImPlot3DPoint QImPlot3DPoint::operator+(const QImPlot3DPoint& rhs) const
{
    return QImPlot3DPoint(x + rhs.x, y + rhs.y, z + rhs.z);
}

/**
 * \if ENGLISH
 * @brief Subtract two points
 * @details Returns a new point with component-wise subtraction.
 * \endif
 *
 * \if CHINESE
 * @brief 两点相减
 * @details 返回逐分量相减的新点。
 * \endif
 */
QImPlot3DPoint QImPlot3DPoint::operator-(const QImPlot3DPoint& rhs) const
{
    return QImPlot3DPoint(x - rhs.x, y - rhs.y, z - rhs.z);
}

/**
 * \if ENGLISH
 * @brief Component-wise multiply two points
 * @details Returns a new point with each component multiplied by the corresponding component in rhs.
 * \endif
 *
 * \if CHINESE
 * @brief 逐分量相乘
 * @details 返回各分量与rhs对应分量相乘的新点。
 * \endif
 */
QImPlot3DPoint QImPlot3DPoint::operator*(const QImPlot3DPoint& rhs) const
{
    return QImPlot3DPoint(x * rhs.x, y * rhs.y, z * rhs.z);
}

/**
 * \if ENGLISH
 * @brief Component-wise divide two points
 * @details Returns a new point with each component divided by the corresponding component in rhs.
 * \endif
 *
 * \if CHINESE
 * @brief 逐分量相除
 * @details 返回各分量与rhs对应分量相除的新点。
 * \endif
 */
QImPlot3DPoint QImPlot3DPoint::operator/(const QImPlot3DPoint& rhs) const
{
    return QImPlot3DPoint(x / rhs.x, y / rhs.y, z / rhs.z);
}

/**
 * \if ENGLISH
 * @brief Unary negation
 * @details Returns a new point with all components negated.
 * \endif
 *
 * \if CHINESE
 * @brief 一元取负
 * @details 返回所有分量取反的新点。
 * \endif
 */
QImPlot3DPoint QImPlot3DPoint::operator-() const
{
    return QImPlot3DPoint(-x, -y, -z);
}

/**
 * \if ENGLISH
 * @brief Multiply-assign scalar
 * @details Multiplies each component by rhs and returns reference to self.
 * \endif
 *
 * \if CHINESE
 * @brief 乘赋标量
 * @details 各分量乘以rhs并返回自身引用。
 * \endif
 */
QImPlot3DPoint& QImPlot3DPoint::operator*=(double rhs)
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
}

/**
 * \if ENGLISH
 * @brief Divide-assign scalar
 * @details Divides each component by rhs and returns reference to self.
 * \endif
 *
 * \if CHINESE
 * @brief 除赋标量
 * @details 各分量除以rhs并返回自身引用。
 * \endif
 */
QImPlot3DPoint& QImPlot3DPoint::operator/=(double rhs)
{
    x /= rhs;
    y /= rhs;
    z /= rhs;
    return *this;
}

/**
 * \if ENGLISH
 * @brief Add-assign point
 * @details Adds rhs components to self and returns reference to self.
 * \endif
 *
 * \if CHINESE
 * @brief 加赋点
 * @details 将rhs各分量加到自身并返回自身引用。
 * \endif
 */
QImPlot3DPoint& QImPlot3DPoint::operator+=(const QImPlot3DPoint& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

/**
 * \if ENGLISH
 * @brief Subtract-assign point
 * @details Subtracts rhs components from self and returns reference to self.
 * \endif
 *
 * \if CHINESE
 * @brief 减赋点
 * @details 将rhs各分量从自身减去并返回自身引用。
 * \endif
 */
QImPlot3DPoint& QImPlot3DPoint::operator-=(const QImPlot3DPoint& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

/**
 * \if ENGLISH
 * @brief Component-wise multiply-assign point
 * @details Multiplies each component by corresponding rhs component and returns reference to self.
 * \endif
 *
 * \if CHINESE
 * @brief 逐分量乘赋点
 * @details 各分量乘以rhs对应分量并返回自身引用。
 * \endif
 */
QImPlot3DPoint& QImPlot3DPoint::operator*=(const QImPlot3DPoint& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
}

/**
 * \if ENGLISH
 * @brief Component-wise divide-assign point
 * @details Divides each component by corresponding rhs component and returns reference to self.
 * \endif
 *
 * \if CHINESE
 * @brief 逐分量除赋点
 * @details 各分量除以rhs对应分量并返回自身引用。
 * \endif
 */
QImPlot3DPoint& QImPlot3DPoint::operator/=(const QImPlot3DPoint& rhs)
{
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    return *this;
}

/**
 * \if ENGLISH
 * @brief Equality comparison
 * @details Returns true if all components are exactly equal.
 * \endif
 *
 * \if CHINESE
 * @brief 相等比较
 * @details 所有分量完全相等时返回true。
 * \endif
 */
bool QImPlot3DPoint::operator==(const QImPlot3DPoint& rhs) const
{
    return x == rhs.x && y == rhs.y && z == rhs.z;
}

/**
 * \if ENGLISH
 * @brief Inequality comparison
 * @details Returns true if any component differs.
 * \endif
 *
 * \if CHINESE
 * @brief 不等比较
 * @details 任一分量不同时返回true。
 * \endif
 */
bool QImPlot3DPoint::operator!=(const QImPlot3DPoint& rhs) const
{
    return !(*this == rhs);
}

/**
 * \if ENGLISH
 * @brief Dot product
 * @details Returns the scalar dot product of this point with rhs.
 * \endif
 *
 * \if CHINESE
 * @brief 点积
 * @details 返回此点与rhs的标量点积。
 * \endif
 */
double QImPlot3DPoint::Dot(const QImPlot3DPoint& rhs) const
{
    return x * rhs.x + y * rhs.y + z * rhs.z;
}

/**
 * \if ENGLISH
 * @brief Cross product
 * @details Returns the 3D cross product vector of this point with rhs.
 * \endif
 *
 * \if CHINESE
 * @brief 叉积
 * @details 返回此点与rhs的三维叉积向量。
 * \endif
 */
QImPlot3DPoint QImPlot3DPoint::Cross(const QImPlot3DPoint& rhs) const
{
    return QImPlot3DPoint(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
}

/**
 * \if ENGLISH
 * @brief Vector length
 * @details Returns the Euclidean length (magnitude) of the vector.
 * \endif
 *
 * \if CHINESE
 * @brief 向量长度
 * @details 返回向量的欧几里得长度（模）。
 * \endif
 */
double QImPlot3DPoint::Length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

/**
 * \if ENGLISH
 * @brief Vector squared length
 * @details Returns the squared Euclidean length, avoiding sqrt for performance.
 * \endif
 *
 * \if CHINESE
 * @brief 向量平方长度
 * @details 返回欧几里得长度的平方，避免sqrt以提升性能。
 * \endif
 */
double QImPlot3DPoint::LengthSquared() const
{
    return x * x + y * y + z * z;
}

/**
 * \if ENGLISH
 * @brief Normalize in place
 * @details Divides each component by the vector length, making it a unit vector.
 * \endif
 *
 * \if CHINESE
 * @brief 就地归一化
 * @details 各分量除以向量长度，使其成为单位向量。
 * \endif
 */
void QImPlot3DPoint::Normalize()
{
    double l = Length();
    x /= l;
    y /= l;
    z /= l;
}

/**
 * \if ENGLISH
 * @brief Return normalized copy
 * @details Returns a new unit vector without modifying the original.
 * \endif
 *
 * \if CHINESE
 * @brief 返回归一化副本
 * @details 返回新的单位向量，不修改原始向量。
 * \endif
 */
QImPlot3DPoint QImPlot3DPoint::Normalized() const
{
    double l = Length();
    return QImPlot3DPoint(x / l, y / l, z / l);
}

/**
 * \if ENGLISH
 * @brief Left scalar multiplication (commutative)
 * @details Allows scalar * point syntax by multiplying lhs with each rhs component.
 * \endif
 *
 * \if CHINESE
 * @brief 左侧标量乘法（交换律）
 * @details 允许标量*点语法，lhs乘以rhs各分量。
 * \endif
 */
QImPlot3DPoint operator*(double lhs, const QImPlot3DPoint& rhs)
{
    return QImPlot3DPoint(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}

/**
 * \if ENGLISH
 * @brief Check if any component is NaN
 * @details Returns true if x, y, or z is NaN (not-a-number).
 * \endif
 *
 * \if CHINESE
 * @brief 检查是否有分量为NaN
 * @details 当x、y或z为NaN（非数字）时返回true。
 * \endif
 */
bool QImPlot3DPoint::IsNaN() const
{
    return std::isnan(x) || std::isnan(y) || std::isnan(z);
}

// ===============================================================
// QImPlot3DBox Implementation
// ===============================================================

/**
 * \if ENGLISH
 * @brief Expand box to include a point
 * @details Updates Min and Max corners so the box contains the given point.
 * \endif
 *
 * \if CHINESE
 * @brief 扩展包围盒以包含一个点
 * @details 更新Min和Max角点使包围盒包含给定点。
 * \endif
 */
void QImPlot3DBox::Expand(const QImPlot3DPoint& point)
{
    Min.x = std::min(Min.x, point.x);
    Min.y = std::min(Min.y, point.y);
    Min.z = std::min(Min.z, point.z);
    Max.x = std::max(Max.x, point.x);
    Max.y = std::max(Max.y, point.y);
    Max.z = std::max(Max.z, point.z);
}

/**
 * \if ENGLISH
 * @brief Check if point is inside the box
 * @details Returns true if point coordinates are within Min and Max bounds on all axes.
 * \endif
 *
 * \if CHINESE
 * @brief 检查点是否在包围盒内
 * @details 当点坐标在各轴的Min和Max范围内时返回true。
 * \endif
 */
bool QImPlot3DBox::Contains(const QImPlot3DPoint& point) const
{
    return (point.x >= Min.x && point.x <= Max.x)
        && (point.y >= Min.y && point.y <= Max.y)
        && (point.z >= Min.z && point.z <= Max.z);
}

/**
 * \if ENGLISH
 * @brief Clip a line segment against the box (Liang-Barsky 3D algorithm)
 * @details Clips the line segment p0-p1 against the box boundaries.
 * Returns true if any part of the segment is inside the box,
 * and outputs the clipped endpoints via p0_clipped and p1_clipped.
 * \endif
 *
 * \if CHINESE
 * @brief 对包围盒裁剪线段（Liang-Barsky 3D算法）
 * @details 将线段p0-p1裁剪至包围盒边界。
 * 若线段有部分在盒内则返回true，
 * 并通过p0_clipped和p1_clipped输出裁剪后的端点。
 * \endif
 */
bool QImPlot3DBox::ClipLineSegment(const QImPlot3DPoint& p0, const QImPlot3DPoint& p1,
                                    QImPlot3DPoint& p0_clipped, QImPlot3DPoint& p1_clipped) const
{
    // Check if the line segment is completely inside the box
    if (Contains(p0) && Contains(p1)) {
        p0_clipped = p0;
        p1_clipped = p1;
        return true;
    }

    // Perform Liang-Barsky 3D clipping
    double t0 = 0.0;
    double t1 = 1.0;
    QImPlot3DPoint d = p1 - p0;

    // Define the clipping boundaries
    const double xmin = Min.x, xmax = Max.x;
    const double ymin = Min.y, ymax = Max.y;
    const double zmin = Min.z, zmax = Max.z;

    // Lambda function to update t0 and t1
    auto update = [&](double p, double q) -> bool {
        if (p == 0.0) {
            if (q < 0.0)
                return false; // Line is parallel and outside the boundary
            else
                return true; // Line is parallel and inside or coincident with boundary
        }
        double r = q / p;
        if (p < 0.0) {
            if (r > t1)
                return false; // Line is outside
            if (r > t0)
                t0 = r; // Move up t0
        } else {
            if (r < t0)
                return false; // Line is outside
            if (r < t1)
                t1 = r; // Move down t1
        }
        return true;
    };

    // Clip against each boundary
    if (!update(-d.x, p0.x - xmin))
        return false; // Left
    if (!update(d.x, xmax - p0.x))
        return false; // Right
    if (!update(-d.y, p0.y - ymin))
        return false; // Bottom
    if (!update(d.y, ymax - p0.y))
        return false; // Top
    if (!update(-d.z, p0.z - zmin))
        return false; // Near
    if (!update(d.z, zmax - p0.z))
        return false; // Far

    // Compute clipped points
    p0_clipped = p0 + d * t0;
    p1_clipped = p0 + d * t1;

    return true;
}

// ===============================================================
// QImPlot3DRange Implementation
// ===============================================================

/**
 * \if ENGLISH
 * @brief Expand range to include a value
 * @details Updates Min and Max so the range includes the given value.
 * \endif
 *
 * \if CHINESE
 * @brief 扩展范围以包含一个值
 * @details 更新Min和Max使范围包含给定值。
 * \endif
 */
void QImPlot3DRange::Expand(double value)
{
    Min = std::min(Min, value);
    Max = std::max(Max, value);
}

/**
 * \if ENGLISH
 * @brief Check if value is within range
 * @details Returns true if value is between Min and Max (inclusive).
 * \endif
 *
 * \if CHINESE
 * @brief 检查值是否在范围内
 * @details 当值在Min和Max之间（含边界）时返回true。
 * \endif
 */
bool QImPlot3DRange::Contains(double value) const
{
    return value >= Min && value <= Max;
}

}  // namespace QIM