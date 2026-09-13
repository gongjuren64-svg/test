#pragma once

// 二维向量:统一表示游戏中的位置、速度和偏移
// 通过运算符重载,让向量运算写起来像内置类型一样自然
struct Vec2
{
    float x;
    float y;

    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(float x, float y) : x(x), y(y) {}

    // ---------- 运算符重载 ----------

    Vec2 operator+(const Vec2& r) const { return Vec2(x + r.x, y + r.y); }
    Vec2 operator-(const Vec2& r) const { return Vec2(x - r.x, y - r.y); }
    Vec2 operator*(float s) const       { return Vec2(x * s, y * s); }

    Vec2& operator+=(const Vec2& r) { x += r.x; y += r.y; return *this; }
    Vec2& operator-=(const Vec2& r) { x -= r.x; y -= r.y; return *this; }
    Vec2& operator*=(float s)       { x *= s;   y *= s;   return *this; }

    // 长度的平方:圆形碰撞检测用平方比较即可,省一次开方
    float LengthSq() const { return x * x + y * y; }
};

// 标量在左侧的乘法 2.0f * v(全局运算符重载,与成员版本构成重载)
inline Vec2 operator*(float s, const Vec2& v) { return v * s; }
