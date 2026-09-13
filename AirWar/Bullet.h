#pragma once

#include "GameObject.h"

// 玩家子弹:继承 GameObject,向上直飞,飞出屏幕后删除
class Bullet : public GameObject
{
public:
    // 构造函数重载:向量坐标 / 一对数值坐标 都能创建
    explicit Bullet(const Vec2& pos);
    Bullet(float x, float y);

    virtual void Update() override;   // 重写:基类匀速移动 + 出界判定
    virtual void Draw() override;
};
