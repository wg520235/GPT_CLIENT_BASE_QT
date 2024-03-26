#include <QDataStream>
#include <QAudioFormat>


#include "AudioRecorder.h"

AudioRecorder::AudioRecorder(QObject *parent) : QObject(parent)
{
    // 定义音频格式
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    // 检查默认音频输入设备对该格式的支持
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format)) {
        qWarning("Default format not supported, trying to use the nearest.");
        format = info.nearestFormat(format);
    }
}

AudioRecorder::~AudioRecorder()
{
    stopRecording();
}

void AudioRecorder::startRecording(const QString &fileName)
{
    if (audioInput) {
        stopRecording(); // 如果已经在录音，先停止当前的录音
    }

    outputFile.setFileName(fileName);
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning("Failed to open file for recording");
        return;
    }

    audioInput = new QAudioInput(format, this);
    audioInput->start(&outputFile);
}

void AudioRecorder::stopRecording()
{
    if (audioInput) {
        audioInput->stop();
        outputFile.close();
        delete audioInput;
        audioInput = nullptr;
    }
}

// 函数: 将PCM数据保存为WAV文件
void AudioRecorder::savePCMAsWAV(const QString &pcmFilePath, const QString &wavFilePath,
                  int sampleRate, int channels, int bitDepth) {
    QFile pcmFile(pcmFilePath);
    QFile wavFile(wavFilePath);

    if (!pcmFile.open(QIODevice::ReadOnly)) {
        qWarning("Cannot open PCM file");
        return;
    }

    if (!wavFile.open(QIODevice::WriteOnly)) {
        qWarning("Cannot open WAV file");
        return;
    }

    // 读取PCM数据
    QByteArray pcmData = pcmFile.readAll();
    pcmFile.close();

    // 准备WAV文件头
    WAVHeader wavHeader;
    wavHeader.num_channels = channels;
    wavHeader.sample_rate = sampleRate;
    wavHeader.bit_depth = bitDepth;
    wavHeader.byte_rate = (sampleRate * channels * bitDepth) / 8;
    wavHeader.sample_alignment = (channels * bitDepth) / 8;
    wavHeader.data_bytes = pcmData.size();
    wavHeader.wav_size = 36 + wavHeader.data_bytes;

    // 写入WAV文件
    QDataStream out(&wavFile);
    out.writeRawData(reinterpret_cast<const char*>(&wavHeader), sizeof(WAVHeader));
    out.writeRawData(pcmData.constData(), pcmData.size());

    wavFile.close();
}

