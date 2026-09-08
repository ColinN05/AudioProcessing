#include "AudioEffect.h"

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