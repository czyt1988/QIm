#include "TestFunction.h"

/**
 * \if ENGLISH
 * @brief Construct a new Test Function object
 * @param parent Parent object
 * \endif
 * 
 * \if CHINESE
 * @brief 构造新的测试函数对象
 * @param parent 父对象
 * \endif
 */
TestFunction::TestFunction(QObject* parent)
    : QObject(parent) {
}

/**
 * \if ENGLISH
 * @brief Destroy the Test Function object
 * \endif
 * 
 * \if CHINESE
 * @brief 销毁测试函数对象
 * \endif
 */
TestFunction::~TestFunction() {
}

/**
 * \if ENGLISH
 * @brief Cleanup plot nodes and clear registrations
 * 
 * Default implementation clears m_registrations and m_plotNodes.
 * Does NOT delete QIm nodes - they are managed by the figure widget.
 * \endif
 * 
 * \if CHINESE
 * @brief 清理绘图节点并清除注册
 * 
 * 默认实现清除 m_registrations 和 m_plotNodes。
 * 不会删除 QIm 节点 - 它们由图表控件管理。
 * \endif
 */
void TestFunction::cleanupPlot() {
    clearRegistrations();
    m_plotNodes.clear();
}

/**
 * \if ENGLISH
 * @brief Get list of created plot nodes
 * @return List of QObject pointers to plot nodes
 * \endif
 * 
 * \if CHINESE
 * @brief 获取已创建的绘图节点列表
 * @return 指向绘图节点的 QObject 指针列表
 * \endif
 */
QList<QObject*> TestFunction::getPlotNodes() const {
    return m_plotNodes;
}

/**
 * \if ENGLISH
 * @brief Get list of registered properties
 * @return Const copy of registered properties
 * \endif
 * 
 * \if CHINESE
 * @brief 获取已注册的属性列表
 * @return 已注册属性的常量副本
 * \endif
 */
QList<TestFunction::PropertyRegistration> TestFunction::getRegisteredProperties() const {
    return m_registrations;
}

/**
 * \if ENGLISH
 * @brief Register a property for editing
 * @param reg Property registration metadata
 * \endif
 * 
 * \if CHINESE
 * @brief 注册一个属性用于编辑
 * @param reg 属性注册元数据
 * \endif
 */
void TestFunction::registerProperty(const PropertyRegistration& reg) {
    m_registrations.append(reg);
}

/**
 * \if ENGLISH
 * @brief Clear all property registrations
 * \endif
 * 
 * \if CHINESE
 * @brief 清除所有属性注册
 * \endif
 */
void TestFunction::clearRegistrations() {
    m_registrations.clear();
}
