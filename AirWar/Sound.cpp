#include "Sound.h"

#include <Windows.h>
#include <mmsystem.h>
#include <tchar.h>
#include <cstdio>

#pragma comment(lib, "winmm.lib")

namespace
{
    // 短音效的通道数:同名音效轮流用 shoot0..shoot3 播放,
    // 快速连发时新声音不会把上一个截断
    const int SHOOT_CHANNELS = 4;
    const int BOOM_CHANNELS  = 4;
    const int POWER_CHANNELS = 2;   // 道具音效两通道:连续拾取不互相截断

    int shootIndex = 0;
    int boomIndex  = 0;
    int powerIndex = 0;

    // 打开一个音频文件并起别名
    // type mpegvideo:让 wav 也走 DirectShow,支持多实例混音与 repeat 循环
    void MciOpen(LPCTSTR file, LPCTSTR alias, int volume)
    {
        TCHAR cmd[256];

        _stprintf_s(cmd, _T("open \"%s\" type mpegvideo alias %s"), file, alias);
        mciSendString(cmd, NULL, 0, NULL);

        _stprintf_s(cmd, _T("setaudio %s volume to %d"), alias, volume);   // 0 ~ 1000
        mciSendString(cmd, NULL, 0, NULL);
    }

    // 从头播放一次(已在播放时自动回到开头重播)
    void MciPlayFromStart(LPCTSTR alias)
    {
        TCHAR cmd[128];
        _stprintf_s(cmd, _T("play %s from 0"), alias);
        mciSendString(cmd, NULL, 0, NULL);
    }
}

void Sound::Init()
{
    TCHAR alias[32];

    MciOpen(_T("sounds/bgm.wav"), _T("bgm"), 450);

    for (int i = 0; i < SHOOT_CHANNELS; i++)
    {
        _stprintf_s(alias, _T("shoot%d"), i);
        MciOpen(_T("sounds/shoot.wav"), alias, 320);
    }

    for (int i = 0; i < BOOM_CHANNELS; i++)
    {
        _stprintf_s(alias, _T("boom%d"), i);
        MciOpen(_T("sounds/explode.wav"), alias, 550);
    }

    for (int i = 0; i < POWER_CHANNELS; i++)
    {
        _stprintf_s(alias, _T("power%d"), i);
        MciOpen(_T("sounds/powerup.wav"), alias, 500);
    }

    MciOpen(_T("sounds/select.wav"), _T("select"), 400);
}

void Sound::Shutdown()
{
    mciSendString(_T("close all"), NULL, 0, NULL);
}

void Sound::PlayBGM()
{
    mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);
}

void Sound::StopBGM()
{
    mciSendString(_T("stop bgm"), NULL, 0, NULL);
}

void Sound::PlayShoot()
{
    TCHAR alias[32];
    _stprintf_s(alias, _T("shoot%d"), shootIndex);

    shootIndex = (shootIndex + 1) % SHOOT_CHANNELS;   // 通道轮转
    MciPlayFromStart(alias);
}

void Sound::PlayExplosion()
{
    TCHAR alias[32];
    _stprintf_s(alias, _T("boom%d"), boomIndex);

    boomIndex = (boomIndex + 1) % BOOM_CHANNELS;
    MciPlayFromStart(alias);
}

void Sound::PlaySelect()
{
    MciPlayFromStart(_T("select"));
}

void Sound::PlayPowerUp()
{
    TCHAR alias[32];
    _stprintf_s(alias, _T("power%d"), powerIndex);

    powerIndex = (powerIndex + 1) % POWER_CHANNELS;
    MciPlayFromStart(alias);
}
