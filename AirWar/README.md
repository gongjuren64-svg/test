# Air War — C++ / EasyX 飞机大战

800×600 双缓冲窗口,约 66 FPS。流程:开始界面(选飞机) → 游戏 → 结算界面 → Enter 重新开始 / Esc 返回菜单。窗口只创建一次,图片/音效只加载一次。

## 操作

| 按键 | 作用 |
|---|---|
| ← → (开始界面) | 切换飞机 |
| Enter | 开始游戏 / 重新开始 |
| Esc | 退出(开始界面) / 返回菜单(结算界面) |
| WASD / 方向键 | 移动 |
| 空格(按住) | 连续射击 |
| Q | 全屏炸弹:当前所有敌机原地爆炸并计分(开局 3 枚,每 1500 分奖励 1 枚) |

## 三种机型

| 机型 | 特点 | 数值 |
|---|---|---|
| Falcon | 速度快 | 速度 7,射击间隔 12 帧 |
| Eagle | 火力强(双发) | 速度 5,射击间隔 18 帧,一次两发 |
| Lightning | 射速快 | 速度 6,射击间隔 6 帧 |

## 三类敌机(六种贴图)

| 类型 | 贴图 | 行为 | HP | 分数 |
|---|---|---|---|---|
| ScoutEnemy 侦察机 | enemy1~3(小型,随机) | 直线快速俯冲 | 1 | 10 |
| RaiderEnemy 突击机 | enemy4~5(中型,随机) | 左右正弦摆动前进 | 2 | 20 |
| BomberEnemy 轰炸机 | enemy6(大型宽体) | 缓慢推进,皮糙肉厚 | 3 | 30 |

规则:击毁敌机得对应分数并播放爆炸动画+音效;敌机生成间隔和速度随时间渐增;玩家 3 条生命,被撞掉一条并短暂无敌闪烁,生命归零游戏结束;最高分存在 `best.txt`。

**强化道具**:敌机被击毁后按机型概率掉落射速道具(侦察机 15% / 突击机 20% / 轰炸机 25%),道具缓慢下落并左右漂移(呼吸光环 + 闪电图标,纯图形绘制);接住后射速等级 +1(上限 5 级),每级射击间隔缩短 2 帧、最快 3 帧一发;满级后再拾取折算 50 分;HUD 第二行显示炸弹数与射速等级。

## 面向对象设计

- **继承 + 多态**
  - `GameObject`(抽象基类,纯虚 `Draw`、虚析构)→ `Player` / `Bullet` / `Explosion` / `Enemy` / `PowerUp`
  - `Enemy` → `ScoutEnemy` / `RaiderEnemy` / `BomberEnemy`,重写 `Update` 实现各自的移动方式;`EnemyManager` 用 `unique_ptr<Enemy>` 基类指针统一管理,更新/绘制走虚函数
  - `Scene`(场景基类,模板方法 `Run` 统一主循环)→ `StartScene` / `Game` / `EndScene`
  - `Enemy::CreateRandom` 工厂函数按概率生成三种机型
- **函数重载**:`DrawAlpha`(整图/裁剪)、`ExplosionManager::Add`(整数/向量坐标)、`Bullet` 构造函数、`GameObject::CollideWith`(对物体/对点)、`Scene::DrawTextCenter`(带色/不带色)
- **运算符重载**:`Vec2` 的 `+ - * += -= *=`(含标量左乘的全局重载);`HitResult` 的 `+=` 累加战果

## 模块职责

- `main.cpp` — 创建窗口、加载资源、初始化音效、`GameState` 状态机
- `GameState.h` — 流程状态枚举(START / PLAYING / GAMEOVER / EXIT)
- `Vec2.h` / `GameObject.h` — 二维向量(运算符重载)/ 游戏物体抽象基类
- `Scene` — 场景基类:主循环、滚动背景、按键边沿检测、文字工具
- `StartScene` / `EndScene` — 开始界面(飞机选择、浮动动画、属性星级)/ 结算界面
- `Game` — 一局游戏:更新、绘制、玩家碰撞、HUD、帧率控制
- `Player` / `Bullet` — 玩家(机型差异、生命、无敌)/ 子弹
- `Enemy` / `EnemyManager` — 敌机基类与三个派生机型 / 生成、删除、子弹碰撞(圆形,按机型半径)、全屏炸弹 `KillAll`、道具掉落
- `PowerUp` — 射速强化道具:下落 + 正弦漂移,呼吸光环 + 闪电图标(纯图形绘制,不占图片资源)
- `Explosion` / `ExplosionManager` — 单个爆炸动画(512×64 精灵图,8 帧 64×64)/ 多爆炸管理
- `Resource` — 所有图片一次性加载
- `Sound` — MCI 音效:BGM 循环 + 射击/爆炸多通道轮播(连发不断音)
- `AlphaDraw` — PNG 透明通道绘制(整图 + 精灵图裁剪两个重载)

## 资源

- `images/`:player1/2/3.png(64×64)及 _big 放大版(80×80)、enemy1~6.png(三小/两中/一大,由 enemy.png 精灵图切割)、bullet.png(16×32)、explosion.png(512×64 精灵图)、bg.jpg(800×1029,去水印后做过首尾交叉淡化,可无缝纵向循环)、icon.png/ico。原始素材备份在 `images/backup_original/`。
- `sounds/`:bgm.wav(16 秒循环)、shoot.wav、explode.wav、select.wav、powerup.wav(拾取道具,上行琶音;全部程序合成)。

## 构建

Visual Studio 2022 打开 `test.sln`,x64 Debug/Release 直接 F5。EasyX(26.1.1)已随工程自带于 `easyx/` 目录(头文件 + x86/x64 库),**无需系统安装 EasyX**。源文件均为 UTF-8(带 BOM)。直接运行 exe 时需保证工作目录下有 `images/` 和 `sounds/` 文件夹。
