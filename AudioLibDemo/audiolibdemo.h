#ifndef AUDIOLIBDEMO_H
#define AUDIOLIBDEMO_H

#include "AudioLib/AudioEffect.h"
#include "card.h"
#include "texteditstreambuf.h"

#include "audioplayer.h"

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class AudioLibDemo;
}
QT_END_NAMESPACE

class AudioLibDemo : public QMainWindow
{
    Q_OBJECT

public:
    AudioLibDemo(QWidget *parent = nullptr);
    ~AudioLibDemo();

private:
    void createGainEffectCard();
    void createMonoToStereoCard();
    void createStereoToMonoCard();
    void createPanEffectCard();
    void createClipCard();
    void createResampleCard();
    void createCompressorCard();
    void createBandPassFilterCard();
    void createConvolutionReverbCard();
    void createNoiseCard();

    void applyEffects();
    void applyGain(AudioLib::AudioFile& audioFile);
    void applyMonoToStereo(AudioLib::AudioFile& audioFile);
    void applyStereoToMono(AudioLib::AudioFile& audioFile);
    void applyPan(AudioLib::AudioFile& audioFile);
    void applyClip(AudioLib::AudioFile& audioFile);
    void applyResample(AudioLib::AudioFile& audioFile);
    void applyCompressor(AudioLib::AudioFile& audioFile);
    void applyBandPassFilter(AudioLib::AudioFile& audioFile);
    void applyConvolutionReverb(AudioLib::AudioFile& audioFile);
    void applyNoise(AudioLib::AudioFile& audioFile);

    QVBoxLayout* m_EffectCardLayout;
    Card* m_GainEffectCard, *m_MonoToStereoCard, *m_StereoToMonoCard,
         *m_PanEffectCard, *m_ClipCard, *m_ResampleCard,
         *m_CompressorCard, *m_BandPassFilterCard, *m_ConvolutionReverbCard,
         *m_NoiseCard;
    AudioPlayer* m_InputAudioPlayer, *m_OutputAudioPlayer;
    TextEditStreamBuf* m_OutputStreamBuf;
    QPushButton* m_ApplyEffectsButton;
    Ui::AudioLibDemo *ui;
};
#endif // AUDIOLIBDEMO_H
