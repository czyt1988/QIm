#ifndef QIMPLOT3DAXISFORMATTER_H
#define QIMPLOT3DAXISFORMATTER_H

#include <QObject>
#include "QImAPI.h"

namespace QIM
{

/**
 * \if ENGLISH
 * @brief Abstract base class for custom axis tick label formatting in 3D plots
 * @details Subclass this to provide custom formatting logic for axis tick labels.
 *          The format() method is called by ImPlot3D during rendering to convert
 *          numeric tick values into display strings.
 * @note The formatter object must remain alive during plot rendering. If the formatter
 *       is deleted before the plot renders, undefined behavior will occur.
 * @see QImPlot3DDefaultAxisFormatter, QImPlot3DAxisInfo::setAxisFormatter()
 * \endif
 *
 * \if CHINESE
 * @brief 3D 绘图坐标轴刻度标签自定义格式的抽象基类
 * @details 继承此类以提供坐标轴刻度标签的自定义格式化逻辑。
 *          format() 方法在渲染期间由 ImPlot3D 调用，将数值刻度值转换为显示字符串。
 * @note 格式化器对象在绘图渲染期间必须保持存活。若格式化器在绘图渲染之前被删除，
 *       将导致未定义行为。
 * @see QImPlot3DDefaultAxisFormatter, QImPlot3DAxisInfo::setAxisFormatter()
 * \endif
 */
class QIM_CORE_API QImPlot3DAxisFormatter
{
public:
    virtual ~QImPlot3DAxisFormatter();

    // Format a numeric tick value into a display string
    virtual QByteArray format(double value, const QByteArray& prefix) = 0;
};

/**
 * \if ENGLISH
 * @brief Default axis formatter providing standard numeric formatting (%g style)
 * @details Uses QByteArray::number(value, 'g', 6) for general-format numeric output.
 *          If a prefix is provided, it is prepended to the formatted number.
 * @see QImPlot3DAxisFormatter
 * \endif
 *
 * \if CHINESE
 * @brief 默认坐标轴格式化器，提供标准数值格式化（%g 风格）
 * @details 使用 QByteArray::number(value, 'g', 6) 进行通用格式数值输出。
 *          若提供了前缀，则将其附加在格式化数值之前。
 * @see QImPlot3DAxisFormatter
 * \endif
 */
class QIM_CORE_API QImPlot3DDefaultAxisFormatter : public QImPlot3DAxisFormatter
{
public:
    ~QImPlot3DDefaultAxisFormatter() override;

    // Format a numeric value using standard %g notation, prepending prefix if non-empty
    QByteArray format(double value, const QByteArray& prefix) override;
};

}  // namespace QIM

#endif  // QIMPLOT3DAXISFORMATTER_H