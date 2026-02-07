#ifndef QIMAPI_H
#define QIMAPI_H
#include <memory>
#if defined(_WIN32) || defined(__CYGWIN__)
#ifndef QIM_EXPORT
#define QIM_EXPORT __declspec(dllexport)
#endif
#ifndef QIM_IMPORT
#define QIM_IMPORT __declspec(dllimport)
#endif
#define MY_LOCAL /* 空，Windows 不支持隐藏符号 */
#else
#if __GNUC__ >= 4
#ifndef QIM_EXPORT
#define QIM_EXPORT __attribute__((visibility("default")))
#endif
#ifndef QIM_IMPORT
#define QIM_IMPORT
#endif
#endif
#endif

// 把ImDrawIdx设置为unsigned int，支持更多的节点
#ifndef ImDrawIdx
#define ImDrawIdx unsigned int
#endif

#if defined(QIM_CORE_DLL)
#if defined(QIM_CORE_BUILDLIB)
#ifndef QIM_CORE_API
#define QIM_CORE_API QIM_EXPORT
#endif
#ifndef IMGUI_API
#define IMGUI_API QIM_EXPORT
#endif
#ifndef IMPLOT_API
#define IMPLOT_API QIM_EXPORT
#endif
#ifndef IMPLOT3D_API
#define IMPLOT3D_API QIM_EXPORT
#endif
#else
#ifndef QIM_CORE_API
#define QIM_CORE_API QIM_IMPORT
#endif
#ifndef IMGUI_API
#define IMGUI_API QIM_IMPORT
#endif
#ifndef IMPLOT_API
#define IMPLOT_API QIM_IMPORT
#endif
#ifndef IMPLOT3D_API
#define IMPLOT3D_API QIM_IMPORT
#endif

#endif
#endif

#if defined(QIM_WIDGETS_DLL)
#if defined(QIM_WIDGETS_BUILDLIB)
#ifndef QIM_WIDGETS_API
#define QIM_WIDGETS_API QIM_EXPORT
#endif
#else
#ifndef QIM_WIDGETS_API
#define QIM_WIDGETS_API QIM_IMPORT
#endif
#endif
#endif

#ifndef QIMPLOTCORE_API
#define QIMPLOTCORE_API
#endif

#ifndef QIM_WIDGETS_API
#define QIM_WIDGETS_API
#endif

/**
 * @def QIM_DECLARE_PRIVATE
 * @brief 模仿Q_DECLARE_PRIVATE，但不用前置声明而是作为一个内部类
 *
 * 例如:
 *
 * @code
 * //header
 * class A
 * {
 *  QIM_DECLARE_PRIVATE(A)
 * };
 * @endcode
 *
 * 其展开效果为：
 *
 * @code
 * class A{
 *  class PrivateData;
 *  friend class A::PrivateData;
 *  std::unique_ptr< PrivateData > d_ptr;
 * }
 * @endcode
 *
 * 这样前置声明了一个内部类PrivateData，在cpp文件中建立这个内部类的实现
 *
 * @code
 * //cpp
 * class A::PrivateData{
 *  QIM_DECLARE_PUBLIC(A)
 *  PrivateData(A* p):q_ptr(p){
 *  }
 * };
 *
 * A::A():d_ptr(new PrivateData(this)){
 * }
 * @endcode
 *
 */
#ifndef QIM_DECLARE_PRIVATE
#define QIM_DECLARE_PRIVATE(classname)                                                                                 \
    class PrivateData;                                                                                                 \
    friend class classname::PrivateData;                                                                               \
    std::unique_ptr< PrivateData > d_ptr;                                                                              \
    inline PrivateData* d_func()                                                                                       \
    {                                                                                                                  \
        return (d_ptr.get());                                                                                          \
    }                                                                                                                  \
    inline const PrivateData* d_func() const                                                                           \
    {                                                                                                                  \
        return (d_ptr.get());                                                                                          \
    }
#endif

/**
 * @def QIM_DECLARE_PUBLIC
 * @brief 模仿Q_DECLARE_PUBLIC
 *
 * 配套QIM_DECLARE_PRIVATE使用
 */
#ifndef QIM_DECLARE_PUBLIC
#define QIM_DECLARE_PUBLIC(classname)                                                                                  \
    friend class classname;                                                                                            \
    classname* q_ptr { nullptr };                                                                                      \
    inline classname* q_func()                                                                                         \
    {                                                                                                                  \
        return (static_cast< classname* >(q_ptr));                                                                     \
    }                                                                                                                  \
    inline const classname* q_func() const                                                                             \
    {                                                                                                                  \
        return (static_cast< const classname* >(q_ptr));                                                               \
    }
#endif

/**
 * @def  QIM_PIMPL_CONSTRUCT
 *
 * 配套QIM_DECLARE_PRIVATE使用,在构造函数中构建privatedata
 */
#ifndef QIM_PIMPL_CONSTRUCT
#define QIM_PIMPL_CONSTRUCT d_ptr(std::make_unique< PrivateData >(this))
#endif

/**
 *@def QIM_D
 *@brief impl获取指针，参考Q_D
 */
#ifndef QIM_D
#define QIM_D(pointerName) PrivateData* pointerName = d_func()
#endif

/**
 *@def QIM_DC
 *@brief impl获取指针，参考Q_D
 */
#ifndef QIM_DC
#define QIM_DC(pointerName) const PrivateData* pointerName = d_func()
#endif

/**
 *@def QIM_Q
 *@brief impl获取指针，参考Q_Q
 */
#ifndef QIM_Q
#define QIM_Q(classname, pointerName) classname* pointerName = q_func()
#endif

/**
 *@def QIM_QC
 *@brief impl获取指针，参考Q_Q
 */
#ifndef QIM_QC
#define QIM_QC(classname, pointerName) const classname* pointerName = q_func()
#endif
#endif  // QIMAPI_H
