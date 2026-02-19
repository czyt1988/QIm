# Dear ImGui：字体使用指南

本文为ImGui官方文档的中文翻译

（您也可以在 https://github.com/ocornut/imgui/blob/master/docs/FONTS.md 浏览此文档，或使用任意 Markdown 查看器阅读本文件）

Dear ImGui 的 `imgui.cpp` 代码中内嵌了 [ProggyClean.ttf](http://proggyfonts.net)（作者：Tristan Grimmer）的副本，这是一款高度为 13 像素、像素完美的默认字体。我们将其嵌入源代码中，以便您无需文件系统访问即可使用 Dear ImGui。ProggyClean 字体无法平滑缩放，因此建议在追求美观外观并需支持多种分辨率的应用程序中加载您自己的字体文件。

您也可以加载外部的 .TTF/.OTF 字体文件。在 [misc/fonts/](https://github.com/ocornut/imgui/tree/master/misc/fonts) 文件夹中，我们提供了一些推荐字体作为便利参考。

另请阅读 FAQ：https://www.dearimgui.com/faq（其中包含字体相关章节！）

## 目录
- [故障排查](#故障排查)
- [新增！1.92 版本（2025 年 6 月）的动态字体系统](#新增192-版本2025-年-6-月的动态字体系统)
- [如何在应用程序中处理 DPI？](#如何在应用程序中处理-dpi)
- [字体加载指南](#字体加载指南)
- [从内存加载字体数据](#从内存加载字体数据)
- [从源代码嵌入字体数据](#从源代码嵌入字体数据)
- [使用图标字体](#使用图标字体)
- [排除重叠范围](#排除重叠范围)
- [使用 FreeType 光栅化器 (imgui_freetype)](#使用-freetype-光栅化器-imgui_freetype)
- [使用彩色字形/表情符号](#使用彩色字形表情符号)
- [使用自定义字形范围](#使用自定义字形范围)
- [使用自定义彩色图标](#使用自定义彩色图标)
- [关于文件名](#关于文件名)
- [关于 UTF-8 编码](#关于-utf-8-编码)
- [调试工具](#调试工具)
- [仓库中包含字体的版权/许可信息](#仓库中包含字体的版权许可信息)
- [字体资源链接](#字体资源链接)

## 故障排查

绝大多数字体和文本相关问题源于以下 4 个原因：

**(1) 由于使用 `\` 或工作目录意外导致的无效文件名**  
请参阅 [关于文件名](#关于文件名)。`AddFontXXX()` 函数在文件名不正确时应触发断言。

**(2) 非 ASCII 字符串的 UTF-8 编码无效**  
请参阅 [关于 UTF-8 编码](#关于-utf-8-编码)。使用编码查看器确认源代码中字符串字面量的编码是否正确。

**(3) 缺少字形范围**  
**自 1.92 起，配合更新的后端：不再需要指定字形范围。**  
**1.92 之前**：若要使用非 ASCII 字符，需在加载字体时显式指定字形范围。请参阅 [字体加载指南](#字体加载指南)。使用 [调试工具](#调试工具) 确认已加载的字体和字形范围。

这是 Dear ImGui 之前的限制（已在 1.92 版本解除）：加载字体时需要指定要加载哪些字符的字形。所有已加载字体的字形会预先渲染到**单个纹理图集**中。调用 `io.Fonts->GetTexDataAsAlpha8()`、`io.Fonts->GetTexDataAsRGBA32()` 或 `io.Fonts->Build()` 任一函数都会构建该图集。此操作通常由渲染后端调用，例如 `ImGui_ImplDX11_NewFrame()` 会调用它。**若使用自定义字形范围，请确保该数组在调用 `GetTexDataAsAlpha8()/GetTexDataAsRGBA32()/Build()` 期间保持持久存在且可用。**

**(4) 字体图集纹理无法上传至 GPU**  
**自 1.92 起，配合更新的后端：图集采用增量构建并可动态调整大小，此问题发生概率降低**  
此问题通常是第 3 点的衍生结果。如果您有大量字形或多个字体，纹理可能超出图形 API 的限制。上传纹理失败的典型表现是所有字形或内容显示为空白的白色矩形。请注意某些图形驱动程序存在纹理尺寸限制。若您正在开发 PC 应用程序，请注意用户可能使用硬件限制低于您开发环境的设备。

![纹理上传失败示例](https://github.com/user-attachments/assets/68b50fb5-8b9d-4c38-baec-6ac384f06d26)

**解决方案：**
- 降低过采样率，例如 `font_config.OversampleH = 1`，可将纹理大小减半（但会损失质量）
  - 注意：虽然 `OversampleH = 2` 在大多数情况下视觉效果与 3 非常接近，但 `OversampleH = 1` 会导致明显质量下降。请阅读[此处](https://github.com/nothings/stb/blob/master/tests/oversample)了解过采样
- 通过源语言本地化数据计算字形范围以减少范围
  - 可使用 `ImFontGlyphRangesBuilder` 实现此目的，并在需要新字符时在帧之间重建图集。这是最有效的优化！
- 设置 `io.Fonts.Flags |= ImFontAtlasFlags_NoPowerOfTwoHeight;` 以禁用将纹理高度向上取整为 2 的幂

[返回目录](#目录)

## 新增！1.92 版本（2025 年 6 月）的动态字体系统

v1.92 引入了更新的动态字体系统。它要求后端支持 `ImGuiBackendFlags_HasTextures` 特性：

- 使用图标、亚洲语言及非英语语言的用户**无需预先构建所有字形**。节省加载时间与内存，同时减少缺失字形的问题。**不再需要指定字形范围。**
- 可随时使用 `PushFont(NULL, new_size)` 改变字体大小
- 打包自定义矩形更加便捷，像素可立即写入
- 以往对字体的任何更新都需要后端特定调用来重新上传纹理，且这些调用无法在不同后端间移植。现在可以缩放字体等操作，**无需进行后端特定调用**
- 可将自定义加载器/后端插入到任意字体源

详见 [#8465](https://github.com/ocornut/imgui/issues/8465)

[返回目录](#目录)

## 如何在应用程序中处理 DPI？

请参阅 [FAQ 条目](https://github.com/ocornut/imgui/blob/master/docs/FAQ.md#q-how-should-i-handle-dpi-in-my-application)

[返回目录](#目录)

## 字体加载指南

**加载默认字体：**
```cpp
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontDefault();
```

**加载 .TTF/.OTF 文件：**
**自 1.92 起，配合更新的后端：无需传递尺寸参数**
```cpp
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontFromFileTTF("font.ttf");
```

 **1.92 之前，或未更新的后端：**
```cpp
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels);
```
若收到 "Could not load font file!" 断言，您的字体文件名可能不正确。请仔细阅读 [关于文件名](#关于文件名)。

**加载多个字体：**
```cpp
// 初始化
ImGuiIO& io = ImGui::GetIO();
ImFont* font1 = io.Fonts->AddFontFromFileTTF("font.ttf", 0.0f);
ImFont* font2 = io.Fonts->AddFontFromFileTTF("anotherfont.otf", 0.0f);
```

在应用程序循环中，选择要使用的字体：
```cpp
ImGui::Text("Hello"); // 使用默认字体（即首个加载的字体）
ImGui::PushFont(font2, 0.0f); // 切换字体，保持当前大小
ImGui::Text("Hello with another font");
ImGui::PopFont();
```

**高级选项**：创建 `ImFontConfig` 结构体并传递给 `AddFont()` 函数（内部会复制该结构）：
```cpp
ImFontConfig config;
config.OversampleH = 1.0f;
ImFont* font = io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels, &config);
```

**将多个字体合并为一个：**
**自 1.92 起，配合更新的后端：无需指定字形范围**
```cpp
// 加载首个字体
ImFont* font = io.Fonts->AddFontDefault();

ImFontConfig config;
config.MergeMode = true;
io.Fonts->AddFontFromFileTTF("DroidSans.ttf", 0.0f, &config);           // 合并到首个字体以添加例如亚洲字符
io.Fonts->AddFontFromFileTTF("fontawesome-webfont.ttf", 0.0f, &config); // 合并到首个字体以添加图标
```

 **1.92 之前，或未更新的后端：**
```cpp
// 加载首个字体
ImFont* font = io.Fonts->AddFontDefault();

// 添加字符范围并合并到前一个字体
// ranges 数组不会被 AddFont* 函数复制，且会延迟使用
// 因此请确保在构建或调用 GetTexDataAsRGBA32() 时该数组仍可用
static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 }; // 不会被 AddFont* 复制，需保持作用域
ImFontConfig config;
config.MergeMode = true;
io.Fonts->AddFontFromFileTTF("DroidSans.ttf", 18.0f, &config, io.Fonts->GetGlyphRangesJapanese()); // 合并日文字符
io.Fonts->AddFontFromFileTTF("fontawesome-webfont.ttf", 18.0f, &config, icons_ranges);             // 合并图标
io.Fonts->Build();
```

**添加第四个参数以仅烘焙特定字体范围：**
**自 1.92 起，配合更新的后端：无需指定字形范围。所有 GetGlyphRangesXXX() 函数已标记为过时。**

 **1.92 之前：**
```cpp
// 基本拉丁文、扩展拉丁文
io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels, nullptr, io.Fonts->GetGlyphRangesDefault());

// Default + 2500 个简体中文常用汉字
io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels, nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

// Default + 平假名、片假名、半角、1946 个常用汉字
io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels, nullptr, io.Fonts->GetGlyphRangesJapanese());
```
请参阅 [使用自定义字形范围](#使用自定义字形范围) 章节以创建您自己的范围。

**加载并使用日文字体示例：**
**自 1.92 起，配合更新的后端：**
```cpp
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontFromFileTTF("NotoSansCJKjp-Medium.otf");
```

 **1.92 之前，或未更新的后端：**
```cpp
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontFromFileTTF("NotoSansCJKjp-Medium.otf", 20.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
ImGui::Text(u8"こんにちは！テスト %d", 123);
if (ImGui::Button(u8"ロード"))
{
    // 执行操作
}
ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
```
![日文示例](https://raw.githubusercontent.com/wiki/ocornut/imgui/web/v160/code_sample_02_jp.png)  
（设置：暗色风格（左），亮色风格（右）/ 字体：NotoSansCJKjp-Medium, 20px / 圆角：5）

[返回目录](#目录)

## 从内存加载字体数据

```cpp
ImFont* font = io.Fonts->AddFontFromMemoryTTF(data, data_size, size_pixels, ...);
```

**重要**：`AddFontFromMemoryTTF()` 默认会将数据缓冲区的所有权转移给字体图集，图集会在析构时尝试释放该缓冲区。  
这是为了避免不必要的复制，但可能不是理想的 API（未来版本将重新设计）。

若您希望保留数据所有权并自行释放，需要清除 `FontDataOwnedByAtlas` 字段：
```cpp
ImFontConfig font_cfg;
font_cfg.FontDataOwnedByAtlas = false;
ImFont* font = io.Fonts->AddFontFromMemoryTTF(data, data_size, size_pixels, &font_cfg);
```

[返回目录](#目录)

## 从源代码嵌入字体数据

编译并使用 [binary_to_compressed_c.cpp](https://github.com/ocornut/imgui/blob/master/misc/fonts/binary_to_compressed_c.cpp) 创建可嵌入源代码的压缩 C 风格数组。

请参阅 [binary_to_compressed_c.cpp](https://github.com/ocornut/imgui/blob/master/misc/fonts/binary_to_compressed_c.cpp) 中的文档了解使用工具的说明。

您可以在演示二进制包中找到 Windows 预编译版本 `binary_to_compressed_c.exe`（参见 [README](https://github.com/ocornut/imgui/blob/master/docs/README.md)）。

该工具可选择输出 Base85 编码以减小源代码大小，但实际二进制中的只读数组会增大 20% 左右。

然后使用以下方式加载字体：
```cpp
ImFont* font = io.Fonts->AddFontFromMemoryCompressedTTF(compressed_data, compressed_data_size, size_pixels, ...);
```
或
```cpp
ImFont* font = io.Fonts->AddFontFromMemoryCompressedBase85TTF(compressed_data_base85, size_pixels, ...);
```

[返回目录](#目录)

## 使用图标字体

使用图标字体（如 [FontAwesome](http://fontawesome.io) 或 [OpenFontIcons](https://github.com/traverseda/OpenFontIcons)）是在 Dear ImGui 应用程序中使用图标的简便实用方法。

常见模式是将图标字体合并到主字体中，这样可以直接在字符串中嵌入图标，无需频繁切换字体。

要从 C++ 代码中引用图标的 UTF-8 码点，可使用 Juliette Foucaut 创建的头文件：https://github.com/juliettef/IconFontCppHeaders

这样您就可以使用 `ICON_FA_SEARCH` 作为字符串，渲染为"搜索"图标。

**自 1.92 起，配合更新的后端：无需指定字形范围。可省略此参数。**

**示例设置：**
```cpp
// 将图标合并到默认工具字体
#include "IconsFontAwesome.h"
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontDefault();

ImFontConfig config;
config.MergeMode = true;
config.GlyphMinAdvanceX = 13.0f; // 若希望图标等宽可使用此设置
io.Fonts->AddFontFromFileTTF("fonts/fontawesome-webfont.ttf", 13.0f, &config);
```

 **1.92 之前：**
```cpp
// 将图标合并到默认工具字体
#include "IconsFontAwesome.h"
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontDefault();

ImFontConfig config;
config.MergeMode = true;
config.GlyphMinAdvanceX = 13.0f; // 若希望图标等宽可使用此设置
static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
io.Fonts->AddFontFromFileTTF("fonts/fontawesome-webfont.ttf", 13.0f, &config, icon_ranges);
```

**示例用法：**
```cpp
// 用法示例
ImGui::Text("%s among %d items", ICON_FA_SEARCH, count);
ImGui::Button(ICON_FA_SEARCH " Search");
// C 字符串字面量可在编译时连接，例如 "hello" " world" 会变成 "AB"
// ICON_FA_SEARCH 定义为字符串字面量，因此等同于 "A" "B" 变成 "AB"
```

**等宽图标**  
若要使图标看起来更等宽并便于对齐，加载图标字体时可设置 `ImFontConfig::GlyphMinAdvanceX` 值。  
若您设置了 `GlyphMinAdvanceX`，则需要向 `AddFontXXX()` 调用传递 `font_size`，因为 MinAdvanceX 值将针对给定尺寸指定，否则会被缩放。

**截图**  
使用图标的示例应用程序（"Avoyd", https://www.avoyd.com）：  
![Avoyd 应用截图](https://user-images.githubusercontent.com/8225057/81696852-c15d9e80-9464-11ea-9cab-2a4d4fc84396.jpg)

[返回目录](#目录)

## 排除重叠范围

**自 1.92 起，配合更新的后端：字形范围被忽略：加载字形时，按顺序查询合并列表中的输入字体。首个包含该字形的字体会加载它。**

**合并多个字体时，可能出现不希望的重叠范围。可使用 `ImFontConfig::GlyphExcludeRanges[]` 指定在给定输入中要忽略的范围。**

```cpp
// 添加字体源 1 但忽略 ICON_MIN_FA..ICON_MAX_FA 范围
static ImWchar exclude_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
ImFontConfig cfg1;
cfg1.GlyphExcludeRanges = exclude_ranges;
io.Fonts->AddFontFromFileTTF("segoeui.ttf", 0.0f, &cfg1);

// 添加字体源 2，期望使用上述范围
ImFontConfig cfg2;
cfg2.MergeMode = true;
io.Fonts->AddFontFromFileTTF("FontAwesome4.ttf", 0.0f, &cfg2);
```

另一个（略显荒谬的）示例：
```cpp
// 从首个字体中移除 'A'-'Z'
static ImWchar exclude_ranges[] = { 'A', 'Z', 0 };
ImFontConfig cfg1;
cfg1.GlyphExcludeRanges = exclude_ranges;
io.Fonts->AddFontFromFileTTF("segoeui.ttf", 0.0f, &cfg1);

// 加载另一字体填补空缺
ImFontConfig cfg2;
cfg2.MergeMode = true;
io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 0.0f, &cfg2);
```
![排除范围示例](https://github.com/user-attachments/assets/f3d751d3-1aee-4698-bd9b-f511902f56bb)

您可使用 `Metrics/Debugger->Fonts->Font->Input Glyphs Overlap Detection Tool` 查看在多个字体源中可用的字形列表。这有助于理解哪个字体输入提供了哪个字形。

[返回目录](#目录)

## 使用 FreeType 光栅化器 (imgui_freetype)

Dear ImGui 使用 [stb_truetype.h](https://github.com/nothings/stb/) 光栅化字体（可选过采样）。该技术及其实现在小尺寸渲染字体时并不理想，字体可能略显模糊或难以阅读。

不过，您可以使用 [misc/freetype/](https://github.com/ocornut/imgui/tree/master/misc/freetype) 文件夹中的 `imgui_freetype.cpp`。编译此文件并在 `imconfig.h` 或构建系统中添加 `#define IMGUI_ENABLE_FREETYPE` 以自动激活它。

FreeType 支持自动微调（auto-hinting），往往能提高小字体的可读性。尤其在较低分辨率下效果显著。

请阅读 [misc/freetype/](https://github.com/ocornut/imgui/tree/master/misc/freetype) 文件夹中的文档。

正确的 sRGB 空间混合将对字体渲染质量产生重要影响。

[返回目录](#目录)

## 使用彩色字形/表情符号

imgui_freetype 配合 FreeType 2.10+ 支持彩色表情符号渲染。

您需要使用 `ImGuiFreeTypeBuilderFlags_LoadColor` 标志加载字体。

表情符号通常编码在 Unicode 高层（字符码 >0x10000），需要将 dear imgui 编译为 `IMGUI_USE_WCHAR32`。

目前 FreeType 尚未支持所有类型的彩色字体。

文本渲染器不支持带状态的 Unicode 特性（如肤色修饰符）。

![彩色表情符号示例](https://user-images.githubusercontent.com/8225057/106171241-9dc4ba80-6191-11eb-8a69-ca1467b206d1.png)

```cpp
io.Fonts->AddFontFromFileTTF("../../../imgui_dev/data/fonts/NotoSans-Regular.ttf", 16.0f);
static ImWchar ranges[] = { 0x1, 0x1FFFF, 0 };
static ImFontConfig cfg;
cfg.MergeMode = true;
cfg.FontLoaderFlags |= ImGuiFreeTypeLoaderFlags_LoadColor;
io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\seguiemj.ttf", 16.0f, &cfg);
```

[返回目录](#目录)

## 使用自定义字形范围

**自 1.92 起，配合更新的后端：无需指定字形范围。因此此功能已不太实用。**

 **1.92 之前：**  
可使用 `ImFontGlyphRangesBuilder` 辅助工具基于文本输入创建字形范围。例如：对于脚本已知的游戏，可将整个脚本输入其中，仅构建游戏所需的字符。

```cpp
ImVector<ImWchar> ranges;
ImFontGlyphRangesBuilder builder;
builder.AddText("Hello world");                        // 添加字符串（此处 "Hello world" 包含 7 个唯一字符）
builder.AddChar(0x7262);                               // 添加特定字符
builder.AddRanges(io.Fonts->GetGlyphRangesJapanese()); // 添加默认范围之一
builder.BuildRanges(&ranges);                          // 构建最终结果（有序范围，包含所有提交的唯一字符）

io.Fonts->AddFontFromFileTTF("myfontfile.ttf", size_in_pixels, nullptr, ranges.Data);
io.Fonts->Build();                                     // 构建图集时 'ranges' 必须仍在作用域内且未被删除
```

[返回目录](#目录)

## 使用自定义彩色图标

**自 1.92 起，配合更新的后端：此系统已彻底重构。**  
**TL;DR**：新系统推荐您创建自定义 `ImFontLoader` 并通过它注册字体。  
`AddCustomRectFontGlyph()` 已被废弃，因为其 API 在配合可调整大小的字体时意义不大。  
如有疑问请在 [#8466](https://github.com/ocornut/imgui/issues/8466) 提问。

 **1.92 之前：**  
作为使用 imgui_freetype 配合 `ImGuiFreeTypeBuilderFlags_LoadColor` 渲染彩色字形的替代方案，您可在纹理图集中分配自己的空间并自行写入数据。（此为 BETA API，仅建议熟悉 dear imgui 及渲染后端的用户使用）

您可使用 `ImFontAtlas::AddCustomRect()` 和 `ImFontAtlas::AddCustomRectFontGlyph()` API 注册将被打包到字体图集纹理中的矩形。在构建图集前注册它们，然后调用 `Build()`。

之后可使用 `ImFontAtlas::GetCustomRect(int)` 查询矩形在纹理中的位置/大小，并将任意图形数据复制/绘制到这些矩形中。

此 API 为 beta 版，因为它可能为支持多 DPI（多个显示器上具有不同 DPI 缩放的多个视口）而改变。

伪代码：
```cpp
// 添加字体，然后为该字体的 'a' 和 'b' 字形注册两个 13x13 自定义矩形
ImFont* font = io.Fonts->AddFontDefault();
int rect_ids[2];
rect_ids[0] = io.Fonts->AddCustomRectFontGlyph(font, 'a', 13, 13, 13+1);
rect_ids[1] = io.Fonts->AddCustomRectFontGlyph(font, 'b', 13, 13, 13+1);

// 构建图集
io.Fonts->Build();

// 以 RGBA 格式获取纹理
unsigned char* tex_pixels = nullptr;
int tex_width, tex_height;
io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_width, &tex_height);

for (int rect_n = 0; rect_n < IM_ARRAYSIZE(rect_ids); rect_n++)
    if (const ImTextureRect* rect = io.Fonts->GetCustomRect(rect_ids[rect_n]))
    {
        // 用红色像素填充自定义矩形（实际中您会在此绘制/复制位图数据！）
        for (int y = 0; y < rect->Height; y++)
        {
            ImU32* p = (ImU32*)tex_pixels + (rect->Y + y) * tex_width + (rect->X);
            for (int x = rect->Width; x > 0; x--)
                *p++ = IM_COL32(255, 0, 0, 255);
        }
    }
```

[返回目录](#目录)

## 关于文件名

请注意，许多 C/C++ 新手因提供的文件名错误而无法加载文件，这通常源于对当前目录的错误假设。

需注意两点：

**(1) 在 C/C++ 及大多数编程语言中，若要在字符串字面量中使用反斜杠 `\`，需写成双反斜杠 `\\`。恰好 Windows 使用反斜杠作为路径分隔符，因此需特别注意。**
```cpp
io.Fonts->AddFontFromFileTTF("MyFiles\MyImage01.jpg", ...);   // 错误！！
io.Fonts->AddFontFromFileTTF("MyFiles\\MyImage01.jpg", ...);  // 正确
```
在某些情况下，Windows 下也可使用 `/` 作为路径分隔符。

**(2) 确保 IDE/调试器设置从正确的工作（当前）目录启动可执行文件。**  
在 Visual Studio 中，可在项目 `属性 > 常规 > 调试 > 工作目录` 中更改工作目录。人们常假设执行会从项目根文件夹开始，但实际上通常从对象或可执行文件存储的文件夹开始。
```cpp
io.Fonts->AddFontFromFileTTF("MyImage01.jpg", ...);       // 相对文件名取决于程序运行时的工作目录！
io.Fonts->AddFontFromFileTTF("../MyImage01.jpg", ...);    // 从工作目录的父文件夹加载
```

[返回目录](#目录)

## 关于 UTF-8 编码

对于非 ASCII 字符显示，常见用户问题是未正确传递 UTF-8 编码的字符串。

**(1) 我们提供 `ImGui::DebugTextEncoding(const char* text)` 函数，可用于验证 UTF-8 字符串内容。**  
这是确认编码正确的便捷方式。
```cpp
ImGui::SeparatorText("正确");
ImGui::DebugTextEncoding(u8"こんにちは");

ImGui::SeparatorText("错误");
ImGui::DebugTextEncoding("こんにちは");
```
![UTF-8 编码验证示例](https://github.com/ocornut/imgui/assets/8225057/61c1696a-9a94-46c5-9627-cf91211111f0)

您也可在 `Metrics/Debuggers->Tools->UTF-8 Encoding viewer` 中找到此工具以从剪贴板粘贴内容，但这不会验证编译器完成的 UTF-8 编码。

**(2) 如何进行 UTF-8 编码：**  
也有编译器特定方式默认强制使用 UTF-8 编码：
- Visual Studio 编译器：命令行标志 `/utf-8`
- Visual Studio 编译器：代码中使用 `#pragma execution_character_set("utf-8")`
- 自 2023 年 5 月起，我们已将所有示例的 Visual Studio 项目更改为使用 `/utf-8`（[参见提交](https://github.com/ocornut/imgui/commit/513af1efc9080857bbd10000d98f98f2a0c96803)）

或自 C++11 起，可使用 `u8"my text"` 语法将字符串字面量编码为 UTF-8，例如：
```cpp
ImGui::Text(u8"hello");
ImGui::Text(u8"こんにちは");   // 此字符串始终以 UTF-8 编码
ImGui::Text("こんにちは");     // 此字符串的编码取决于编译器设置/标志，可能不正确
```

自 C++20 起，C++ 委员会决定将 `u8""` 语法的返回类型从 `const char*` 改为新类型 `const char8_t*`，后者无法隐式转换为 `const char*`。

因此 C++20 中使用 `u8""` 略显繁琐：
```cpp
ImGui::Text((const char*)u8"こんにちは");
```

不过，您可使用编译器选项完全禁用此行为：
- MSVC：`/Zc:char8_t-`（[文档](https://learn.microsoft.com/en-us/cpp/build/reference/zc-char8-tview=msvc-170)）
- Clang 和 GCC：`-fno-char8_t`（[文档](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1423r3.html)）

[返回目录](#目录)

## 调试工具

**Metrics/Debugger->Fonts**  
您可使用 `Metrics/Debugger` 窗口（在 `Demo>Tools` 中可用）浏览字体并排查问题。也可在 `Demo->Tools->Style Editor->Fonts` 中访问。相同信息也可在 Style Editor 的 Fonts 部分找到。

![字体调试工具](https://user-images.githubusercontent.com/8225057/135429892-0e41ef8d-33c5-4991-bcf6-f997a0bcfd6b.png)

**UTF-8 编码查看器**  
您可使用 `Metrics/Debugger` 中的 `UTF-8 Encoding viewer` 验证 UTF-8 字符串内容。从 C/C++ 代码中，可调用 `ImGui::DebugTextEncoding("my string");` 函数验证 UTF-8 编码是否正确。

![UTF-8 编码查看器](https://user-images.githubusercontent.com/8225057/166505963-8a0d7899-8ee8-4558-abb2-1ae523dc02f9.png)

[返回目录](#目录)

## 仓库中包含字体的版权/许可信息

`misc/fonts/` 文件夹中提供了一些字体文件：

- **Roboto-Medium.ttf**，作者：Christian Robertson  
  Apache License 2.0  
  https://fonts.google.com/specimen/Roboto

- **Cousine-Regular.ttf**，作者：Steve Matteson  
  Digitized data copyright (c) 2010 Google Corporation.  
  Licensed under the SIL Open Font License, Version 1.1  
  https://fonts.google.com/specimen/Cousine

- **DroidSans.ttf**，作者：Steve Matteson  
  Apache License 2.0  
  https://www.fontsquirrel.com/fonts/droid-sans

- **ProggyClean.ttf**，作者：Tristan Grimmer  
  MIT License  
  （推荐加载设置：Size = 13.0, GlyphOffset.y = +1）  
  http://www.proggyfonts.net/

- **ProggyTiny.ttf**，作者：Tristan Grimmer  
  MIT License  
  （推荐加载设置：Size = 10.0, GlyphOffset.y = +1）  
  http://www.proggyfonts.net/

- **Karla-Regular.ttf**，作者：Jonathan Pinhorn  
  SIL OPEN FONT LICENSE Version 1.1

[返回目录](#目录)

## 字体资源链接

**图标字体**
- C/C++ 图标字体头文件（源代码字符串字面量中使用的码点 #define）  
  https://github.com/juliettef/IconFontCppHeaders
- FontAwesome  
  https://fortawesome.github.io/Font-Awesome
- OpenFontIcons  
  https://github.com/traverseda/OpenFontIcons
- Google Icon Fonts  
  https://design.google.com/icons/
- Kenney Icon Font（游戏控制器图标）  
  https://github.com/nicodinh/kenney-icon-font
- IcoMoon - 自定义图标字体构建器  
  https://icomoon.io/app

**常规字体**
- Google Noto Fonts（全球语言）  
  https://www.google.com/get/noto/
- Open Sans Fonts  
  https://fonts.google.com/specimen/Open+Sans
- （日文）M+ fonts by Coji Morishita  
  http://mplus-fonts.sourceforge.jp/mplus-outline-fonts/index-en.html

**等宽字体**
![等宽字体示例](https://github.com/user-attachments/assets/c9702534-4877-41c9-ae0d-252933c26ced)

*像素完美：*
- Proggy Fonts，作者：Tristan Grimmer  
  http://www.proggyfonts.net 或 http://upperboundsinteractive.com/fonts.php
- Sweet16, Sweet16 Mono，作者：Martin Sedlak（Latin + Supplemental + Extended A）  
  https://github.com/kmar/Sweet16Font（也包含可直接在 dear imgui 中使用的 .inl 文件）

*常规：*
- ProggyVector（若希望旧版 Dear ImGui 字体可缩放）  
  https://github.com/bluescan/proggyfonts
- Google Noto Mono Fonts  
  https://www.google.com/get/noto/
- 用于美化源代码的字体  
  https://github.com/chrissimpkins/codeface
- 编程字体比较  
  http://s9w.github.io/font_compare/
- Inconsolata  
  http://www.levien.com/type/myfonts/inconsolata.html
- Adobe Source Code Pro：面向 UI 与编程环境的等宽字体系列  
  https://github.com/adobe-fonts/source-code-pro
- 等宽/固定宽度程序员字体  
  http://www.lowing.org/fonts/
- 或使用 Windows 自带的 Arial Unicode 等 Unicode 字体以获得完整字符覆盖（许可信息不确定）

[返回目录](#目录)