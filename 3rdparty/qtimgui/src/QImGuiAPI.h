#ifndef QIMGUIAPI_H
#define QIMGUIAPI_H
#include <QtGlobal>
#if defined(QTIMGUI_DLL)
#if defined(QTIMGUI_BUILDLIB)
#ifndef QTIMGUI_API
#define QTIMGUI_API Q_DECL_EXPORT
#endif
#else
#ifndef QTIMGUI_API
#define QTIMGUI_API Q_DECL_IMPORT
#endif
#endif
#endif

#ifndef QTIMGUI_API
#define QTIMGUI_API
#endif

#endif  // QIMPLOTAPI_H
