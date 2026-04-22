#ifndef QIMPLOT3DAXISTRANSFORM_H
#define QIMPLOT3DAXISTRANSFORM_H

#include "QImAPI.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Abstract interface for custom axis scale transforms
 * @details Provides a virtual interface that bridges Qt-style axis transform logic
 *          to ImPlot3D's C callback-based SetupAxisScale API. Subclass this to implement
 *          custom forward (data→screen) and inverse (screen→data) transform functions.
 * @note Lifetime is managed externally — QImPlot3DAxisInfo does NOT own the transform object.
 * @see QImPlot3DAxisInfo::setAxisTransform(), ImPlot3D::SetupAxisScale()
 * \endif
 *
 * \if CHINESE
 * @brief 自定义坐标轴刻度变换的抽象接口
 * @details 提供虚接口，将 Qt 风格的轴变换逻辑桥接到 ImPlot3D 的 C 回调式 SetupAxisScale API。
 *          继承此类实现自定义的 forward（数据→屏幕）和 inverse（屏幕→数据）变换函数。
 * @note 生命周期由外部管理 — QImPlot3DAxisInfo 不拥有变换对象。
 * @see QImPlot3DAxisInfo::setAxisTransform(), ImPlot3D::SetupAxisScale()
 * \endif
 */
class QIM_CORE_API QImPlot3DAxisTransform
{
public:
    virtual ~QImPlot3DAxisTransform() = default;

    // Forward transform: data value → screen coordinate
    virtual double forward(double value) = 0;

    // Inverse transform: screen coordinate → data value
    virtual double inverse(double value) = 0;
};

}  // namespace QIM

#endif  // QIMPLOT3DAXISTRANSFORM_H