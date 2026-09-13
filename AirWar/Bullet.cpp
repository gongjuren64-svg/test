#include "Bullet.h"

#include "Resource.h"
#include "AlphaDraw.h"

// 子弹速度:每帧向上 9 像素
static const Vec2 BULLET_VEL(0.0f, -9.0f);

Bullet::Bullet(const Vec2& pos)
    : GameObject(pos, BULLET_VEL)
{
}

Bullet::Bullet(float x, float y)
    : GameObject(Vec2(x, y), BULLET_VEL)
{
}

void Bullet::Update()
{
    GameObject::Update();       // 复用基类的匀速移动

    // 完全飞出屏幕顶部后删除
    if (pos.y < -32.0f)
        Die();
}

void Bullet::Draw()
{
    // 子弹图 16x32,pos 是中心
    DrawAlpha(GetX() - 8, GetY() - 16, &Resource::bullet);
}
