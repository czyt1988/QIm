#include "PropertyPanelWidget.h"
#include "ColorButton.h"
#include <QVBoxLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QSlider>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QFontComboBox>
#include <QMap>
#include <QDebug>

/**
 * \if ENGLISH
 * @brief Internal class to store editor-widget mapping
 * \endif
 * 
 * \if CHINESE
 * @brief 用于存储编辑器 - 属性映射的内部类
 * \endif
 */
struct EditorMapping {
    QWidget* editor;
    TestFunction::PropertyRegistration registration;
};

PropertyPanelWidget::PropertyPanelWidget(QWidget *parent)
    : QWidget(parent)
    , m_treeWidget(new QTreeWidget(this))
{
    // Setup tree widget
    m_treeWidget->setColumnCount(2);
    m_treeWidget->setHeaderLabels(QStringList() << tr("Property") << tr("Value"));
    m_treeWidget->setRootIsDecorated(true);
    m_treeWidget->setAlternatingRowColors(true);
    m_treeWidget->setExpandsOnDoubleClick(false);
    
    // Setup layout
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_treeWidget);
}

PropertyPanelWidget::~PropertyPanelWidget() = default;

void PropertyPanelWidget::setProperties(const QList<TestFunction::PropertyRegistration>& properties)
{
    clearPanel();
    m_properties = properties;
    buildPropertyTree();
}

void PropertyPanelWidget::clearPanel()
{
    m_treeWidget->clear();
    m_properties.clear();
}

void PropertyPanelWidget::buildPropertyTree()
{
    // Group properties by category and subcategory
    QMap<QString, QMap<QString, QList<TestFunction::PropertyRegistration>>> grouped;
    
    for (const auto &reg : m_properties) {
        grouped[reg.category][reg.subcategory].append(reg);
    }
    
    // Create tree items
    for (auto categoryIt = grouped.begin(); categoryIt != grouped.end(); ++categoryIt) {
        const QString &categoryName = categoryIt.key();
        
        // Create category item
        auto *categoryItem = new QTreeWidgetItem(m_treeWidget);
        categoryItem->setText(0, categoryName);
        categoryItem->setFlags(categoryItem->flags() | Qt::ItemIsUserCheckable);
        categoryItem->setCheckState(0, Qt::Checked);
        categoryItem->setExpanded(true);
        
        // Create subcategory items
        for (auto subcategoryIt = categoryIt.value().begin(); subcategoryIt != categoryIt.value().end(); ++subcategoryIt) {
            const QString &subcategoryName = subcategoryIt.key();
            const QList<TestFunction::PropertyRegistration> &regs = subcategoryIt.value();
            
            auto *subcategoryItem = new QTreeWidgetItem(categoryItem);
            subcategoryItem->setText(0, subcategoryName);
            subcategoryItem->setFlags(subcategoryItem->flags() | Qt::ItemIsUserCheckable);
            subcategoryItem->setCheckState(0, Qt::Checked);
            subcategoryItem->setExpanded(true);
            
            // Create property items with editors
            for (const auto &reg : regs) {
                auto *propertyItem = new QTreeWidgetItem(subcategoryItem);
                propertyItem->setText(0, reg.displayName);
                
                // Create editor widget (without setting initial value to avoid premature signal)
                QWidget *editor = createEditorForType(reg.editorType, reg);
                if (editor) {
                    m_treeWidget->setItemWidget(propertyItem, 1, editor);
                    
                    // Connect editor signals BEFORE setting initial value
                    connectEditorSignal(editor, reg);
                    
                    // Set initial value (signals are now connected, will trigger proper updates)
                    setEditorInitialValue(editor, reg);
                }
            }
        }
    }
    
    // Resize columns to fit content
    m_treeWidget->resizeColumnToContents(0);
    m_treeWidget->setColumnWidth(1, 200);
}

QWidget* PropertyPanelWidget::createEditorForType(TestFunction::EditorType type, const TestFunction::PropertyRegistration& reg)
{
    QWidget *editor = nullptr;
    
    switch (type) {
    case TestFunction::EditorType::ColorPicker: {
        editor = new ColorButton();
        // Initial value will be set in setEditorInitialValue
        break;
    }
    
    case TestFunction::EditorType::CheckBox: {
        editor = new QCheckBox();
        break;
    }
    
    case TestFunction::EditorType::SpinBox: {
        auto *spinBox = new QSpinBox();
        if (reg.minValue.canConvert<int>()) {
            spinBox->setMinimum(reg.minValue.toInt());
        }
        if (reg.maxValue.canConvert<int>()) {
            spinBox->setMaximum(reg.maxValue.toInt());
        }
        if (reg.stepValue.canConvert<int>()) {
            spinBox->setSingleStep(reg.stepValue.toInt());
        }
        editor = spinBox;
        break;
    }
    
    case TestFunction::EditorType::DoubleSpinBox: {
        auto *doubleSpinBox = new QDoubleSpinBox();
        if (reg.minValue.canConvert<double>()) {
            doubleSpinBox->setMinimum(reg.minValue.toDouble());
        }
        if (reg.maxValue.canConvert<double>()) {
            doubleSpinBox->setMaximum(reg.maxValue.toDouble());
        }
        if (reg.stepValue.canConvert<double>()) {
            doubleSpinBox->setSingleStep(reg.stepValue.toDouble());
        }
        editor = doubleSpinBox;
        break;
    }
    
    case TestFunction::EditorType::Slider: {
        auto *sliderWidget = new QWidget();
        auto *layout = new QHBoxLayout(sliderWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(5);
        
        auto *slider = new QSlider(Qt::Horizontal);
        if (reg.minValue.canConvert<int>()) {
            slider->setMinimum(reg.minValue.toInt());
        }
        if (reg.maxValue.canConvert<int>()) {
            slider->setMaximum(reg.maxValue.toInt());
        }
        if (reg.stepValue.canConvert<int>()) {
            slider->setSingleStep(reg.stepValue.toInt());
        }
        
        auto *label = new QLabel();
        label->setMinimumWidth(40);
        
        layout->addWidget(slider);
        layout->addWidget(label);
        
        // Update label when slider value changes
        connect(slider, &QSlider::valueChanged, label, [label](int value) {
            label->setText(QString::number(value));
        });
        
        // Store slider pointer for signal connection and initial value setting
        sliderWidget->setProperty("slider", QVariant::fromValue(slider));
        editor = sliderWidget;
        break;
    }
    
    case TestFunction::EditorType::ComboBox: {
        auto *comboBox = new QComboBox();
        comboBox->addItems(reg.comboBoxOptions);
        editor = comboBox;
        break;
    }
    
    case TestFunction::EditorType::EnumComboBox: {
        auto *comboBox = new QComboBox();
        comboBox->addItems(reg.comboBoxOptions);
        // For EnumComboBox, defaultValue is the enum index
        if (reg.defaultValue.canConvert<int>()) {
            comboBox->setCurrentIndex(reg.defaultValue.toInt());
        }
        editor = comboBox;
        break;
    }
    
    case TestFunction::EditorType::LineEdit: {
        editor = new QLineEdit();
        break;
    }
    
    case TestFunction::EditorType::FontComboBox: {
        editor = new QFontComboBox();
        break;
    }
    }
    
    return editor;
}

void PropertyPanelWidget::connectEditorSignal(QWidget* editor, const TestFunction::PropertyRegistration& reg)
{
    if (!editor || !reg.target) {
        qDebug() << "[PropertyPanel] connectEditorSignal: editor or target is null, skipping";
        return;
    }
    
    qDebug() << "[PropertyPanel] connectEditorSignal: propertyName=" << reg.propertyName
             << ", target=" << reg.target << ", editorType=" << static_cast<int>(reg.editorType);
    
    // Helper lambda to emit propertyChanged signal
    auto emitChange = [this, reg](const QVariant &value) {
        qDebug() << "[PropertyPanel] emitChange: propertyName=" << reg.propertyName
                 << ", value=" << value << ", target=" << reg.target;
        if (reg.target) {
            bool success = reg.target->setProperty(reg.propertyName.toUtf8(), value);
            qDebug() << "[PropertyPanel] setProperty result:" << success;
            emit propertyChanged(reg.target, reg.propertyName, value);
            qDebug() << "[PropertyPanel] propertyChanged signal emitted";
        }
    };
    
    // Connect based on editor type - ORDER MATTERS! Specific types before QWidget base class
    if (auto *colorBtn = qobject_cast<ColorButton*>(editor)) {
        qDebug() << "[PropertyPanel] Connecting ColorButton signal";
        connect(colorBtn, &ColorButton::colorChanged, this, [emitChange](const QColor &color) {
            emitChange(QVariant::fromValue(color));
        });
    }
    else if (auto *checkBox = qobject_cast<QCheckBox*>(editor)) {
        qDebug() << "[PropertyPanel] Connecting QCheckBox signal";
        connect(checkBox, &QCheckBox::toggled, this, [emitChange](bool checked) {
            emitChange(QVariant::fromValue(checked));
        });
    }
    else if (auto *spinBox = qobject_cast<QSpinBox*>(editor)) {
        qDebug() << "[PropertyPanel] Connecting QSpinBox signal";
        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [emitChange](int value) {
            emitChange(QVariant::fromValue(value));
        });
    }
    else if (auto *doubleSpinBox = qobject_cast<QDoubleSpinBox*>(editor)) {
        qDebug() << "[PropertyPanel] Connecting QDoubleSpinBox signal";
        connect(doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [emitChange](double value) {
            emitChange(QVariant::fromValue(value));
        });
    }
    else if (auto *comboBox = qobject_cast<QComboBox*>(editor)) {
        qDebug() << "[PropertyPanel] Connecting QComboBox signal";
        // Check if this is an EnumComboBox (value is index) or regular ComboBox (value is text)
        if (reg.editorType == TestFunction::EditorType::EnumComboBox) {
            // EnumComboBox: value is currentIndex (matches enum value)
            connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [emitChange](int index) {
                emitChange(QVariant::fromValue(index));
            });
        } else {
            // Regular ComboBox: value is currentText
            connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [emitChange, comboBox]() {
                emitChange(QVariant::fromValue(comboBox->currentText()));
            });
        }
    }
    else if (auto *lineEdit = qobject_cast<QLineEdit*>(editor)) {
        qDebug() << "[PropertyPanel] Connecting QLineEdit signal";
        connect(lineEdit, &QLineEdit::textChanged, this, [emitChange](const QString &text) {
            emitChange(QVariant::fromValue(text));
        });
    }
    else if (auto *fontComboBox = qobject_cast<QFontComboBox*>(editor)) {
        qDebug() << "[PropertyPanel] Connecting QFontComboBox signal";
        connect(fontComboBox, QOverload<const QFont&>::of(&QFontComboBox::currentFontChanged), this, [emitChange](const QFont &font) {
            emitChange(QVariant::fromValue(font));
        });
    }
    else if (editor->property("slider").isValid()) {
        // For slider widget (container with slider property), get the internal slider
        qDebug() << "[PropertyPanel] Connecting Slider signal (via property)";
        auto *slider = qobject_cast<QSlider*>(editor->property("slider").value<QSlider*>());
        if (slider) {
            connect(slider, &QSlider::valueChanged, this, [emitChange](int value) {
                emitChange(QVariant::fromValue(value));
            });
        }
    }
    else {
        qDebug() << "[PropertyPanel] WARNING: Unknown editor type, no signal connected";
    }
}

void PropertyPanelWidget::setEditorInitialValue(QWidget* editor, const TestFunction::PropertyRegistration& reg)
{
    if (!editor) {
        return;
    }
    
    qDebug() << "[PropertyPanel] setEditorInitialValue: propertyName=" << reg.propertyName
             << ", defaultValue=" << reg.defaultValue;
    
    // Block signals temporarily to prevent initialization from triggering updates
    const bool wasBlocked = editor->blockSignals(true);
    
    // Set initial value based on editor type
    if (auto *colorBtn = qobject_cast<ColorButton*>(editor)) {
        if (reg.defaultValue.canConvert<QColor>()) {
            colorBtn->setColor(reg.defaultValue.value<QColor>());
        }
    }
    else if (auto *checkBox = qobject_cast<QCheckBox*>(editor)) {
        if (reg.defaultValue.canConvert<bool>()) {
            checkBox->setChecked(reg.defaultValue.toBool());
        }
    }
    else if (auto *spinBox = qobject_cast<QSpinBox*>(editor)) {
        if (reg.defaultValue.canConvert<int>()) {
            spinBox->setValue(reg.defaultValue.toInt());
        }
    }
    else if (auto *doubleSpinBox = qobject_cast<QDoubleSpinBox*>(editor)) {
        if (reg.defaultValue.canConvert<double>()) {
            doubleSpinBox->setValue(reg.defaultValue.toDouble());
        }
    }
    else if (auto *comboBox = qobject_cast<QComboBox*>(editor)) {
        if (reg.editorType == TestFunction::EditorType::EnumComboBox) {
            // EnumComboBox: defaultValue is enum index
            if (reg.defaultValue.canConvert<int>()) {
                comboBox->setCurrentIndex(reg.defaultValue.toInt());
            }
        } else {
            // Regular ComboBox: defaultValue is text, find matching index
            if (!reg.comboBoxOptions.isEmpty()) {
                int defaultIndex = reg.comboBoxOptions.indexOf(reg.defaultValue.toString());
                if (defaultIndex >= 0) {
                    comboBox->setCurrentIndex(defaultIndex);
                }
            }
        }
    }
    else if (auto *lineEdit = qobject_cast<QLineEdit*>(editor)) {
        if (reg.defaultValue.canConvert<QString>()) {
            lineEdit->setText(reg.defaultValue.toString());
        }
    }
    else if (auto *fontComboBox = qobject_cast<QFontComboBox*>(editor)) {
        if (reg.defaultValue.canConvert<QString>()) {
            fontComboBox->setCurrentFont(QFont(reg.defaultValue.toString()));
        }
    }
    else if (editor->property("slider").isValid()) {
        auto *slider = qobject_cast<QSlider*>(editor->property("slider").value<QSlider*>());
        if (slider && reg.defaultValue.canConvert<int>()) {
            slider->setValue(reg.defaultValue.toInt());
            // Update the label that shows the value
            if (auto *container = qobject_cast<QWidget*>(editor)) {
                // Find the QLabel child that shows the value
                for (auto *child : container->children()) {
                    if (auto *label = qobject_cast<QLabel*>(child)) {
                        label->setText(QString::number(slider->value()));
                        break;
                    }
                }
            }
        }
    }
    
    // Restore signal blocking state
    editor->blockSignals(wasBlocked);
}
