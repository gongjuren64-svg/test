#include "Game.h"

#include <graphics.h>
#include <tchar.h>
#include <cstdio>
#include <algorithm>

#include "Resource.h"
#include "Sound.h"

Game::Game(int planeType)
    : player(planeType)
{
    score = 0;
    destroyed = 0;
    overTimer = 0;

    bombs = 3;              // 开局自带三枚全屏炸弹
    nextBombScore = 1500;   // 之后每 1500 分奖励一枚
    bombFlash = 0;

    fps = 0;
    fpsCount = 0;
    fpsTick = GetTickCount();

    result = GameState::GAMEOVER;   // 一局结束后进入结算界面
}

int Game::FrameMs() const
{
    return 15;
}

void Game::Update()
{
    player.Update();
    enemyManager.Update();

    // 子弹打敌机:战果用 HitResult 的 += 逐帧累加(运算符重载)
    HitResult hits = enemyManager.CheckBulletCollision(
        player.GetBullets(), explosionManager, powerups);

    score += hits.score;
    destroyed += hits.killed;

    // Q 键刚按下:引爆全屏炸弹(边沿检测,按住不放只触发一次)
    if (KeyPressed('Q'))
        UseBomb();

    UpdatePowerUps();

    CheckPlayerCollision();

    explosionManager.Update();

    // 分数达到奖励线:补发一枚炸弹
    if (score >= nextBombScore)
    {
        bombs++;
        nextBombScore += 1500;
        Sound::PlaySelect();
    }

    if (bombFlash > 0)
        bombFlash--;

    ScrollBackground(2);

    // 玩家死亡后再跑一小段,让爆炸动画播完
    if (!player.IsAlive())
    {
        overTimer++;
        if (overTimer >= 70)
            running = false;
    }

    // FPS 统计
    fpsCount++;
    DWORD now = GetTickCount();
    if (now - fpsTick >= 1000)
    {
        fps = fpsCount;
        fpsCount = 0;
        fpsTick = now;
    }
}

void Game::CheckPlayerCollision()
{
    if (!player.IsAlive() || player.IsInvincible())
        return;

    for (auto& enemy : enemyManager.GetEnemies())
    {
        if (!enemy->IsAlive())
            continue;

        // 圆形碰撞:各机型半径不同 + 玩家机身半径
        if (player.CollideWith(*enemy, enemy->GetRadius() + 18.0f))
        {
            enemy->Die();
            explosionManager.Add(enemy->GetPos());

            player.Hit();

            // 生命归零:玩家位置也爆炸
            if (!player.IsAlive())
                explosionManager.Add(player.GetPos());

            break;
        }
    }
}

// Q 键全屏炸弹:有存货且玩家存活时,全屏敌机原地爆炸并正常计分、掉落道具
void Game::UseBomb()
{
    if (bombs <= 0 || !player.IsAlive())
        return;

    bombs--;

    HitResult hits = enemyManager.KillAll(explosionManager, powerups);
    score += hits.score;
    destroyed += hits.killed;

    bombFlash = 12;     // 触发白闪 + 冲击波特效
}

// 道具逐帧下落;玩家碰到即拾取:未满级提升射速,满级折算 50 分
void Game::UpdatePowerUps()
{
    for (auto& powerup : powerups)
    {
        powerup.Update();

        if (powerup.IsAlive() && player.IsAlive() &&
            player.CollideWith(powerup, 36.0f))
        {
            powerup.Die();
            Sound::PlayPowerUp();

            if (!player.UpgradeFireRate())
                score += 50;
        }
    }

    // 删除已拾取或漏接出界的道具
    powerups.erase(
        std::remove_if(powerups.begin(), powerups.end(),
            [](const PowerUp& p) { return !p.IsAlive(); }),
        powerups.end());
}

void Game::Draw()
{
    DrawBackground();       // 滚动循环背景

    enemyManager.Draw();

    for (auto& powerup : powerups)
        powerup.Draw();

    player.Draw();
    explosionManager.Draw();

    DrawBombEffect();
    DrawHUD();

    FlushBatchDraw();
}

// 炸弹特效:前 3 帧整屏白闪,之后从玩家位置向外扩散双层冲击波圆环
void Game::DrawBombEffect()
{
    if (bombFlash <= 0)
        return;

    if (bombFlash > 9)
    {
        setfillcolor(WHITE);
        solidrectangle(0, 0, 800, 600);
    }
    else
    {
        int r = (10 - bombFlash) * 55;

        setlinecolor(RGB(255, 250, 200));
        setlinestyle(PS_SOLID, 6);
        circle(player.GetX(), player.GetY(), r);

        setlinestyle(PS_SOLID, 2);
        circle(player.GetX(), player.GetY(), r + 16);
    }
}

void Game::DrawHUD()
{
    setbkmode(TRANSPARENT);
    SetFont(22, false, _T("Consolas"));

    TCHAR buf[64];

    settextcolor(WHITE);

    _stprintf_s(buf, _T("Score: %d"), score);
    outtextxy(20, 12, buf);

    _stprintf_s(buf, _T("Life: %d"), player.GetLives());
    outtextxy(200, 12, buf);

    _stprintf_s(buf, _T("Destroyed: %d"), destroyed);
    outtextxy(330, 12, buf);

    settextcolor(RGB(150, 160, 175));
    _stprintf_s(buf, _T("FPS: %d"), fps);
    outtextxy(700, 12, buf);

    // 第二行:炸弹存量(用光变灰)与射速强化等级
    settextcolor(bombs > 0 ? RGB(255, 220, 60) : RGB(120, 120, 120));
    _stprintf_s(buf, _T("Bomb x%d [Q]"), bombs);
    outtextxy(20, 40, buf);

    settextcolor(RGB(180, 255, 150));
    _stprintf_s(buf, _T("Rapid Lv.%d/%d"),
                player.GetFireRateLevel(), Player::MAX_FIRE_LEVEL);
    outtextxy(200, 40, buf);
}

int Game::GetScore() const
{
    return score;
}

int Game::GetDestroyed() const
{
    return destroyed;
}
