#pragma once

#include <vector>

#include "Explosion.h"

// 爆炸管理器:统一管理多个同时播放的爆炸动画
class ExplosionManager
{
public:
    // 函数重载:整数坐标 / 向量坐标 都能添加爆炸
    // 添加的同时播放爆炸音效
    void Add(int x, int y);
    void Add(const Vec2& pos);

    void Update();
    void Draw();

private:
    std::vector<Explosion> explosions;
};
