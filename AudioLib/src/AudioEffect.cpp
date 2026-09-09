#include "AudioEffect.h"

#include <cassert>
#include <iostream>
#include <cmath>

namespace AudioLib
{
    AudioEffect::AudioEffect(const std::string& name)
        : m_Name(name) {}

    AudioFile AudioEffect::Apply(const AudioFile& audioFile)
    {
        AudioFile copy(audioFile);
        ApplyInPlace(copy);
        return copy;
    }
    
    void CompositeAudioEffect::ApplyInPlace(AudioFile& audioFile)
    {
        for (const std::unique_ptr<AudioEffect>& effect : m_Effects)
        {
            effect->ApplyInPlace(audioFile);
        }
    }

    MonoToStereoEffect::MonoToStereoEffect(const std::string& name, bool conservePower)
        : AudioEffect(name), m_ConservePower(conservePower) {}

    void MonoToStereoEffect::ApplyInPlace(AudioFile& audioFile)
    {
        if (audioFile.GetChannels() != 1)
        {
            std::cout << "Audio file is not mono!\n";
            assert(false && "Audio file is not mono!");
            return;
        }
        const std::vector<float>& monoSamples = audioFile.GetSamples();
        size_t monoSampleCount = monoSamples.size();
        size_t stereoSampleCount = monoSampleCount*2;
        std::vector<float> stereoSamples(stereoSampleCount);
        for (size_t i = 0; i < monoSampleCount; ++i)
        {
            float monoSample = monoSamples[i];
            stereoSamples[2*i] = monoSample;
            stereoSamples[2*i+1] = monoSample;
        }
        if (m_ConservePower)
        {
            for (float& sample : stereoSamples)
            {
                sample *= 0.7071f;
            }
        }
        audioFile.m_Samples = std::move(stereoSamples);
        audioFile.m_Channels = 2;
    }

    StereoToMonoEffect::StereoToMonoEffect(const std::string& name)
        : AudioEffect(name) {}

    void StereoToMonoEffect::ApplyInPlace(AudioFile& audioFile)
    {
        if (audioFile.GetChannels() != 2)
        {
            std::cout << "Audio file is not stereo!\n";
            assert(false && "Audio file is not stereo!");
            return;
        }
        const std::vector<float>& stereoSamples = audioFile.GetSamples();
        size_t stereoSampleCount = stereoSamples.size();
        assert((stereoSampleCount%2)==0 && "Stereo audio file should have even number of samples.");
        size_t monoSampleCount = stereoSampleCount/2;
        std::vector<float> monoSamples(monoSampleCount);
        for (size_t i = 0; i < monoSampleCount; ++i)
        {
            float stereoSampleLeft = stereoSamples[2*i];
            float stereoSampleRight = stereoSamples[2*i+1];
            monoSamples[i] = (stereoSampleLeft+stereoSampleRight)*0.5f;
        }
        audioFile.m_Samples = std::move(monoSamples);
        audioFile.m_Channels = 1;
    }
    
    GainEffect::GainEffect(const std::string& name, float gainFactor)
        : AudioEffect(name), m_GainFactor(gainFactor) {}

    void GainEffect::ApplyInPlace(AudioFile& audioFile)
    {
        for (float& sample : audioFile.GetSamples())
        {
            sample *= m_GainFactor;
        }
    }

    PanEffect::PanEffect(const std::string& name, float angle)
    : AudioEffect(name), m_Angle(angle) {}

    void PanEffect::ApplyInPlace(AudioFile& audioFile)
    {
        unsigned int channels = audioFile.GetChannels();
        if (channels == 1)
        {
            ApplyInPlaceMono(audioFile);
        }
        else if (channels == 2)
        {
            ApplyInPlaceStereo(audioFile);
        }
        else
        {
            std::cout << "Invalid number of channels. Pan effect is only supported for mono and stereo audio files.\n";
            assert(false && "Invalid number of channels.");
        }
    }

    void PanEffect::ApplyInPlaceMono(AudioFile& audioFile)
    {
        MonoToStereoEffect monoToStereo("m2s", false);
        monoToStereo.ApplyInPlace(audioFile);
        float leftGain = (std::sinf(m_Angle)-std::cosf(m_Angle))*0.7071f;
        float rightGain = -leftGain;
        auto clampPositive = [](float& x) {if (x<0) x=0;};
        clampPositive(leftGain);
        clampPositive(rightGain);
        std::vector<float>& samples = audioFile.GetSamples();
        size_t frameCount = audioFile.GetFrameCount();
        for (int i = 0; i < frameCount; ++i)
        {
            samples[2*i] *= leftGain;
            samples[2*i+1] *= rightGain;
        }
    }

    void PanEffect::ApplyInPlaceStereo(AudioFile& audioFile)
    {
        float c = std::cosf(m_Angle-3.1416f/4.0f);
        float ll = c;
        float lr = -c;
        float rl = -c;
        float rr = c;

        auto clampPositive = [](float& x) {if (x<0) x=0;};
        clampPositive(ll);
        clampPositive(lr);
        clampPositive(rl);
        clampPositive(rr);

        std::vector<float>& samples = audioFile.GetSamples();
        size_t frameCount = audioFile.GetFrameCount();
        for (int i = 0; i < frameCount; ++i)
        {
            float l = samples[2*i];
            float r = samples[2*i+1];
            samples[2*i] = ll*l+lr*r;
            samples[2*i+1] = rl*l+rr*r;
        }
    }

    ClipEffect::ClipEffect(const std::string& name, float maxAmplitude)
        : AudioEffect(name), m_MaxAmplitude(maxAmplitude)
    {
        if (m_MaxAmplitude < 0.0f || m_MaxAmplitude > 1.0f)
        {
            std::cout << "Max amplitude must be between 0 and 1.\n";
            assert(false && "Max amplitude must be between 0 and 1");
        }
    }

    void ClipEffect::ApplyInPlace(AudioFile& audioFile)
    {
        std::vector<float>& samples = audioFile.GetSamples();
        for (float& sample : samples)
        {
            if (sample < -m_MaxAmplitude)
            {
                sample = -m_MaxAmplitude;
            }
            else if (sample > m_MaxAmplitude)
            {
                sample = m_MaxAmplitude;
            }
        }
    }
}