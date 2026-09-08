#include "AudioEffect.h"

#include <cassert>
#include <iostream>

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
    
    GainEffect::GainEffect(const std::string& name, float gainFactor)
        : AudioEffect(name), m_GainFactor(gainFactor) {}

    void GainEffect::ApplyInPlace(AudioFile& audioFile)
    {
        for (float& sample : audioFile.GetSamples())
        {
            sample *= m_GainFactor;
        }
    }
}