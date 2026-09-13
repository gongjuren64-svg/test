#include "EndScene.h"

#include <graphics.h>
#include <tchar.h>
#include <cstdio>

#include "Resource.h"
#include "Sound.h"

EndScene::EndScene(int score, int destroyed, int bestScore, bool newRecord)
{
    this->score = score;
    this->destroyed = destroyed;
    this->bestScore = bestScore;
    this->newRecord = newRecord;

    result = GameState::START;
}

void EndScene::Update()
{
    ScrollBackground(1);

    if (KeyPressed(VK_RETURN))
    {
        Sound::PlaySelect();
        result = GameState::PLAYING;    // 重新开始
        running = false;
    }

    if (KeyPressed(VK_ESCAPE))
    {
        Sound::PlaySelect();
        result = GameState::START;      // 返回开始菜单
        running = false;
    }
}

void EndScene::Draw()
{
    DrawBackground();

    setbkmode(TRANSPARENT);

    // GAME OVER(带阴影,DrawTextCenter 带颜色版本为函数重载)
    SetFont(68, true, _T("Consolas"));
    DrawTextCenter(403, 113, _T("GAME OVER"), RGB(80, 15, 15));
    DrawTextCenter(400, 110, _T("GAME OVER"), RGB(235, 70, 60));

    // 成绩
    TCHAR line[64];
    SetFont(26, false, _T("微软雅黑"));

    _stprintf_s(line, _T("得分 Score:  %d"), score);
    DrawTextCenter(400, 240, line, WHITE);

    _stprintf_s(line, _T("击落 Destroyed:  %d"), destroyed);
    DrawTextCenter(400, 285, line, WHITE);

    _stprintf_s(line, _T("最高分 Best:  %d"), bestScore);
    DrawTextCenter(400, 330, line, RGB(255, 220, 60));

    // 破纪录闪烁提示
    if (newRecord && (frame / 25) % 2 == 0)
    {
        SetFont(24, true, _T("微软雅黑"));
        DrawTextCenter(400, 380, _T("★ 新纪录 NEW RECORD! ★"), RGB(255, 160, 40));
    }

    // 操作提示
    SetFont(20, false, _T("微软雅黑"));
    DrawTextCenter(400, 480, _T("Enter 重新开始"), RGB(190, 200, 210));
    DrawTextCenter(400, 515, _T("Esc 返回开始菜单"), RGB(190, 200, 210));

    FlushBatchDraw();
}
