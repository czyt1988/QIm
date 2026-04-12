#ifndef PROPERTYPANELWIDGET_H
#define PROPERTYPANELWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QList>
#include <QVariant>
#include "../functions/TestFunction.h"

QT_BEGIN_NAMESPACE
class QSpinBox;
class QDoubleSpinBox;
class QCheckBox;
class QSlider;
class QLabel;
class QComboBox;
class QLineEdit;
class QFontComboBox;
QT_END_NAMESPACE

class ColorButton;

/**
 * \if ENGLISH
 * @brief Property panel widget that displays a categorized property tree
 * 
 * PropertyPanelWidget uses QTreeWidget to display properties grouped by category
 * and subcategory. It creates appropriate editor widgets for each EditorType
 * and emits propertyChanged signal when values are modified.
 * 
 * @section usage Usage Example
 * @code
 * PropertyPanelWidget *panel = new PropertyPanelWidget(parent);
 * panel->setProperties(function->getRegisteredProperties());
 * connect(panel, &PropertyPanelWidget::propertyChanged,
 *         this, &MyClass::onPropertyChanged);
 * @endcode
 * 
 * @see TestFunction::PropertyRegistration
 * @see TestFunction::EditorType
 * \endif
 * 
 * \if CHINESE
 * @brief 显示分类属性树的属性面板控件
 * 
 * PropertyPanelWidget 使用 QTreeWidget 显示按分类和子分类分组的属性。
 * 它为每个 EditorType 创建适当的编辑器控件，并在值修改时触发 propertyChanged 信号。
 * 
 * @section usage 使用示例
 * @code
 * PropertyPanelWidget *panel = new PropertyPanelWidget(parent);
 * panel->setProperties(function->getRegisteredProperties());
 * connect(panel, &PropertyPanelWidget::propertyChanged,
 *         this, &MyClass::onPropertyChanged);
 * @endcode
 * 
 * @see TestFunction::PropertyRegistration
 * @see TestFunction::EditorType
 * \endif
 */
class PropertyPanelWidget : public QWidget
{
    Q_OBJECT

public:
    // Constructor for PropertyPanelWidget (English only)
    explicit PropertyPanelWidget(QWidget *parent = nullptr);
    
    // Destructor for PropertyPanelWidget (English only)
    ~PropertyPanelWidget() override;

    /**
     * \if ENGLISH
     * @brief Set properties to display in the tree
     * @param properties List of property registrations to display
     * \endif
     * 
     * \if CHINESE
     * @brief 设置要在树中显示的属性
     * @param properties 要显示的属性注册列表
     * \endif
     */
    void setProperties(const QList<TestFunction::PropertyRegistration>& properties);

    /**
     * \if ENGLISH
     * @brief Clear the property tree
     * \endif
     * 
     * \if CHINESE
     * @brief 清除属性树
     * \endif
     */
    void clearPanel();

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when a property value changes
     * @param target The target object whose property was modified
     * @param propertyName The name of the modified property
     * @param newValue The new value of the property
     * \endif
     * 
     * \if CHINESE
     * @brief 属性值改变时触发
     * @param target 属性被修改的目标对象
     * @param propertyName 被修改的属性名称
     * @param newValue 属性的新值
     * \endif
     */
    void propertyChanged(QObject* target, QString propertyName, QVariant newValue);

private:
    /**
     * \if ENGLISH
     * @brief Build the property tree from registered properties
     * \endif
     * 
     * \if CHINESE
     * @brief 从已注册的属性构建属性树
     * \endif
     */
    void buildPropertyTree();

    /**
     * \if ENGLISH
     * @brief Create an editor widget for the specified editor type
     * @param type The type of editor to create
     * @param reg Property registration metadata
     * @return Pointer to the created editor widget
     * \endif
     * 
     * \if CHINESE
     * @brief 为指定的编辑器类型创建编辑器控件
     * @param type 要创建的编辑器类型
     * @param reg 属性注册元数据
     * @return 指向创建的编辑器控件的指针
     * \endif
     */
    QWidget* createEditorForType(TestFunction::EditorType type, const TestFunction::PropertyRegistration& reg);

    /**
     * \if ENGLISH
     * @brief Connect editor widget signals to property change handler
     * @param editor The editor widget to connect
     * @param reg Property registration metadata
     * \endif
     * 
     * \if CHINESE
     * @brief 将编辑器控件信号连接到属性改变处理器
     * @param editor 要连接的编辑器控件
     * @param reg 属性注册元数据
     * \endif
     */
    void connectEditorSignal(QWidget* editor, const TestFunction::PropertyRegistration& reg);

private:
    QTreeWidget *m_treeWidget;  ///< \if ENGLISH Tree widget for displaying properties \endif \if CHINESE 用于显示属性的树控件 \endif
    QList<TestFunction::PropertyRegistration> m_properties;  ///< \if ENGLISH List of registered properties \endif \if CHINESE 已注册的属性列表 \endif
};

#endif // PROPERTYPANELWIDGET_H
