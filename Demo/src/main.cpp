#include <iostream>

#include "AudioLib/AudioFile.h"

int main()
{
    AudioLib::AudioFile audioFile("/home/colin/dev/AudioProcessing/TestAudioFiles/test.wav");
    std::cout << "File info:\n"
              << "\tFilepath: " << audioFile.GetFilepath() << '\n'
              << "\tType: " << audioFile.GetType() << '\n'
              << "\tChannels: " << audioFile.GetChannels() << '\n'
              << "\tSample Rate: " << audioFile.GetSampleRate() << '\n'
              << "\tFrame Count: " << audioFile.GetFrameCount() << '\n'
              << "\tDuration Seconds: " << audioFile.GetDurationSeconds() << '\n';
}