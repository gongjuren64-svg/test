#pragma once

#include "GameObject.h"

// 射速强化道具:敌机被击毁后按概率掉落,缓慢下落并左右漂移,
// 玩家接住后提升射速(满级后折算为奖励分),漏接出界自动删除
class PowerUp : public GameObject
{
public:
    explicit PowerUp(const Vec2& pos);

    virtual void Update() override;   // 重写:匀速下落 + 正弦漂移 + 出界删除
    virtual void Draw() override;     // 呼吸光环 + 闪电图标(纯图形绘制,不占用图片资源)

private:
    float phase;   // 漂移与光环呼吸的相位
};
