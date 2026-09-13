#pragma once

#include <Windows.h>
#include <tchar.h>

#include "GameState.h"

// 场景抽象基类:开始界面 / 战斗 / 结算界面的公共部分
// 统一提供 主循环(模板方法)、滚动背景、按键边沿检测、文字工具,
// 每帧具体做什么由派生类重写 Update / Draw 决定(继承 + 多态)
class Scene
{
public:
    Scene();
    virtual ~Scene();

    // 模板方法:统一的 更新 -> 绘制 -> 帧率控制 循环
    GameState Run();

protected:
    virtual void Update() = 0;          // 纯虚函数:派生类必须实现
    virtual void Draw() = 0;
    virtual int  FrameMs() const;       // 每帧时长(ms),默认 12,Game 重写为 15

    void ScrollBackground(int speed);   // 背景向下滚动 speed 像素
    void DrawBackground();              // 画两张首尾相接的循环背景

    // 按键“刚按下”检测:按住不放只触发一次,场景切换时残留按键不误触发
    bool KeyPressed(int vk);

    // ---------- 文字工具(函数重载:带颜色 / 不带颜色) ----------

    static void SetFont(int height, bool bold, LPCTSTR face);
    static void DrawTextCenter(int cx, int y, LPCTSTR text);
    static void DrawTextCenter(int cx, int y, LPCTSTR text, COLORREF color);

protected:
    bool      running;      // 置 false 退出主循环
    GameState result;       // Run 的返回值,由派生类设置
    int       frame;        // 帧计数,驱动动画

private:
    int  bgY;               // 背景滚动偏移 0 ~ 背景高
    bool keyLast[256];      // 上一帧每个虚拟键的按下状态
};
