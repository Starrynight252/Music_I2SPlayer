#include <SPI.h>
#include <Wire.h>
#include "Music_I2SPlayer.h"

int Music_I2SPlayer::getCommandStatus(uint32_t timeoutMs = 600)
{
    unsigned long start = millis();
    String resp = "";

    while (serials->available())
    {
        char c = serials->read();

        // 行结束
        if (c == '\n' || c == '\r')
        {
            break;
        }
        else
        {
            resp += c;
        }

        if (start && (millis() - start >= timeoutMs))
        {
            return -2;
        }
        delay(1);
    }

    if (resp.length() > 0)
    {
        if (resp.indexOf(Music_ERR) != -1)
        {
            return -1;
        }
        else if (resp.indexOf(Music_OK) != -1)
        {
            return 0;
        }
        else
        {
            size_t pos = strlen(Music_OK_VALUE);
            return atoi(resp.substring(pos).c_str());
        }
    }
    else
    {
        return -3;
    }

    return -3; // 防止未定义返回
}

void Music_I2SPlayer::begin()
{
    // mp3 播放标志位 0停止/空闲 1 播放 2录音
    isPlaying = false;
    // PCM 播放标志位 0停止/空闲 1 播放 2录音
    PcmPlayerBit = 0;
    serials->begin(115200);
}

/**
 * 重新加载音频文件，并且停止播放。
 * 如果之前调用过 FilePath 或 FileName 修改了播放文件，则加载修改后的文件；
 * 如果未修改过文件，则加载默认文件 "test.mp3"。。
 *
 * @param none
 * 注意：调用此函数前请确保指定的文件存在，否则可能无法播放。
 */
void Music_I2SPlayer::Reload()
{
    sendCommand(Music_CMD(Music_RELOAD));
}
/*
 * 停止播放音乐，并且关闭功放
 * @param enable 是否使能功放关闭，默认关闭
 * @return 见getCommandStatus() 函数
 */
void Music_I2SPlayer::stop()
{
    sendCommand(Music_CMD(Music_STOP));
}

/*
 * 开始播放音乐，并且打开功放
 * @param enable 是否使能功放打开，默认打开
 * @return 见getCommandStatus() 函数
 */
void Music_I2SPlayer::play()
{
    sendCommand(Music_CMD(Music_PLAY));
}

int Music_I2SPlayer::FilePath(const char *file)
{
    if (file == nullptr)
    {
        return -4;
    }

    String cmm = Music_CMD(Music_FILEPATH);
    cmm += file;
    sendCommand(cmm.c_str());

    return getCommandStatus();
}

/*
 * 设置播放文件名（路径使用默认）
 * @param file 文件名字符串指针
 * @return 见getCommandStatus() 函数
 */
int Music_I2SPlayer::FileName(const char *file)
{
    if (file == nullptr)
    {
        return -4; // false;
    }

    String cmm = Music_CMD(Music_FILENAME);
    cmm += file;
    sendCommand(cmm.c_str());

    return getCommandStatus();
}

int Music_I2SPlayer::SetGain(int gain)
{
    String cmm = Music_CMD(Music_GAIN);
    cmm += String(gain);
    sendCommand(cmm.c_str());

    return getCommandStatus();
}

/*
 * 获取当前增益
 * @param none
 * @return 见getCommandStatus() 函数
 */
int Music_I2SPlayer::GetGain()
{
    sendCommand(Music_CMD(Music_GAIN));
    return getCommandStatus();
}

/*
 * 设置音量
 * @param vol 音量，0~100
 * @return 见getCommandStatus() 函数
 */
int Music_I2SPlayer::SetVolume(int vol)
{
    String cmm = Music_CMD(Music_VOLUME);
    cmm += String(vol);
    sendCommand(cmm.c_str());

    return getCommandStatus();
}

/*
 * 获取当前音量
 * @param none
 * @return 见getCommandStatus() 函数
 */
int Music_I2SPlayer::GetVolume()
{
    sendCommand(Music_CMD(Music_VOLUME));
    return getCommandStatus();
}

// 设置pcm路径
int Music_I2SPlayer::FilePath_PCM(const char *filePath)
{
    if (filePath == nullptr)
    {
        return -4;
    }

    String cmm = PCM_CMD(Music_FILEPATH);
    cmm += filePath;
    sendCommand(cmm.c_str());

    return getCommandStatus();
}

// 默认路径设置文件名称
int Music_I2SPlayer::FileName_PCM(const char *file)
{
    if (file == nullptr)
    {
        return -4; // false;
    }

    String cmm = PCM_CMD(Music_FILENAME);
    cmm += file;
    sendCommand(cmm.c_str());

    return getCommandStatus();
}

/* 是否正在播放
 * @param none
 * @return
 */
int Music_I2SPlayer::IsPlaying()
{
    sendCommand(Music_CMD_WRITE(Music_PLAY));

    return getCommandStatus();
}

// 重新加载PCM音频文件，并且停止播放。
int Music_I2SPlayer::Reload_PCM()
{
    sendCommand(PCM_CMD(Music_RELOAD));
    return getCommandStatus();
}

// 开始录音
int Music_I2SPlayer::StartRecording_PCM(int durationSeconds)
{
    String cmm = PCM_CMD(Music_RECORD);
    cmm += String(durationSeconds);
    sendCommand(cmm.c_str());

    return getCommandStatus();
}

// 播放录音
int Music_I2SPlayer::PlayRecording_PCM()
{
    sendCommand(PCM_CMD(Music_PLAY));
    return getCommandStatus();
}

// 停止播放/录音
void Music_I2SPlayer::StopRecording_PCM()
{
    sendCommand(PCM_CMD(Music_STOP));
}

int Music_I2SPlayer::IsPlaying_PCM()
{
    sendCommand(PCM_CMD_WRITE(Music_PLAY));
    return getCommandStatus();
}