#pragma once

class QWidget;
class QWindow;
#include "QImGuiAPI.h"
namespace QtImGui {

typedef void* RenderRef;

#ifdef QT_WIDGETS_LIB
RenderRef QTIMGUI_API initialize(QWidget *window, bool defaultRender = true);
#endif

RenderRef QTIMGUI_API initialize(QWindow *window, bool defaultRender = true);
void QTIMGUI_API newFrame(RenderRef ref = nullptr);
void QTIMGUI_API render(RenderRef ref = nullptr);

}
