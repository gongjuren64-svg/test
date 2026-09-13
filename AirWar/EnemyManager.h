#pragma once

#include <memory>
#include <vector>

#include "Enemy.h"
#include "Bullet.h"
#include "PowerUp.h"
#include "ExplosionManager.h"

// 一轮子弹结算的战果
// 重载 += 后可以像整数一样逐项累加(运算符重载)
struct HitResult
{
    int killed = 0;     // 击毁架数
    int score  = 0;     // 获得分数

    HitResult& operator+=(const HitResult& r)
    {
        killed += r.killed;
        score  += r.score;
        return *this;
    }
};

// 敌机管理器:负责敌机的生成、更新、绘制、删除和子弹碰撞检测
// 敌机以基类指针统一存储,更新/绘制走虚函数,机型差异由多态体现
class EnemyManager
{
public:
    EnemyManager();

    void Update();
    void Draw();

    // 玩家与敌机的碰撞检测需要用
    const std::vector<std::unique_ptr<Enemy>>& GetEnemies() const;

    // 子弹与敌机的圆形碰撞:命中扣 HP,击毁的敌机生成爆炸并概率掉落道具
    // 返回本轮战果(Game 用来结算分数与击落数)
    HitResult CheckBulletCollision(std::vector<Bullet>& bullets,
                                   ExplosionManager& explosionManager,
                                   std::vector<PowerUp>& drops);

    // Q 键全屏炸弹:炸毁当前全部存活敌机(逐架爆炸、计分、掉落道具)
    HitResult KillAll(ExplosionManager& explosionManager,
                      std::vector<PowerUp>& drops);

private:
    // 敌机被击毁后按机型概率掉落射速道具(分值越高的机型概率越大)
    void TryDropPowerUp(const Enemy& enemy, std::vector<PowerUp>& drops);

private:
    std::vector<std::unique_ptr<Enemy>> enemies;

    int spawnTimer;   // 生成计时器
    int frameCount;   // 总帧数,驱动难度渐增
};
