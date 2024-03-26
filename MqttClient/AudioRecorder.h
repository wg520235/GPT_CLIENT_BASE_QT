#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QObject>
#include <QAudioInput>
#include <QFile>

// WAV文件头的结构
struct WAVHeader {
    // RIFF Header
    char riff_header[4] = {'R', 'I', 'F', 'F'};
    uint32_t wav_size;
    char wave_header[4] = {'W', 'A', 'V', 'E'};

    // Format Header
    char fmt_header[4] = {'f', 'm', 't', ' '};
    uint32_t fmt_chunk_size = 16;
    uint16_t audio_format = 1;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t sample_alignment;
    uint16_t bit_depth;

    // Data Header
    char data_header[4] = {'d', 'a', 't', 'a'};
    uint32_t data_bytes;
};

class AudioRecorder : public QObject
{
    Q_OBJECT
public:
    explicit AudioRecorder(QObject *parent = nullptr);
    ~AudioRecorder();

    void startRecording(const QString &fileName);
    void stopRecording();
    void savePCMAsWAV(const QString &pcmFilePath, const QString &wavFilePath,
                      int sampleRate, int channels, int bitDepth);
signals:

private:
    QAudioInput *audioInput = nullptr;
    QFile outputFile;
    QAudioFormat format;
};

#endif // AUDIORECORDER_H
