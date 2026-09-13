#pragma once

// 音效管理:程序启动时打开所有音频通道,游戏中随时触发播放
// - BGM 单通道循环播放
// - 短音效(射击/爆炸)各开多个通道轮流播,连续触发不会互相打断
// 所有接口都是静态函数,与 Resource 的用法保持一致
class Sound
{
public:
    static void Init();             // 打开全部音频通道(main 中调用一次)
    static void Shutdown();         // 关闭全部通道(退出前调用)

    static void PlayBGM();          // 循环播放背景音乐
    static void StopBGM();

    static void PlayShoot();        // 射击音效
    static void PlayExplosion();    // 爆炸音效
    static void PlaySelect();       // 菜单切换 / 确认音效
    static void PlayPowerUp();      // 拾取道具音效
};
