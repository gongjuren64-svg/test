#pragma once

#include "Vec2.h"

// 所有游戏物体的抽象基类(继承体系的根)
// 派生类:Player / Bullet / Enemy(及其三个子类)/ Explosion
// 统一持有 位置 + 速度 + 存活标志,更新与绘制通过虚函数多态分派
class GameObject
{
public:
    GameObject() : alive(true) {}
    GameObject(const Vec2& pos, const Vec2& vel)
        : pos(pos), vel(vel), alive(true) {}

    // 虚析构:保证通过基类指针 delete 派生对象时行为正确
    virtual ~GameObject() {}

    // 默认行为:按速度匀速移动;派生类可重写出自己的移动方式(多态)
    virtual void Update() { pos += vel; }

    // 纯虚函数:每种物体的画法不同,必须由派生类实现
    virtual void Draw() = 0;

    bool IsAlive() const { return alive; }
    void Die()           { alive = false; }

    int GetX() const { return (int)(pos.x + 0.5f); }
    int GetY() const { return (int)(pos.y + 0.5f); }
    const Vec2& GetPos() const { return pos; }

    // ---------- 圆形碰撞检测(函数重载:与物体撞 / 与点撞) ----------

    bool CollideWith(const GameObject& other, float radius) const
    {
        return (pos - other.pos).LengthSq() <= radius * radius;
    }

    bool CollideWith(const Vec2& point, float radius) const
    {
        return (pos - point).LengthSq() <= radius * radius;
    }

protected:
    Vec2 pos;    // 中心位置
    Vec2 vel;    // 每帧速度
    bool alive;  // false 后由所属管理器统一删除
};
