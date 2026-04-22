#include "QImPlot3DAxisFormatter.h"

namespace QIM
{

//===============================================================
// QImPlot3DAxisFormatter
//===============================================================

/**
 * \if ENGLISH
 * @brief Destructor for QImPlot3DAxisFormatter
 * \endif
 *
 * \if CHINESE
 * @brief QImPlot3DAxisFormatter 析构函数
 * \endif
 */
QImPlot3DAxisFormatter::~QImPlot3DAxisFormatter()
{
}

//===============================================================
// QImPlot3DDefaultAxisFormatter
//===============================================================

/**
 * \if ENGLISH
 * @brief Destructor for QImPlot3DDefaultAxisFormatter
 * \endif
 *
 * \if CHINESE
 * @brief QImPlot3DDefaultAxisFormatter 析构函数
 * \endif
 */
QImPlot3DDefaultAxisFormatter::~QImPlot3DDefaultAxisFormatter()
{
}

/**
 * \if ENGLISH
 * @brief Formats a numeric tick value using standard %g notation
 * @param value The numeric value to format
 * @param prefix Optional prefix text (prepended if non-empty)
 * @return Formatted string: prefix + QByteArray::number(value, 'g', 6)
 * @details Uses QByteArray::number(value, 'g', 6) which produces general-format
 *          floating point output with up to 6 significant digits, similar to printf %g.
 *          If prefix is non-empty, it is prepended to the formatted number.
 * \endif
 *
 * \if CHINESE
 * @brief 使用标准 %g 格式化数值刻度值
 * @param value 需要格式化的数值
 * @param prefix 可选前缀文本（非空时附加在数值前）
 * @return 格式化字符串：prefix + QByteArray::number(value, 'g', 6)
 * @details 使用 QByteArray::number(value, 'g', 6)，生成通用格式浮点数输出，
 *          最多6位有效数字，类似于 printf %g。若 prefix 非空，则附加在格式化数值之前。
 * \endif
 */
QByteArray QImPlot3DDefaultAxisFormatter::format(double value, const QByteArray& prefix)
{
    QByteArray result = QByteArray::number(value, 'g', 6);
    if (!prefix.isEmpty()) {
        result = prefix + result;
    }
    return result;
}

}  // namespace QIM