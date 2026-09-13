#pragma once

#include <graphics.h>

// 带透明通道的图片绘制(EasyX 的 putimage 不支持 PNG 透明)
// 两个版本构成函数重载:整张绘制 / 子区域绘制

// 绘制整张图片
void DrawAlpha(int x, int y, IMAGE* img);

// 绘制图片的一个子区域(精灵图裁剪用)
// (sx, sy) 为源图裁剪起点,width/height 为裁剪大小
void DrawAlpha(int x, int y, int sx, int sy, int width, int height, IMAGE* img);
