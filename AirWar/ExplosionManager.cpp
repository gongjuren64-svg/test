#include "ExplosionManager.h"

#include <algorithm>

#include "Sound.h"

void ExplosionManager::Add(int x, int y)
{
    Add(Vec2((float)x, (float)y));      // 转发给向量版本的重载
}

void ExplosionManager::Add(const Vec2& pos)
{
    explosions.emplace_back(pos);
    Sound::PlayExplosion();             // 每个爆炸都伴随音效
}

void ExplosionManager::Update()
{
    for (auto& explosion : explosions)
        explosion.Update();

    // 删除播放完的爆炸
    explosions.erase(
        std::remove_if(explosions.begin(), explosions.end(),
            [](const Explosion& e) { return !e.IsAlive(); }),
        explosions.end());
}

void ExplosionManager::Draw()
{
    for (auto& explosion : explosions)
        explosion.Draw();
}
