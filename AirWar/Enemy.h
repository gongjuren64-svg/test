#pragma once

#include "GameObject.h"

// 敌机基类:继承 GameObject,从屏幕顶部生成向下飞,出界或被击毁后删除
// 三个派生类通过重写 Update 实现不同的移动方式(多态):
//   ScoutEnemy  侦察机  小巧快速,直线俯冲      1 HP / 10 分
//   RaiderEnemy 突击机  中型,左右正弦摆动前进  2 HP / 20 分
//   BomberEnemy 轰炸机  重型,缓慢推进皮糙肉厚  3 HP / 30 分
class Enemy : public GameObject
{
public:
    Enemy(const Vec2& pos, const Vec2& vel,
          int hp, float radius, int score, int imgIndex);
    virtual ~Enemy() {}

    virtual void Update() override;   // 默认:直线下落 + 出界删除
    virtual void Draw() override;     // 按 imgIndex 画对应贴图

    // 被一颗子弹击中:HP 减一,归零死亡;返回是否被击毁
    bool TakeHit();

    float GetRadius() const { return radius; }
    int   GetScore() const  { return score; }

    // 工厂函数:按概率随机生成一架敌机(堆上创建,基类指针返回)
    // difficulty 0~1.5,随游戏时间增大,敌机速度随之加快
    static Enemy* CreateRandom(float difficulty);

protected:
    int   hp;         // 剩余耐久
    float radius;     // 碰撞半径
    int   score;      // 击毁得分
    int   imgIndex;   // 贴图编号 0~5,对应 Resource::enemies
};

// 侦察机:三种小型机皮肤随机
class ScoutEnemy : public Enemy
{
public:
    ScoutEnemy(float x, float speed);
};

// 突击机:两种中型机皮肤随机,左右摆动前进
class RaiderEnemy : public Enemy
{
public:
    RaiderEnemy(float x, float speed);

    virtual void Update() override;   // 重写:下落 + 正弦横移

private:
    float baseX;    // 摆动中心
    float phase;    // 正弦相位
    float swing;    // 摆动幅度(像素)
};

// 轰炸机:大型机,移动方式与基类相同,靠高 HP 和大身板体现差异
class BomberEnemy : public Enemy
{
public:
    BomberEnemy(float x, float speed);
};
