#include "Explosion.h"

#include "Resource.h"
#include "AlphaDraw.h"

Explosion::Explosion(const Vec2& pos)
    : GameObject(pos, Vec2())      // 爆炸不移动,速度为零
{
    frame = 0;
    timer = 0;
}

void Explosion::Update()
{
    // 不调用基类移动,只负责推进动画:游戏每 4 帧切换一帧
    timer++;
    if (timer >= 4)
    {
        timer = 0;
        frame++;

        if (frame >= 8)
            Die();
    }
}

void Explosion::Draw()
{
    if (!IsAlive())
        return;

    // 精灵图 512x64:一行 8 帧,每帧 64x64(裁剪版 DrawAlpha,函数重载)
    DrawAlpha(GetX() - 32, GetY() - 32, frame * 64, 0, 64, 64, &Resource::explosion);
}
