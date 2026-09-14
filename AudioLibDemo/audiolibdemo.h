#ifndef AUDIOLIBDEMO_H
#define AUDIOLIBDEMO_H

#include "card.h"

#include <QMainWindow>
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

    QVBoxLayout* m_EffectCardLayout;
    Card* m_GainEffectCard, *m_MonoToStereoCard, *m_StereoToMonoCard,
         *m_PanEffectCard, *m_ClipCard, *m_ResampleCard,
         *m_CompressorCard, *m_BandPassFilterCard, *m_ConvolutionReverbCard,
         *m_NoiseCard;

    Ui::AudioLibDemo *ui;
};
#endif // AUDIOLIBDEMO_H
