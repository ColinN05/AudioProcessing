#include "audiolibdemo.h"
#include "./ui_audiolibdemo.h"

#include "card.h"
#include "effectswidget.h"

#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollArea>
#include <QSlider>
#include <QTimer>

#include <iostream>

AudioLibDemo::AudioLibDemo(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AudioLibDemo)
{
    ui->setupUi(this);

    QScrollArea* scrollArea = new QScrollArea(this);
    ui->horizontalLayout->insertWidget(0,scrollArea);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMaximumSize(250,1'000'000);
    QWidget* content = new QWidget();
    m_EffectCardLayout = new QVBoxLayout(content);

    scrollArea->setWidget(content);

    createGainEffectCard();
    createConvolutionReverbCard();
    createNoiseCard();
    createBandPassFilterCard();
    createPanEffectCard();
    createMonoToStereoCard();
    createStereoToMonoCard();
    createClipCard();
    createResampleCard();
    createCompressorCard();

    auto* m_OutputStreamBuf = new TextEditStreamBuf(ui->output);
    std::cout.rdbuf(m_OutputStreamBuf);
    auto* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, 
    [=]() {
        std::string text = m_OutputStreamBuf->readBuffer();
        if (!text.empty()) 
        {
            ui->output->appendPlainText(QString::fromStdString(text));
        }
    });
    timer->start(16);
}

void AudioLibDemo::createGainEffectCard()
{
    m_GainEffectCard = new Card("Gain");
    
    auto* slider = new QSlider(Qt::Horizontal);
    slider->setRange(1,300);
    slider->setValue(100);

    QLabel* valueLabel = new QLabel("gain factor: 1.00");

    QObject::connect(slider, &QSlider::valueChanged,
    [valueLabel](int value) 
    {
        valueLabel->setText(QString("gain factor: ") + QString::number(value / 100.0, 'f', 2));
    });

    m_GainEffectCard->addWidget(valueLabel);
    m_GainEffectCard->addWidget(slider);
    m_EffectCardLayout->addWidget(m_GainEffectCard, 0, Qt::AlignTop);
}

void AudioLibDemo::createMonoToStereoCard()
{
    m_MonoToStereoCard = new Card("Mono to Stereo");
    m_EffectCardLayout->addWidget(m_MonoToStereoCard, 0, Qt::AlignTop);
}

void AudioLibDemo::createStereoToMonoCard()
{
    m_StereoToMonoCard = new Card("Stereo to Mono");
    m_EffectCardLayout->addWidget(m_StereoToMonoCard, 0, Qt::AlignTop);
}

void AudioLibDemo::createPanEffectCard()
{
    m_PanEffectCard = new Card("Pan");

    auto* slider = new QSlider(Qt::Horizontal);
    slider->setRange(0,90);
    slider->setValue(45);

    QLabel* valueLabel = new QLabel("angle: 45");

    QObject::connect(slider, &QSlider::valueChanged,
    [valueLabel](int value) {
        valueLabel->setText(QString("angle: ") + QString::number(value, 'f', 2));
    });

    m_PanEffectCard->addWidget(valueLabel);
    m_PanEffectCard->addWidget(slider);

    m_EffectCardLayout->addWidget(m_PanEffectCard, 0, Qt::AlignTop);
}

void AudioLibDemo::createClipCard()
{
    m_ClipCard = new Card("Clip");
    
    auto* startSlider = new QSlider(Qt::Horizontal);
    startSlider->setRange(0,100);
    startSlider->setValue(0);

    QLabel* startValueLabel = new QLabel("start: 0%");

    QObject::connect(startSlider, &QSlider::valueChanged,
    [startValueLabel](int value) {
        startValueLabel->setText(QString("start: ") + QString::number(value, 'f', 2) + QString("%"));
    });

    m_ClipCard->addWidget(startValueLabel);
    m_ClipCard->addWidget(startSlider);

    auto* endSlider = new QSlider(Qt::Horizontal);
    endSlider->setRange(0,100);
    endSlider->setValue(100);

    QLabel* endValueLabel = new QLabel("end: 100%");

    QObject::connect(endSlider, &QSlider::valueChanged,
    [endValueLabel](int value) {
        endValueLabel->setText(QString("end: ") + QString::number(value, 'f', 2) + QString("%"));
    });

    m_ClipCard->addWidget(endValueLabel);
    m_ClipCard->addWidget(endSlider);

    m_EffectCardLayout->addWidget(m_ClipCard, 0, Qt::AlignTop);
}

void AudioLibDemo::createResampleCard()
{
    m_ResampleCard = new Card("Resample");

    auto* slider = new QSlider(Qt::Horizontal);
    slider->setRange(1'000,48'000);
    slider->setValue(24'000);

    QLabel* valueLabel = new QLabel("target sample rate: 24000hz");

    QObject::connect(slider, &QSlider::valueChanged,
    [valueLabel](int value) {
        valueLabel->setText(QString("target sample rate: ") + QString::number(value) + QString("hz"));
    });

    m_ResampleCard->addWidget(valueLabel);
    m_ResampleCard->addWidget(slider);

    m_EffectCardLayout->addWidget(m_ResampleCard, 0, Qt::AlignTop);
}

void AudioLibDemo::createCompressorCard()
{
    m_CompressorCard = new Card("Compressor");
    
    // threshold
    {
    auto* slider = new QSlider(Qt::Horizontal);
    slider->setRange(1,100);
    slider->setValue(50);

    QLabel* valueLabel = new QLabel("threshold: 0.50");

    QObject::connect(slider, &QSlider::valueChanged,
    [valueLabel](int value) {
        valueLabel->setText(QString("threshold: ") + QString::number(value/100.0f, 'f', 2));
    });

    m_CompressorCard->addWidget(valueLabel);
    m_CompressorCard->addWidget(slider);
    }

    // attack
    {
    auto* slider = new QSlider(Qt::Horizontal);
    slider->setRange(1, 200);
    slider->setValue(25);

    QLabel* valueLabel = new QLabel("attack: 0.25s");

    QObject::connect(slider, &QSlider::valueChanged,
    [valueLabel](int value) {
        valueLabel->setText(QString("attack: ") + QString::number(value/100.0f, 'f', 2) + QString("s"));
    });

    m_CompressorCard->addWidget(valueLabel);
    m_CompressorCard->addWidget(slider);
    }

    m_EffectCardLayout->addWidget(m_CompressorCard, 0, Qt::AlignTop);
}

void AudioLibDemo::createBandPassFilterCard()
{
    m_BandPassFilterCard = new Card("Band Pass Filter");
    
    // low freq
    {
    auto* slider = new QSlider(Qt::Horizontal);
    slider->setRange(0,10'000);
    slider->setValue(0);

    QLabel* valueLabel = new QLabel("low frequency: 0hz");

    QObject::connect(slider, &QSlider::valueChanged,
    [valueLabel](int value) {
        valueLabel->setText(QString("low frequency: ") + QString::number(value) + QString("hz"));
    });

    m_BandPassFilterCard->addWidget(valueLabel);
    m_BandPassFilterCard->addWidget(slider);
    }

    // high freq
    {
    auto* slider = new QSlider(Qt::Horizontal);
    slider->setRange(0,10'000);
    slider->setValue(10'000);

    QLabel* valueLabel = new QLabel("high frequency: 10000hz");

    QObject::connect(slider, &QSlider::valueChanged,
    [valueLabel](int value) {
        valueLabel->setText(QString("high frequency: ") + QString::number(value) + QString("hz"));
    });

    m_BandPassFilterCard->addWidget(valueLabel);
    m_BandPassFilterCard->addWidget(slider);
    }

    m_EffectCardLayout->addWidget(m_BandPassFilterCard, 0, Qt::AlignTop);
}

void AudioLibDemo::createConvolutionReverbCard()
{
    m_ConvolutionReverbCard = new Card("Convolution Reverb");

    auto* lightOption = new QRadioButton("light");
    auto* heavyOption = new QRadioButton("heavy");
    lightOption->setChecked(true);
    m_ConvolutionReverbCard->addWidget(lightOption);
    m_ConvolutionReverbCard->addWidget(heavyOption);
    m_EffectCardLayout->addWidget(m_ConvolutionReverbCard, 0, Qt::AlignTop);
}

void AudioLibDemo::createNoiseCard()
{
    m_NoiseCard = new Card("Noise");

    auto* whiteOption = new QRadioButton("white");
    auto* brownianOption = new QRadioButton("brownian");
    whiteOption->setChecked(true);
    m_NoiseCard->addWidget(whiteOption);
    m_NoiseCard->addWidget(brownianOption);

    m_EffectCardLayout->addWidget(m_NoiseCard);
}

AudioLibDemo::~AudioLibDemo()
{
    delete m_OutputStreamBuf;
    delete ui;
}
