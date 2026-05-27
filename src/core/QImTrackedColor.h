#ifndef QIMTRACKEDCOLOR_H
#define QIMTRACKEDCOLOR_H
#include "QImTrackedValue.hpp"
#include "QtImGuiUtils.h"
struct ImVec4;
namespace QIM
{

/**
 * @brief Tracked ImVec4 color value with dirty flag
 * @details Wraps ImVec4 with change detection, used when tracking color
 *          changes and emitting signals.
 */
using QImTrackedColor = QImTrackedValue<ImVec4, ImVecComparator<ImVec4>>;


}
#endif // QIMTRACKEDCOLOR_H
