#include "AudioFile.h"

#include <cassert>
#include <filesystem>
#include <iostream>

#include "dr_wav.h"

namespace AudioLib
{
    AudioFile::AudioFile(const std::string& pathstring)
        : m_Filepath(pathstring)
    {
        std::filesystem::path filepath(pathstring);

        float* samples;

        m_Type = filepath.extension();

        if (m_Type == ".wav")
        {
            drwav_uint64 frameCount;
            samples = drwav_open_file_and_read_pcm_frames_f32(
                filepath.c_str(), &m_Channels, &m_SampleRate, &frameCount, nullptr
            );
            m_FrameCount = static_cast<size_t>(frameCount);
        }
        else
        {
            std::cout << "Only .wav audio files are supported.\n";
            assert(false && "Unsupported audio file type.");
        }   

        if (!samples)
        {
            std::cout << "Failed to load audio file." << '\n';
            assert(false && "Failed to load audio file.");
        }

        m_Samples = std::vector<float>(samples, samples+m_FrameCount*m_Channels);
        
        if (m_Type == ".wav")
        {
            drwav_free(samples, nullptr);
        }

        m_DurationSeconds = static_cast<float>(m_FrameCount) / m_SampleRate;
    }

    void AudioFile::Write(const std::string& pathstring)
    {
        
    }
}