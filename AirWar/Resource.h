#pragma once

#include <graphics.h>

// 资源类:所有图片在程序启动时一次性加载,其它类只用不加载
class Resource
{
public:
    static void Load();

    static IMAGE player1;       // 64x64
    static IMAGE player2;
    static IMAGE player3;

    static IMAGE player1Big;    // 80x80,开始界面选中时的放大版
    static IMAGE player2Big;
    static IMAGE player3Big;

    // 六种敌机:0~2 侦察机(小)/ 3~4 突击机(中)/ 5 轰炸机(大)
    static IMAGE enemies[6];

    static IMAGE bullet;        // 16x32
    static IMAGE bg;            // 800 宽,可无缝纵向循环滚动
    static IMAGE explosion;     // 512x64,一行 8 帧,每帧 64x64

    // 按机型取玩家图,type: 0/1/2
    static IMAGE* GetPlayer(int type);
    static IMAGE* GetPlayerBig(int type);

    // 按编号取敌机图,index: 0~5(越界自动收拢)
    static IMAGE* GetEnemy(int index);
};
