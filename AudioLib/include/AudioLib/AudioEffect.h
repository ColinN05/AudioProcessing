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
        template <typename... Effects>
        CompositeAudioEffect(const std::string& name, Effects&&... effects)
            : AudioEffect(name)
        {
            m_Effects.reserve(sizeof...(Effects));
            (m_Effects.emplace_back(std::forward<Effects>(effects)), ...);
        }
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

    class ClipEffect : public AudioEffect
    {
    public:
        ClipEffect(const std::string& name, float maxAmplitude);
        void ApplyInPlace(AudioFile& audioFile) override;
    private:
        float m_MaxAmplitude;
    };

    class MixEffect : public AudioEffect
    {
    public:
        MixEffect(const std::string& name, AudioFile* mixer, float mix, float offsetSeconds);
        void ApplyInPlace(AudioFile& audioFile);
    private:
        AudioFile* m_Mixer;
        float m_Mix;
        float m_OffsetSeconds;
    };

    class ResampleEffect : public AudioEffect
    {
    public:
        ResampleEffect(const std::string& name, unsigned int targetSampleRate);
        void ApplyInPlace(AudioFile& audioFile) override;
    private:
        unsigned int m_TargetSampleRate;
    };

    class CompressorEffect : public AudioEffect
    {
    public:
        CompressorEffect(const std::string& name, float threshold, float rate, float attackSeconds, float fadeSeconds);
        void ApplyInPlace(AudioFile& audioFile) override;
    private:
        float m_Threshold;
        float m_Rate;
        float m_AttackSeconds;
        float m_FadeSeconds;
    };

    class BandPassFilterEffect : public AudioEffect
    {
    public:
        BandPassFilterEffect(const std::string& name, float lowFreq, float highFreq);
        void ApplyInPlace(AudioFile& audioFile) override;
    private:
        float m_LowFreq;
        float m_HighFreq;
    };
}