#pragma once

#include "Scene.h"

// 结算界面:继承 Scene,显示本局成绩与最高分
// Enter 重新开始(同机型再来一局),Esc 返回开始菜单
class EndScene : public Scene
{
public:
    EndScene(int score, int destroyed, int bestScore, bool newRecord);

protected:
    virtual void Update() override;
    virtual void Draw() override;

private:
    int  score;
    int  destroyed;
    int  bestScore;
    bool newRecord;     // 本局是否刷新最高分
};
