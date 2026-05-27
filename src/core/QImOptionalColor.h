#ifndef QIMOPTIONALCOLOR_H
#define QIMOPTIONALCOLOR_H
#include "QImTrackedColor.h"
#include <optional>
namespace QIM
{
/**
 * @brief Optional tracked color value
 * @details Used for Plot Item color properties:
 *          - std::nullopt: user hasn't set a color, use ImPlot default
 *          - has value: user set a color, or captured ImPlot default
 *          Compatible with deferred initialization pattern in beginDraw().
 */
using QImOptionalColor = std::optional<QImTrackedColor>;
}
#endif // QIMOPTIONALCOLOR_H
