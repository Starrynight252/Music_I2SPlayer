/*music_i2splayer.h
 * 文件最后修改时间：2026-10-28
 * 该文件定义了音乐播放器的抽象类型 Music_I2SPlayer 类。
 * 该类提供了与音乐播放器相关的功能，包括初始化、播放、停止、设置音量和增益等操作。
 * 该类使用 HardwareSerial 进行串口通信，并通过发送指令与音乐播放器进行交互。
*/


#ifndef MUSIC_I2SPLAYER_H
#define MUSIC_I2SPLAYER_H

#include "HardwareSerial.h"
#include "Music_Comms.h"

/* 音乐播放 抽象类型 */
class Music_I2SPlayer
{
private:
    // 获取指令返回值缓冲区
    String respBuffer = "";
    // mp3 播放标志位 0停止/空闲 1 播放 2录音
    volatile int isPlaying = false;
    // WVA_RECORD_ 播放标志位 0停止/空闲 1 播放 2录音
    volatile short WVA_RECORD_PlayerBit = 0;
    HardwareSerial *serials;

    /*
     * 发送指令：
     */
    inline void sendCommand(const char *cmm)
    {
        if (cmm != nullptr)
        {
            serials->println(cmm);
        }
    }

public:
    /*
     *默认使用 Serial2 ，在unoR4Wifil 上Serial2 作为AT串口
     */
    Music_I2SPlayer(HardwareSerial *ats)
        : serials(ats ? ats : &Serial2) {}

    Music_I2SPlayer()
        : serials(&Serial2) {}

    ~Music_I2SPlayer() {}

    /*
     *  初始化音乐结构，串口也会在这初始化
     * @param none
     * @return void
     */
    void begin();

    /* 重新初始化播放器
     * @param true 初始化成功，false 初始化失败
     */
    bool music_begin();

    /*获取sd卡初始化状态
     * @param true 初始化成功，false 初始化失败
     */
    bool sdcard_begin();

    /*
     * 获取指令返回状态并且写入respBuffer缓冲区
     * @param timeoutMs 超时时间，单位毫秒，默认600ms
     * @return
     *##获取状态:
     * -3 无数据
     * -2 超时
     *##指令返回：
     * -1 指令为错误 （Music_ERR || Music_ERR_UNKNOWN）
     * 0 指令为正确没有返回值（Music_OK）
     * 1 指令正确且有返回值（Music_OK_VALUE）
     */
    int getCommandStatus(uint32_t timeoutMs);

    /*
     * 指令返回值解析为字符串类型,在使用该函数之前，请确保已经调用过 getCommandStatus() 函数，并且返回值为 1（即指令正确且有返回值）。
     * 无参数。
     * @return
     * 缓冲区 respBuffer 中的返回值被解析为字符串类型并返回。
     * 如果 respBuffer 中的内容无法解析为字符串，将返回空字符串。
     */
    char *getCommandStatusString();

    /*
     * 指令返回值解析为整数类型,在使用该函数之前，请确保已经调用过 getCommandStatus() 函数，并且返回值为 1（即指令正确且有返回值）。
     * 无参数。
     * @return
     * 缓冲区 respBuffer 中的返回值被解析为整数类型并返回。
     * 如果 respBuffer 中的内容无法解析为整数，将返回 0。
     */
    int getCommandStatusInt();

    /*
     * 指令返回值解析为浮点数类型,在使用该函数之前，请确保已经调用过 getCommandStatus() 函数，并且返回值为 1（即指令正确且有返回值）。
     * 无参数。
     * @return
     * 缓冲区 respBuffer 中的返回值被解析为浮点数类型并返回。
     * 如果 respBuffer 中的内容无法解析为浮点数，将返回 0.0
     */
    float getCommandStatusFloat();

    /**
     * 重新加载音频文件，并且停止播放。
     * 如果之前调用过 FilePath 或 FileName 修改了播放文件，则加载修改后的文件；
     * 如果未修改过文件，则加载默认文件 "test.mp3"。。
     *
     * @param none
     *
     * 注意：调用此函数前请确保指定的文件存在，否则可能无法播放。
     */
    void Reload();

    /*
     * 停止播放音乐，并且关闭功放
     * @param enable 是否使能功放关闭，默认关闭
     * @return void
     */
    void stop();

    /*
     * 开始播放音乐，并且打开功放
     * @param enable 是否使能功放打开，默认打开
     * @return void
     */
    void play();

    /*
     * 设置播放文件路径+文件名
     * @param file 文件路径+文件名字符串指针
     * @return 见getCommandStatus() 函数
     *
     * 参数不要换行符直接字符串字面量就可以！！
     * 例如： "/music/test.mp3"
     */
    int FilePath(const char *file);

    /*
     * 设置播放文件名（路径使用默认）
     * @param file 文件名字符串指针
     * @return 见getCommandStatus() 函数 见getCommandStatus() 函数
     *
     * 参数 不要换行符直接字符串字面量就可以！！
     * 例如： "test.mp3"
     */
    int FileName(const char *file);

    /*
     * 设置增益
     * @param gain 增益，范围0~100
     * @return 见getCommandStatus() 函数
     */
    int SetGain(int gain);

    /*
     * 获取当前增益
     * @param none
     * @return 见getCommandStatus() 函数
     */
    int GetGain();

    /*
     * 设置音量
     * @param vol 音量，0~100
     * @return 见getCommandStatus() 函数
     */
    int SetVolume(int vol);

    /*
     * 获取当前音量
     * @param none
     * @return 见getCommandStatus() 函数
     */
    int GetVolume();

    // 获取环境声音/麦克风音量，返回 0~100
    int Environmental_sound();

    /*
     * 是否正在播放
     * @param none
     * @return
     * 见getCommandStatus() 函数
     * 其中：
     * 1 正在播放，未播放返回其他
     */
    int IsPlaying();

    // 设置WVA_RECORD_路径
    int FilePath_WVA_RECORD(const char *filePath);
    // 默认路径设置文件名称
    int FileName_WVA_RECORD(const char *file);

    // 重新加载WVA_RECORD_音频文件，并且停止播放。
    int Reload_WVA_RECORD();
    // 开始录音
    int StartRecording_WVA_RECORD(int durationSeconds);
    // 播放录音
    int PlayRecording_WVA_RECORD();
    // 停止播放/录音
    void StopRecording_WVA_RECORD();
    /*
     * 是否正在播放
     * @param none
     * @return 见getCommandStatus() 函数
     * 其中：
     * 1 正在播放
     * 2 录音中
     * 未播放返回其他
     */
    int IsPlaying_WVA_RECORD();

    // 获取版本号
    char* GetVersion();
};

#endif
