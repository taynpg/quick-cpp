# 1.简介

主要用于使用`模板`快速创建一个 `Visual Studio Code` 方便使用的 `CMake初始工程`。

当前可以包含以下库的初始工程：

- 默认

- Boost

- Qt

# 2.说明

- 本工具生成的工程使用的是`clangd`索引，[官网下载](https://github.com/clangd/clangd/releases)，解压后请添加环境变量，并在`vscode`中安装`clangd`插件，生成器使用的是`Ninja`，[Ninja下载](https://github.com/ninja-build/ninja/releases)后添加环境变量。
- 请安装`c++插件`（新版的cpp插件好像包含了`cmake-tools`插件，没有的再安装一下）。
- 如果您使用的是`Mingw64`编译器，类型也选择`gnu`。
- 如果您发现了BUG，欢迎及时提issue或建议。

# 3.其他

## v1.0.3 快捷键设置功能

`code`中的`cmake-tools`插件默认的`Ctrl + F5`调试按键按起来有点麻烦，`F12`代码跳转按键在右上方，每次要伸手去按，也不方便，故给这两个最常用的功能添加一个快捷键设置。

本人设置为：`F5`调试，`Alt+g`跳转。

**注意：** 快捷键设置本质就是修改`code`的`keybindings.json`文件，如果你已经存在该文件，该文件中不能有注释，否则设置功能无效。

# 4.其他

本工具代码理论上是跨平台的，但平时`windows`下使用最多，故其他平台暂无测试，有需要的可以细节调整后自行编译，如果可以，可以贡献您的修改。
