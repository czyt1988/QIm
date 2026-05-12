#pragma once

#include <QColor>
#include <QHash>
#include <QString>
#include <QStringList>
#include <QVector>
#include "imgui.h"

namespace QImSystemMonitor {

// Paul Tol's 22-color Qualitative palette (colorblind-friendly)
static const QVector<QColor> kColorPalette = {
    QColor("#332288"),  // 0: dark blue
    QColor("#88CCEE"),  // 1: light blue
    QColor("#44AA99"),  // 2: teal
    QColor("#117733"),  // 3: green
    QColor("#99CC55"),  // 4: light green
    QColor("#DDCC77"),  // 5: yellow
    QColor("#CC9944"),  // 6: orange
    QColor("#EE8866"),  // 7: light red
    QColor("#CC6677"),  // 8: pink
    QColor("#AA4499"),  // 9: magenta
    QColor("#882255"),  // 10: dark red
    QColor("#AAAAAA"),  // 11: light gray
    QColor("#555555"),  // 12: dark gray
    QColor("#6699AA"),  // 13: steel blue
    QColor("#997755"),  // 14: brown
    QColor("#CCAA99"),  // 15: tan
    QColor("#AACC99"),  // 16: pale green
    QColor("#779977"),  // 17: olive
    QColor("#DDDDAA"),  // 18: pale yellow
    QColor("#BBAA88"),  // 19: beige
    QColor("#EEBB99"),  // 20: peach
    QColor("#BB9999"),  // 21: mauve
};

// ColorManager provides deterministic, index-based color assignment
// First-seen process gets index 0, subsequent first-seen processes get incrementing indices
// Same process always gets same color across runs (order-based, NOT hash-based)
class ColorManager {
public:
    // Get color for a process name (first-seen assigns new index, subsequent returns fixed color)
    QColor colorFor(const QString& processName) {
        if (!colorIndex_.contains(processName)) {
            colorIndex_[processName] = colorIndex_.size();
        }
        return kColorPalette[colorIndex_[processName] % kColorPalette.size()];
    }

    // Get color by explicit index (wraps around palette)
    QColor colorForIndex(int index) {
        return kColorPalette[index % kColorPalette.size()];
    }

    // Batch convert process names to ImVec4 colors for ImPlot custom colormap
    std::vector<ImVec4> toImVec4Colors(const QStringList& names) {
        std::vector<ImVec4> colors;
        colors.reserve(names.size());
        for (const QString& name : names) {
            QColor c = colorFor(name);
            colors.push_back({static_cast<float>(c.redF()), static_cast<float>(c.greenF()), static_cast<float>(c.blueF()), 1.0f});
        }
        return colors;
    }

    // Get current number of registered processes
    int registeredCount() const {
        return colorIndex_.size();
    }

    // Clear all registered process-color mappings (for reset scenarios)
    void clear() {
        colorIndex_.clear();
    }

private:
    QHash<QString, int> colorIndex_;
};

} // namespace QImSystemMonitor