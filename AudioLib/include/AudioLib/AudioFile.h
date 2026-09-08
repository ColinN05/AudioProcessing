#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace AudioLib
{
    class AudioFile
    {
    public:
        AudioFile(const std::string& pathstring);
        AudioFile(const AudioFile& other) = default;

        void Write(const std::string& pathstring);

        inline const std::string& GetFilepath() const { return m_Filepath; }
        inline const std::string& GetType() const { return m_Type; }
        inline unsigned int GetChannels() const { return m_Channels; }
        inline unsigned int GetSampleRate() const { return m_SampleRate; }
        inline size_t GetFrameCount() const { return m_FrameCount; }
        inline float GetDurationSeconds() const { return m_DurationSeconds; }
        inline const std::vector<float>& GetSamples() const { return m_Samples; }
        inline std::vector<float>& GetSamples() { return m_Samples; }
    private:
        std::string m_Filepath;
        std::string m_Type;
        unsigned int m_Channels;
        unsigned int m_SampleRate;
        size_t m_FrameCount;
        float m_DurationSeconds;
        std::vector<float> m_Samples;
    };
}