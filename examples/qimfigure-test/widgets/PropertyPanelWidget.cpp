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
                
                // Create editor widget
                QWidget *editor = createEditorForType(reg.editorType, reg);
                if (editor) {
                    m_treeWidget->setItemWidget(propertyItem, 1, editor);
                    
                    // Connect editor signals
                    connectEditorSignal(editor, reg);
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
        auto *colorBtn = new ColorButton();
        if (reg.defaultValue.canConvert<QColor>()) {
            colorBtn->setColor(reg.defaultValue.value<QColor>());
        } else {
            colorBtn->setColor(Qt::black);
        }
        editor = colorBtn;
        break;
    }
    
    case TestFunction::EditorType::CheckBox: {
        auto *checkBox = new QCheckBox();
        if (reg.defaultValue.canConvert<bool>()) {
            checkBox->setChecked(reg.defaultValue.toBool());
        }
        editor = checkBox;
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
        if (reg.defaultValue.canConvert<int>()) {
            spinBox->setValue(reg.defaultValue.toInt());
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
        if (reg.defaultValue.canConvert<double>()) {
            doubleSpinBox->setValue(reg.defaultValue.toDouble());
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
        if (reg.defaultValue.canConvert<int>()) {
            slider->setValue(reg.defaultValue.toInt());
        }
        
        auto *label = new QLabel();
        label->setText(QString::number(slider->value()));
        label->setMinimumWidth(40);
        
        layout->addWidget(slider);
        layout->addWidget(label);
        
        // Update label when slider value changes
        connect(slider, &QSlider::valueChanged, label, [label](int value) {
            label->setText(QString::number(value));
        });
        
        // Store slider pointer for signal connection
        sliderWidget->setProperty("slider", QVariant::fromValue(slider));
        editor = sliderWidget;
        break;
    }
    
    case TestFunction::EditorType::ComboBox: {
        auto *comboBox = new QComboBox();
        comboBox->addItems(reg.comboBoxOptions);
        if (!reg.comboBoxOptions.isEmpty()) {
            int defaultIndex = reg.comboBoxOptions.indexOf(reg.defaultValue.toString());
            if (defaultIndex >= 0) {
                comboBox->setCurrentIndex(defaultIndex);
            }
        }
        editor = comboBox;
        break;
    }
    
    case TestFunction::EditorType::LineEdit: {
        auto *lineEdit = new QLineEdit();
        if (reg.defaultValue.canConvert<QString>()) {
            lineEdit->setText(reg.defaultValue.toString());
        }
        editor = lineEdit;
        break;
    }
    
    case TestFunction::EditorType::FontComboBox: {
        auto *fontComboBox = new QFontComboBox();
        if (reg.defaultValue.canConvert<QString>()) {
            fontComboBox->setCurrentFont(QFont(reg.defaultValue.toString()));
        }
        editor = fontComboBox;
        break;
    }
    }
    
    return editor;
}

void PropertyPanelWidget::connectEditorSignal(QWidget* editor, const TestFunction::PropertyRegistration& reg)
{
    if (!editor || !reg.target) {
        return;
    }
    
    // Helper lambda to emit propertyChanged signal
    auto emitChange = [this, reg](const QVariant &value) {
        if (reg.target) {
            reg.target->setProperty(reg.propertyName.toUtf8(), value);
            emit propertyChanged(reg.target, reg.propertyName, value);
        }
    };
    
    // Connect based on editor type
    if (auto *colorBtn = qobject_cast<ColorButton*>(editor)) {
        connect(colorBtn, &ColorButton::colorChanged, this, [emitChange](const QColor &color) {
            emitChange(QVariant::fromValue(color));
        });
    }
    else if (auto *checkBox = qobject_cast<QCheckBox*>(editor)) {
        connect(checkBox, &QCheckBox::toggled, this, [emitChange](bool checked) {
            emitChange(QVariant::fromValue(checked));
        });
    }
    else if (auto *spinBox = qobject_cast<QSpinBox*>(editor)) {
        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [emitChange](int value) {
            emitChange(QVariant::fromValue(value));
        });
    }
    else if (auto *doubleSpinBox = qobject_cast<QDoubleSpinBox*>(editor)) {
        connect(doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [emitChange](double value) {
            emitChange(QVariant::fromValue(value));
        });
    }
    else if (auto *sliderWidget = qobject_cast<QWidget*>(editor)) {
        // For slider widget, get the internal slider
        if (sliderWidget->property("slider").isValid()) {
            auto *slider = qobject_cast<QSlider*>(sliderWidget->property("slider").value<QSlider*>());
            if (slider) {
                connect(slider, &QSlider::valueChanged, this, [emitChange](int value) {
                    emitChange(QVariant::fromValue(value));
                });
            }
        }
    }
    else if (auto *comboBox = qobject_cast<QComboBox*>(editor)) {
        connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [emitChange, comboBox]() {
            emitChange(QVariant::fromValue(comboBox->currentText()));
        });
    }
    else if (auto *lineEdit = qobject_cast<QLineEdit*>(editor)) {
        connect(lineEdit, &QLineEdit::textChanged, this, [emitChange](const QString &text) {
            emitChange(QVariant::fromValue(text));
        });
    }
    else if (auto *fontComboBox = qobject_cast<QFontComboBox*>(editor)) {
        connect(fontComboBox, QOverload<const QFont&>::of(&QFontComboBox::currentFontChanged), this, [emitChange](const QFont &font) {
            emitChange(QVariant::fromValue(font));
        });
    }
}
