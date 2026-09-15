#include "audioplayer.h"

#include <QLabel>

#include <QAudioDevice>
#include <QAudioFormat>
#include <QMediaDevices>

#include <iostream>

AudioPlayer::AudioPlayer(const QString& title, bool output, QWidget* parent)
    : QWidget(parent), m_Output(output)
{
    m_MediaPlayer = new QMediaPlayer(this);
    m_AudioOutput = new QAudioOutput(this);
    m_MediaPlayer->setAudioOutput(m_AudioOutput);
    m_AudioOutput->setVolume(1.0);
    
    m_PlayButton = new QPushButton("Play");
    connect(m_PlayButton, &QPushButton::clicked, this, 
    [=]() {
        if (m_MediaPlayer->source().isEmpty())
        {
            std::cout << "Cannot play. No source has been selected.\n";
            return;
        }

        if (m_MediaPlayer->playbackState() == QMediaPlayer::PlayingState) 
        {
            m_MediaPlayer->pause();
            m_PlayButton->setText("Play");
        }
        else 
        {
            m_MediaPlayer->play();
            m_PlayButton->setText("Pause");
        }
    });

    if (!m_Output)
    {
        m_FileButton = new QPushButton("Select File");

        connect(m_FileButton, &QPushButton::clicked, this, 
        [=]() {
            QString fileName = QFileDialog::getOpenFileName(this,"Select Audio File",QString(),"Audio Files (*.wav);;All Files (*)");

            if (!fileName.isEmpty()) 
            {
                m_MediaPlayer->setSource(QUrl::fromLocalFile(fileName));
            }
        });
    }

    m_ProgressSlider = new QSlider(Qt::Horizontal);
    m_ProgressSlider->setRange(0, 0);

    connect(m_MediaPlayer, &QMediaPlayer::durationChanged, this,
    [=](qint64 duration) 
    {
        m_ProgressSlider->setRange(0, duration);
    });

    connect(m_MediaPlayer, &QMediaPlayer::positionChanged, this,
    [=](qint64 position) 
    {
        m_ProgressSlider->setValue(position);

        if (m_ProgressSlider->value() >= m_ProgressSlider->maximum())
        {
            m_ProgressSlider->setValue(m_ProgressSlider->maximum());
            m_MediaPlayer->pause();
            m_PlayButton->setText("Play");
        }
    });

    connect(m_ProgressSlider, &QSlider::sliderMoved, m_MediaPlayer, &QMediaPlayer::setPosition);

    m_Layout = new QVBoxLayout(this);
    m_Layout->addWidget(new QLabel(title));
    if (!m_Output)
    {
        m_Layout->addWidget(m_FileButton);
    }
    m_Layout->addWidget(m_PlayButton);
    m_Layout->addWidget(m_ProgressSlider);
}

void AudioPlayer::setSource(const std::string& source)
{
    m_MediaPlayer->stop();
    m_MediaPlayer->setSource(QUrl::fromLocalFile(QString::fromStdString(source)));
}