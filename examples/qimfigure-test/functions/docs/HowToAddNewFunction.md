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

| EditorType | Widget | Use Case |
|------------|---------|----------|
| `ColorPicker` | ColorButton (Office-style) | Color selection |
| `CheckBox` | QCheckBox | Boolean toggle |
| `SpinBox` | QSpinBox | Integer values |
| `DoubleSpinBox` | QDoubleSpinBox | Floating-point values |
| `Slider` | QSlider + QLabel | Range selection |
| `ComboBox` | QComboBox | Predefined options |
| `LineEdit` | QLineEdit | Text input |
| `FontComboBox` | QFontComboBox | Font selection |

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

## Testing Checklist

1. Build compiles without errors
2. Function appears in tree navigation
3. Clicking function shows correct plot
4. Properties display in correct categories
5. Editing properties updates plot live
6. Value tracker works correctly

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