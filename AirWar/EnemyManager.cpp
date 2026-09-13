#include "EnemyManager.h"

#include <algorithm>
#include <cstdlib>

EnemyManager::EnemyManager()
{
    spawnTimer = 0;
    frameCount = 0;
}

void EnemyManager::Update()
{
    frameCount++;
    spawnTimer++;

    // 生成间隔随时间缓慢缩短(50 帧 -> 最快 22 帧),难度渐增
    int interval = 50 - frameCount / 500;
    if (interval < 22)
        interval = 22;

    if (spawnTimer >= interval)
    {
        spawnTimer = 0;

        // 难度系数 0 ~ 1.5,随时间加快敌机速度
        float difficulty = frameCount / 4000.0f;
        if (difficulty > 1.5f)
            difficulty = 1.5f;

        // 工厂函数创建随机机型,unique_ptr 自动管理生命周期
        enemies.emplace_back(Enemy::CreateRandom(difficulty));
    }

    // 虚函数调用:侦察机直线、突击机摆动、轰炸机慢推(多态)
    for (auto& enemy : enemies)
        enemy->Update();

    // 删除死亡或飞出屏幕的敌机
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& e) { return !e->IsAlive(); }),
        enemies.end());
}

void EnemyManager::Draw()
{
    for (auto& enemy : enemies)
        enemy->Draw();
}

const std::vector<std::unique_ptr<Enemy>>& EnemyManager::GetEnemies() const
{
    return enemies;
}

HitResult EnemyManager::CheckBulletCollision(std::vector<Bullet>& bullets,
                                             ExplosionManager& explosionManager,
                                             std::vector<PowerUp>& drops)
{
    HitResult total;

    for (auto& bullet : bullets)
    {
        if (!bullet.IsAlive())
            continue;

        for (auto& enemy : enemies)
        {
            if (!enemy->IsAlive())
                continue;

            // 圆形碰撞:敌机半径 + 子弹半径
            if (bullet.CollideWith(*enemy, enemy->GetRadius() + 6.0f))
            {
                bullet.Die();

                // 击毁才有爆炸和得分,只是打掉一层 HP 则子弹白白消耗
                if (enemy->TakeHit())
                {
                    explosionManager.Add(enemy->GetPos());  // 内部同时播放爆炸音效
                    total += HitResult{ 1, enemy->GetScore() };

                    TryDropPowerUp(*enemy, drops);          // 击毁后概率掉落道具
                }
                break;      // 这颗子弹已消耗,检查下一颗
            }
        }
    }

    return total;
}

HitResult EnemyManager::KillAll(ExplosionManager& explosionManager,
                                std::vector<PowerUp>& drops)
{
    HitResult total;

    // 全屏炸弹:所有存活敌机原地爆炸,战果与掉落规则和子弹击毁一致
    for (auto& enemy : enemies)
    {
        if (!enemy->IsAlive())
            continue;

        enemy->Die();
        explosionManager.Add(enemy->GetPos());
        total += HitResult{ 1, enemy->GetScore() };

        TryDropPowerUp(*enemy, drops);
    }

    return total;
}

void EnemyManager::TryDropPowerUp(const Enemy& enemy, std::vector<PowerUp>& drops)
{
    // 掉落概率与机型分值挂钩:侦察机 15% / 突击机 20% / 轰炸机 25%
    int chance = 10 + enemy.GetScore() / 2;

    if (rand() % 100 < chance)
        drops.emplace_back(enemy.GetPos());
}
