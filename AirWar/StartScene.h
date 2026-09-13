#pragma once

#include "Scene.h"

// 开始界面:继承 Scene,滚动背景 + 标题 + 三架飞机选择
// 左右键切换,Enter 开始,Esc 退出
class StartScene : public Scene
{
public:
    StartScene();

    // 场景结束后取选中的机型 0/1/2
    int GetPlaneType() const;

protected:
    virtual void Update() override;
    virtual void Draw() override;

private:
    int planeIndex;     // 当前选中的飞机 0/1/2
};
