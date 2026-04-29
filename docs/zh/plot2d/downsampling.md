# 数据降采样

当你用折线图展示传感器采集的实时温度，一天下来可能积累了上百万个采样点。而你的屏幕宽度不过一千多个像素——这意味着绝大多数数据点被挤在同一个像素里互相重叠，用户根本看不清细节，GPU却在拼命渲染那些永远不会被眼睛分辨的点。

这就是大数据量绘图面临的核心矛盾：**数据量远超屏幕的表达能力**。降采样（Downsampling）正是为了解决这个矛盾而生的——它从海量数据中挑出少量视觉上最有代表性的点，让渲染只处理这些点，在保持曲线"看起来一样"的前提下把帧率拉回可用水平。

## 为什么需要降采样

假设你有一百万个数据点要画在一条折线图上，一个 1920×1080 的屏幕横向也只有 1920 个像素。就算曲线占满整个屏幕宽度，平均每个像素也要承载 520 个数据点。这意味着：

1. **GPU渲染压力**：每帧要向GPU提交100万条线段的顶点数据，绘制命令本身的开销就足以拖低帧率。实测中，100万点无降采样时帧率可能跌到10 FPS以下。
2. **亚像素冗余**：520个点挤在同一像素列里，无论你画多精细，最终呈现的只是这520个点的"包络线"——最高点和最低点之间的填充区域。中间那些点画了也看不见。
3. **交互卡顿**：当用户缩放或平移视图时，每帧都要重新处理100万点的渲染管线，拖慢交互响应。

降采样并不改变原始数据，它只是在渲染层插入一个代理：原始数据还在内存里完整保存，但给GPU看的只是精心挑选的少量代表点。这是一种"视觉近似"策略——人眼分辨不出百万点和几千点画出来的曲线有什么区别，但GPU的负担从百万级降到了千级。

### 多少点就够了

一个很自然的问题：降采样后保留多少点才够？答案是**和屏幕像素数相关**。QIm采用"像素感知"策略：

```
目标点数 = 屏幕像素宽度 × 5
```

为什么乘以5？因为一个像素列里至少需要几个点才能描绘出该位置的波形细节（峰值、谷值、斜率变化）。5倍系数是经验值，能在各种曲线形状下保证视觉保真。一块800像素宽的绘图区域，目标点数就是4000点；一块1920像素宽的区域，目标点数是9600点。无论原始数据是10万还是1000万，最终渲染的数据量只取决于你的屏幕有多宽。

## 常见降采样算法

降采样算法的研究始于2013年Sveinn Steinarsson的LTTB论文，此后学术界和工业界陆续提出了多种变体。下面介绍几种主流算法，帮助你理解QIm选择了什么以及为什么。

### M4——最简单但最粗糙

M4（min-max-first-last）是最直觉的降采样方式：把数据分成若干桶，每个桶只保留四个点——第一个点、最后一个点、最小值点和最大值点。

- **优点**：实现极简，速度极快，确保峰值和谷值不会丢失。
- **缺点**：曲线在桶边界处会出现不自然的"锯齿"，因为桶与桶之间的过渡只靠首尾点衔接，斜率变化完全被忽略。视觉上曲线显得粗糙，有明显的阶梯感。

M4适合那些对视觉质量要求不高的场景（比如纯监控告警，只关心有没有超过阈值），但在需要"看起来像原始曲线"的场合表现不佳。

### LTTB——经典平衡之选

LTTB（Largest-Triangle-Three-Buckets）由Sveinn Steinarsson于2013年在论文《Downsampling Time Series for Visual Representation》中提出，是目前最广泛使用的可视化降采样算法。

它的核心思想可以用一个几何类比来理解：想象你在画一条曲线，已经画了前一个点，现在要从当前这桶数据里选一个点作为下一个笔触。怎么选？LTTB的办法是——构造一个三角形：三个顶点分别是"已选的上一个点"、"当前桶里的候选点"、"下一个桶的平均点"，然后选让这个三角形面积最大的那个候选点。

面积最大意味着什么？意味着这个候选点离"直线插值"最远——它不是平庸地落在两个端点的连线附近，而是偏离最显著的那个点。这正是视觉上最"醒目"的点——峰值、谷值、急转弯都会产生大三角形面积。

**LTTB的完整流程**：

1. 固定保留第一个点和最后一个点（端点对齐）
2. 把中间的数据分成 `(目标点数 - 2)` 个桶
3. 对每个桶：
   - 计算下一个桶里所有点的平均位置（虚拟"第三点"）
   - 对当前桶里的每个点，计算它与"上一个已选点"和"第三点"构成的三角形面积
   - 选面积最大的那个点
4. 依次处理所有桶，串联出降采样后的曲线

- **优点**：O(n)时间复杂度（每个桶只需线性扫描），视觉效果优秀——曲线形状、峰值、谷值都被很好地保留，是公认的"最佳视觉保真降采样算法"之一。
- **缺点**：每个桶内需要逐点计算三角形面积，这个计算量虽然单次很轻（一次叉积），但当桶很大时（数据量远超目标点数时，每个桶可能包含数千个点），遍历所有点就变成了瓶颈。100万点降到4000点，意味着平均每个桶约250个点要逐一计算面积——总共25万次面积计算。

LTTB适合10万点以内的场景，计算开销可控，视觉质量无可挑剔。但当数据量攀升到百万级时，它的线性开销开始变得明显。

### MinMaxLTTB——LTTB的加速版

MinMaxLTTB由比利时根特大学predict-idlab团队提出（参考：[predict-idlab/MinMaxLTTB](https://github.com/predict-idlab/MinMaxLTTB)），是对LTTB的巧妙加速。

它的洞察在于：LTTB的性能瓶颈在于"每个桶内逐点计算面积"，但绝大多数点的面积值很小——只有极值点（桶内最大值和最小值）附近的点才可能产生大的三角形面积。那为什么不先快速找出极值点，只在极值点上计算面积呢？

**MinMaxLTTB的两阶段流程**：

**第一阶段：MinMax预筛选**

每个桶不是直接遍历所有点计算面积，而是先把桶再细分成若干子区间（sub-intervals），在每个子区间里找出最大值点和最小值点，把它们标记为"候选点"。这样每个桶原本有几百个点，经过预筛选后只剩下几个到几十个候选点。

子区间数量由 `preselection_ratio` 参数控制：`子区间数 = ceil(桶大小 / preselection_ratio)`。默认 ratio=4.0意味着平均每4个数据点对应一个子区间，每个子区间贡献2个候选点（max + min），所以每个桶大约有 `桶大小/2` 个候选点——相比原来的全部遍历，候选点数减少了一半。

你可能会问：既然候选点还是桶大小的一半，能快多少？关键在于——**面积计算只在候选点上进行**，而极值查找（argmin/argmax）可以用SIMD指令在4个double上一口气完成，比逐点面积计算快得多。后面会详细解释这一点。

**第二阶段：LTTB面积选择**

有了候选点列表后，计算每个候选点的三角形面积（与LTTB完全相同的公式），选面积最大的那个。因为候选点只有原始点数的几分之一，这一步的计算量大幅减少。

- **优点**：视觉上与纯LTTB几乎不可区分——因为极值点本身就覆盖了绝大多数"视觉醒目"的位置（峰值、谷值），遗漏的只是少量非极值的斜率变化点，人眼几乎察觉不到。性能提升10-30倍——100万点从约500ms降到约20ms。
- **缺点**：极少数情况下可能遗漏非极值的重要点（比如拐点恰好不是局部极值），但增加preselection_ratio可以缓解。参数敏感——ratio太小则预筛选不够精细，ratio太大则候选点太多、加速效果减弱。默认4.0是经过大量实验验证的平衡值。

### 其他算法简介

学术界还有一些更新的算法：

- **FPCS（Four-Phase Continuous Selection）**：IEEE VIS 2024论文《FPCS: Feature Preserving Compensated Sampling of Streaming Time Series Data》提出，四阶段流水线式选择，特别针对流式数据（在线/增量处理）优化，理论上在视觉保真度（SSIM指标）上优于LTTB，但实现复杂度较高，目前尚未有成熟的C++工业实现。
- **VT+LTTB**：结合视口（Viewport）信息的LTTB变体，只在当前可见范围内降采样，QIm的MinMaxLTTB已经内置了视口感知重采样功能。
- **动态MinMax**：纯MinMax（不接LTTB面积选择），速度最快但视觉质量逊于MinMaxLTTB。

QIm目前选择了LTTB和MinMaxLTTB作为内置算法，因为它们在视觉保真和计算效率之间提供了最好的平衡，且有大量工业验证。未来可能引入FPCS等新算法。

## QIm中的降采样算法

QIm内置两种降采样算法，通过装饰器（Decorator）模式实现——降采样器是`QImAbstractXYDataSeries`的透明代理，对外接口与原始数据完全一致，内部决定是否需要降采样以及用什么算法。

### 标准LTTB——QImLTTBDownsampler

`QImLTTBDownsampler`实现了经典的LTTB算法，适合10万点以内的数据量。

它的核心方法`lttb()`严格遵循Steinarsson论文的流程：固定端点 + 桶划分 + 最大三角形面积选择。代码中还处理了Y-only模式（只有Y数据、X由`xStart+xScale*index`计算得出）和NaN值的安全跳过。

**LTTB的关键代码逻辑**：

```cpp
// 对每个桶：
// 1. 计算下一个桶的平均点（avg_x, avg_y）
for (int j = bucket_start; j < bucket_end; ++j) {
    avg_x += getX(j); avg_y += getY(j); valid_count++;
}
avg_x /= valid_count; avg_y /= valid_count;

// 2. 在当前桶中找三角形面积最大的点
for (int j = bucket_start; j < bucket_end; ++j) {
    double area = fabs((curr_x - last_x) * (avg_y - last_y)
                     - (avg_x - last_x) * (curr_y - last_y));
    if (area > max_area) { max_area = area; max_idx = j; }
}

// 3. 选出这个点
m_cached_x.push_back(getX(max_idx));
m_cached_y.push_back(getY(max_idx));
```

注意第二次遍历——对桶内每个点都计算面积。这就是LTTB的性能瓶颈所在。

### MinMaxLTTB——QImMinMaxLTTBDownsampler

`QImMinMaxLTTBDownsampler`实现了MinMax预筛选 + LTTB面积选择的两阶段算法，适合10万点以上的大数据量场景。

**MinMaxLTTB的关键代码逻辑**：

```cpp
// 对每个桶的每个子区间：
// 第一阶段：MinMax预筛选——找出极值点作为候选
auto result = simdArgMinMax(y_ptr + sub_pos, sub_len);
int local_max_idx = result.max_idx + sub_pos;
int local_min_idx = result.min_idx + sub_pos;
candidate_stack[candidate_count++] = local_max_idx;
if (local_max_idx != local_min_idx)
    candidate_stack[candidate_count++] = local_min_idx;

// 第二阶段：LTTB面积选择——只在候选点上计算面积
for (int ci = 0; ci < candidate_count; ++ci) {
    const int idx = candidate_stack[ci];
    double area = fabs((getXValue(idx) - last_x) * (avg_y - last_y)
                     - (avg_x - last_x) * (y_ptr[idx] - last_y));
    if (area > max_area) { max_area = area; best_idx = idx; }
}
```

对比LTTB的代码：原来对桶内所有点逐一计算面积，现在只对几个候选点计算面积。候选点数量通常是原始点数的1/2到1/4，所以面积计算次数减少2-4倍。再加上SIMD加速的极值查找（后面会详述），整体加速可达10-30倍。

### 两种算法对比

| 特性 | LTTB | MinMaxLTTB |
|------|------|------------|
| 时间复杂度 | O(n)，每点1次面积计算 | O(n)，但面积计算只在候选点上 |
| 100万点耗时 | ~500ms | ~20ms |
| 视觉保真 | 最佳 | 极佳（几乎不可区分） |
| 峰值/谷值保留 | 通过面积选择间接保留 | MinMax直接确保极值不遗漏 |
| 适用数据量 | < 10万点 | > 10万点 |
| 参数 | target_points | target_points + preselection_ratio |

## MinMaxLTTB的性能瓶颈与SIMD加速

MinMaxLTTB虽然比纯LTTB快10-30倍，但它内部仍然有一个瓶颈：**子区间极值查找**。每个子区间里的argmin和argmax是标量逐点比较——一个循环遍历所有点，逐一与当前最小值/最大值做比较。当数据量极大时（比如100万点、几千个子区间），这个循环的次数仍然可观。

更深层的问题是：标量比较每次只处理1个double，而现代CPU的SIMD寄存器可以同时处理4个double（AVX2）或2个double（SSE4.2）。这意味着标量代码只利用了CPU计算能力的1/4甚至更少。

### SIMD是什么

SIMD（Single Instruction Multiple Data）是CPU提供的一种并行计算能力。一条SIMD指令可以同时对多个数据执行相同的操作。以AVX2为例：

- 标量代码：`if (data[i] < min_val)` —— 一条比较指令只处理1个double
- AVX2代码：`_mm256_cmp_pd(vals, min_vec, _CMP_LT_OQ)` —— 一条指令同时比较4个double

这就像从"一个人逐个检查4扇门"变成"一个人同时检查4扇门"——效率提升不是4倍（因为还有其他开销），但3-5倍是现实可达的。

### QIm的SIMD加速实现——QImSimdArgMinMax

QIm为此专门实现了一个SIMD加速的argmin+argmax工具模块`QImSimdArgMinMax`。它提供`simdArgMinMax()`函数，在一条遍历中同时找到最小值和最大值的索引及值。

**三条执行路径**：

| 路径 | SIMD宽度 | 覆盖CPU | 加速比 |
|------|---------|---------|--------|
| AVX2 | 4 doubles/迭代 | 2013年后的x86（Intel Haswell+、AMD Excavator+） | 3-5x |
| SSE4.2 | 2 doubles/迭代 | 2010年后的x86（几乎覆盖所有现代CPU） | 2-3x |
| 标量 | 1 double/迭代 | 兜底路径 | 1x |

**运行时CPU检测**：程序启动时通过`__cpuid`（Windows）或`__builtin_cpu_supports`（Linux/macOS）检测当前CPU支持的SIMD级别，然后用函数指针锁定最优路径。之后每次调用`simdArgMinMax()`都直接走函数指针，没有任何分支判断开销。同一个二进制文件在不同CPU上自动选择最优路径——AVX2 CPU走AVX2，老旧CPU走标量，无需分发多个版本。

**AVX2路径的核心逻辑**（简化示意）：

```
初始化：min_vec = [DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX]
         max_vec = [-DBL_MAX, -DBL_MAX, -DBL_MAX, -DBL_MAX]

每次迭代（处理4个double）：
  vals = load 4 doubles from data[i..i+3]
  cmp_min = (vals < min_vec) ? 1 : 0  // 同时比较4个
  min_idx = blend(min_idx, current_idx, cmp_min)  // 同时更新4个索引
  min_vec = min(min_vec, vals)  // 同时更新4个最小值
  cmp_max = (vals > max_vec) ? 1 : 0
  max_idx = blend(max_idx, current_idx, cmp_max)
  max_vec = max(max_vec, vals)

最后：从4个lane中选出最终的min和max
```

一次迭代处理4个double，同时追踪最小值和最大值——这就是SIMD的威力：**单遍历、双结果、4倍吞吐**。

**NaN安全性**：IEEE 754浮点标准规定，NaN与任何值的比较（<、>）都返回false。SIMD比较指令遵循同样的语义，所以NaN永远不会被选为最小值或最大值——它被自然跳过。对于全NaN的子区间，`simdArgMinMax()`返回哨兵值`{0, 0, DBL_MAX, -DBL_MAX}`，MinMaxLTTB的后续逻辑会正确处理这种情况。

### NaN快速路径

实际数据中是否含有NaN值，对性能路径的选择至关重要：

- **无NaN数据**（常见场景）：可以直接走SIMD路径——极值查找用`simdArgMinMax()`，平均值计算不需要跳过NaN检查，整条路径干净高效。
- **有NaN数据**（如传感器断连产生的空值）：SIMD的极值查找本身对NaN安全（NaN不会成为min/max），但平均值计算需要跳过NaN点，否则会污染平均值。此外，候选点的三角形面积计算也需要跳过NaN候选点。

QIm的优化策略是在`minMaxLTTB()`开头做一次快速预扫描——遍历所有Y值，只要发现一个NaN就标记`hasNaN=true`。这个扫描本身是O(n)的，但它只做最简单的`isnan()`判断，开销极低。之后整个算法根据`hasNaN`标志分两条路径：

- `hasNaN=false` → SIMD极值查找 + 无分支的平均值累加
- `hasNaN=true` → 标量极值查找 + NaN-safe的平均值累加

绝大多数真实数据不含NaN，所以绝大多数时候走的是高效SIMD路径。

### 其他微优化

除了SIMD加速，MinMaxLTTB还做了4项微优化，与SIMD叠加产生额外收益：

1. **消除lambda间接访问**：原始代码用`getX`和`getY`两个lambda来获取数据值，每次调用都有函数调用开销。重构后改为直接指针偏移——`x_ptr[local_idx]`和`y_ptr[local_idx]`，编译器可以直接生成内存访问指令，省去间接调用。

2. **栈数组替代vector**：每个桶的候选点原本用`std::vector<int>`存储，每次清空和push_back都有堆内存操作开销。改为栈上固定数组`int candidate_stack[64]`——零分配开销，且数据在栈上缓存命中率更高。当候选点数超过64时（极端情况），多余的候选点被静默丢弃——MinMaxLTTB是启发式算法，少几个候选点只轻微影响精度，不会导致错误。

3. **NaN快速路径**：如上所述，预扫描`hasNaN`标志决定走SIMD还是标量路径。

4. **全NaN检测合并到主循环**：原始代码在降采样结束后额外遍历一遍来检测"所有Y值都是NaN"的情况。重构后改为在主循环中用`anyValidY`标志追踪——每选一个点就检查其Y值是否非NaN，循环结束时直接判断`anyValidY`，省掉了一次O(k)的额外遍历。

这些微优化各自贡献不大（1.1x-1.3x），但叠加在一起约1.5-2x，与SIMD的3-5x叠加，综合加速5-10x。

## 降采样控制器

QIm的降采样不仅是一个算法选择，还包含一整套自适应管理机制，由`QImDownsamplingController`统一协调。

### 算法选择

通过`QImDownsampleAlgorithm`枚举指定算法：

| 枚举值 | 说明 | 适用场景 |
|--------|------|---------|
| `None` | 不降采样 | 数据量小（<1万点），需要精确渲染每个点 |
| `LTTB` | 标准LTTB | 1万-10万点，追求最佳视觉保真 |
| `MinMaxLTTB` | MinMax预筛选+LTTB | 10万点以上，追求性能 |
| `Auto` | 自动选择（默认） | 系统根据数据量和阈值自动决定 |

Auto模式的启发式规则：当数据量超过阈值（默认20000点）时，根据数据量选择算法——1万到10万点选LTTB，超过10万点选MinMaxLTTB。低于阈值则不降采样。

### 视口感知重采样

当用户缩放图表（放大或缩小X轴范围）时，可见区域内的数据点密度会变化——放大后可见点变少可能不需要降采样了，缩小后可见点变多可能需要重新降采样。

QIm的`QImDownsamplingController`内置了智能重采样触发机制：

- **缩放**：X轴范围缩小超过33%（放大）或扩大超过50%（缩小）时重新降采样
- **窗口调整**：绘图区域像素宽度变化超过10%时重新降采样
- **平移**：纯平移操作不触发重新降采样——因为可见点数没变

这种策略避免了每帧都重新降采样的开销，只在真正需要时才计算。

### 像素感知目标点数

`QImDownsamplingController`的`pixelAwareTargetPoints()`方法根据屏幕像素宽度自动计算最优目标点数：

```
目标点数 = clamp(pixelWidth × 5, 100, 50000)
```

800像素宽 → 4000点；50像素宽 → 100点（下限保护）；10000像素宽 → 50000点（上限保护）。

## 在QIm中使用降采样

### 默认配置（自动模式）

`QImPlotLineItemNode`和`QImPlotScatterItemNode`默认启用自适应降采样，算法自动选择：

```cpp
auto* line = new QIM::QImPlotLineItemNode(plot);
line->setData(x, y);

// 默认配置：
// - downsampleAlgorithm() == QImDownsampleAlgorithm::Auto
// - downsampleThreshold() == 20000
```

只要数据量超过20000点，降采样就会自动生效，算法由Auto模式根据数据量决定。你不需要写任何额外代码。

### 指定降采样算法

如果你想手动控制算法：

```cpp
// 使用LTTB算法（适合中等数据量，视觉保真最佳）
line->setDownsampleAlgorithm(QIM::QImDownsampleAlgorithm::LTTB);

// 使用MinMaxLTTB算法（适合大数据量，性能最佳）
line->setDownsampleAlgorithm(QIM::QImDownsampleAlgorithm::MinMaxLTTB);

// 关闭降采样（精确渲染每个点，大数据量时帧率会很低）
line->setDownsampleAlgorithm(QIM::QImDownsampleAlgorithm::None);

// 恢复自动选择
line->setDownsampleAlgorithm(QIM::QImDownsampleAlgorithm::Auto);
```

### 配置降采样阈值

阈值决定"多少个数据点以上才触发降采样"。默认值20000适合大多数场景，你可以根据需求调整：

```cpp
// 更激进：5000点以上就开始降采样（交互更流畅，但小数据量也会被压缩）
line->setDownsampleThreshold(5000);

// 更保守：100000点以上才开始降采样（小数据量完全精确，但中等数据量可能卡顿）
line->setDownsampleThreshold(100000);

// 查看当前阈值
int threshold = line->downsampleThreshold();
```

### 响应算法变更信号

当降采样算法或阈值发生变化时，会发射对应的信号：

```cpp
connect(line, &QIM::QImPlotLineItemNode::downsampleAlgorithmChanged,
        [](QIM::QImDownsampleAlgorithm algo) {
    qDebug() << "降采样算法切换为:" << static_cast<int>(algo);
});

connect(line, &QIM::QImPlotLineItemNode::downsampleThresholdChanged,
        [](int threshold) {
    qDebug() << "降采样阈值变更为:" << threshold;
});
```

### 通过Q_PROPERTY绑定

如果你在QML或需要动态属性绑定的场景中使用：

```cpp
line->setProperty("downsampleAlgorithm",
                  QVariant::fromValue(QIM::QImDownsampleAlgorithm::MinMaxLTTB));
line->setProperty("downsampleThreshold", 30000);
```

## 性能对比

以下是QIm内置两种降采样算法的性能数据（100万点，渲染100次取平均）：

### 降采样计算耗时

| 算法 | 100万点降采样耗时 | 说明 |
|------|-------------------|------|
| LTTB | ~500ms | 每个桶内逐点计算面积 |
| MinMaxLTTB | ~20ms | MinMax预筛选 + SIMD加速极值查找 |

### 渲染帧率

| 数据量 | 无降采样 | LTTB降采样 | MinMaxLTTB降采样 |
|--------|---------|-----------|-----------------|
| 10万 | ~60 FPS | ~60 FPS | ~60 FPS |
| 100万 | ~10 FPS | ~25 FPS | ~27 FPS |
| 500万 | ~2 FPS | ~4 FPS | ~5 FPS |

注意：降采样后帧率的提升幅度取决于降采样计算本身的开销和渲染管线。当降采样计算足够快（MinMaxLTTB的20ms），帧率瓶颈转移到GPU渲染阶段——此时LTTB和MinMaxLTTB的帧率差距缩小（因为两者都把渲染数据量压缩到了相近的水平），但MinMaxLTTB的降采样计算更快意味着交互响应更灵敏。

## 最佳实践

| 数据量 | 推荐配置 | 原因 |
|--------|---------|------|
| < 1万点 | `None` 或 `Auto` | 数据量小，降采样反而增加开销 |
| 1万-10万点 | `Auto` 或 `LTTB` | LTTB视觉保真最佳，计算开销可控 |
| 10万-100万点 | `Auto` 或 `MinMaxLTTB` | MinMaxLTTB性能优势明显 |
| > 100万点 | `MinMaxLTTB` | 只有MinMaxLTTB能在合理时间内完成降采样 |

**几个实用建议**：

- 默认配置（Auto + 阈值20000）对大多数场景已经足够好，不需要手动调整。
- 如果你的数据不含NaN值（大多数传感器数据确实不含），MinMaxLTTB会走SIMD加速路径，性能最优。
- 如果你需要精确渲染每一个点（比如数据量只有几百点），直接关闭降采样——降采样对小数据量反而有额外开销。
- 缩放交互时不需要担心性能——QIm的视口感知重采样只在显著缩放时才重新计算，平移操作零开销。

## 自定义降采样器

QIm的降采样架构基于装饰器模式——降采样器是`QImAbstractXYDataSeries`的子类，可以无缝替换原始数据。如果你想实现自己的降采样算法：

1. 继承`QImAbstractXYDataSeries`，实现所有虚函数（`size()`、`xRawData()`、`yRawData()`等）
2. 在构造函数中接收原始数据指针（不拥有所有权）
3. 内部维护降采样后的缓存数据
4. 在`QImDownsampleAlgorithm`枚举中添加新值
5. 在`QImDownsamplingController::rebuild()`中添加对应的case分支

```cpp
// 示例：自定义降采样器骨架
class QImCustomDownsampler : public QImAbstractXYDataSeries
{
    QImAbstractXYDataSeries* m_source;
    std::vector<double> m_cached_x, m_cached_y;
    bool m_cached_valid = false;

public:
    explicit QImCustomDownsampler(QImAbstractXYDataSeries* source, int target_points)
        : m_source(source) { /* 执行降采样 */ }

    int size() const override {
        return m_cached_valid ? m_cached_x.size() : m_source->size();
    }
    const double* xRawData() const override {
        return m_cached_valid ? m_cached_x.data() : m_source->xRawData();
    }
    const double* yRawData() const override {
        return m_cached_valid ? m_cached_y.data() : m_source->yRawData();
    }
    // ... 其他虚函数
};
```

如果你的自定义算法也需要极值查找，可以直接复用`QImSimdArgMinMax`的`simdArgMinMax()`函数——它已经是SIMD加速的、经过充分测试的工具函数。

## 参考

- LTTB论文：Sveinn Steinarsson, *Downsampling Time Series for Visual Representation*, 2013
- MinMaxLTTB：[predict-idlab/MinMaxLTTB](https://github.com/predict-idlab/MinMaxLTTB)
- SIMD argminmax参考实现：[tsdownsample/argminmax](https://github.com/predict-idlab/tsdownsample)
- QIm源码：`QImLTTBDownsampler.h/.cpp`、`QImMinMaxLTTBDownsampler.h/.cpp`、`QImSimdArgMinMax.h/.cpp`、`QImDownsamplingController.h/.cpp`
- 相关文档：[性能对比](performance.md)、[渲染性能规范](../dev/render-guidelines.md)