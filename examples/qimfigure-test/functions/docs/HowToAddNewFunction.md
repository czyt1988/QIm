# How to Add a New Test Function

## Overview

This guide explains how to add a new plotting test function to the `qimfigure-test` application. The application uses a `TestFunction` base class pattern where each test function inherits from `TestFunction` and registers properties for interactive editing.

## Step-by-Step Guide

### Step 1: Choose Category Folder

Select the appropriate subfolder under `functions/`:

| Folder | Purpose |
|--------|---------|
| `functions/line/` | Line-based plots (curves, polylines) |
| `functions/datapoints/` | Scatter, bars, stairs plots |
| `functions/shaded/` | Filled area plots |
| `functions/error/` | Error visualization (error bars) |
| `functions/other/` | Miscellaneous plots (stems, infinite lines) |
| `functions/3d/` | 3D plots (reserved for future) |

### Step 2: Create Header File (.h)

Create header file with the following structure:

```cpp
#ifndef NEWFUNCTION_H
#define NEWFUNCTION_H

#include "../TestFunction.h"  // Adjust path based on folder depth
#include <QObject>
#include <QColor>

namespace QIM {
class QImFigureWidget;
}

class NewFunction : public TestFunction {
    Q_OBJECT
    
    // Declare Q_PROPERTY for each editable property
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    // ... more properties
    
public:
    explicit NewFunction(QObject* parent = nullptr);
    ~NewFunction() override;
    
    void createPlot(QIM::QImFigureWidget* figure) override;
    
    // Property accessors
    QString title() const { return m_title; }
    void setTitle(const QString& title);
    QColor color() const { return m_color; }
    void setColor(const QColor& color);
    
Q_SIGNALS:
    void titleChanged(const QString& title);
    void colorChanged(const QColor& color);
    
private:
    QString m_title = QStringLiteral("Default Title");
    QColor m_color = Qt::blue;
    
    QIM::QImPlotNode* m_plotNode = nullptr;
    // ... other plot node pointers
};

#endif // NEWFUNCTION_H
```

### Step 3: Create Implementation File (.cpp)

Create implementation with:

```cpp
#include "NewFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
// ... include necessary QIm headers

NewFunction::NewFunction(QObject* parent)
    : TestFunction(parent)
{
    // Register properties with tr() for internationalization
    PropertyRegistration titleReg;
    titleReg.category = tr("Plot");           // "图表"
    titleReg.subcategory = tr("Basic Info");   // "基本信息"
    titleReg.displayName = tr("Title");        // "标题"
    titleReg.briefDesc = tr("Plot title");     // "图表标题"
    titleReg.detailDesc = tr("Sets the title text displayed at the top");
    titleReg.editorType = EditorType::LineEdit;
    titleReg.defaultValue = m_title;
    titleReg.propertyName = "title";
    titleReg.target = this;
    registerProperty(titleReg);
    
    // Register more properties...
}

void NewFunction::createPlot(QIM::QImFigureWidget* figure)
{
    clearRegistrations();  // Always call first
    
    m_plotNode = figure->createPlotNode();
    m_plotNode->setTitle(m_title);
    m_plotNode->x1Axis()->setLabel(tr("x"));
    m_plotNode->y1Axis()->setLabel(tr("y"));
    
    // Create plot items and set data
    auto* item = new QIM::QImPlotLineItemNode(m_plotNode);
    item->setData(xData, yData);
    item->setColor(m_color);
    m_plotNode->addChildNode(item);
    
    // Add value tracker
    auto* tracker = new QIM::QImPlotValueTrackerNode(m_plotNode);
    m_plotNode->addChildNode(tracker);
}

void NewFunction::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
        if (m_plotNode) {
            m_plotNode->setTitle(title);  // Live update
        }
    }
}

// Implement other setters similarly...
```

### Setter Implementation Guidelines

Property setters should follow this pattern for real-time updates:

```cpp
void NewFunction::setColor(const QColor& color)
{
    // 1. Check if value changed (optimization)
    if (m_color != color) {
        // 2. Update member variable
        m_color = color;
        // 3. Emit Qt signal (for property system)
        emit colorChanged(color);
        // 4. Update plot node for immediate visual change
        if (m_lineNode) {
            m_lineNode->setColor(color);  // Live rendering update
        }
    }
}

// For enum properties with EnumComboBox:
void NewFunction::setMarkerShape(int shape)
{
    if (m_markerShape != shape) {
        m_markerShape = shape;
        emit markerShapeChanged(shape);
        if (m_scatterNode) {
            m_scatterNode->setMarkerShape(shape);
        }
    }
}
```

**Important**: Always update the plot node in the setter for immediate visual feedback. QIm's real-time rendering advantage relies on this pattern.

### Step 4: Register in TestFunctionManager

Add to `TestFunctionManager.cpp` in `create2DFunctions()` or `create3DFunctions()`:

```cpp
#include "category/NewFunction.h"  // Add include at top

void TestFunctionManager::create2DFunctions()
{
    // ... existing registrations
    
    registerFunction(tr("2D"), tr("Category"), tr("Display Name"),
                     "2d_category_name", new NewFunction(this));
}
```

### Step 5: ID Naming Convention

Follow the naming pattern:

| Pattern | Example |
|---------|---------|
| 2D functions | `2d_<category>_<name>` | `2d_line_10k` |
| 3D functions | `3d_<category>_<name>` | `3d_surface_basic` |

### Step 6: Build System

`CMakeLists.txt` uses `GLOB_RECURSE` for the `functions/` directory, so new files are automatically included. No manual update needed.

## Property Registration Reference

### EditorType Options

| EditorType | Widget | Use Case | Value Type |
|------------|---------|----------|------------|
| `ColorPicker` | ColorButton (Office-style) | Color selection | `QColor` |
| `CheckBox` | QCheckBox | Boolean toggle | `bool` |
| `SpinBox` | QSpinBox | Integer values | `int` |
| `DoubleSpinBox` | QDoubleSpinBox | Floating-point values | `double`/`float` |
| `Slider` | QSlider + QLabel | Range selection | `int` |
| `ComboBox` | QComboBox | Predefined options (text value) | `QString` |
| `EnumComboBox` | QComboBox | **Enum selection (index value)** | `int` |
| `LineEdit` | QLineEdit | Text input | `QString` |
| `FontComboBox` | QFontComboBox | Font selection | `QFont` |

### EnumComboBox Usage

For enum properties (e.g., marker shape, line style), use `EnumComboBox`:

```cpp
// Example: Marker shape enum (ImPlotMarker)
PropertyRegistration shapeReg;
shapeReg.category = tr("Scatter");
shapeReg.subcategory = tr("Marker");
shapeReg.displayName = tr("Shape");
shapeReg.briefDesc = tr("Marker shape");
shapeReg.detailDesc = tr("Sets the shape of scatter markers");
shapeReg.editorType = EditorType::EnumComboBox;
shapeReg.comboBoxOptions = QStringList{
    tr("Circle"),     // Index 0 = ImPlotMarker_Circle
    tr("Square"),     // Index 1 = ImPlotMarker_Square
    tr("Diamond"),    // Index 2 = ImPlotMarker_Diamond
    // ... more options
};
shapeReg.defaultValue = 0;  // 0 = Circle (index matches enum value)
shapeReg.propertyName = "markerShape";
shapeReg.target = this;
registerProperty(shapeReg);
```

**Key points:**
- `defaultValue` is the **enum index** (integer), not a string
- `comboBoxOptions` order must match enum values
- Property setter receives `int` value (currentIndex)
- Q_PROPERTY type should be `int` for enum properties

### PropertyRegistration Fields

```cpp
struct PropertyRegistration {
    QString category;        // Main group (e.g., "Plot", "Line")
    QString subcategory;     // Sub group (e.g., "Style", "Axis")
    QString displayName;     // Property name shown to user
    QString briefDesc;       // Short description
    QString detailDesc;      // Detailed tooltip text
    EditorType editorType;   // Widget type
    QVariant defaultValue;   // Initial value
    QVariant minValue;       // Min (for SpinBox/DoubleSpinBox/Slider)
    QVariant maxValue;       // Max (for SpinBox/DoubleSpinBox/Slider)
    QVariant stepValue;      // Step (for SpinBox/DoubleSpinBox/Slider)
    QStringList comboBoxOptions; // Options (for ComboBox)
    QString propertyName;    // Q_PROPERTY name
    QObject* target;         // Object to update (this or plot node)
};
```

## Internationalization

- Use `tr()` for all display text (English original)
- Comments explain Chinese translation: `// "中文含义"`
- Doxygen comments use bilingual format:
```cpp
/**
 * \if ENGLISH
 * @brief English description
 * \endif
 * \if CHINESE
 * @brief 中文描述
 * \endif
 */
```

## Common Pitfalls

### 1. EnumComboBox vs ComboBox

| Type | Value | Use Case |
|------|-------|----------|
| `ComboBox` | `QString` (selected text) | String options (e.g., "Auto", "Manual") |
| `EnumComboBox` | `int` (selected index) | Enum values (e.g., marker shape = 0, 1, 2...) |

**Wrong**:
```cpp
// Using ComboBox for enum - setter receives QString, not int
shapeReg.editorType = EditorType::ComboBox;  // Wrong!
// Setter: void setShape(int shape) - won't work with QString!
```

**Correct**:
```cpp
// Using EnumComboBox for enum - setter receives int (currentIndex)
shapeReg.editorType = EditorType::EnumComboBox;  // Correct!
// Setter: void setShape(int shape) - matches int value
```

### 2. Property Setter Not Updating Plot

If property changes don't reflect in plot, check:

1. **Is plot node pointer valid?** - Ensure node is created in `createPlot()`
2. **Is setter calling node method?** - Must update node in setter, not just emit signal
3. **Is `requestRender()` called?** - MainWindow handles this via propertyChanged signal

```cpp
// Correct pattern:
void setBarColor(const QColor& color) {
    if (m_barColor != color) {
        m_barColor = color;
        emit barColorChanged(color);
        if (m_barsNode) {           // Check node exists
            m_barsNode->setColor(color);  // Update node immediately
        }
    }
}
```

### 3. Q_PROPERTY Name Mismatch

Property registration `propertyName` must match Q_PROPERTY declaration exactly:

```cpp
// Header:
Q_PROPERTY(QColor barColor READ barColor WRITE setBarColor NOTIFY barColorChanged)

// Registration - must match:
colorReg.propertyName = "barColor";  // Correct
// NOT "BarColor", "bar_color", "color" etc.
```

### 4. Function ID Format

Follow the `2d_<category>_<name>` or `3d_<category>_<name>` pattern:

```cpp
// Correct:
registerFunction(tr("2D"), tr("Data Points"), tr("Bars"),
                 "2d_datapoints_bars", new BarsFunction(this));

// Wrong (will not match tree selection):
registerFunction(tr("2D"), tr("Data Points"), tr("Bars"),
                 "bars", new BarsFunction(this));  // Missing prefix
```

## Testing Checklist

1. Build compiles without errors
2. Function appears in tree navigation
3. Clicking function shows correct plot
4. Properties display in correct categories
5. Editing properties updates plot **immediately** (real-time)
6. Property values persist when switching between functions
7. Value tracker works correctly
8. **Color changes**: Test color picker - changes should apply instantly
9. **Enum properties**: Test EnumComboBox - selected option should update plot

## Example: Adding HistogramFunction

### File: `functions/datapoints/HistogramFunction.h`

```cpp
#ifndef HISTOGRAMFUNCTION_H
#define HISTOGRAMFUNCTION_H

#include "../TestFunction.h"
#include <QObject>
#include <QColor>

namespace QIM { class QImFigureWidget; }

class HistogramFunction : public TestFunction {
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(int bins READ bins WRITE setBins NOTIFY binsChanged)
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)
    
public:
    explicit HistogramFunction(QObject* parent = nullptr);
    ~HistogramFunction() override;
    void createPlot(QIM::QImFigureWidget* figure) override;
    
    QString title() const { return m_title; }
    void setTitle(const QString& title);
    int bins() const { return m_bins; }
    void setBins(int bins);
    QColor fillColor() const { return m_fillColor; }
    void setFillColor(const QColor& color);
    
Q_SIGNALS:
    void titleChanged(const QString&);
    void binsChanged(int);
    void fillColorChanged(const QColor&);
    
private:
    QString m_title = QStringLiteral("Histogram");
    int m_bins = 10;
    QColor m_fillColor = QColor(100, 150, 200, 180);
    QIM::QImPlotNode* m_plotNode = nullptr;
};

#endif
```

### File: `functions/datapoints/HistogramFunction.cpp`

```cpp
#include "HistogramFunction.h"
#include "QImFigureWidget.h"
#include "plot/QImPlotNode.h"
#include "plot/QImPlotBarsItemNode.h"
#include "plot/QImPlotValueTrackerNode.h"
#include <random>

HistogramFunction::HistogramFunction(QObject* parent) : TestFunction(parent) {
    PropertyRegistration binsReg;
    binsReg.category = tr("Histogram");
    binsReg.subcategory = tr("Settings");
    binsReg.displayName = tr("Bins");
    binsReg.briefDesc = tr("Number of bins");
    binsReg.detailDesc = tr("Sets the number of histogram bins");
    binsReg.editorType = EditorType::SpinBox;
    binsReg.defaultValue = m_bins;
    binsReg.minValue = 1;
    binsReg.maxValue = 100;
    binsReg.stepValue = 1;
    binsReg.propertyName = "bins";
    binsReg.target = this;
    registerProperty(binsReg);
    
    // Register other properties...
}

void HistogramFunction::createPlot(QIM::QImFigureWidget* figure) {
    clearRegistrations();
    m_plotNode = figure->createPlotNode();
    m_plotNode->setTitle(m_title);
    
    // Generate histogram data...
    // Create bars item...
    // Add tracker...
}
```

### Registration in TestFunctionManager.cpp

```cpp
#include "datapoints/HistogramFunction.h"

void TestFunctionManager::create2DFunctions() {
    // ...
    registerFunction(tr("2D"), tr("Data Points"), tr("Histogram"),
                     "2d_datapoints_histogram", new HistogramFunction(this));
}
```