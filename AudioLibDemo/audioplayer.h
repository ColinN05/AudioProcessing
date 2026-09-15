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
    AudioPlayer(const QString& title, bool output = false, QWidget* parent = nullptr);
    void setSource(const std::string& source);
    QUrl getSource() const { return m_MediaPlayer->source(); }
private:
    bool m_Output;
    QMediaPlayer* m_MediaPlayer;
    QAudioOutput* m_AudioOutput;
    QPushButton* m_PlayButton, *m_FileButton;
    QSlider* m_ProgressSlider;
    QVBoxLayout* m_Layout;
};