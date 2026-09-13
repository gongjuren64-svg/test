#include "PowerUp.h"

#include <graphics.h>
#include <cmath>
#include <cstdlib>

// 道具下落速度:比敌机慢,给玩家留出接取时间
static const Vec2 POWERUP_VEL(0.0f, 1.8f);

PowerUp::PowerUp(const Vec2& pos)
    : GameObject(pos, POWERUP_VEL)
{
    phase = (float)(rand() % 628) / 100.0f;   // 随机初始相位 0 ~ 2π
}

void PowerUp::Update()
{
    GameObject::Update();           // 复用基类的匀速下落

    // 左右轻微漂移,增加接取的操作感
    phase += 0.08f;
    pos.x += sinf(phase) * 0.5f;

    // 漏接:飞出屏幕底部后删除
    if (pos.y > 640.0f)
        Die();
}

void PowerUp::Draw()
{
    int x = GetX();
    int y = GetY();

    // 光环半径随相位呼吸(13 ~ 17),提示这是可拾取物
    int r = 15 + (int)(sinf(phase * 2.0f) * 2.0f);

    // 外圈细光环
    setlinecolor(RGB(255, 240, 160));
    setlinestyle(PS_SOLID, 2);
    circle(x, y, r + 3);

    // 橙黄双层圆片作底
    setfillcolor(RGB(255, 185, 40));
    solidcircle(x, y, r);

    setfillcolor(RGB(255, 235, 130));
    solidcircle(x, y, r - 4);

    // 中间画一道闪电折线,表示“射速”
    POINT bolt[4] =
    {
        { x + 4, y - 8 },
        { x - 3, y + 1 },
        { x + 3, y - 1 },
        { x - 4, y + 8 },
    };
    setlinecolor(RGB(120, 60, 0));
    setlinestyle(PS_SOLID, 3);
    polyline(bolt, 4);
}
