QIm 构建指南，详细内容请参阅项目根目录的 [build.md](../../build.md)

## Windows PowerShell 快速构建（Agent 专用）

```powershell
# 配置（首次或重新配置时）
cmake -S . -B build -G "Visual Studio 16 2019" -A x64 -DCMAKE_PREFIX_PATH="C:/Qt/6.7.3/msvc2019_64"
# 编译
cmake --build build --config Release
```

**重要提示**：
- 务必使用 Visual Studio 生成器（`-G "Visual Studio 16 2019"`），不要使用 Ninja
- 在 PowerShell 中 `vcvars64.bat` 无法正确注入 MSVC 环境变量，VS 生成器可自动检测编译器
- 如果构建目录损坏，先 `Remove-Item -Recurse -Force build` 再重新配置