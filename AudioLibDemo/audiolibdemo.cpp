#include "audiolibdemo.h"
#include "./ui_audiolibdemo.h"

#include "card.h"
#include "effectswidget.h"
#include "audioplayer.h"

#include <QFileDialog>
#include <QLabel>
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
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    createGainEffectCard();
    createConvolutionReverbCard();
    createNoiseCard();
    createBandPassFilterCard();
    createPanEffectCard();
    // createClipCard();
    createResampleCard();
    createCompressorCard();
    createMonoToStereoCard();
    createStereoToMonoCard();

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

    m_InputAudioPlayer = new AudioPlayer("Input Audio");
    ui->audioPlayerHorizontalLayout->addWidget(m_InputAudioPlayer);
    m_OutputAudioPlayer = new AudioPlayer("Output Audio", true);
    ui->audioPlayerHorizontalLayout->addWidget(m_OutputAudioPlayer);

    m_ApplyEffectsButton = new QPushButton("Apply Effects");
    ui->verticalLayout->insertWidget(1, m_ApplyEffectsButton);

    connect(m_ApplyEffectsButton, &QPushButton::clicked, this, [=](){applyEffects();});
}

void AudioLibDemo::createGainEffectCard()
{
    m_GainEffectCard = new Card("Gain");
    
    auto* slider = new QSlider(Qt::Horizontal);
    slider->setRange(1,1000);
    slider->setValue(100);
    slider->setObjectName("gainFactorSlider");

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
    slider->setObjectName("angleSlider");

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
    slider->setRange(8'000,48'000);
    slider->setValue(24'000);
    slider->setObjectName("targetSampleRateSlider");

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
    slider->setObjectName("thresholdSlider");

    QLabel* valueLabel = new QLabel("threshold: 0.050");

    QObject::connect(slider, &QSlider::valueChanged,
    [valueLabel](int value) {
        valueLabel->setText(QString("threshold: ") + QString::number(value/1000.0f, 'f', 3));
    });

    m_CompressorCard->addWidget(valueLabel);
    m_CompressorCard->addWidget(slider);
    }

    // attack
    {
    auto* slider = new QSlider(Qt::Horizontal);
    slider->setRange(1, 200);
    slider->setValue(25);
    slider->setObjectName("attackSlider");

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
    slider->setObjectName("lowFreqSlider");

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
    slider->setObjectName("highFreqSlider");

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
    lightOption->setObjectName("lightOption");
    auto* heavyOption = new QRadioButton("heavy");
    heavyOption->setObjectName("heavyOption");
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
    whiteOption->setObjectName("whiteOption");
    m_NoiseCard->addWidget(whiteOption);
    m_NoiseCard->addWidget(brownianOption);

    auto* slider = new QSlider(Qt::Horizontal);
    slider->setRange(10,1000);
    slider->setValue(50);
    slider->setObjectName("intensitySlider");

    QLabel* valueLabel = new QLabel("intensity: 0.50");

    QObject::connect(slider, &QSlider::valueChanged,
    [valueLabel](int value) {
        valueLabel->setText(QString("intensity: ") + QString::number(value/100.0f, 'f', 2));
    });

    m_NoiseCard->addWidget(valueLabel);
    m_NoiseCard->addWidget(slider);

    m_EffectCardLayout->addWidget(m_NoiseCard);
}

void AudioLibDemo::applyEffects()
{
    QUrl sourceUrl = m_InputAudioPlayer->getSource();
    if (m_InputAudioPlayer->getSource().isEmpty())
    {
        std::cout << "No source has been selected.\n";
        return;
    }

    std::string source = sourceUrl.toString().toStdString().substr(7);

    AudioLib::AudioFile inputFile(source);
    std::cout << "Applying audio effects . . .\n";
    applyGain(inputFile);
    applyMonoToStereo(inputFile);
    applyStereoToMono(inputFile);
    applyPan(inputFile);
    applyClip(inputFile);
    applyResample(inputFile);
    applyCompressor(inputFile);
    applyBandPassFilter(inputFile);
    applyConvolutionReverb(inputFile);
    applyNoise(inputFile);
    inputFile.Write(OUTPUT_AUDIO_DIR"/output.wav");
    m_OutputAudioPlayer->hide();
    ui->audioPlayerHorizontalLayout->removeWidget(m_OutputAudioPlayer);
    m_OutputAudioPlayer = new AudioPlayer("Output Audio", true);
    ui->audioPlayerHorizontalLayout->addWidget(m_OutputAudioPlayer);
    m_OutputAudioPlayer->setSource(OUTPUT_AUDIO_DIR"/output.wav");
    std::cout << "Finished.\n";
}

void AudioLibDemo::applyGain(AudioLib::AudioFile& audioFile)
{
    if (!m_GainEffectCard->getEnabled())
    {
        return;
    }

    AudioLib::GainEffect gain("gain", m_GainEffectCard->findChild<QSlider*>("gainFactorSlider")->value()/100.0f);
    gain.ApplyInPlace(audioFile);
}

void AudioLibDemo::applyMonoToStereo(AudioLib::AudioFile& audioFile)
{
    if (!m_MonoToStereoCard->getEnabled())
    {
        return;
    }

    AudioLib::MonoToStereoEffect monoToStereo("monoToStereo");
    monoToStereo.ApplyInPlace(audioFile);
}

void AudioLibDemo::applyStereoToMono(AudioLib::AudioFile& audioFile)
{
    if (!m_StereoToMonoCard->getEnabled())
    {
        return;
    }

    AudioLib::StereoToMonoEffect stereoToMono("stereoToMono");
    stereoToMono.ApplyInPlace(audioFile);
}

void AudioLibDemo::applyPan(AudioLib::AudioFile& audioFile)
{
    if (!m_PanEffectCard->getEnabled())
    {
        return;
    }

    float angle = static_cast<float>(m_PanEffectCard->findChild<QSlider*>("angleSlider")->value());
    AudioLib::PanEffect pan("pan", angle);
    pan.ApplyInPlace(audioFile);
}

void AudioLibDemo::applyClip(AudioLib::AudioFile& audioFile)
{

}

void AudioLibDemo::applyResample(AudioLib::AudioFile& audioFile)
{
    if (!m_ResampleCard->getEnabled())
    {
        return;
    }

    unsigned int targetSampleRate = static_cast<unsigned int>(m_ResampleCard->findChild<QSlider*>("targetSampleRateSlider")->value());

    AudioLib::ResampleEffect resample("resample", targetSampleRate);
    resample.ApplyInPlace(audioFile);
}

void AudioLibDemo::applyCompressor(AudioLib::AudioFile& audioFile)
{
    if (!m_CompressorCard->getEnabled())
    {
        return;
    }

    float threshold = m_CompressorCard->findChild<QSlider*>("thresholdSlider")->value() / 1000.0f;
    float attackSeconds = m_CompressorCard->findChild<QSlider*>("attackSlider")->value() / 100.0f;
    AudioLib::CompressorEffect compressor("compressor", threshold, 4.0f, attackSeconds, 0.1f);
    compressor.ApplyInPlace(audioFile);
}

void AudioLibDemo::applyBandPassFilter(AudioLib::AudioFile& audioFile)
{
    if (!m_BandPassFilterCard->getEnabled())
    {
        return;
    }

    float lowFreq = static_cast<float>(m_BandPassFilterCard->findChild<QSlider*>("lowFreqSlider")->value());
    float highFreq = static_cast<float>(m_BandPassFilterCard->findChild<QSlider*>("highFreqSlider")->value());
    AudioLib::BandPassFilterEffect bandPassFilter("bandPassFilter", lowFreq, highFreq);
    bandPassFilter.ApplyInPlace(audioFile);
}

void AudioLibDemo::applyConvolutionReverb(AudioLib::AudioFile& audioFile)
{
    if (!m_ConvolutionReverbCard->getEnabled())
    {
        return;
    }

    std::unique_ptr<AudioLib::AudioFile> reverbFile;

    bool light = m_ConvolutionReverbCard->findChild<QRadioButton*>("lightOption")->isChecked();

    if (light)
    {
        reverbFile = std::make_unique<AudioLib::AudioFile>(TEST_AUDIO_DIR"/WireGrind_m_0.3s_06w_100Hz_02m.wav");
    }
    else
    {
        reverbFile = std::make_unique<AudioLib::AudioFile>(TEST_AUDIO_DIR"/WireGrind_m_4.8s_99w_900Hz_30m.wav");
    }

    AudioLib::ConvolutionReverbEffect convolutionReverb("convolutionReverb", std::move(reverbFile));
    convolutionReverb.ApplyInPlace(audioFile);
}

void AudioLibDemo::applyNoise(AudioLib::AudioFile& audioFile)
{
    if (!m_NoiseCard->getEnabled())
    {
        return;
    }

    using Type = AudioLib::NoiseEffect::Type;
    Type type = m_NoiseCard->findChild<QRadioButton*>("whiteOption")->isChecked() ? Type::White : Type::Brownian;
    float intensity = m_NoiseCard->findChild<QSlider*>("intensitySlider")->value() / 100.0f;
    if (type == Type::Brownian) intensity *= 0.075f;
    AudioLib::NoiseEffect noise("noise", type, intensity);
    noise.ApplyInPlace(audioFile);
}

AudioLibDemo::~AudioLibDemo()
{
    delete ui;
}
