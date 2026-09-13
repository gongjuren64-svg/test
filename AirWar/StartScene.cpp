#include "StartScene.h"

#include <graphics.h>
#include <tchar.h>
#include <cmath>
#include <cstdio>

#include "Resource.h"
#include "AlphaDraw.h"
#include "Sound.h"

// ---------- 三架飞机的展示数据 ----------

static LPCTSTR planeNames[3] = { _T("Falcon"), _T("Eagle"), _T("Lightning") };

// 速度 / 攻击 / 射速 星级(满分 5)
static const int planeStats[3][3] =
{
    { 5, 3, 3 },    // Falcon    速度快
    { 3, 5, 3 },    // Eagle     攻击高(双发)
    { 4, 3, 5 },    // Lightning 射速快
};

static LPCTSTR statNames[3] = { _T("速度"), _T("攻击"), _T("射速") };

// 生成 "★★★☆☆" 样式的星级字符串
static void MakeStars(int n, TCHAR* buf, int bufSize)
{
    int i = 0;
    for (; i < 5 && i < bufSize - 1; i++)
        buf[i] = (i < n) ? _T('★') : _T('☆');
    buf[i] = _T('\0');
}

// ---------- StartScene ----------

StartScene::StartScene()
{
    planeIndex = 0;
    result = GameState::PLAYING;
}

int StartScene::GetPlaneType() const
{
    return planeIndex;
}

void StartScene::Update()
{
    ScrollBackground(1);    // 背景缓慢滚动

    if (KeyPressed(VK_LEFT))
    {
        planeIndex = (planeIndex + 2) % 3;
        Sound::PlaySelect();
    }

    if (KeyPressed(VK_RIGHT))
    {
        planeIndex = (planeIndex + 1) % 3;
        Sound::PlaySelect();
    }

    if (KeyPressed(VK_RETURN))
    {
        Sound::PlaySelect();
        result = GameState::PLAYING;
        running = false;
    }

    if (KeyPressed(VK_ESCAPE))
    {
        result = GameState::EXIT;
        running = false;
    }
}

void StartScene::Draw()
{
    DrawBackground();

    setbkmode(TRANSPARENT);

    // 标题(带阴影,DrawTextCenter 带颜色版本为函数重载)
    SetFont(64, true, _T("Consolas"));
    DrawTextCenter(403, 51, _T("AIR WAR"), RGB(15, 45, 80));
    DrawTextCenter(400, 48, _T("AIR WAR"), RGB(90, 200, 255));

    SetFont(26, false, _T("微软雅黑"));
    DrawTextCenter(400, 140, _T("请选择你的飞机"), WHITE);

    // 三架飞机
    const int centerX[3] = { 160, 400, 640 };
    const int centerY = 290;

    for (int i = 0; i < 3; i++)
    {
        int cx = centerX[i];

        if (i == planeIndex)
        {
            // 选中:轻微放大 + 上下浮动 + 黄色边框
            int dy = (int)(sin(frame * 0.08) * 6.0);
            int cy = centerY + dy;

            DrawAlpha(cx - 40, cy - 40, Resource::GetPlayerBig(i));

            setlinecolor(RGB(255, 220, 60));
            setlinestyle(PS_SOLID, 2);
            roundrect(cx - 54, cy - 54, cx + 54, cy + 54, 12, 12);

            SetFont(24, true, _T("Consolas"));
            DrawTextCenter(cx, centerY + 70, planeNames[i], RGB(255, 220, 60));
        }
        else
        {
            DrawAlpha(cx - 32, centerY - 32, Resource::GetPlayer(i));

            SetFont(22, false, _T("Consolas"));
            DrawTextCenter(cx, centerY + 70, planeNames[i], RGB(150, 160, 175));
        }
    }

    // 选中飞机的属性(速度/攻击/射速)
    SetFont(22, false, _T("微软雅黑"));
    for (int s = 0; s < 3; s++)
    {
        TCHAR stars[8];
        MakeStars(planeStats[planeIndex][s], stars, 8);

        TCHAR line[32];
        _stprintf_s(line, _T("%s  %s"), statNames[s], stars);

        COLORREF color = s == 0 ? RGB(120, 220, 255)
                       : s == 1 ? RGB(255, 150, 120)
                                : RGB(180, 255, 150);
        DrawTextCenter(400, 415 + s * 34, line, color);
    }

    // 操作提示
    SetFont(20, false, _T("微软雅黑"));
    DrawTextCenter(400, 530, _T("← → 切换飞机      Enter 开始      Esc 退出"), RGB(190, 200, 210));
    DrawTextCenter(400, 560, _T("空格 射击      Q 全屏炸弹      接住闪电道具提升射速"), RGB(190, 200, 210));

    FlushBatchDraw();
}
