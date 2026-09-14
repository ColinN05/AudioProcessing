#pragma once

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QFileDialog>

class AudioPlayer : public QWidget
{
public:
    AudioPlayer(const QString& title, QWidget* parent = nullptr);
private:
    QMediaPlayer* m_MediaPlayer;
    QAudioOutput* m_AudioOutput;
    QPushButton* m_PlayButton, *m_FileButton;
    QSlider* m_ProgressSlider;
    QVBoxLayout* m_Layout;
};