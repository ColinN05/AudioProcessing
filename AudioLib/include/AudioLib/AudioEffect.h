#pragma once

#include "AudioFile.h"

#include <memory>

namespace AudioLib
{
    class AudioEffect
    {
    public:
        AudioEffect(const std::string& name);
        virtual ~AudioEffect() = default;
        AudioFile Apply(const AudioFile& audioFile);
        virtual void ApplyInPlace(AudioFile& audioFile) = 0;
    protected:
        std::string m_Name;
    };

    class CompositeAudioEffect : public AudioEffect
    {
    public:
        CompositeAudioEffect(const std::string& name, const std::vector<std::unique_ptr<AudioEffect>>& effects);
        void ApplyInPlace(AudioFile& audioFile) override;
    private:
        std::vector<std::unique_ptr<AudioEffect>> m_Effects;
    };

    class MonoToStereoEffect : public AudioEffect
    {
    public:
        MonoToStereoEffect(const std::string& name, bool conservePower = false);
        void ApplyInPlace(AudioFile& audioFile) override;
    private:
        bool m_ConservePower;
    };

    class StereoToMonoEffect : public AudioEffect
    {
    public:
        StereoToMonoEffect(const std::string& name);
        void ApplyInPlace(AudioFile& audioFile) override;
    private:
    };

    class GainEffect : public AudioEffect
    {
    public:
        GainEffect(const std::string& name, float gainFactor);
        void ApplyInPlace(AudioFile& audioFile) override;
    private:
        float m_GainFactor;
    };

    class PanEffect : public AudioEffect
    {
    public:
        PanEffect(const std::string& name, float angle);
        void ApplyInPlace(AudioFile& audioFile) override;
    private:
        void ApplyInPlaceMono(AudioFile& audioFile);
        void ApplyInPlaceStereo(AudioFile& audioFile);
        float m_Angle;
    };
}