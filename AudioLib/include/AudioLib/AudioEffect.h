#pragma once

#include "AudioFile.h"

namespace AudioLib
{
    class AudioEffect
    {
    public:
        AudioEffect(const std::string& name);
        AudioFile Apply(const AudioFile& audioFile);
        virtual void ApplyInPlace(AudioFile& audioFile) = 0;
    private:
        std::string m_Name;
    };

    class GainEffect : public AudioEffect
    {
    public:
        GainEffect(const std::string& name, float gainFactor);
        void ApplyInPlace(AudioFile& audioFile) override;
    private:
        float m_GainFactor;
    };
}