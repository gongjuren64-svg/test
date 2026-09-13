#include <graphics.h>
#include <tchar.h>
#include <ctime>
#include <cstdlib>
#include <fstream>

#include "GameState.h"
#include "Resource.h"
#include "Sound.h"
#include "StartScene.h"
#include "Game.h"
#include "EndScene.h"

// 最高分存档(best.txt 在工作目录下)
static int LoadBestScore()
{
    std::ifstream fin("best.txt");
    int best = 0;
    if (fin)
        fin >> best;
    return best;
}

static void SaveBestScore(int best)
{
    std::ofstream fout("best.txt");
    if (fout)
        fout << best;
}

int main()
{
    // 窗口只创建一次
    initgraph(800, 600);
    SetWindowText(GetHWnd(), _T("Air War"));

    // 窗口图标
    HICON hIcon = (HICON)LoadImage(NULL, _T("images/icon.ico"),
        IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
    if (hIcon)
    {
        SendMessage(GetHWnd(), WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        SendMessage(GetHWnd(), WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    }

    srand((unsigned int)time(nullptr));

    Resource::Load();       // 图片只加载一次
    Sound::Init();          // 打开音效通道
    Sound::PlayBGM();       // 背景音乐循环播放

    BeginBatchDraw();

    // 状态机:开始界面 -> 游戏 -> 结算 -> (重新开始 / 返回菜单 / 退出)
    // 三个场景都是 Scene 的派生类,统一用 Run() 驱动(多态)
    GameState state = GameState::START;

    int planeType = 0;
    int bestScore = LoadBestScore();
    int lastScore = 0;
    int lastDestroyed = 0;
    bool newRecord = false;

    while (state != GameState::EXIT)
    {
        switch (state)
        {
        case GameState::START:
        {
            StartScene scene;
            state = scene.Run();
            planeType = scene.GetPlaneType();
            break;
        }

        case GameState::PLAYING:
        {
            Game game(planeType);
            state = game.Run();

            lastScore = game.GetScore();
            lastDestroyed = game.GetDestroyed();

            newRecord = lastScore > bestScore;
            if (newRecord)
            {
                bestScore = lastScore;
                SaveBestScore(bestScore);
            }
            break;
        }

        case GameState::GAMEOVER:
        {
            EndScene scene(lastScore, lastDestroyed, bestScore, newRecord);
            state = scene.Run();
            break;
        }

        default:
            state = GameState::EXIT;
            break;
        }
    }

    EndBatchDraw();
    Sound::Shutdown();
    closegraph();

    return 0;
}
