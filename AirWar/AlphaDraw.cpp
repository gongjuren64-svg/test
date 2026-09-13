#include "AlphaDraw.h"

void DrawAlpha(int x, int y, IMAGE* img)
{
    DrawAlpha(x, y, 0, 0, img->getwidth(), img->getheight(), img);
}

void DrawAlpha(int x, int y, int sx, int sy, int width, int height, IMAGE* img)
{
    DWORD* dst = GetImageBuffer();      // 当前绘图缓冲(配合 BatchDraw)
    DWORD* src = GetImageBuffer(img);

    int sw = img->getwidth();
    int sh = img->getheight();

    int dw = getwidth();
    int dh = getheight();

    for (int iy = 0; iy < height; iy++)
    {
        for (int ix = 0; ix < width; ix++)
        {
            // 源图越界保护
            if (sx + ix < 0 || sx + ix >= sw) continue;
            if (sy + iy < 0 || sy + iy >= sh) continue;

            int dx = x + ix;
            int dy = y + iy;

            // 窗口越界保护
            if (dx < 0 || dx >= dw) continue;
            if (dy < 0 || dy >= dh) continue;

            DWORD s = src[(sy + iy) * sw + (sx + ix)];

            BYTE sa = (s >> 24) & 0xff;   // 源像素透明度
            if (sa == 0) continue;

            DWORD d = dst[dy * dw + dx];

            // 按 alpha 混合源色与底色(读写通道顺序一致,可直接用 RGB 宏)
            BYTE sr = GetRValue(s), sg = GetGValue(s), sb = GetBValue(s);
            BYTE dr = GetRValue(d), dg = GetGValue(d), db = GetBValue(d);

            BYTE r = (sr * sa + dr * (255 - sa)) / 255;
            BYTE g = (sg * sa + dg * (255 - sa)) / 255;
            BYTE b = (sb * sa + db * (255 - sa)) / 255;

            dst[dy * dw + dx] = RGB(r, g, b);
        }
    }
}
