# Dear ImGui: Font Usage Guide

This document is an English translation of the official ImGui documentation

(You may also browse this document at https://github.com/ocornut/imgui/blob/master/docs/FONTS.md, or read this file using any Markdown viewer)

Dear ImGui's `imgui.cpp` code embeds a copy of [ProggyClean.ttf](http://proggyfonts.net) (author: Tristan Grimmer), a 13-pixel-height, pixel-perfect default font. We embed it in the source code so you can use Dear ImGui without filesystem access. The ProggyClean font cannot be smoothly scaled, so it is recommended to load your own font files in applications that pursue aesthetic appearance and need to support multiple resolutions.

You can also load external .TTF/.OTF font files. In the [misc/fonts/](https://github.com/ocornut/imgui/tree/master/misc/fonts) folder, we provide some recommended fonts as a convenient reference.

Also read the FAQ: https://www.dearimgui.com/faq (which includes a font-related section!)

## Table of Contents
- [Troubleshooting](#troubleshooting)
- [New! Dynamic Font System in 1.92 (June 2025)](#new-dynamic-font-system-in-192-june-2025)
- [How to handle DPI in your application?](#how-to-handle-dpi-in-your-application)
- [Font Loading Guide](#font-loading-guide)
- [Loading Font Data from Memory](#loading-font-data-from-memory)
- [Embedding Font Data in Source Code](#embedding-font-data-in-source-code)
- [Using Icon Fonts](#using-icon-fonts)
- [Excluding Overlapping Ranges](#excluding-overlapping-ranges)
- [Using FreeType Rasterizer (imgui_freetype)](#using-freetype-rasterizer-imgui_freetype)
- [Using Color Glyphs/Emojis](#using-color-glyphsemojis)
- [Using Custom Glyph Ranges](#using-custom-glyph-ranges)
- [Using Custom Color Icons](#using-custom-color-icons)
- [About File Names](#about-file-names)
- [About UTF-8 Encoding](#about-utf-8-encoding)
- [Debugging Tools](#debugging-tools)
- [Copyright/License Information for Fonts Included in the Repository](#copyrightlicense-information-for-fonts-included-in-the-repository)
- [Font Resource Links](#font-resource-links)

## Troubleshooting

The vast majority of font and text-related issues stem from these 4 causes:

**(1) Invalid file names due to use of `\` or unexpected working directory**
See [About File Names](#about-file-names). The `AddFontXXX()` functions should trigger assertions when file names are incorrect.

**(2) Invalid UTF-8 encoding of non-ASCII strings**
See [About UTF-8 Encoding](#about-utf-8-encoding). Use the encoding viewer to confirm that the encoding of string literals in your source code is correct.

**(3) Missing glyph ranges**
**Since 1.92, with updated backends: specifying glyph ranges is no longer required.**
**Before 1.92**: To use non-ASCII characters, you need to explicitly specify glyph ranges when loading fonts. See [Font Loading Guide](#font-loading-guide). Use the [Debugging Tools](#debugging-tools) to confirm loaded fonts and glyph ranges.

This was a previous limitation of Dear ImGui (lifted in version 1.92): when loading fonts, you needed to specify which characters' glyphs to load. All loaded font glyphs are pre-rendered into a **single texture atlas**. Calling any of `io.Fonts->GetTexDataAsAlpha8()`, `io.Fonts->GetTexDataAsRGBA32()`, or `io.Fonts->Build()` will build the atlas. This operation is typically called by the rendering backend, for example `ImGui_ImplDX11_NewFrame()` calls it. **If using custom glyph ranges, ensure the array remains persistent and available during `GetTexDataAsAlpha8()/GetTexDataAsRGBA32()/Build()` calls.**

**(4) Font atlas texture fails to upload to GPU**
**Since 1.92, with updated backends: the atlas is built incrementally and can dynamically resize, reducing the likelihood of this issue**
This problem is typically a derivative of point 3. If you have many glyphs or multiple fonts, the texture may exceed your graphics API's limits. The typical manifestation of a failed texture upload is all glyphs or content displaying as blank white rectangles. Note that some graphics drivers have texture size limitations. If you are developing a PC application, be aware that users may have hardware with lower limits than your development environment.

![Example of texture upload failure](https://github.com/user-attachments/assets/68b50fb5-8b9d-4c38-baec-6ac384f06d26)

**Solutions:**
- Reduce oversampling rate, e.g. `font_config.OversampleH = 1`, can halve the texture size (but loses quality)
  - Note: while `OversampleH = 2` is visually very close to 3 in most cases, `OversampleH = 1` results in noticeable quality loss. Read [here](https://github.com/nothings/stb/blob/master/tests/oversample) about oversampling
- Calculate glyph ranges from source language localization data to reduce ranges
  - You can use `ImFontGlyphRangesBuilder` for this purpose, and rebuild the atlas between frames when new characters are needed. This is the most effective optimization!
- Set `io.Fonts.Flags |= ImFontAtlasFlags_NoPowerOfTwoHeight;` to disable rounding up texture height to powers of two

[Back to TOC](#table-of-contents)

## New! Dynamic Font System in 1.92 (June 2025)

v1.92 introduces an updated dynamic font system. It requires backends to support the `ImGuiBackendFlags_HasTextures` feature:

- Users using icons, Asian languages, and non-English languages **no longer need to pre-build all glyphs**. Saves loading time and memory while reducing missing glyph issues. **Specifying glyph ranges is no longer required.**
- Font size can be changed at any time using `PushFont(NULL, new_size)`
- Packing custom rectangles is more convenient, with pixels writable immediately
- Previously, any font updates required backend-specific calls to re-upload textures, and these calls were not portable across backends. Now you can scale fonts, etc., **without backend-specific calls**
- Custom loaders/backends can be plugged into any font source

See [#8465](https://github.com/ocornut/imgui/issues/8465) for details

[Back to TOC](#table-of-contents)

## How to handle DPI in your application?

See the [FAQ entry](https://github.com/ocornut/imgui/blob/master/docs/FAQ.md#q-how-should-i-handle-dpi-in-my-application)

[Back to TOC](#table-of-contents)

## Font Loading Guide

**Loading default font:**
```cpp
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontDefault();
```

**Loading .TTF/.OTF files:**
**Since 1.92, with updated backends: no need to pass a size parameter**
```cpp
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontFromFileTTF("font.ttf");
```

 **Before 1.92, or with unupdated backends:**
```cpp
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels);
```
If you receive a "Could not load font file!" assertion, your font file name may be incorrect. Please carefully read [About File Names](#about-file-names).

**Loading multiple fonts:**
```cpp
// Initialization
ImGuiIO& io = ImGui::GetIO();
ImFont* font1 = io.Fonts->AddFontFromFileTTF("font.ttf", 0.0f);
ImFont* font2 = io.Fonts->AddFontFromFileTTF("anotherfont.otf", 0.0f);
```

In the application loop, select which font to use:
```cpp
ImGui::Text("Hello"); // Use the default font (i.e. the first loaded font)
ImGui::PushFont(font2, 0.0f); // Switch font, keep current size
ImGui::Text("Hello with another font");
ImGui::PopFont();
```

**Advanced options**: Create an `ImFontConfig` struct and pass it to the `AddFont()` function (the struct is copied internally):
```cpp
ImFontConfig config;
config.OversampleH = 1.0f;
ImFont* font = io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels, &config);
```

**Merging multiple fonts into one:**
**Since 1.92, with updated backends: no need to specify glyph ranges**
```cpp
// Load the first font
ImFont* font = io.Fonts->AddFontDefault();

ImFontConfig config;
config.MergeMode = true;
io.Fonts->AddFontFromFileTTF("DroidSans.ttf", 0.0f, &config);           // Merge into first font to add e.g. Asian characters
io.Fonts->AddFontFromFileTTF("fontawesome-webfont.ttf", 0.0f, &config); // Merge into first font to add icons
```

 **Before 1.92, or with unupdated backends:**
```cpp
// Load the first font
ImFont* font = io.Fonts->AddFontDefault();

// Add character ranges and merge into the previous font
// The ranges array is not copied by the AddFont* functions and is used lazily
// So ensure the array is still available when building or calling GetTexDataAsRGBA32()
static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 }; // Not copied by AddFont*, needs to remain in scope
ImFontConfig config;
config.MergeMode = true;
io.Fonts->AddFontFromFileTTF("DroidSans.ttf", 18.0f, &config, io.Fonts->GetGlyphRangesJapanese()); // Merge Japanese characters
io.Fonts->AddFontFromFileTTF("fontawesome-webfont.ttf", 18.0f, &config, icons_ranges);             // Merge icons
io.Fonts->Build();
```

**Add a fourth parameter to bake only specific font ranges:**
**Since 1.92, with updated backends: no need to specify glyph ranges. All GetGlyphRangesXXX() functions are marked as deprecated.**

 **Before 1.92:**
```cpp
// Basic Latin, Extended Latin
io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels, nullptr, io.Fonts->GetGlyphRangesDefault());

// Default + 2500 common Simplified Chinese characters
io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels, nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

// Default + Hiragana, Katakana, Half-width, 1946 common Kanji
io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels, nullptr, io.Fonts->GetGlyphRangesJapanese());
```
See the [Using Custom Glyph Ranges](#using-custom-glyph-ranges) section to create your own ranges.

**Example of loading and using a Japanese font:**
**Since 1.92, with updated backends:**
```cpp
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontFromFileTTF("NotoSansCJKjp-Medium.otf");
```

 **Before 1.92, or with unupdated backends:**
```cpp
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontFromFileTTF("NotoSansCJKjp-Medium.otf", 20.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
ImGui::Text(u8"こんにちは！テスト %d", 123);
if (ImGui::Button(u8"ロード"))
{
    // perform action
}
ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
```
![Japanese example](https://raw.githubusercontent.com/wiki/ocornut/imgui/web/v160/code_sample_02_jp.png)
(Settings: Dark style (left), Light style (right) / Font: NotoSansCJKjp-Medium, 20px / Rounding: 5)

[Back to TOC](#table-of-contents)

## Loading Font Data from Memory

```cpp
ImFont* font = io.Fonts->AddFontFromMemoryTTF(data, data_size, size_pixels, ...);
```

**Important**: `AddFontFromMemoryTTF()` by default transfers ownership of the data buffer to the font atlas, which will attempt to free the buffer on destruction.
This is to avoid unnecessary copying, but may not be an ideal API (future versions will redesign it).

If you wish to retain ownership of the data and free it yourself, you need to clear the `FontDataOwnedByAtlas` field:
```cpp
ImFontConfig font_cfg;
font_cfg.FontDataOwnedByAtlas = false;
ImFont* font = io.Fonts->AddFontFromMemoryTTF(data, data_size, size_pixels, &font_cfg);
```

[Back to TOC](#table-of-contents)

## Embedding Font Data in Source Code

Compile and use [binary_to_compressed_c.cpp](https://github.com/ocornut/imgui/blob/master/misc/fonts/binary_to_compressed_c.cpp) to create compressed C-style arrays that can be embedded in source code.

See the documentation in [binary_to_compressed_c.cpp](https://github.com/ocornut/imgui/blob/master/misc/fonts/binary_to_compressed_c.cpp) for instructions on using the tool.

You can find a pre-compiled Windows version `binary_to_compressed_c.exe` in the demo binary package (see [README](https://github.com/ocornut/imgui/blob/master/docs/README.md)).

The tool can optionally output Base85 encoding to reduce source code size, but the read-only array in the actual binary will increase by about 20%.

Then load the font using:
```cpp
ImFont* font = io.Fonts->AddFontFromMemoryCompressedTTF(compressed_data, compressed_data_size, size_pixels, ...);
```
or
```cpp
ImFont* font = io.Fonts->AddFontFromMemoryCompressedBase85TTF(compressed_data_base85, size_pixels, ...);
```

[Back to TOC](#table-of-contents)

## Using Icon Fonts

Using icon fonts (such as [FontAwesome](http://fontawesome.io) or [OpenFontIcons](https://github.com/traverseda/OpenFontIcons)) is a simple and practical way to use icons in Dear ImGui applications.

A common pattern is to merge the icon font into the main font, allowing icons to be embedded directly in strings without frequently switching fonts.

To reference icon UTF-8 codepoints from C++ code, you can use the header files created by Juliette Foucaut: https://github.com/juliettef/IconFontCppHeaders

This allows you to use `ICON_FA_SEARCH` as a string, rendered as a "search" icon.

**Since 1.92, with updated backends: no need to specify glyph ranges. This parameter can be omitted.**

**Example setup:**
```cpp
// Merge icons into the default tool font
#include "IconsFontAwesome.h"
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontDefault();

ImFontConfig config;
config.MergeMode = true;
config.GlyphMinAdvanceX = 13.0f; // Use this if you want icons to be monospaced
io.Fonts->AddFontFromFileTTF("fonts/fontawesome-webfont.ttf", 13.0f, &config);
```

 **Before 1.92:**
```cpp
// Merge icons into the default tool font
#include "IconsFontAwesome.h"
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontDefault();

ImFontConfig config;
config.MergeMode = true;
config.GlyphMinAdvanceX = 13.0f; // Use this if you want icons to be monospaced
static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
io.Fonts->AddFontFromFileTTF("fonts/fontawesome-webfont.ttf", 13.0f, &config, icon_ranges);
```

**Example usage:**
```cpp
// Usage examples
ImGui::Text("%s among %d items", ICON_FA_SEARCH, count);
ImGui::Button(ICON_FA_SEARCH " Search");
// C string literals can be concatenated at compile time, e.g. "hello" " world" becomes "hello world"
// ICON_FA_SEARCH is defined as a string literal, so this is equivalent to "A" "B" becoming "AB"
```

**Monospaced Icons**
To make icons appear more monospaced and easier to align, you can set the `ImFontConfig::GlyphMinAdvanceX` value when loading the icon font.
If you set `GlyphMinAdvanceX`, you need to pass `font_size` to the `AddFontXXX()` call, because the MinAdvanceX value will be specified for a given size, otherwise it will be scaled.

**Screenshot**
Example application using icons ("Avoyd", https://www.avoyd.com):
![Avoyd application screenshot](https://user-images.githubusercontent.com/8225057/81696852-c15d9e80-9464-11ea-9cab-2a4d4fc84396.jpg)

[Back to TOC](#table-of-contents)

## Excluding Overlapping Ranges

**Since 1.92, with updated backends: glyph ranges are ignored: when loading glyphs, the input fonts in the merge list are queried in order. The first font that contains the glyph will load it.**

**When merging multiple fonts, there may be unwanted overlapping ranges. You can use `ImFontConfig::GlyphExcludeRanges[]` to specify ranges to ignore in a given input.**

```cpp
// Add font source 1 but ignore the ICON_MIN_FA..ICON_MAX_FA range
static ImWchar exclude_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
ImFontConfig cfg1;
cfg1.GlyphExcludeRanges = exclude_ranges;
io.Fonts->AddFontFromFileTTF("segoeui.ttf", 0.0f, &cfg1);

// Add font source 2, expecting to use the above range
ImFontConfig cfg2;
cfg2.MergeMode = true;
io.Fonts->AddFontFromFileTTF("FontAwesome4.ttf", 0.0f, &cfg2);
```

Another (slightly absurd) example:
```cpp
// Remove 'A'-'Z' from the first font
static ImWchar exclude_ranges[] = { 'A', 'Z', 0 };
ImFontConfig cfg1;
cfg1.GlyphExcludeRanges = exclude_ranges;
io.Fonts->AddFontFromFileTTF("segoeui.ttf", 0.0f, &cfg1);

// Load another font to fill the gap
ImFontConfig cfg2;
cfg2.MergeMode = true;
io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 0.0f, &cfg2);
```
![Exclude ranges example](https://github.com/user-attachments/assets/f3d751d3-1aee-4698-bd9b-f511902f56bb)

You can use `Metrics/Debugger->Fonts->Font->Input Glyphs Overlap Detection Tool` to view the list of glyphs available in multiple font sources. This helps understand which font input provides which glyph.

[Back to TOC](#table-of-contents)

## Using FreeType Rasterizer (imgui_freetype)

Dear ImGui uses [stb_truetype.h](https://github.com/nothings/stb/) to rasterize fonts (with optional oversampling). This technique and its implementation are not ideal for rendering fonts at small sizes, where fonts may appear slightly blurry or hard to read.

However, you can use `imgui_freetype.cpp` from the [misc/freetype/](https://github.com/ocornut/imgui/tree/master/misc/freetype) folder. Compile this file and add `#define IMGUI_ENABLE_FREETYPE` in `imconfig.h` or your build system to automatically activate it.

FreeType supports auto-hinting, which tends to improve readability at small font sizes. This is especially noticeable at lower resolutions.

Read the documentation in the [misc/freetype/](https://github.com/ocornut/imgui/tree/master/misc/freetype) folder.

Proper sRGB space blending will have a significant impact on font rendering quality.

[Back to TOC](#table-of-contents)

## Using Color Glyphs/Emojis

imgui_freetype with FreeType 2.10+ supports rendering color emojis.

You need to load the font using the `ImGuiFreeTypeBuilderFlags_LoadColor` flag.

Emojis are typically encoded in the Unicode higher planes (codepoints >0x10000), requiring dear imgui to be compiled with `IMGUI_USE_WCHAR32`.

Currently, FreeType does not support all types of color fonts.

The text renderer does not support stateful Unicode features (such as skin tone modifiers).

![Color emoji example](https://user-images.githubusercontent.com/8225057/106171241-9dc4ba80-6191-11eb-8a69-ca1467b206d1.png)

```cpp
io.Fonts->AddFontFromFileTTF("../../../imgui_dev/data/fonts/NotoSans-Regular.ttf", 16.0f);
static ImWchar ranges[] = { 0x1, 0x1FFFF, 0 };
static ImFontConfig cfg;
cfg.MergeMode = true;
cfg.FontLoaderFlags |= ImGuiFreeTypeLoaderFlags_LoadColor;
io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\seguiemj.ttf", 16.0f, &cfg);
```

[Back to TOC](#table-of-contents)

## Using Custom Glyph Ranges

**Since 1.92, with updated backends: no need to specify glyph ranges. Therefore this feature is less practical.**

 **Before 1.92:**
You can use the `ImFontGlyphRangesBuilder` helper to create glyph ranges based on text input. For example: for a game with a known script, you could feed the entire script into it and only build the characters needed for the game.

```cpp
ImVector<ImWchar> ranges;
ImFontGlyphRangesBuilder builder;
builder.AddText("Hello world");                        // Add a string (here "Hello world" contains 7 unique characters)
builder.AddChar(0x7262);                               // Add a specific character
builder.AddRanges(io.Fonts->GetGlyphRangesJapanese()); // Add one of the default ranges
builder.BuildRanges(&ranges);                          // Build the final result (ordered ranges, containing all submitted unique characters)

io.Fonts->AddFontFromFileTTF("myfontfile.ttf", size_in_pixels, nullptr, ranges.Data);
io.Fonts->Build();                                     // When building the atlas, 'ranges' must still be in scope and not deleted
```

[Back to TOC](#table-of-contents)

## Using Custom Color Icons

**Since 1.92, with updated backends: this system has been completely redesigned.**
**TL;DR**: The new system recommends you create a custom `ImFontLoader` and register fonts through it.
`AddCustomRectFontGlyph()` has been deprecated, as its API makes little sense with resizable fonts.
If you have questions, please ask at [#8466](https://github.com/ocornut/imgui/issues/8466).

 **Before 1.92:**
As an alternative to using imgui_freetype with `ImGuiFreeTypeBuilderFlags_LoadColor` for rendering color glyphs, you can allocate your own space in the texture atlas and write data to it yourself. (This is a BETA API and is only recommended for users familiar with dear imgui and rendering backends)

You can use the `ImFontAtlas::AddCustomRect()` and `ImFontAtlas::AddCustomRectFontGlyph()` APIs to register rectangles that will be packed into the font atlas texture. Register them before building the atlas, then call `Build()`.

Afterwards, you can use `ImFontAtlas::GetCustomRect(int)` to query the position/size of the rectangle in the texture, and copy/draw arbitrary graphics data into those rectangles.

This API is beta because it may change to support multiple DPIs (multiple viewports with different DPI scaling on multiple monitors).

Pseudocode:
```cpp
// Add a font, then register two 13x13 custom rectangles for the font's 'a' and 'b' glyphs
ImFont* font = io.Fonts->AddFontDefault();
int rect_ids[2];
rect_ids[0] = io.Fonts->AddCustomRectFontGlyph(font, 'a', 13, 13, 13+1);
rect_ids[1] = io.Fonts->AddCustomRectFontGlyph(font, 'b', 13, 13, 13+1);

// Build the atlas
io.Fonts->Build();

// Get the texture in RGBA format
unsigned char* tex_pixels = nullptr;
int tex_width, tex_height;
io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_width, &tex_height);

for (int rect_n = 0; rect_n < IM_ARRAYSIZE(rect_ids); rect_n++)
    if (const ImTextureRect* rect = io.Fonts->GetCustomRect(rect_ids[rect_n]))
    {
        // Fill the custom rectangle with red pixels (in practice you would draw/copy bitmap data here!)
        for (int y = 0; y < rect->Height; y++)
        {
            ImU32* p = (ImU32*)tex_pixels + (rect->Y + y) * tex_width + (rect->X);
            for (int x = rect->Width; x > 0; x--)
                *p++ = IM_COL32(255, 0, 0, 255);
        }
    }
```

[Back to TOC](#table-of-contents)

## About File Names

Note that many C/C++ novices fail to load files due to providing incorrect file names, which often stems from incorrect assumptions about the current directory.

Two things to be aware of:

**(1) In C/C++ and most programming languages, if you want to use a backslash `\` in a string literal, you need to write it as a double backslash `\\`. Coincidentally, Windows uses backslashes as path separators, so special attention is needed.**
```cpp
io.Fonts->AddFontFromFileTTF("MyFiles\MyImage01.jpg", ...);   // WRONG!!
io.Fonts->AddFontFromFileTTF("MyFiles\\MyImage01.jpg", ...);  // Correct
```
In some cases, you can also use `/` as a path separator on Windows.

**(2) Ensure your IDE/debugger settings launch the executable from the correct working (current) directory.**
In Visual Studio, you can change the working directory in the project `Properties > General > Debugging > Working Directory`. People often assume execution starts from the project root folder, but it typically starts from the folder where objects or executables are stored.
```cpp
io.Fonts->AddFontFromFileTTF("MyImage01.jpg", ...);       // Relative file names depend on the working directory when the program runs!
io.Fonts->AddFontFromFileTTF("../MyImage01.jpg", ...);    // Load from the parent folder of the working directory
```

[Back to TOC](#table-of-contents)

## About UTF-8 Encoding

For displaying non-ASCII characters, a common user problem is not passing correctly UTF-8 encoded strings.

**(1) We provide the `ImGui::DebugTextEncoding(const char* text)` function, which can be used to verify UTF-8 string content.**
This is a convenient way to confirm correct encoding.
```cpp
ImGui::SeparatorText("Correct");
ImGui::DebugTextEncoding(u8"こんにちは");

ImGui::SeparatorText("Incorrect");
ImGui::DebugTextEncoding("こんにちは");
```
![UTF-8 encoding verification example](https://github.com/ocornut/imgui/assets/8225057/61c1696a-9a94-46c5-9627-cf91211111f0)

You can also find this tool in `Metrics/Debuggers->Tools->UTF-8 Encoding viewer` to paste content from the clipboard, but this won't verify the UTF-8 encoding done by the compiler.

**(2) How to perform UTF-8 encoding:**
There are also compiler-specific ways to force UTF-8 encoding by default:
- Visual Studio compiler: command line flag `/utf-8`
- Visual Studio compiler: use `#pragma execution_character_set("utf-8")` in code
- Since May 2023, we have changed all example Visual Studio projects to use `/utf-8` ([see commit](https://github.com/ocornut/imgui/commit/513af1efc9080857bbd10000d98f98f2a0c96803))

Or since C++11, you can use the `u8"my text"` syntax to encode string literals as UTF-8, e.g.:
```cpp
ImGui::Text(u8"hello");
ImGui::Text(u8"こんにちは");   // This string is always UTF-8 encoded
ImGui::Text("こんにちは");     // This string's encoding depends on compiler settings/flags, and may be incorrect
```

Since C++20, the C++ committee decided to change the return type of `u8""` syntax from `const char*` to a new type `const char8_t*`, which cannot be implicitly converted to `const char*`.

Therefore, using `u8""` in C++20 is somewhat awkward:
```cpp
ImGui::Text((const char*)u8"こんにちは");
```

However, you can use compiler options to completely disable this behavior:
- MSVC: `/Zc:char8_t-` ([documentation](https://learn.microsoft.com/en-us/cpp/build/reference/zc-char8-tview=msvc-170))
- Clang and GCC: `-fno-char8_t` ([documentation](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1423r3.html))

[Back to TOC](#table-of-contents)

## Debugging Tools

**Metrics/Debugger->Fonts**
You can use the `Metrics/Debugger` window (available in `Demo>Tools`) to browse fonts and troubleshoot issues. It can also be accessed in `Demo->Tools->Style Editor->Fonts`. The same information is also available in the Fonts section of the Style Editor.

![Font debugging tools](https://user-images.githubusercontent.com/8225057/135429892-0e41ef8d-33c5-4991-bcf6-f997a0bcfd6b.png)

**UTF-8 Encoding Viewer**
You can use the `UTF-8 Encoding viewer` in `Metrics/Debugger` to verify UTF-8 string content. From C/C++ code, you can call the `ImGui::DebugTextEncoding("my string");` function to verify that UTF-8 encoding is correct.

![UTF-8 Encoding Viewer](https://user-images.githubusercontent.com/8225057/166505963-8a0d7899-8ee8-4558-abb2-1ae523dc02f9.png)

[Back to TOC](#table-of-contents)

## Copyright/License Information for Fonts Included in the Repository

Some font files are provided in the `misc/fonts/` folder:

- **Roboto-Medium.ttf**, by Christian Robertson
  Apache License 2.0
  https://fonts.google.com/specimen/Roboto

- **Cousine-Regular.ttf**, by Steve Matteson
  Digitized data copyright (c) 2010 Google Corporation.
  Licensed under the SIL Open Font License, Version 1.1
  https://fonts.google.com/specimen/Cousine

- **DroidSans.ttf**, by Steve Matteson
  Apache License 2.0
  https://www.fontsquirrel.com/fonts/droid-sans

- **ProggyClean.ttf**, by Tristan Grimmer
  MIT License
  (Recommended loading settings: Size = 13.0, GlyphOffset.y = +1)
  http://www.proggyfonts.net/

- **ProggyTiny.ttf**, by Tristan Grimmer
  MIT License
  (Recommended loading settings: Size = 10.0, GlyphOffset.y = +1)
  http://www.proggyfonts.net/

- **Karla-Regular.ttf**, by Jonathan Pinhorn
  SIL OPEN FONT LICENSE Version 1.1

[Back to TOC](#table-of-contents)

## Font Resource Links

**Icon Fonts**
- C/C++ icon font header files (codepoint #defines used in source code string literals)
  https://github.com/juliettef/IconFontCppHeaders
- FontAwesome
  https://fortawesome.github.io/Font-Awesome
- OpenFontIcons
  https://github.com/traverseda/OpenFontIcons
- Google Icon Fonts
  https://design.google.com/icons/
- Kenney Icon Font (game controller icons)
  https://github.com/nicodinh/kenney-icon-font
- IcoMoon - Custom icon font builder
  https://icomoon.io/app

**General Fonts**
- Google Noto Fonts (global languages)
  https://www.google.com/get/noto/
- Open Sans Fonts
  https://fonts.google.com/specimen/Open+Sans
- (Japanese) M+ fonts by Coji Morishita
  http://mplus-fonts.sourceforge.jp/mplus-outline-fonts/index-en.html

**Monospace Fonts**
![Monospace fonts example](https://github.com/user-attachments/assets/c9702534-4877-41c9-ae0d-252933c26ced)

*Pixel-perfect:*
- Proggy Fonts, by Tristan Grimmer
  http://www.proggyfonts.net or http://upperboundsinteractive.com/fonts.php
- Sweet16, Sweet16 Mono, by Martin Sedlak (Latin + Supplemental + Extended A)
  https://github.com/kmar/Sweet16Font (also includes .inl files that can be used directly in dear imgui)

*Regular:*
- ProggyVector (if you want the legacy Dear ImGui font to be scalable)
  https://github.com/bluescan/proggyfonts
- Google Noto Mono Fonts
  https://www.google.com/get/noto/
- Fonts for beautifying source code
  https://github.com/chrissimpkins/codeface
- Programming font comparison
  http://s9w.github.io/font_compare/
- Inconsolata
  http://www.levien.com/type/myfonts/inconsolata.html
- Adobe Source Code Pro: monospace font family for UI and programming environments
  https://github.com/adobe-fonts/source-code-pro
- Monospace/fixed-width programmer fonts
  http://www.lowing.org/fonts/
- Or use Unicode fonts like Arial Unicode that come with Windows for full character coverage (license information uncertain)

[Back to TOC](#table-of-contents)

---

## QIm Font Utility Class

QIm provides the `QImFontFileHelper` static utility class for locating system font file paths via `QFont` objects.
This is a non-QObject static utility class where all methods are static, suitable for loading system fonts in QIm applications.

### QImFontFileHelper

`QImFontFileHelper` scans OS font directories and caches the mapping between font family names and file paths,
allowing subsequent lookup of TTF file paths via `QFont` objects.

!!! note "Non-QObject Class"
    `QImFontFileHelper` is a regular C++ class (using PIMPL pattern), not derived from QObject.
    All methods are static and can be called without instantiation.

#### Initialization

Call `preloadCommonFonts()` at program startup to preload the system font cache:

```cpp
#include "QImFontFileHelper.h"

// Preload at program startup (recommended to call in main() or early initialization)
QIM::QImFontFileHelper::preloadCommonFonts();
```

This method scans the OS font directory (e.g. `C:\Windows\Fonts` on Windows),
caching all available font family names and their corresponding file paths.

#### API Reference

| Method | Return Type | Description |
|--------|-------------|-------------|
| `getFontFiles(font)` | `QList<QString>` | Get the corresponding font file path list based on a QFont object (includes bold, italic, etc. variants, deduplicated) |
| `getAvailableFamilies()` | `QList<QString>` | Get all cached font family name list |
| `clearCache()` | void | Clear the font cache |
| `preloadCommonFonts()` | void | Preload system common fonts (scan OS font directory and cache) |
| `getFontPixelSize(qtFont)` | float | Calculate pixel size from QFont (automatically converts from point size) |
| `getRecommendedChineseFontPath()` | `std::string` | Get the recommended Chinese font file path |

#### Usage Example

```cpp
#include "QImFontFileHelper.h"

// 1. Preload font cache at program startup
QIM::QImFontFileHelper::preloadCommonFonts();

// 2. Create QFont and look up corresponding font files
QFont font("Microsoft YaHei", 12);
QList<QString> fontFiles = QIM::QImFontFileHelper::getFontFiles(font);
if (!fontFiles.isEmpty()) {
    qDebug() << "Found font file:" << fontFiles.first();
}

// 3. Get pixel size (automatically converts from point size)
float pixelSize = QIM::QImFontFileHelper::getFontPixelSize(font);

// 4. Get recommended Chinese font path
std::string chineseFontPath =
    QIM::QImFontFileHelper::getRecommendedChineseFontPath();

// 5. Query cached font family list
QList<QString> families = QIM::QImFontFileHelper::getAvailableFamilies();
qDebug() << "Available font families:" << families.size();
```

!!! tip "Caching Mechanism"
    `getFontFiles()` uses internal caching to improve query performance. Once a font family is queried, results are cached,
    and subsequent identical queries return cached results. Call `clearCache()` to reset the cache state.

!!! info "getRecommendedChineseFontPath"
    This method returns the QIm-recommended Chinese font file path for loading appropriate Chinese fonts in your application.
    The return type is `std::string`, the byte format required by the underlying ImGui.