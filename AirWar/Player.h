#pragma once

#include <vector>

#include "GameObject.h"
#include "Bullet.h"

// 玩家飞机:继承 GameObject,负责键盘移动、射击、生命与无敌闪烁
// 三种机型: 0 Falcon(速度快) / 1 Eagle(火力强,双发) / 2 Lightning(射速快)
class Player : public GameObject
{
public:
    explicit Player(int type = 0);

    virtual void Update() override;   // 重写:键盘移动 + 射击 + 子弹更新
    virtual void Draw() override;     // 重写:无敌期间闪烁

    std::vector<Bullet>& GetBullets();

    int  GetLives() const;
    bool IsInvincible() const;

    // 被敌机撞到:扣一条命,进入短暂无敌;生命归零则死亡
    void Hit();

    // 拾取射速道具:强化一级,缩短射击间隔;已满级返回 false
    bool UpgradeFireRate();
    int  GetFireRateLevel() const;

    static const int MAX_FIRE_LEVEL = 5;   // 射速强化等级上限

private:
    void Move();
    void Shoot();

private:
    float speed;            // 每帧移动像素
    int   planeType;

    int   lives;            // 剩余生命
    int   invincibleTimer;  // 无敌剩余帧数(被撞后闪烁)

    int   fireInterval;     // 基础射击间隔(帧),由机型决定
    int   fireTimer;        // 射击冷却计时
    int   fireLevel;        // 射速强化等级 0 ~ MAX_FIRE_LEVEL,吃道具提升
    bool  doubleShot;       // 是否双发

    std::vector<Bullet> bullets;
};
