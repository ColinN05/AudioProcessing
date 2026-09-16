#pragma once

#include "AudioLib/AudioFile.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>

class AudioPlayerGraph : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public: 
    AudioPlayerGraph(QWidget* parent = nullptr);
    ~AudioPlayerGraph() = default;
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
private:
    friend class AudioPlayer;
    std::unique_ptr<AudioLib::AudioFile> m_File;
    float m_Progress;
    QTimer *m_updateTimer;
};