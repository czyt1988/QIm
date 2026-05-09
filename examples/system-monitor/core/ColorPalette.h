#pragma once

#include <QColor>
#include <QString>
#include <QVector>

namespace {
    // Fixed 22-color palette (based on tableau palette)
    static const QVector<QColor> kColorPalette = {
        QColor(31, 119, 180),    // 0: blue
        QColor(255, 127, 14),    // 1: orange
        QColor(44, 160, 44),     // 2: green
        QColor(214, 39, 40),     // 3: red
        QColor(148, 103, 189),   // 4: purple
        QColor(140, 86, 75),     // 5: brown
        QColor(227, 119, 194),   // 6: pink
        QColor(127, 127, 127),   // 7: gray
        QColor(188, 189, 34),    // 8: yellow
        QColor(23, 190, 207),    // 9: cyan
        QColor(77, 174, 237),    // 10: light blue
        QColor(117, 219, 97),    // 11: light green
        QColor(255, 187, 120),   // 12: peach
        QColor(207, 170, 112),   // 13: tan
        QColor(98, 86, 120),     // 14: dark purple
        QColor(148, 170, 65),    // 15: olive
        QColor(200, 82, 0),      // 16: dark orange
        QColor(166, 218, 148),   // 17: light mint
        QColor(128, 43, 43),     // 18: dark red
        QColor(180, 180, 180),   // 19: light gray
        QColor(20, 80, 150),     // 20: navy
        QColor(240, 200, 120),   // 21: light yellow
    };

    // Get deterministic color for a process name using hash
    QColor getProcessColor(const QString& processName) {
        uint hash = qHash(processName);
        return kColorPalette[hash % kColorPalette.size()];
    }

    // Get color by index (wraps around palette)
    QColor getColorForIndex(int index) {
        return kColorPalette[index % kColorPalette.size()];
    }

    // Get first N colors from palette
    QVector<QColor> getProcessColors(int count) {
        QVector<QColor> colors;
        colors.reserve(count);
        for (int i = 0; i < count; ++i) {
            colors.append(getColorForIndex(i));
        }
        return colors;
    }

    // Get gray color for "Other" aggregated category
    QColor getOtherColor() {
        return QColor(150, 150, 150);
    }
}
