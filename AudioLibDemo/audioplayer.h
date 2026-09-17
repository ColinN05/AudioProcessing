#pragma once

#include "audioplayergraph.h"

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QFileDialog>

#ifdef Q_OS_LINUX
    #define AUDIO_FILEPATH_STRIP 7
#elif defined(Q_OS_WIN) 
    #define AUDIO_FILEPATH_STRIP 8
#else
    static_assert(false && "Unsupported platform!");
#endif

class AudioPlayer : public QWidget
{
public:
    AudioPlayer(const QString& title, bool output = false, QWidget* parent = nullptr);
    void setSource(const std::string& source);
    QUrl getSource() const { return m_MediaPlayer->source(); }
    void stop() { m_MediaPlayer->stop(); }
private:
    bool m_Output;
    QMediaPlayer* m_MediaPlayer;
    QAudioOutput* m_AudioOutput;
    QPushButton* m_PlayButton, *m_FileButton;
    QSlider* m_ProgressSlider;
    QVBoxLayout* m_Layout;
    AudioPlayerGraph* m_Graph;
};