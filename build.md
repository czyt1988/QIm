# QIm 构建指南

## 前置依赖

| 依赖 | 最低版本 | 说明 |
|------|---------|------|
| CMake | 3.15+ | 项目使用 CMake 构建系统 |
| C++17 编译器 | - | MSVC 2019+ / GCC 7+ / Clang 5+ |
| Qt | 5.14+ | 需要 Core、Gui、Widgets、OpenGL 模块 |

## Windows 构建方法

### 环境要求

- **CMake**: 已安装并在 PATH 中（当前环境路径：`C:\Program Files (x86)\cmake3.27.9\bin\cmake.exe`）
- **Qt**: 安装到 `C:\Qt\6.x.x\msvc2019_64`（当前环境：`C:\Qt\6.7.3\msvc2019_64`）
- **Visual Studio 2019**: 安装 MSVC v142 生成工具（当前环境：VS2019 Community）

### 方法一：Visual Studio 生成器（推荐）

**这是最可靠的方式**，CMake 的 VS 生成器会自动检测 MSVC 编译器，无需手动配置环境变量。

```powershell
# 1. 配置项目（首次或需要重新配置时）
cmake -S . -B build -G "Visual Studio 16 2019" -A x64 -DCMAKE_PREFIX_PATH="C:/Qt/6.7.3/msvc2019_64"

# 2. 编译
cmake --build build --config Release

# 3. 增量编译（后续修改代码后只需执行此步）
cmake --build build --config Release
```

### 方法二：Ninja 生成器

如果安装了 Ninja，构建速度更快：

```powershell
# 需要先在 Developer Command Prompt 中运行，或手动设置 MSVC 环境
# 然后配置 + 编译
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH="C:/Qt/6.7.3/msvc2019_64" -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### 方法三：使用 build.cmd 脚本

项目根目录有 `build.cmd` 脚本，会自动检测 Qt 和生成器：

```cmd
:: 首次构建（自动配置 + 编译）
build.cmd

:: 重新配置
build.cmd reconfig

:: Debug 构建
build.cmd debug

:: 清理
build.cmd clean
```

> **注意**：`build.cmd` 是 Windows 批处理脚本，需在 CMD 中运行，不支持 PowerShell 直接调用。

## 常见构建问题

### 1. `fatal error C1083: 无法打开包括文件: "memory"/"type_traits"`

**原因**：MSVC 编译器环境未正确配置。当使用 Ninja 生成器但未在 Developer Command Prompt 中运行时会出现此问题。

**解决方案**：
- **推荐**：改用 Visual Studio 生成器（方法一），VS 生成器会自动找到编译器
- 或者先在 Developer Command Prompt 中运行 `vcvars64.bat`，再执行 Ninja 构建

### 2. `vcvars64.bat` 在 PowerShell 中不生效

**原因**：在 PowerShell 中通过 `& vcvars64.bat` 调用后，环境变量不会传递给 PowerShell 后续命令。这是 PowerShell 与 bat 脚本交互的已知限制。

**解决方案**：使用 VS 生成器代替 Ninja，或直接在 CMD（Developer Command Prompt）中操作。

### 3. `Ninja not found` / `CMAKE_MAKE_PROGRAM is not set`

**原因**：系统未安装 Ninja。

**解决方案**：使用 Visual Studio 生成器，不依赖 Ninja：
```powershell
cmake -S . -B build -G "Visual Studio 16 2019" -A x64 -DCMAKE_PREFIX_PATH="C:/Qt/6.7.3/msvc2019_64"
```

### 4. 构建目录损坏需要重新配置

```powershell
# 删除构建目录后重新配置
Remove-Item -Recurse -Force build
cmake -S . -B build -G "Visual Studio 16 2019" -A x64 -DCMAKE_PREFIX_PATH="C:/Qt/6.7.3/msvc2019_64"
cmake --build build --config Release
```

> **注意**：如果 `build/bin` 下有正在运行的程序占用文件，需要先关闭该程序再删除。

### 5. Qt 路径找不到

CMake 配置时通过 `CMAKE_PREFIX_PATH` 指定 Qt 路径。常见的 Qt 安装路径：

```powershell
# Qt 6.7.3
-DCMAKE_PREFIX_PATH="C:/Qt/6.7.3/msvc2019_64"
# Qt 6.8.0
-DCMAKE_PREFIX_PATH="C:/Qt/6.8.0/msvc2019_64"
```

## CMake 构建选项

| 选项 | 默认值 | 说明 |
|------|--------|------|
| `QIM_BUILD_EXAMPLES` | ON | 构建示例程序 |
| `QIM_BUILD_SHARED` | ON | 构建为动态库 |
| `QIM_BUILD_WIDGETS` | ON | 构建 Qt Widgets 兼容层 |
| `QIM_ENABLE_DEBUG_PRINT` | ON | 启用调试输出 |
| `QIM_ENABLE_DEBUG_PRINT_FPS` | ON | 启用 FPS 调试输出 |
| `QIM_ENABLE_BENCHMARK` | ON | 构建性能基准测试 |
| `QIM_BUILD_QML` | OFF | 构建 QML 兼容层（未完成） |

使用示例：
```powershell
cmake -S . -B build -G "Visual Studio 16 2019" -A x64 `
    -DCMAKE_PREFIX_PATH="C:/Qt/6.7.3/msvc2019_64" `
    -DQIM_BUILD_EXAMPLES=OFF `
    -DQIM_BUILD_SHARED=ON
```

## 构建产物

构建完成后，产物位于 `build/` 目录下：

```
build/
├── bin/Release/          # 可执行文件和 DLL
│   ├── QImCore.dll       # 核心库
│   ├── QImWidgets.dll    # Widgets 兼容层
│   └── *.exe             # 示例程序
└── lib/Release/          # 导入库（.lib）
    ├── QImCore.lib
    └── QImWidgets.lib
```

## 快速参考（Agent 专用）

在 Windows PowerShell 环境中构建 QIm 的完整流程：

```powershell
# 配置（首次）
cmake -S . -B build -G "Visual Studio 16 2019" -A x64 -DCMAKE_PREFIX_PATH="C:/Qt/6.7.3/msvc2019_64"
# 编译
cmake --build build --config Release
```

> **务必使用 Visual Studio 生成器**，不要使用 Ninja，因为在 PowerShell 中 MSVC 编译器环境无法通过 `vcvars64.bat` 正确注入。
