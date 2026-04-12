#ifndef TESTFUNCTION_H
#define TESTFUNCTION_H

#include <QObject>
#include <QList>
#include <QString>
#include <QVariant>
#include <QStringList>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

namespace QIM {
class QImFigureWidget;
}

/**
 * \if ENGLISH
 * @brief Abstract base class for all test functions with property registration system
 * 
 * TestFunction provides a standardized interface for creating test functions that demonstrate
 * QIm figure capabilities. Each test function can register properties that will be displayed
 * in a property editor panel, allowing users to interactively modify plot parameters.
 * 
 * @section usage_example Usage Example
 * 
 * \code
 * class SineWaveFunction : public TestFunction {
 *     Q_OBJECT
 *     Q_PROPERTY(double frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)
 *     
 * public:
 *     explicit SineWaveFunction(QObject* parent = nullptr) : TestFunction(parent) {
 *         PropertyRegistration reg;
 *         reg.category = "Wave";
 *         reg.subcategory = "Parameters";
 *         reg.displayName = "Frequency";
 *         reg.briefDesc = "Wave frequency";
 *         reg.detailDesc = "Sets the frequency of the sine wave in Hz";
 *         reg.editorType = EditorType::DoubleSpinBox;
 *         reg.defaultValue = 1.0;
 *         reg.minValue = 0.1;
 *         reg.maxValue = 100.0;
 *         reg.stepValue = 0.1;
 *         reg.propertyName = "frequency";
 *         reg.target = this;
 *         registerProperty(reg);
 *     }
 *     
 *     void createPlot(QIM::QImFigureWidget* figure) override {
 *         // Create plot nodes here
 *     }
 *     
 * private:
 *     double m_frequency = 1.0;
 *     
 * public:
 *     double frequency() const { return m_frequency; }
 *     void setFrequency(double freq) {
 *         m_frequency = freq;
 *         emit frequencyChanged(freq);
 *     }
 *     
 * Q_SIGNALS:
 *     void frequencyChanged(double freq);
 * };
 * \endcode
 * 
 * @see PropertyRegistration
 * @see EditorType
 * \endif
 * 
 * \if CHINESE
 * @brief 所有测试函数的抽象基类，带有属性注册系统
 * 
 * TestFunction 为创建演示 QIm 图表功能的测试函数提供了标准化接口。每个测试函数可以注册属性，
 * 这些属性将显示在属性编辑器面板中，允许用户交互式地修改绘图参数。
 * 
 * @section usage_example 使用示例
 * 
 * \code
 * class SineWaveFunction : public TestFunction {
 *     Q_OBJECT
 *     Q_PROPERTY(double frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)
 *     
 * public:
 *     explicit SineWaveFunction(QObject* parent = nullptr) : TestFunction(parent) {
 *         PropertyRegistration reg;
 *         reg.category = "Wave";
 *         reg.subcategory = "Parameters";
 *         reg.displayName = "Frequency";
 *         reg.briefDesc = "Wave frequency";
 *         reg.detailDesc = "Sets the frequency of the sine wave in Hz";
 *         reg.editorType = EditorType::DoubleSpinBox;
 *         reg.defaultValue = 1.0;
 *         reg.minValue = 0.1;
 *         reg.maxValue = 100.0;
 *         reg.stepValue = 0.1;
 *         reg.propertyName = "frequency";
 *         reg.target = this;
 *         registerProperty(reg);
 *     }
 *     
 *     void createPlot(QIM::QImFigureWidget* figure) override {
 *         // Create plot nodes here
 *     }
 *     
 * private:
 *     double m_frequency = 1.0;
 *     
 * public:
 *     double frequency() const { return m_frequency; }
 *     void setFrequency(double freq) {
 *         m_frequency = freq;
 *         emit frequencyChanged(freq);
 *     }
 *     
 * Q_SIGNALS:
 *     void frequencyChanged(double freq);
 * };
 * \endcode
 * 
 * @see PropertyRegistration
 * @see EditorType
 * \endif
 */
class TestFunction : public QObject {
    Q_OBJECT

public:
    /**
     * \if ENGLISH
     * @brief Editor types for property editor widgets
     * \endif
     * 
     * \if CHINESE
     * @brief 属性编辑器控件的类型
     * \endif
     */
    enum class EditorType {
        ColorPicker,    ///< \if ENGLISH ColorButton with Office-style color menu \endif \if CHINESE 带 Office 风格颜色菜单的颜色按钮 \endif
        CheckBox,       ///< \if ENGLISH QCheckBox \endif \if CHINESE 复选框 \endif
        SpinBox,        ///< \if ENGLISH QSpinBox with min/max/step configuration \endif \if CHINESE 带最小/最大/步长配置的整数输入框 \endif
        DoubleSpinBox,  ///< \if ENGLISH QDoubleSpinBox with min/max/step configuration \endif \if CHINESE 带最小/最大/步长配置的小数输入框 \endif
        Slider,         ///< \if ENGLISH QSlider + QLabel showing current value (min/max/step) \endif \if CHINESE 滑块 + 显示当前值的标签（带最小/最大/步长） \endif
        ComboBox,       ///< \if ENGLISH QComboBox with options from comboBoxOptions field, value is currentText \endif \if CHINESE 下拉组合框（选项来自 comboBoxOptions 字段，值为当前文本） \endif
        EnumComboBox,   ///< \if ENGLISH QComboBox for enum, value is currentIndex (matches enum value) \endif \if CHINESE 枚举下拉组合框（值为当前索引，对应枚举值） \endif
        LineEdit,       ///< \if ENGLISH QLineEdit for text input \endif \if CHINESE 文本输入框 \endif
        FontComboBox    ///< \if ENGLISH QFontComboBox for font selection \endif \if CHINESE 字体选择框 \endif
    };

    /**
     * \if ENGLISH
     * @brief Property registration metadata for creating property editor widgets
     * 
     * PropertyRegistration contains all necessary information to create an editor widget
     * for a specific property and bind it to the target object's property.
     * \endif
     * 
     * \if CHINESE
     * @brief 用于创建属性编辑器控件的属性注册元数据
     * 
     * PropertyRegistration 包含创建属性编辑器控件所需的所有信息，并将其绑定到目标对象的属性。
     * \endif
     */
    struct PropertyRegistration {
        QString category;           ///< \if ENGLISH Main category, e.g. "Plot" \endif \if CHINESE 主分类，例如"图表" \endif
        QString subcategory;        ///< \if ENGLISH Sub category, e.g. "Style" \endif \if CHINESE 子分类，例如"样式" \endif
        QString displayName;        ///< \if ENGLISH Display name, e.g. "Color" \endif \if CHINESE 显示名称，例如"颜色" \endif
        QString briefDesc;          ///< \if ENGLISH Brief description \endif \if CHINESE 简要描述 \endif
        QString detailDesc;         ///< \if ENGLISH Detailed description (tooltip) \endif \if CHINESE 详细描述（工具提示） \endif
        EditorType editorType;      ///< \if ENGLISH Editor type \endif \if CHINESE 编辑器类型 \endif
        QVariant defaultValue;      ///< \if ENGLISH Default value \endif \if CHINESE 默认值 \endif
        QVariant minValue;          ///< \if ENGLISH Min value (for numeric editors) \endif \if CHINESE 最小值（用于数值编辑器） \endif
        QVariant maxValue;          ///< \if ENGLISH Max value (for numeric editors) \endif \if CHINESE 最大值（用于数值编辑器） \endif
        QVariant stepValue;         ///< \if ENGLISH Step value (for numeric editors) \endif \if CHINESE 步长值（用于数值编辑器） \endif
        QStringList comboBoxOptions; ///< \if ENGLISH Options for ComboBox editor \endif \if CHINESE 组合框编辑器的选项 \endif
        QString propertyName;       ///< \if ENGLISH Q_PROPERTY name \endif \if CHINESE Q_PROPERTY 属性名称 \endif
        QObject* target;            ///< \if ENGLISH Target object (plot node, line item, etc.) \endif \if CHINESE 目标对象（绘图节点、线条项等） \endif
    };

    // Constructor for TestFunction (English only)
    explicit TestFunction(QObject* parent = nullptr);
    
    // Destructor for TestFunction (English only)
    virtual ~TestFunction();

    /**
     * \if ENGLISH
     * @brief Create plot nodes in the given figure widget
     * @param figure Pointer to the figure widget where plots will be created
     * \endif
     * 
     * \if CHINESE
     * @brief 在指定的图表控件中创建绘图节点
     * @param figure 将要创建绘图的图表控件指针
     * \endif
     */
    virtual void createPlot(QIM::QImFigureWidget* figure) = 0;

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
    virtual void cleanupPlot();

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
    virtual QList<QObject*> getPlotNodes() const;

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
    QList<PropertyRegistration> getRegisteredProperties() const;

protected:
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
    void registerProperty(const PropertyRegistration& reg);

    /**
     * \if ENGLISH
     * @brief Clear all property registrations
     * \endif
     * 
     * \if CHINESE
     * @brief 清除所有属性注册
     * \endif
     */
    void clearRegistrations();

private:
    QList<PropertyRegistration> m_registrations;  ///< \if ENGLISH Registered properties \endif \if CHINESE 已注册的属性 \endif
    QList<QObject*> m_plotNodes;                  ///< \if ENGLISH References to created plot nodes \endif \if CHINESE 指向已创建绘图节点的引用 \endif
};

Q_DECLARE_METATYPE(TestFunction::EditorType)

#endif // TESTFUNCTION_H
