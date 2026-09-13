#include "Enemy.h"

#include <cmath>
#include <cstdlib>

#include "Resource.h"
#include "AlphaDraw.h"

// ==================== Enemy 基类 ====================

Enemy::Enemy(const Vec2& pos, const Vec2& vel,
             int hp, float radius, int score, int imgIndex)
    : GameObject(pos, vel)
{
    this->hp = hp;
    this->radius = radius;
    this->score = score;
    this->imgIndex = imgIndex;
}

void Enemy::Update()
{
    GameObject::Update();       // 默认:按 vel 匀速直线下落

    // 飞出屏幕底部后删除
    if (pos.y > 680.0f)
        Die();
}

void Enemy::Draw()
{
    // 各机型贴图尺寸不同,按图片实际大小居中绘制
    IMAGE* img = Resource::GetEnemy(imgIndex);
    DrawAlpha(GetX() - img->getwidth() / 2,
              GetY() - img->getheight() / 2, img);
}

bool Enemy::TakeHit()
{
    hp--;

    if (hp <= 0)
    {
        Die();
        return true;
    }
    return false;
}

Enemy* Enemy::CreateRandom(float difficulty)
{
    int roll = rand() % 100;

    if (roll < 55)      // 55% 侦察机
    {
        float x = (float)(rand() % 701 + 50);                            // 50 ~ 750
        float speed = 2.2f + (float)(rand() % 100) / 50.0f + difficulty; // 2.2 ~ 4.2 + 难度
        return new ScoutEnemy(x, speed);
    }

    if (roll < 85)      // 30% 突击机
    {
        float x = (float)(rand() % 561 + 120);                           // 120 ~ 680
        float speed = 1.6f + (float)(rand() % 100) / 100.0f + difficulty * 0.6f;
        return new RaiderEnemy(x, speed);
    }

    // 15% 轰炸机
    float x = (float)(rand() % 641 + 80);                                // 80 ~ 720
    float speed = 1.0f + (float)(rand() % 60) / 100.0f + difficulty * 0.4f;
    return new BomberEnemy(x, speed);
}

// ==================== 侦察机 ====================

ScoutEnemy::ScoutEnemy(float x, float speed)
    : Enemy(Vec2(x, -50.0f), Vec2(0.0f, speed),
            1, 26.0f, 10, rand() % 3)          // 皮肤:enemy1~3 随机
{
}

// ==================== 突击机 ====================

RaiderEnemy::RaiderEnemy(float x, float speed)
    : Enemy(Vec2(x, -50.0f), Vec2(0.0f, speed),
            2, 30.0f, 20, 3 + rand() % 2)      // 皮肤:enemy4~5 随机
{
    baseX = x;
    phase = (float)(rand() % 628) / 100.0f;    // 随机初始相位 0 ~ 2π
    swing = 40.0f + (float)(rand() % 30);      // 摆动幅度 40 ~ 70

    // 收拢摆动中心,保证整个摆动范围都在屏幕内
    if (baseX - swing < 30.0f)  baseX = 30.0f + swing;
    if (baseX + swing > 770.0f) baseX = 770.0f - swing;
}

void RaiderEnemy::Update()
{
    // 重写基类移动:纵向匀速下落,横向绕摆动中心正弦往复
    phase += 0.045f;
    pos.y += vel.y;
    pos.x = baseX + sinf(phase) * swing;

    if (pos.y > 680.0f)
        Die();
}

// ==================== 轰炸机 ====================

BomberEnemy::BomberEnemy(float x, float speed)
    : Enemy(Vec2(x, -70.0f), Vec2(0.0f, speed),
            3, 38.0f, 30, 5)                   // 皮肤:enemy6 宽体机
{
}
