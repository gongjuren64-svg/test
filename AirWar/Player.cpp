#include "Player.h"

#include <Windows.h>
#include <algorithm>

#include "Resource.h"
#include "AlphaDraw.h"
#include "Sound.h"

Player::Player(int type)
    : GameObject(Vec2(400.0f, 520.0f), Vec2())   // 出生在屏幕下方中央
{
    planeType = type;

    lives = 3;
    invincibleTimer = 0;
    fireTimer = 0;
    fireLevel = 0;          // 射速强化从零级开始,靠道具积累

    // 三种机型属性差异
    switch (planeType)
    {
    case 1:                     // Eagle:火力强(双发),速度一般
        speed = 5.0f;
        fireInterval = 18;
        doubleShot = true;
        break;
    case 2:                     // Lightning:射速快,速度一般
        speed = 6.0f;
        fireInterval = 6;
        doubleShot = false;
        break;
    default:                    // Falcon:速度快,攻击一般
        speed = 7.0f;
        fireInterval = 12;
        doubleShot = false;
        break;
    }
}

void Player::Move()
{
    // WASD 或 方向键
    if ((GetAsyncKeyState(VK_LEFT)  & 0x8000) || (GetAsyncKeyState('A') & 0x8000)) pos.x -= speed;
    if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState('D') & 0x8000)) pos.x += speed;
    if ((GetAsyncKeyState(VK_UP)    & 0x8000) || (GetAsyncKeyState('W') & 0x8000)) pos.y -= speed;
    if ((GetAsyncKeyState(VK_DOWN)  & 0x8000) || (GetAsyncKeyState('S') & 0x8000)) pos.y += speed;

    // 不能飞出窗口
    if (pos.x < 24.0f)  pos.x = 24.0f;
    if (pos.x > 776.0f) pos.x = 776.0f;
    if (pos.y < 24.0f)  pos.y = 24.0f;
    if (pos.y > 576.0f) pos.y = 576.0f;
}

void Player::Shoot()
{
    if (fireTimer > 0)
        fireTimer--;

    // 按住空格连续发射,间隔由机型的射速决定
    if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && fireTimer <= 0)
    {
        if (doubleShot)
        {
            // 双发:机身两侧各一颗,偏移用 Vec2 加法算(运算符重载)
            bullets.emplace_back(pos + Vec2(-14.0f, -24.0f));
            bullets.emplace_back(pos + Vec2( 14.0f, -24.0f));
        }
        else
        {
            bullets.emplace_back(pos + Vec2(0.0f, -34.0f));
        }

        Sound::PlayShoot();

        // 实际间隔 = 机型基础间隔 - 强化等级 × 2,最快 3 帧一发
        int interval = fireInterval - fireLevel * 2;
        if (interval < 3)
            interval = 3;

        fireTimer = interval;
    }
}

void Player::Update()
{
    if (IsAlive())
    {
        Move();
        Shoot();

        if (invincibleTimer > 0)
            invincibleTimer--;
    }

    // 玩家死后已发射的子弹继续飞完
    for (auto& bullet : bullets)
        bullet.Update();

    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return !b.IsAlive(); }),
        bullets.end());
}

void Player::Draw()
{
    // 无敌期间闪烁(每 4 帧显示/隐藏交替)
    bool visible = (invincibleTimer == 0) || ((invincibleTimer / 4) % 2 == 0);

    if (IsAlive() && visible)
        DrawAlpha(GetX() - 32, GetY() - 32, Resource::GetPlayer(planeType));

    for (auto& bullet : bullets)
        bullet.Draw();
}

std::vector<Bullet>& Player::GetBullets()
{
    return bullets;
}

int Player::GetLives() const
{
    return lives;
}

bool Player::IsInvincible() const
{
    return invincibleTimer > 0;
}

// 拾取射速道具:未满级则升一级并返回 true;已满级返回 false(Game 折算奖励分)
bool Player::UpgradeFireRate()
{
    if (fireLevel >= MAX_FIRE_LEVEL)
        return false;

    fireLevel++;
    return true;
}

int Player::GetFireRateLevel() const
{
    return fireLevel;
}

void Player::Hit()
{
    if (!IsAlive() || invincibleTimer > 0)
        return;

    lives--;

    if (lives <= 0)
        Die();
    else
        invincibleTimer = 100;   // 被撞后约 1.5 秒无敌
}
