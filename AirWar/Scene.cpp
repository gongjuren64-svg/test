#include "Scene.h"

#include <graphics.h>

#include "Resource.h"

Scene::Scene()
{
    running = true;
    result = GameState::EXIT;
    frame = 0;
    bgY = 0;

    // 用当前按键状态初始化,防止上个场景残留的按键在本场景误触发
    for (int vk = 0; vk < 256; vk++)
        keyLast[vk] = (GetAsyncKeyState(vk) & 0x8000) != 0;
}

Scene::~Scene()
{
}

GameState Scene::Run()
{
    while (running)
    {
        DWORD frameStart = GetTickCount();

        frame++;
        Update();       // 虚函数调用:各场景做各自的事(多态)
        Draw();

        // 帧率控制:一帧耗时不足 FrameMs 时补足
        DWORD elapsed = GetTickCount() - frameStart;
        DWORD frameMs = (DWORD)FrameMs();
        if (elapsed < frameMs)
            Sleep(frameMs - elapsed);
    }

    return result;
}

int Scene::FrameMs() const
{
    return 12;
}

void Scene::ScrollBackground(int speed)
{
    int bgHeight = Resource::bg.getheight();

    bgY += speed;
    if (bgY >= bgHeight)
        bgY -= bgHeight;
}

void Scene::DrawBackground()
{
    // 背景图可无缝纵向循环,两张首尾相接即可铺满窗口
    int bgHeight = Resource::bg.getheight();

    putimage(0, bgY - bgHeight, &Resource::bg);
    putimage(0, bgY, &Resource::bg);
}

bool Scene::KeyPressed(int vk)
{
    bool down = (GetAsyncKeyState(vk) & 0x8000) != 0;
    bool pressed = down && !keyLast[vk];

    keyLast[vk] = down;
    return pressed;
}

void Scene::SetFont(int height, bool bold, LPCTSTR face)
{
    LOGFONT f;
    gettextstyle(&f);
    f.lfHeight = height;
    f.lfWidth = 0;
    f.lfWeight = bold ? FW_BOLD : FW_NORMAL;
    f.lfQuality = ANTIALIASED_QUALITY;
    _tcscpy_s(f.lfFaceName, face);
    settextstyle(&f);
}

void Scene::DrawTextCenter(int cx, int y, LPCTSTR text)
{
    outtextxy(cx - textwidth(text) / 2, y, text);
}

void Scene::DrawTextCenter(int cx, int y, LPCTSTR text, COLORREF color)
{
    settextcolor(color);
    DrawTextCenter(cx, y, text);    // 复用不带颜色的重载版本
}
