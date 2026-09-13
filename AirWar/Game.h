#pragma once

#include <Windows.h>

#include <vector>

#include "Scene.h"
#include "Player.h"
#include "EnemyManager.h"
#include "ExplosionManager.h"
#include "PowerUp.h"

// 战斗场景:继承 Scene,负责一局游戏的更新、绘制、碰撞与 HUD
// 玩家死亡后返回 GAMEOVER,进入结算界面
class Game : public Scene
{
public:
    explicit Game(int planeType);

    int GetScore() const;
    int GetDestroyed() const;

protected:
    virtual void Update() override;
    virtual void Draw() override;
    virtual int  FrameMs() const override;   // 战斗场景 15ms/帧,约 66 FPS

private:
    // 玩家与敌机的碰撞(子弹与敌机由 EnemyManager 负责)
    void CheckPlayerCollision();

    // Q 键全屏炸弹:消耗一枚存货,清掉当前所有敌机并计分
    void UseBomb();

    // 道具的下落、拾取判定与清理
    void UpdatePowerUps();

    // 炸弹引爆的视觉反馈:整屏白闪 + 从玩家位置扩散的冲击波圆环
    void DrawBombEffect();

    void DrawHUD();

private:
    Player           player;
    EnemyManager     enemyManager;
    ExplosionManager explosionManager;

    std::vector<PowerUp> powerups;   // 场上漂浮的射速道具

    int   score;       // 分数
    int   destroyed;   // 击落数
    int   overTimer;   // 玩家死亡后的缓冲帧(把爆炸播完)

    int   bombs;          // 剩余全屏炸弹数(按 Q 使用)
    int   nextBombScore;  // 下一次奖励炸弹的分数线
    int   bombFlash;      // 炸弹特效剩余帧数,0 表示无特效

    int   fps;         // 实测帧率(HUD 显示)
    int   fpsCount;
    DWORD fpsTick;
};
