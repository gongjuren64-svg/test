#pragma once

// 游戏流程状态:开始界面 -> 游戏 -> 结算界面 -> (重新开始 / 返回菜单 / 退出)
enum class GameState
{
    START,      // 开始界面(选飞机)
    PLAYING,    // 游戏进行中
    GAMEOVER,   // 结算界面
    EXIT        // 退出程序
};
