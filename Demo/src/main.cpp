#include <iostream>

#include "AudioLib/AudioEffect.h"
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
    
    AudioLib::GainEffect tripleVolume("tripleVolume", 3.0f);
    AudioLib::AudioFile audioFileTripleVolume = tripleVolume.Apply(audioFile);
    audioFileTripleVolume.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_triple_volume.wav");
    
    AudioLib::GainEffect oneThirdVolume("oneThirdVolume", 1/3.0f);
    AudioLib::AudioFile audioFileOneThirdVolume = oneThirdVolume.Apply(audioFile);
    audioFileOneThirdVolume.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_one_third_volume.wav");
}