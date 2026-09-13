#pragma once

#include "GameObject.h"

// 单个爆炸动画:继承 GameObject,8 帧精灵图逐帧播放,播完自动删除
class Explosion : public GameObject
{
public:
    explicit Explosion(const Vec2& pos);

    virtual void Update() override;   // 重写:原地不动,只推进动画帧
    virtual void Draw() override;

private:
    int frame;   // 当前帧 0~7
    int timer;   // 帧间计时
};
