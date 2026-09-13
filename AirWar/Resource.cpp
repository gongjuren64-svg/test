#include "Resource.h"

#include <tchar.h>
#include <cstdio>

IMAGE Resource::player1;
IMAGE Resource::player2;
IMAGE Resource::player3;

IMAGE Resource::player1Big;
IMAGE Resource::player2Big;
IMAGE Resource::player3Big;

IMAGE Resource::enemies[6];

IMAGE Resource::bullet;
IMAGE Resource::bg;
IMAGE Resource::explosion;

void Resource::Load()
{
    // 图片文件已经是目标尺寸,按原始大小加载,保留透明通道
    loadimage(&player1, _T("images/player1.png"));
    loadimage(&player2, _T("images/player2.png"));
    loadimage(&player3, _T("images/player3.png"));

    loadimage(&player1Big, _T("images/player1_big.png"));
    loadimage(&player2Big, _T("images/player2_big.png"));
    loadimage(&player3Big, _T("images/player3_big.png"));

    // 六种敌机:images/enemy1.png ~ enemy6.png
    for (int i = 0; i < 6; i++)
    {
        TCHAR path[64];
        _stprintf_s(path, _T("images/enemy%d.png"), i + 1);
        loadimage(&enemies[i], path);
    }

    loadimage(&bullet, _T("images/bullet.png"));
    loadimage(&bg, _T("images/bg.jpg"));
    loadimage(&explosion, _T("images/explosion.png"));
}

IMAGE* Resource::GetPlayer(int type)
{
    switch (type)
    {
    case 1:  return &player2;
    case 2:  return &player3;
    default: return &player1;
    }
}

IMAGE* Resource::GetPlayerBig(int type)
{
    switch (type)
    {
    case 1:  return &player2Big;
    case 2:  return &player3Big;
    default: return &player1Big;
    }
}

IMAGE* Resource::GetEnemy(int index)
{
    if (index < 0) index = 0;
    if (index > 5) index = 5;
    return &enemies[index];
}
