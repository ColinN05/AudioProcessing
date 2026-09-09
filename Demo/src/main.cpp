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

    AudioLib::MonoToStereoEffect monoToStereo("monoToStereo", false);
    AudioLib::AudioFile audioFileStereo = monoToStereo.Apply(audioFile);
    audioFileStereo.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_stereo.wav");

    AudioLib::StereoToMonoEffect stereoToMono("stereoToMono");
    AudioLib::AudioFile audioFileMono = stereoToMono.Apply(audioFileStereo);
    audioFileMono.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_mono.wav");

    constexpr float pi = 3.1415926f;

    AudioLib::PanEffect panRight("panRight", 3*pi/4);
    AudioLib::AudioFile audioFilePannedRight = panRight.Apply(audioFile);
    audioFilePannedRight.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_panned_right.wav");
    AudioLib::AudioFile audioFileStereoPannedRight = panRight.Apply(audioFileStereo);
    audioFileStereoPannedRight.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_stereo_panned_right.wav");

    AudioLib::PanEffect panLeft("panLeft", pi/4);
    AudioLib::AudioFile audioFilePannedLeft = panLeft.Apply(audioFile);
    audioFilePannedLeft.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_panned_left.wav");
    AudioLib::AudioFile audioFileStereoPannedLeft = panLeft.Apply(audioFileStereo);
    audioFileStereoPannedLeft.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_stereo_panned_left.wav");

    AudioLib::ClipEffect volumeClip("volumeClip", 0.005f);
    AudioLib::AudioFile audioFileVolumeClipped = volumeClip.Apply(audioFile);
    audioFileVolumeClipped.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_volume_clipped.wav");

    AudioLib::GainEffect increaseVolume("increaseVolume", 1/0.005f);
    auto audioFileIncreasedVolume = increaseVolume.Apply(audioFileVolumeClipped);
    audioFileIncreasedVolume.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_increased_volume.wav");

    AudioLib::CompositeAudioEffect panLeftAndTripleVolume(
        "panLeftAndTripleVolume",
        std::make_unique<AudioLib::PanEffect>("panLeft", 3*pi/4),
        std::make_unique<AudioLib::GainEffect>("tripleVolume", 3.0f)
    );
    AudioLib::AudioFile audioFilePannedLeftAndTripleVolume = panLeftAndTripleVolume.Apply(audioFile);
    audioFilePannedLeftAndTripleVolume.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_panned_left_and_triple_volume.wav");
}