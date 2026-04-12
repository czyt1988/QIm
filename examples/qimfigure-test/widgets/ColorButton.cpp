#include "ColorButton.h"
#include <QPainter>
#include <QColorDialog>
#include <QGridLayout>
#include <QLabel>
#include <QEvent>


/**
 * \if ENGLISH
 * @brief Event filter for color swatch labels
 * 
 * Handles mouse press events on color swatch labels and emits colorChanged signal
 * \endif
 * 
 * \if CHINESE
 * @brief 颜色色板标签的事件过滤器
 * 
 * 处理颜色色板标签的鼠标按下事件并发射 colorChanged 信号
 * \endif
 */
class ColorSwatchEventFilter : public QObject
{
    Q_OBJECT

public:
    // Constructor for ColorSwatchEventFilter (English only)
    explicit ColorSwatchEventFilter(ColorButton *button) : m_button(button) {}

protected:
    // Event filter to handle swatch clicks (English only)
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (event->type() == QEvent::MouseButtonPress) {
            auto *label = qobject_cast<QLabel*>(obj);
            if (label && m_button) {
                QColor color = label->property("color").value<QColor>();
                m_button->setColor(color);
                return true;
            }
        }
        return QObject::eventFilter(obj, event);
    }

private:
    ColorButton *m_button;
};

/**
 * \if ENGLISH
 * @brief Private data for ColorButton
 * \endif
 * 
 * \if CHINESE
 * @brief ColorButton 的私有数据
 * \endif
 */
class ColorButton::PrivateData
{
    QIM_DECLARE_PUBLIC(ColorButton)

public:
    // Constructor for PrivateData (English only)
    explicit PrivateData(ColorButton *q) : q_ptr(q)
    {
        initPresetColors();
    }

    // Initialize default preset colors (English only)
    void initPresetColors()
    {
        // Row 1: Theme colors
        presetColors << Qt::white << Qt::black 
                     << QColor(139, 0, 0)      // Dark red
                     << QColor(255, 165, 0)    // Orange
                     << Qt::green 
                     << Qt::blue 
                     << Qt::purple 
                     << QColor(128, 128, 128); // Gray

        // Row 2: Standard colors
        presetColors << Qt::red 
                     << QColor(255, 128, 0)    // Orange
                     << Qt::yellow 
                     << QColor(128, 255, 0)    // Lime
                     << Qt::cyan 
                     << QColor(0, 128, 255)    // Aqua
                     << QColor(238, 130, 238)  // Violet
                     << QColor(160, 32, 240);  // Purple

        // Row 3: Light variants
        presetColors << QColor(255, 192, 192)  // Light red
                     << QColor(255, 224, 192)  // Light orange
                     << QColor(255, 255, 192)  // Light yellow
                     << QColor(192, 255, 192)  // Light green
                     << QColor(192, 255, 255)  // Light cyan
                     << QColor(192, 224, 255)  // Light blue
                     << QColor(224, 192, 255)  // Light purple
                     << QColor(224, 224, 224); // Light gray

        // Row 4: Dark variants
        presetColors << QColor(128, 0, 0)      // Dark red
                     << QColor(128, 64, 0)     // Dark orange
                     << QColor(128, 128, 0)    // Dark yellow
                     << QColor(0, 128, 0)      // Dark green
                     << QColor(0, 128, 128)    // Dark cyan
                     << QColor(0, 0, 128)      // Dark blue
                     << QColor(64, 0, 128)     // Dark purple
                     << QColor(64, 64, 64);    // Dark gray
    }

    // Generate color from row and column (English only)
    QColor colorAt(int row, int col) const
    {
        int index = row * 8 + col;
        if (index >= 0 && index < presetColors.size()) {
            return presetColors.at(index);
        }
        return Qt::black;
    }

    QVector<QColor> presetColors;
    QColor currentColor;
    QScopedPointer<ColorSwatchEventFilter> eventFilter;
};

/**
 * \if ENGLISH
 * @brief Construct a new Color Button object
 * @param parent Parent widget
 * \endif
 * 
 * \if CHINESE
 * @brief 构造颜色按钮对象
 * @param parent 父控件
 * \endif
 */
ColorButton::ColorButton(QWidget *parent)
    : QPushButton(parent)
    , d_ptr(new PrivateData(this))
{
    QIM_D(d);
    d->currentColor = Qt::white;
    d->eventFilter.reset(new ColorSwatchEventFilter(this));
    
    setMinimumSize(60, 25);
    setMaximumWidth(120);
    
    connect(this, &ColorButton::clicked, this, &ColorButton::onClicked);
}

/**
 * \if ENGLISH
 * @brief Destroy the Color Button object
 * \endif
 * 
 * \if CHINESE
 * @brief 销毁颜色按钮对象
 * \endif
 */
ColorButton::~ColorButton() = default;

/**
 * \if ENGLISH
 * @brief Get current color
 * @return Current color value
 * \endif
 * 
 * \if CHINESE
 * @brief 获取当前颜色
 * @return 当前颜色值
 * \endif
 */
QColor ColorButton::color() const
{
    QIM_DC(d);
    return d->currentColor;
}

/**
 * \if ENGLISH
 * @brief Set current color
 * @param color New color value
 * \endif
 * 
 * \if CHINESE
 * @brief 设置当前颜色
 * @param color 新的颜色值
 * \endif
 */
void ColorButton::setColor(const QColor &color)
{
    QIM_D(d);
    if (d->currentColor != color) {
        d->currentColor = color;
        update();
        emit colorChanged(color);
    }
}

/**
 * \if ENGLISH
 * @brief Set preset colors for the menu
 * @param colors Vector of preset colors (should be 32 colors for 4x8 grid)
 * \endif
 * 
 * \if CHINESE
 * @brief 设置菜单的预设颜色
 * @param colors 预设颜色向量（应为 32 种颜色以匹配 4x8 网格）
 * \endif
 */
void ColorButton::setPresetColors(const QVector<QColor> &colors)
{
    QIM_D(d);
    d->presetColors = colors;
}

/**
 * \if ENGLISH
 * @brief Get preset colors
 * @return Vector of preset colors
 * \endif
 * 
 * \if CHINESE
 * @brief 获取预设颜色
 * @return 预设颜色向量
 * \endif
 */
QVector<QColor> ColorButton::presetColors() const
{
    QIM_DC(d);
    return d->presetColors;
}

/**
 * \if ENGLISH
 * @brief Paint event handler to draw color preview
 * @param event Paint event
 * \endif
 * 
 * \if CHINESE
 * @brief 绘制事件处理函数，绘制颜色预览
 * @param event 绘制事件
 * \endif
 */
void ColorButton::paintEvent(QPaintEvent *event)
{
    QIM_DC(d);
    
    QPushButton::paintEvent(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw color preview rectangle
    QRect colorRect = rect().adjusted(4, 4, -4, -4);
    
    // Draw border
    painter.setPen(QPen(Qt::darkGray, 1));
    painter.setBrush(d->currentColor);
    painter.drawRect(colorRect);
    
    // Draw inner highlight for 3D effect
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 255, 255, 80));
    QRect highlightRect = colorRect.adjusted(0, 0, 0, -2);
    painter.drawRect(highlightRect);
}

/**
 * \if ENGLISH
 * @brief Click handler to show color menu
 * \endif
 * 
 * \if CHINESE
 * @brief 点击处理函数，显示颜色菜单
 * \endif
 */
void ColorButton::onClicked()
{
    QScopedPointer<QMenu> menu(buildColorMenu());
    if (menu) {
        menu->exec(mapToGlobal(rect().bottomLeft()));
    }
}

/**
 * \if ENGLISH
 * @brief Build color menu with preset swatches
 * @return Pointer to the created menu (caller takes ownership)
 * \endif
 * 
 * \if CHINESE
 * @brief 构建带预设色板的颜色菜单
 * @return 创建的菜单指针（调用者获得所有权）
 * \endif
 */
QMenu* ColorButton::buildColorMenu()
{
    QIM_D(d);
    
    auto *menu = new QMenu(this);
    auto *gridWidget = new QWidget();
    auto *gridLayout = new QGridLayout(gridWidget);
    gridLayout->setSpacing(2);
    gridLayout->setContentsMargins(4, 4, 4, 4);
    
    // Create color swatches (4 rows x 8 columns)
    const int rows = 4;
    const int cols = 8;
    
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            auto *label = new QLabel();
            label->setFixedSize(24, 24);
            label->setAutoFillBackground(true);
            
            QColor color = d->colorAt(row, col);
            label->setProperty("color", color);
            
            // Set background color using stylesheet
            label->setStyleSheet(QString("background-color: rgb(%1, %2, %3); border: 1px solid gray;")
                                 .arg(color.red()).arg(color.green()).arg(color.blue()));
            
            label->setToolTip(color.name());
            label->installEventFilter(d->eventFilter.data());
            
            gridLayout->addWidget(label, row, col);
        }
    }
    
    menu->addWidget(gridWidget);
    menu->addSeparator();
    
    // "More Colors..." action
    auto *moreColorsAction = menu->addAction(tr("More Colors..."));
    connect(moreColorsAction, &QAction::triggered, this, [this]() {
        QColor newColor = QColorDialog::getColor(color(), this);
        if (newColor.isValid()) {
            setColor(newColor);
        }
    });
    
    return menu;
}

#include "ColorButton.moc"
