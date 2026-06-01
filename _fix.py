import sys

path = 'src/core/plot/QImPlotValueTrackerNode.cpp'
with open(path, 'rb') as f:
    data = f.read()

lines = data.split(b'\n')

# Lines to replace: 69 (0-indexed: 68) through 142 (0-indexed: 141, inclusive = closing brace)
# Line 69 (idx 68) is blank \r, line 142 (idx 141) is closing }
# Line 143 (idx 142) is blank \r before pie chart Doxygen

new_text = (
    "\r\n"
    "/**\r\n"
    " * \\if ENGLISH\r\n"
    " * @brief Processes tracking for BarGroups (grouped/stacked bar chart) nodes\r\n"
    " * @details For stacked bars, emits a TrackedValue for every item within the nearest\r\n"
    " *          group to the mouse cursor, using colormap color per item. For clustered\r\n"
    " *          (non-stacked) bars, emits only the nearest item's value.\r\n"
    " * \\endif\r\n"
    " *\r\n"
    " * \\if CHINESE\r\n"
    " * @brief \xe5\xa4\x84\xe7\x90\x86\xe5\x88\x86\xe7\xbb\x84\xe6\x9f\xb1\xe7\x8a\xb6\xe5\x9b\xbe\xe8\x8a\x82\xe7\x82\xb9\xe7\x9a\x84\xe8\xbf\xbd\xe8\xb8\xaa\r\n"
    " * @details \xe5\xaf\xb9\xe4\xba\x8e\xe5\xa0\x86\xe5\x8f\xa0\xe6\x9f\xb1\xe7\x8a\xb6\xe5\x9b\xbe\xef\xbc\x8c\xe4\xb8\xba\xe9\xbc\xa0\xe6\xa0\x87\xe6\x9c\x80\xe8\xbf\x91\xe7\xbb\x84\xe5\x86\x85\xe7\x9a\x84\xe6\xaf\x8f\xe4\xb8\xaa\xe6\x9d\xa1\xe7\x9b\xae\xe5\x8f\x91\xe5\x87\xba\xe8\xbf\xbd\xe8\xb8\xaa\xe5\x80\xbc\xef\xbc\x8c\xe4\xbd\xbf\xe7\x94\xa8\xe6\xaf\x8f\xe6\x9d\xa1\xe7\x9b\xae\xe5\xaf\xb9\xe5\xba\x94\xe7\x9a\x84\r\n"
    " *          colormap \xe9\xa2\x9c\xe8\x89\xb2\xe3\x80\x82\xe5\xaf\xb9\xe4\xba\x8e\xe5\xb9\xb6\xe5\x88\x97\xe6\x9f\xb1\xe7\x8a\xb6\xe5\x9b\xbe\xef\xbc\x8c\xe4\xbb\x85\xe5\x8f\x91\xe5\x87\xba\xe6\x9c\x80\xe8\xbf\x91\xe6\x9d\xa1\xe7\x9b\xae\xe7\x9a\x84\xe8\xbf\xbd\xe8\xb8\xaa\xe5\x80\xbc\xe3\x80\x82\r\n"
    " * \\endif\r\n"
    " */\r\n"
    "void QImPlotValueTrackerNode::PrivateData::processBarGroupsTracking(QImPlotBarGroupsItemNode* barItem)\r\n"
    "{\r\n"
    "    if (!barItem->isVisible() || !barItem->isEnabled()) {\r\n"
    "        return;\r\n"
    "    }\r\n"
    "    auto* series = barItem->data();\r\n"
    "    if (!series || series->itemCount() <= 0 || series->groupCount() <= 0) {\r\n"
    "        return;\r\n"
    "    }\r\n"
    "\r\n"
    "    QPointF plotPos = barItem->pixelsToPlot(mouseScreenPos.x, mouseScreenPos.y);\r\n"
    "    int itemCount   = series->itemCount();\r\n"
    "    int groupCount  = series->groupCount();\r\n"
    "    bool horizontal = barItem->isHorizontal();\r\n"
    "    bool stacked    = barItem->isStacked();\r\n"
    "    double shift    = barItem->shift();\r\n"
    "    double groupWidth = barItem->groupWidth();\r\n"
    "\r\n"
    "    // Determine nearest group position along the group axis\r\n"
    "    double groupAxisCoord = horizontal ? plotPos.y() : plotPos.x();\r\n"
    "    int nearestGroup = static_cast<int>(std::round(groupAxisCoord - shift));\r\n"
    "    nearestGroup = qBound(0, nearestGroup, groupCount - 1);\r\n"
    "\r\n"
    "    int nearestItem = 0;\r\n"
    "    if (!stacked) {\r\n"
    "        // Clustered bars: items are offset within each group\r\n"
    "        // Position of item i in group g: g + shift + (i+0.5)*subsize - groupWidth/2\r\n"
    "        // where subsize = groupWidth / itemCount\r\n"
    "        double subsize = groupWidth / itemCount;\r\n"
    "        if (subsize > 0) {\r\n"
    "            double normalized = (groupAxisCoord - shift - nearestGroup + groupWidth / 2.0) / subsize - 0.5;\r\n"
    "            nearestItem = static_cast<int>(std::round(normalized));\r\n"
    "            nearestItem = qBound(0, nearestItem, itemCount - 1);\r\n"
    "        }\r\n"
    "    }\r\n"
    "\r\n"
    "    // Item labels from series (process names, category names, etc.)\r\n"
    "    QStringList itemLabels = series->labels();\r\n"
    "\r\n"
    "    // Group tick labels (for xValueLabel)\r\n"
    "    QStringList groupLabels;\r\n"
    "    const QImPlotAxisInfo* groupAxis = horizontal ? plotNode->y1Axis() : plotNode->x1Axis();\r\n"
    "    if (groupAxis && !groupAxis->tickLabels().isEmpty()) {\r\n"
    "        for (const auto& tl : groupAxis->tickLabels()) {\r\n"
    "            groupLabels << QString::fromUtf8(tl);\r\n"
    "        }\r\n"
    "    }\r\n"
    "\r\n"
    "    if (stacked) {\r\n"
    "        // Stacked bars: emit a TrackedValue for every item at the nearest group\r\n"
    "        for (int item = 0; item < itemCount; ++item) {\r\n"
    "            double val = series->value(item, nearestGroup);\r\n"
    "            if (skipNanFiniteValues && (std::isnan(val) || std::isinf(val) || !std::isfinite(val))) {\r\n"
    "                continue;\r\n"
    "            }\r\n"
    "\r\n"
    "            TrackedValue tv;\r\n"
    "            tv.sourceType = SourceType::BarGroups;\r\n"
    "            tv.label      = itemLabels.value(item).toStdString();\r\n"
    "            tv.color      = toQColor(ImPlot::GetColormapColor(item));\r\n"
    "            tv.xValue     = nearestGroup;\r\n"
    "            tv.yValue     = val;\r\n"
    "            char buf[64];\r\n"
    "            ImFormatString(buf, sizeof(buf), \"%.3f\", tv.yValue);\r\n"
    "            tv.yValueLabel = buf;\r\n"
    "            if (nearestGroup < groupLabels.size()) {\r\n"
    "                tv.xValueLabel = groupLabels[nearestGroup].toStdString();\r\n"
    "            } else {\r\n"
    "                char xbuf[32];\r\n"
    "                ImFormatString(xbuf, sizeof(xbuf), \"%d\", nearestGroup);\r\n"
    "                tv.xValueLabel = xbuf;\r\n"
    "            }\r\n"
    "            trackedValues.emplace_back(tv);\r\n"
    "        }\r\n"
    "    } else {\r\n"
    "        // Clustered bars: emit only the single nearest item\r\n"
    "        double val = series->value(nearestItem, nearestGroup);\r\n"
    "        if (skipNanFiniteValues && (std::isnan(val) || std::isinf(val) || !std::isfinite(val))) {\r\n"
    "            return;\r\n"
    "        }\r\n"
    "\r\n"
    "        TrackedValue tv;\r\n"
    "        tv.sourceType = SourceType::BarGroups;\r\n"
    "        tv.label      = itemLabels.value(nearestItem).toStdString();\r\n"
    "        tv.color      = toQColor(ImPlot::GetColormapColor(nearestItem));\r\n"
    "        tv.xValue     = nearestGroup;\r\n"
    "        tv.yValue     = val;\r\n"
    "        char buf[64];\r\n"
    "        ImFormatString(buf, sizeof(buf), \"%.3f\", tv.yValue);\r\n"
    "        tv.yValueLabel = buf;\r\n"
    "        if (nearestGroup < groupLabels.size()) {\r\n"
    "            tv.xValueLabel = groupLabels[nearestGroup].toStdString();\r\n"
    "        } else {\r\n"
    "            char xbuf[32];\r\n"
    "            ImFormatString(xbuf, sizeof(xbuf), \"%d\", nearestGroup);\r\n"
    "            tv.xValueLabel = xbuf;\r\n"
    "        }\r\n"
    "        trackedValues.emplace_back(tv);\r\n"
    "    }\r\n"
    "}\r\n"
)

new_bytes = new_text.encode('latin-1')  # preserve raw UTF-8 bytes

before = b'\n'.join(lines[:68])   # lines 1..68 (idx 0..67)
after = b'\n'.join(lines[142:])   # lines 143+ (idx 142+)

result = before + new_bytes + after

with open(path, 'wb') as f:
    f.write(result)

# Verify
with open(path, 'rb') as f:
    new_data = f.read()
new_lines = new_data.split(b'\n')
print(f"Old line count: {len(lines)}, New line count: {len(new_lines)}")
for i, line in enumerate(new_lines):
    if b'processBarGroupsTracking' in line and b'void' in line:
        print(f"Function at line {i+1}")
    if b'processPieChartTracking' in line and b'void' in line:
        print(f"Pie function at line {i+1}")
