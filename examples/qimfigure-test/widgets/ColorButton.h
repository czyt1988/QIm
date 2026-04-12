#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QPushButton>
#include <QColor>
#include <QMenu>
#include <QVector>
#include <QScopedPointer>
#include <QPaintEvent>
#include <QImAPI.h>

/**
 * \if ENGLISH
 * @brief Office-style color picker button widget
 * 
 * A QPushButton subclass that displays the current color as a filled rectangle
 * and shows a popup menu with preset color swatches when clicked. The menu
 * follows Office-style layout with 4 rows x 8 columns (32 colors total) plus
 * a "More Colors..." option that opens QColorDialog.
 * 
 * @section usage Usage Example
 * @code
 * ColorButton *colorBtn = new ColorButton(parent);
 * colorBtn->setColor(Qt::red);
 * connect(colorBtn, &ColorButton::colorChanged,
 *         this, &MyClass::onColorChanged);
 * @endcode
 * 
 * @see QColorDialog, QMenu
 * \endif
 * 
 * \if CHINESE
 * @brief 办公室风格颜色选择按钮控件
 * 
 * QPushButton 子类，将当前颜色显示为填充矩形，点击时显示预设颜色色板弹出菜单。
 * 菜单采用办公室风格布局，4 行 x 8 列（共 32 种颜色），外加"更多颜色..."选项，
 * 点击后打开 QColorDialog。
 * 
 * @section usage 使用示例
 * @code
 * ColorButton *colorBtn = new ColorButton(parent);
 * colorBtn->setColor(Qt::red);
 * connect(colorBtn, &ColorButton::colorChanged,
 *         this, &MyClass::onColorChanged);
 * @endcode
 * 
 * @see QColorDialog, QMenu
 * \endif
 */
class ColorButton : public QPushButton
{
    Q_OBJECT

    /**
     * \if ENGLISH
     * @brief Current color property
     * \endif
     * 
     * \if CHINESE
     * @brief 当前颜色属性
     * \endif
     */
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged USER true)

public:
    // Constructor for ColorButton (English only)
    explicit ColorButton(QWidget *parent = nullptr);

    // Destructor for ColorButton (English only)
    ~ColorButton() override;

    // Get current color (English only)
    QColor color() const;

    // Set preset colors for the menu (English only)
    void setPresetColors(const QVector<QColor> &colors);

    // Get preset colors (English only)
    QVector<QColor> presetColors() const;

Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when color changes
     * @param color The new color value
     * \endif
     * 
     * \if CHINESE
     * @brief 颜色改变时触发
     * @param color 新的颜色值
     * \endif
     */
    void colorChanged(const QColor &color);

public Q_SLOTS:
    // Set current color (English only)
    void setColor(const QColor &color);

protected:
    // Paint event handler to draw color preview (English only)
    void paintEvent(QPaintEvent *event) override;

    // Click handler to show color menu (English only)
    void onClicked();

private:
    // Build color menu with preset swatches (English only)
    QMenu* buildColorMenu();

    // Internal private data class (PIMPL pattern)
    QIM_DECLARE_PRIVATE(ColorButton)
};

#endif // COLORBUTTON_H
