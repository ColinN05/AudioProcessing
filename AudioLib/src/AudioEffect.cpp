#include "AudioEffect.h"

#include "fft.h"

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

    MixEffect::MixEffect(const std::string& name, AudioFile* mixer, float mix, float offsetSeconds)
        : AudioEffect(name), m_Mixer(mixer), m_Mix(mix), m_OffsetSeconds(offsetSeconds) 
    {
        assert(m_Mixer);
        assert(m_OffsetSeconds >= -m_Mixer->GetDurationSeconds());
    }

    void MixEffect::ApplyInPlace(AudioFile& audioFile)
    {
        assert(m_OffsetSeconds <= audioFile.GetDurationSeconds());

        if (m_Mixer->GetSampleRate() != audioFile.GetSampleRate())
        {
            std::cout << "Cannot mix. Files do not have equal sampling rates.\n";
            assert(false && "Files do not have equal sampling rates.");
            return;
        }

        int mixerChannels = m_Mixer->GetChannels();
        int channels = audioFile.GetChannels();
        
        int lowerSeconds = std::max(m_OffsetSeconds,0.0f);
        int lowerIndex = lowerSeconds * audioFile.GetSampleRate();
        int upperSeconds = std::min(m_OffsetSeconds+m_Mixer->GetDurationSeconds(),audioFile.GetDurationSeconds());
        int upperIndex = upperSeconds * audioFile.GetSampleRate();

        const auto& mixerSamples = m_Mixer->GetSamples();
        auto& samples = audioFile.GetSamples();

        int indexOffset = m_OffsetSeconds*m_Mixer->GetSampleRate();

        if (mixerChannels == 1 && channels == 1)
        {
            for (int i = lowerIndex; i <= upperIndex; ++i)
            {
                samples[i] = samples[i] * (1.0f-m_Mix) + mixerSamples[i-indexOffset] * m_Mix;
            }
        }
        else if (mixerChannels == 1 && channels == 2)
        {
            for (int i = lowerIndex; i <= upperIndex; ++i)
            {
                samples[2*i] = samples[2*i] * (1.0f - m_Mix) + mixerSamples[i-indexOffset] * m_Mix;
                samples[2*i+1] = samples[2*i];
            }
        }
        else if (mixerChannels == 2 && channels == 1)
        {
            for (int i = lowerIndex; i <= upperIndex; ++i)
            {
                float avg = (mixerSamples[2*(i-indexOffset)] + mixerSamples[2*(i-indexOffset)+1]) * 0.5f;
                samples[i] = samples[i] * (1.0f - m_Mix) + avg * m_Mix;
            }
        }
        else if (mixerChannels == 2 && channels == 2)
        {
            for (int i = lowerIndex; i <= upperIndex; ++i)
            {
                samples[2*i] = samples[2*i] * (1.0f - m_Mix) + mixerSamples[2*(i-indexOffset)] * m_Mix;
                samples[2*i+1] = samples[2*i+1] * (1.0f - m_Mix) + mixerSamples[2*(i-indexOffset)+1] * m_Mix;
            }
        }
        else
        {
            std::cout << "Cannot mix. Invaid channel counts.\n";
            assert(false && "Invalid channel counts.");
        }
    }

    ResampleEffect::ResampleEffect(const std::string& name, unsigned int targetSampleRate)
        : AudioEffect(name), m_TargetSampleRate(targetSampleRate) 
    {
        if (m_TargetSampleRate < 1)
        {
            std::cout << "Target sample rate must be at least 1hz.\n";
            assert(false && "Invalid target sample rate.");
        }
    }


    void ResampleEffect::ApplyInPlace(AudioFile& audioFile)
    {
        unsigned int sampleRate = audioFile.GetSampleRate();
        unsigned int frameCount = audioFile.GetFrameCount();
        float resampleRatio = static_cast<float>(m_TargetSampleRate) / sampleRate;
        unsigned int resampledFrameCount = static_cast<unsigned int>(frameCount * resampleRatio); 
        unsigned int channels = audioFile.GetChannels();
        std::vector<float>& samples = audioFile.GetSamples();
        std::vector<float> newSamples(resampledFrameCount*channels);
        for (int i = 0; i < resampledFrameCount; ++i)
        {
            unsigned int l = (static_cast<float>(i)/resampledFrameCount)*frameCount;
            unsigned int r = std::min(l+1,frameCount-1);
            float interpFactor = (i-l*resampleRatio)/((r-l)*resampleRatio);

            for (int c = 0; c < channels; ++c)
            {
                newSamples[channels*i+c] = (1.0f-interpFactor)*samples[channels*l+c] + interpFactor*samples[channels*r+c];
            }
        }
        samples = std::move(newSamples);
        audioFile.m_SampleRate = m_TargetSampleRate;
        audioFile.m_FrameCount = resampledFrameCount;
    }

    CompressorEffect::CompressorEffect(const std::string& name, float threshold, float rate, float attackSeconds, float fadeSeconds)
        : AudioEffect(name), m_Threshold(threshold), m_Rate(rate), m_AttackSeconds(attackSeconds), m_FadeSeconds(fadeSeconds)
    {
        assert(m_Rate > 0.0f);
        assert(m_AttackSeconds > 0.0f);
        assert(m_FadeSeconds > 0.0f);
    }

    void CompressorEffect::ApplyInPlace(AudioFile& audioFile)
    {
        std::vector<float>& samples = audioFile.GetSamples();
        int channels = audioFile.GetChannels();
        int frameCount = audioFile.GetFrameCount();
        float dt = 1.0f / audioFile.GetSampleRate();
        std::vector<float> compressorFactors(channels,0.0f);
        for (int i = 0; i < frameCount; ++i)
        {
            for (int c = 0; c < channels; ++c)
            {
                float& sample = samples[channels*i+c];
                float abssample = std::abs(sample);
                float sampleSign = (sample >= 0.0f) ? 1.0f : -1.0f;

                if (abssample > m_Threshold)
                {
                    compressorFactors[c] = std::min(1.0f,compressorFactors[c]+dt/m_AttackSeconds);

                    float compressedSample = ((abssample - m_Threshold) / m_Rate + m_Threshold)*sampleSign;
                    sample = sample * (1.0f-compressorFactors[c]) + compressedSample * compressorFactors[c];
                }
            }
        }
    }

    BandPassFilterEffect::BandPassFilterEffect(const std::string& name, float lowFreq, float highFreq)
        : AudioEffect(name), m_LowFreq(lowFreq), m_HighFreq(highFreq)
    {
        assert(lowFreq >= 0.0f && highFreq >= 0.0f && lowFreq <= highFreq);
    }

    void BandPassFilterEffect::ApplyInPlace(AudioFile& audioFile)
    {
        unsigned int channels = audioFile.GetChannels();
        if (channels < 1)
        {
            std::cout << "Invalid number of channels.\n"; 
            assert(false && "Invalid number of channels.");
            return;
        }

        std::vector<float>& samples = audioFile.GetSamples(); 
        unsigned int frameCount = audioFile.GetFrameCount();

        for (int c = 0; c < channels; ++c)
        {
            std::vector<float> channelSamples(frameCount);

            for (int i = 0; i < frameCount; ++i)
            {
                channelSamples[i] = samples[channels*i+c];
            }

            std::vector<float> channelSamplesPadded = channelSamples;
            
            unsigned int n = 1;
            while (n < frameCount)
            {
                n <<= 1;
            }

            channelSamplesPadded.reserve(n);

            for (int i = channelSamples.size(); i < n; ++i)
            {
                channelSamplesPadded.push_back(0);
            }

            std::vector<complex> dftsamples = fft(channelSamplesPadded);

            float fs = audioFile.GetSampleRate();

            for (int k = 0; k < m_LowFreq*n/fs && k < n; ++k)
            {
                dftsamples[k] = 0;
            }
            for (int k = m_HighFreq*n/fs; k < n; ++k)
            {
                dftsamples[k] = 0;
            }

            std::vector<complex> samplesProcessed = fft(dftsamples, true);
            
            for (int i = 0; i < frameCount; ++i)
            {
                samples[channels*i+c] = samplesProcessed[i].real();
            }
        }
    }

    ConvolutionReverbEffect::ConvolutionReverbEffect(const std::string& name, std::unique_ptr<AudioFile> reverbFile)
        : AudioEffect(name), m_ReverbFile(std::move(reverbFile))
    {
        assert(m_ReverbFile);
        assert(m_ReverbFile->GetChannels() == 1 && "Only single-channel impulse responses are supported.");
    }

    void ConvolutionReverbEffect::ApplyInPlace(AudioFile& audioFile)
    {
        if (audioFile.GetChannels() != 1)
        {
            std::cout << "Convolution reverb is only supported for single channel audio files.\n";
            assert(false && "AudioFile must be single-channel.");
            return;
        }

        AudioFile* reverbFile;
        bool resampleRequired = false;
        if (m_ReverbFile->GetSampleRate() == audioFile.GetSampleRate())
        {
            reverbFile = m_ReverbFile.get();
        }
        else
        {
            resampleRequired = true;
            reverbFile = new AudioFile(*m_ReverbFile);
            ResampleEffect resampler("resampler", audioFile.GetSampleRate());
            resampler.ApplyInPlace(*reverbFile);
        }

        std::vector<float>& samples = audioFile.GetSamples(); 
        std::vector<float> reverbSamples = reverbFile->GetSamples();

        reverbSamples.resize(samples.size(), 0.0f);

        int n = 1;
        while (n < samples.size())
        {
            n <<= 1;
        }
        samples.resize(n,0.0f);
        reverbSamples.resize(n,0.0f);

        auto fftsamples = fft(samples);
        auto fftreverbSamples = fft(reverbSamples);

        std::vector<complex> samplesComplex(samples.size());

        for (int i = 0; i < samples.size(); ++i)
        {
            samplesComplex[i] = fftsamples[i]*fftreverbSamples[i];
        }

        samplesComplex = std::move(fft(samplesComplex,true));

        for (int i = 0; i < samples.size(); ++i)
        {
            samples[i] = samplesComplex[i].real();
        }

        if (resampleRequired)
        {
            delete reverbFile;
        }
    }
}