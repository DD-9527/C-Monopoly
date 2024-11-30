# C-Monopoly

## 项目简介

C-Monopoly 是一个用 C 语言实现的经典大富翁（Monopoly）桌游。这个项目旨在提供一个简单的命令行界面，让用户能够在终端中体验大富翁游戏的乐趣。

## 功能

- 多玩家支持
- 经典的大富翁规则
- 简单易用的命令行界面
- 可扩展的游戏逻辑

## 安装与运行

### 先决条件

- C 编译器（如 GCC）
- IDE（我使用Dev-C++ 6.5）
- 终端或命令行工具

### 克隆仓库

```sh
git clone https://github.com/DD-9527/C-Monopoly.git
cd C-Monopoly
```

### 使用说明 
- 启动游戏后，按照提示选择玩家数量。
- 游戏开始后，轮流掷骰子(SPACE)并移动棋子(←)。
- 根据游戏规则进行买卖地产、支付租金等操作。
- 游戏结束时，显示最终结果。

### 目录
```
C-Monopoly/
├── src/              # 源代码文件
│   ├── main.c        # 主程序入口
│   ├── game.c        # 游戏逻辑
│   ├── player.c      # 玩家相关逻辑
│   ├── board.c       # 棋盘相关逻辑
│   └── utils.c       # 工具函数
├── include/          # 头文件
│   ├── game.h
│   ├── player.h
│   ├── board.h
│   └── utils.h
├── Makefile          # 编译配置
└── README.md         # 项目说明
```
### 贡献指南
我们欢迎社区的贡献！如果你发现了问题或有改进的想法，请提交 Issue 或 Pull Request。

### 提交 Issue
在提交 Issue 之前，请确保你已经搜索过现有的 Issue，以避免重复。
请详细描述问题或建议，并附上相关的代码片段或截图。
### 提交 Pull Request
Fork 本仓库到你的 GitHub 账户。
创建一个新的分支来进行修改。
提交你的更改，并推送分支到你的 Fork。
在原仓库中创建 Pull Request。
### 许可证
本项目采用 GPL-3.0 许可证。

### 联系方式
如果你有任何问题或建议，请联系wyzdzzz@126.com 或在 GitHub 上提交 Issue。

感谢你的关注和支持！希望你喜欢 C-Monopoly！
